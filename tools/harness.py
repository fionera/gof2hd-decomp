#!/usr/bin/env python3
"""Parallel Codex decompilation harness for GOF2HD.

Spawns N isolated git-worktree workers targeting non-overlapping TUs/symbols,
runs `codex exec`, validates each worker with `tools/validate.py check`,
keeps improved branches, and cleans up unimproved worktrees automatically.
"""

from __future__ import annotations

import argparse
import collections
import concurrent.futures
import datetime as dt
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import threading
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent
if str(SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPT_DIR))

import validate  # noqa: E402

PROMPT_TEMPLATE = SCRIPT_DIR / "agent_prompt.md"
_GIT_LOCK = threading.Lock()
_ANSI_ESCAPE_RE = re.compile(r"\x1b\[[0-9;?]*[ -/]*[@-~]")
_WORKER_COLORS = (
    "\x1b[36m",  # cyan
    "\x1b[33m",  # yellow
    "\x1b[32m",  # green
    "\x1b[35m",  # magenta
    "\x1b[34m",  # blue
    "\x1b[96m",  # bright cyan
    "\x1b[93m",  # bright yellow
    "\x1b[92m",  # bright green
)
_ANSI_RESET = "\x1b[0m"


@dataclass
class TargetGroup:
    tu_key: str
    slug: str
    symbols: List[Dict[str, Any]] = field(default_factory=list)

    @property
    def total_diff_bytes(self) -> int:
        return sum(int(s.get("diff_bytes", 0)) for s in self.symbols)


@dataclass
class WorkerLiveState:
    worker: int
    slug: str
    tu: str
    phase: str = "queued"
    started_at: Optional[float] = None
    finished_at: Optional[float] = None
    last_line: str = ""
    recent_lines: collections.deque[str] = field(default_factory=lambda: collections.deque(maxlen=20))
    status: str = "queued"
    delta: int = 0


def _clean_line(text: str) -> str:
    cleaned = _ANSI_ESCAPE_RE.sub("", text)
    cleaned = cleaned.replace("\r", " ").replace("\n", " ").replace("\t", " ")
    return " ".join(cleaned.split())


def _truncate(text: str, width: int) -> str:
    if width <= 0:
        return ""
    if len(text) <= width:
        return text
    if width <= 3:
        return text[:width]
    return text[: width - 3] + "..."


def _fmt_elapsed(seconds: float) -> str:
    secs = max(0, int(seconds))
    mins, secs = divmod(secs, 60)
    hours, mins = divmod(mins, 60)
    if hours > 0:
        return f"{hours}h{mins:02d}m{secs:02d}s"
    return f"{mins:02d}:{secs:02d}"


def resolve_view_mode(view: str, is_tty: bool) -> str:
    if view == "auto":
        return "dashboard" if is_tty else "stream"
    return view


class HarnessMonitor:
    """Thread-safe live worker state tracker and terminal view renderer."""

    def __init__(
        self,
        run_id: str,
        groups: Sequence[TargetGroup],
        log_dir: Path,
        view: str,
        out: Optional[Any] = None,
        is_tty: Optional[bool] = None,
        refresh_interval: float = 0.25,
    ) -> None:
        self.run_id = run_id
        self.log_dir = log_dir
        self.view = view
        self._out = out if out is not None else sys.stdout
        self.is_tty = (
            bool(getattr(self._out, "isatty", lambda: False)())
            if is_tty is None
            else is_tty
        )
        self._refresh_interval = refresh_interval
        self.lock = threading.Lock()
        self.started_at = time.monotonic()
        self.workers: Dict[int, WorkerLiveState] = {
            idx: WorkerLiveState(worker=idx, slug=g.slug, tu=g.tu_key)
            for idx, g in enumerate(groups, 1)
        }
        self.recent_tail: collections.deque[Tuple[int, str]] = collections.deque(maxlen=64)
        self._stop_event = threading.Event()
        self._thread: Optional[threading.Thread] = None
        self._dashboard_started = False

    def set_phase(self, worker_idx: int, phase: str) -> None:
        quiet_msg: Optional[str] = None
        with self.lock:
            st = self.workers.get(worker_idx)
            if st is None:
                return
            was_queued = st.started_at is None
            if was_queued and phase != "queued":
                st.started_at = time.monotonic()
                st.status = "running"
                if self.view == "quiet":
                    quiet_msg = f"[w{worker_idx}] start {st.tu}\n"
            st.phase = phase
        if quiet_msg is not None:
            with self.lock:
                self._out.write(quiet_msg)
                self._out.flush()

    def record_line(self, worker_idx: int, raw_line: str) -> None:
        stripped = raw_line.rstrip("\r\n")
        clean = _clean_line(stripped)
        stream_out: Optional[str] = None
        with self.lock:
            st = self.workers.get(worker_idx)
            if st is None:
                return
            if st.started_at is None:
                st.started_at = time.monotonic()
            if clean:
                st.last_line = clean
                st.recent_lines.append(clean)
                self.recent_tail.append((worker_idx, clean))
            if self.view == "stream":
                prefix = f"[w{worker_idx} {st.slug}] "
                if self.is_tty:
                    color = _WORKER_COLORS[(worker_idx - 1) % len(_WORKER_COLORS)]
                    stream_out = f"{color}{prefix}{_ANSI_RESET}{stripped}\n"
                else:
                    stream_out = f"{prefix}{stripped}\n"
                self._out.write(stream_out)
                self._out.flush()

    def finish_worker(
        self,
        worker_idx: int,
        status: str,
        delta: int = 0,
        message: str = "",
    ) -> None:
        with self.lock:
            st = self.workers.get(worker_idx)
            if st is None:
                return
            now = time.monotonic()
            if st.started_at is None:
                st.started_at = now
            st.finished_at = now
            st.status = status
            st.phase = status
            st.delta = delta
            if message:
                clean_msg = _clean_line(message)
                if clean_msg:
                    st.last_line = clean_msg
            if self.view == "quiet":
                detail = f" - {message}" if message else ""
                self._out.write(f"[w{worker_idx}] {status} ({delta:+d}B) {st.tu}{detail}\n")
                self._out.flush()

    def render_dashboard_text(self) -> str:
        cols, rows = shutil.get_terminal_size(fallback=(100, 30))
        cols = max(40, cols)
        rows = max(12, rows)
        now = time.monotonic()
        elapsed = _fmt_elapsed(now - self.started_at)

        with self.lock:
            states = [self.workers[idx] for idx in sorted(self.workers)]
            tail_items = list(self.recent_tail)

        total = len(states)
        done = sum(1 for s in states if s.finished_at is not None)
        matched = sum(1 for s in states if s.status == "matched")
        improved = sum(1 for s in states if s.status == "improved")
        unchanged = sum(1 for s in states if s.status == "unchanged")
        failed = sum(1 for s in states if s.status in ("regressed", "error"))

        lines: List[str] = []
        header = (
            f"GOF2HD Harness | run={self.run_id} | elapsed={elapsed} | "
            f"done={done}/{total} matched={matched} improved={improved} "
            f"unchanged={unchanged} failed={failed}"
        )
        lines.append(_truncate(header, cols))
        lines.append(_truncate(f"Logs: {self.log_dir}", cols))
        lines.append("-" * cols)

        tu_w = min(28, max(12, cols // 4))
        fixed = 4 + 1 + tu_w + 1 + 10 + 1 + 7 + 1
        act_w = max(8, cols - fixed)

        lines.append(
            _truncate(
                f"{'W':<4} {'TU':<{tu_w}} {'PHASE':<10} {'ELAPSED':>7} {'ACTIVITY'}",
                cols,
            )
        )
        for st in states:
            if st.started_at is None:
                w_elapsed = "--:--"
            else:
                end_t = st.finished_at if st.finished_at is not None else now
                w_elapsed = _fmt_elapsed(end_t - st.started_at)
            w_id = f"w{st.worker}"
            tu_col = _truncate(st.tu, tu_w)
            phase_col = _truncate(st.phase, 10)
            act_col = _truncate(st.last_line, act_w)
            lines.append(
                _truncate(
                    f"{w_id:<4} {tu_col:<{tu_w}} {phase_col:<10} {w_elapsed:>7} {act_col}",
                    cols,
                )
            )

        lines.append("-" * cols)
        lines.append("Live Tail:")
        max_tail = max(3, min(8, rows - len(lines) - 1))
        active_ids = {s.worker for s in states if s.finished_at is None}
        if active_ids:
            active_tail = [item for item in tail_items if item[0] in active_ids]
            shown_tail = active_tail[-max_tail:] if active_tail else tail_items[-max_tail:]
        else:
            shown_tail = tail_items[-max_tail:]
        if shown_tail:
            for w_idx, text in shown_tail:
                lines.append(_truncate(f"[w{w_idx}] {text}", cols))
        else:
            lines.append("  (waiting for worker output...)")

        return "\n".join(lines) + "\n"

    def _draw_dashboard(self) -> None:
        frame = "\x1b[H\x1b[2J" + self.render_dashboard_text()
        with self.lock:
            if not self._dashboard_started:
                return
            self._out.write(frame)
            self._out.flush()

    def start(self) -> None:
        if self.view != "dashboard":
            return
        with self.lock:
            if self._dashboard_started:
                return
            self._dashboard_started = True
            self._out.write("\x1b[?1049h\x1b[?25l")
            self._out.flush()
        self._draw_dashboard()
        self._stop_event.clear()

        def _loop() -> None:
            while not self._stop_event.wait(self._refresh_interval):
                self._draw_dashboard()

        self._thread = threading.Thread(target=_loop, name="harness-dashboard", daemon=True)
        self._thread.start()

    def stop(self) -> None:
        if self.view != "dashboard":
            return
        self._stop_event.set()
        if self._thread is not None:
            self._thread.join(timeout=1.0)
            self._thread = None
        self._draw_dashboard()
        with self.lock:
            if self._dashboard_started:
                self._dashboard_started = False
                self._out.write("\x1b[?25h\x1b[?1049l")
                self._out.flush()


def setup_tmux_session(
    tmux_bin: str,
    session_name: str,
    log_paths: Sequence[Path],
) -> None:
    """Pre-create worker log files and start a detached tmux session with tiled tail -F panes."""
    if not log_paths:
        return
    for p in log_paths:
        p.parent.mkdir(parents=True, exist_ok=True)
        p.touch(exist_ok=True)

    first_cmd = f"tail -F {shlex.quote(str(log_paths[0]))}"
    proc = subprocess.run(
        [tmux_bin, "new-session", "-d", "-s", session_name, first_cmd],
        capture_output=True,
        text=True,
    )
    if proc.returncode != 0:
        raise RuntimeError(
            f"tmux new-session failed (rc={proc.returncode}): {proc.stderr.strip() or proc.stdout.strip()}"
        )

    for p in log_paths[1:]:
        tail_cmd = f"tail -F {shlex.quote(str(p))}"
        sp = subprocess.run(
            [tmux_bin, "split-window", "-t", session_name, tail_cmd],
            capture_output=True,
            text=True,
        )
        if sp.returncode != 0:
            raise RuntimeError(
                f"tmux split-window failed (rc={sp.returncode}): {sp.stderr.strip() or sp.stdout.strip()}"
            )
        subprocess.run(
            [tmux_bin, "select-layout", "-t", session_name, "tiled"],
            capture_output=True,
            text=True,
        )


def _write_log_line(
    log_fh: Any,
    line: str,
    worker_idx: int,
    monitor: Optional[HarnessMonitor] = None,
) -> None:
    out_line = line if line.endswith("\n") else line + "\n"
    log_fh.write(out_line)
    log_fh.flush()
    if monitor is not None:
        monitor.record_line(worker_idx, out_line)


def _run_streaming_cmd(
    cmd: Sequence[str],
    cwd: Path,
    env: Dict[str, str],
    log_fh: Any,
    worker_idx: int,
    monitor: Optional[HarnessMonitor] = None,
    timeout: Optional[int] = None,
) -> int:
    """Run a subprocess streaming stdout+stderr line-by-line to log_fh and monitor."""
    proc = subprocess.Popen(
        cmd,
        cwd=cwd,
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding="utf-8",
        errors="replace",
        bufsize=1,
    )
    timed_out = threading.Event()
    timer: Optional[threading.Timer] = None
    if timeout is not None and timeout > 0:
        def _kill() -> None:
            timed_out.set()
            try:
                proc.kill()
            except OSError:
                pass

        timer = threading.Timer(timeout, _kill)
        timer.daemon = True
        timer.start()

    try:
        assert proc.stdout is not None
        for raw_line in iter(proc.stdout.readline, ""):
            log_fh.write(raw_line)
            log_fh.flush()
            if monitor is not None:
                monitor.record_line(worker_idx, raw_line)
        proc.stdout.close()
        rc = proc.wait()
    finally:
        if timer is not None:
            timer.cancel()

    if timed_out.is_set():
        raise subprocess.TimeoutExpired(cmd=cmd, timeout=timeout or 0)
    return rc


def _sanitize_slug(text: str, max_len: int = 36) -> str:
    base = Path(text).stem if "/" in text or text.endswith((".cpp", ".c")) else text
    slug = re.sub(r"[^a-zA-Z0-9]+", "-", base).strip("-").lower()
    return (slug or "target")[:max_len]


def _extract_class_hint(mangled: str, demangled: str) -> Optional[str]:
    # Try demangled "Namespace::Class::method(...)" -> "Class"
    head = demangled.split("(", 1)[0].strip()
    if "::" in head:
        parts = [p.strip() for p in head.split("::") if p.strip()]
        if len(parts) >= 2:
            # Take the class component right before method name
            cand = parts[-2]
            cand = cand.split("<", 1)[0].strip()
            if cand and cand != "AbyssEngine":
                return cand
    # Fallback: parse Itanium nested name `_ZN<len><name>...`
    m = re.match(r"^_ZN(?:K)?(\d+)([A-Za-z_]\w*)", mangled)
    if m:
        length = int(m.group(1))
        first = m.group(2)[:length]
        if first == "AbyssEngine":
            rest = mangled[len(m.group(0)) - len(m.group(2)) + length :]
            m2 = re.match(r"^(\d+)([A-Za-z_]\w*)", rest)
            if m2:
                return m2.group(2)[: int(m2.group(1))]
        return first
    return None


def build_tu_index(repo_root: Path) -> Dict[str, str]:
    """Map lowercase stem -> repo-relative source path from tools/source_order.txt or src/."""
    index: Dict[str, str] = {}
    order_file = repo_root / "tools" / "source_order.txt"
    if order_file.is_file():
        for line in order_file.read_text(errors="replace").splitlines():
            s = line.strip()
            if s.startswith("src/"):
                index[Path(s).stem.lower()] = s
    src_dir = repo_root / "src"
    if src_dir.is_dir():
        for p in sorted(src_dir.rglob("*.cpp")):
            rel = p.relative_to(repo_root).as_posix()
            index.setdefault(p.stem.lower(), rel)
    return index


def group_targets_by_tu(
    non_exact: Sequence[Dict[str, Any]],
    repo_root: Path,
    explicit_targets: Optional[Sequence[str]] = None,
    max_tasks: int = 16,
) -> List[TargetGroup]:
    """Group non-exact symbols by TU so parallel workers never collide on the same file."""
    tu_index = build_tu_index(repo_root)
    by_name = {item["name"]: item for item in non_exact}

    selected: List[Dict[str, Any]] = []
    if explicit_targets:
        seen_names: set[str] = set()
        for q in explicit_targets:
            q = q.strip()
            if not q:
                continue
            if q in by_name:
                if q not in seen_names:
                    selected.append(by_name[q])
                    seen_names.add(q)
                continue
            hits = [
                item
                for item in non_exact
                if q in item["name"] or q in item.get("demangled", "")
            ]
            if hits:
                for h in hits:
                    if h["name"] not in seen_names:
                        selected.append(h)
                        seen_names.add(h["name"])
            else:
                if q not in seen_names:
                    selected.append(
                        {
                            "name": q,
                            "demangled": q,
                            "diff_bytes": 0,
                            "orig_size": 0,
                            "size_delta": 0,
                            "linked_exact": False,
                        }
                    )
                    seen_names.add(q)
    else:
        selected = list(non_exact)

    groups: Dict[str, TargetGroup] = {}
    group_order: List[str] = []
    for item in selected:
        cls = _extract_class_hint(item["name"], item.get("demangled", item["name"]))
        if cls and cls.lower() in tu_index:
            tu_key = tu_index[cls.lower()]
        elif cls:
            tu_key = cls
        else:
            tu_key = item["name"]

        if tu_key not in groups:
            groups[tu_key] = TargetGroup(tu_key=tu_key, slug=_sanitize_slug(tu_key))
            group_order.append(tu_key)
        groups[tu_key].symbols.append(item)

    result = [groups[k] for k in group_order]
    if not explicit_targets:
        result.sort(key=lambda g: (-g.total_diff_bytes, g.tu_key))
    return result[:max_tasks]


def render_prompt(
    template_text: str,
    group: TargetGroup,
    branch_name: str,
    build_dir_name: str = "cmake-build-match",
) -> str:
    lines = ["| Symbol (demangled) | Mangled | Orig Size | Diff Bytes | Linked Exact |", "|---|---|---:|---:|:---:|"]
    for s in group.symbols:
        lines.append(
            f"| `{s.get('demangled', s['name'])}` | `{s['name']}` | "
            f"{s.get('orig_size', 0)} | {s.get('diff_bytes', 0)} | "
            f"{'yes' if s.get('linked_exact') else 'no'} |"
        )
    primary = group.symbols[0]["name"]
    sym_args = " ".join(shlex.quote(s["name"]) for s in group.symbols)
    replacements = {
        "{{TARGET_TU}}": group.tu_key,
        "{{BRANCH_NAME}}": branch_name,
        "{{TARGET_SYMBOLS_TABLE}}": "\n".join(lines),
        "{{PRIMARY_SYMBOL}}": primary,
        "{{DIFF_CMD}}": f"python3 tools/validate.py diff --build-dir {shlex.quote(build_dir_name)} {sym_args}",
        "{{BUILD_CMD}}": "cmake --build --preset link",
        "{{CHECK_CMD}}": f"python3 tools/validate.py check --build-dir {shlex.quote(build_dir_name)}",
    }
    out = template_text
    for k, v in replacements.items():
        out = out.replace(k, v)
    return out


def _run_git(args: Sequence[str], cwd: Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True)


def _create_worktree(repo_root: Path, wt_path: Path, branch: str) -> None:
    wt_path.parent.mkdir(parents=True, exist_ok=True)
    with _GIT_LOCK:
        proc = _run_git(["worktree", "add", "-b", branch, str(wt_path), "HEAD"], repo_root)
    if proc.returncode != 0:
        raise RuntimeError(f"git worktree add failed: {proc.stderr.strip()}")
    shared_cache = repo_root / ".cache"
    wt_cache = wt_path / ".cache"
    if shared_cache.is_dir() and not wt_cache.exists():
        os.symlink(shared_cache, wt_cache)


def _cleanup_worktree(
    repo_root: Path, wt_path: Path, branch: str, delete_branch: bool
) -> None:
    with _GIT_LOCK:
        _run_git(["worktree", "remove", "--force", str(wt_path)], repo_root)
        if delete_branch:
            _run_git(["branch", "-D", branch], repo_root)


def _has_repo_changes(wt_path: Path, base_head: str) -> Tuple[bool, bool]:
    """Return (has_uncommitted, has_new_commits)."""
    st = _run_git(["status", "--porcelain"], wt_path)
    has_uncommitted = bool(st.stdout.strip())
    cur = _run_git(["rev-parse", "HEAD"], wt_path)
    has_new_commits = cur.returncode == 0 and cur.stdout.strip() != base_head
    return has_uncommitted, has_new_commits


def resolve_external_paths(
    repo_root: Path,
    build_dir: Path,
    original_arg: Optional[str] = None,
    fmod_libdir_arg: Optional[str] = None,
    env: Optional[Dict[str, str]] = None,
) -> Tuple[Optional[Path], Optional[Path]]:
    """Resolve absolute paths for GOF2_ORIGINAL_SO and GOF2_FMOD_LIBDIR.

    Precedence: explicit CLI flag -> environment variable -> <build_dir>/CMakeCache.txt
    -> existing sibling fallback.
    """
    env_map = os.environ if env is None else env
    cache = validate.parse_cmake_cache(build_dir)

    def _resolve(raw: Optional[str], base: Optional[Path] = None) -> Optional[Path]:
        if not raw or not raw.strip():
            return None
        path = Path(raw.strip()).expanduser()
        if not path.is_absolute() and base is not None:
            path = base / path
        return path.resolve()

    orig_path: Optional[Path] = None
    if original_arg:
        orig_path = _resolve(original_arg)
    elif env_map.get("GOF2_ORIGINAL_SO"):
        orig_path = _resolve(env_map.get("GOF2_ORIGINAL_SO"))
    elif cache.get("GOF2_ORIGINAL_SO"):
        orig_path = _resolve(cache.get("GOF2_ORIGINAL_SO"), base=build_dir)
    else:
        cand = repo_root.parent / "_work" / "bins" / "android_2.0.16_libgof2hdaa.so"
        if cand.is_file():
            orig_path = cand.resolve()

    fmod_path: Optional[Path] = None
    if fmod_libdir_arg:
        fmod_path = _resolve(fmod_libdir_arg)
    elif env_map.get("GOF2_FMOD_LIBDIR"):
        fmod_path = _resolve(env_map.get("GOF2_FMOD_LIBDIR"))
    elif cache.get("GOF2_FMOD_LIBDIR"):
        fmod_path = _resolve(cache.get("GOF2_FMOD_LIBDIR"), base=build_dir)
    elif orig_path is not None:
        cand = orig_path.parent / "apk_libs"
        if cand.is_dir():
            fmod_path = cand.resolve()

    return orig_path, fmod_path


def prebuild_worktree(
    wt_path: Path,
    worker_env: Dict[str, str],
    log_fh: Any,
    cmake_bin: str = "cmake",
    timeout: Optional[int] = None,
    worker_idx: int = 0,
    monitor: Optional[HarnessMonitor] = None,
) -> Optional[str]:
    """Configure and build <wt_path>/cmake-build-match before invoking codex.

    Returns None on success or an error message string on failure.
    """
    configure_cmd = [cmake_bin, "--preset", "default"]
    if worker_env.get("GOF2_ORIGINAL_SO"):
        configure_cmd.append(f"-DGOF2_ORIGINAL_SO={worker_env['GOF2_ORIGINAL_SO']}")
    if worker_env.get("GOF2_FMOD_LIBDIR"):
        configure_cmd.append(f"-DGOF2_FMOD_LIBDIR={worker_env['GOF2_FMOD_LIBDIR']}")

    _write_log_line(
        log_fh,
        f"# --- prebuild configure: {' '.join(shlex.quote(c) for c in configure_cmd)} ---",
        worker_idx,
        monitor,
    )
    cfg_rc = _run_streaming_cmd(
        configure_cmd,
        cwd=wt_path,
        env=worker_env,
        log_fh=log_fh,
        worker_idx=worker_idx,
        monitor=monitor,
        timeout=timeout,
    )
    if cfg_rc != 0:
        return f"prebuild configure failed (rc={cfg_rc})"

    build_cmd = [cmake_bin, "--build", "--preset", "link"]
    _write_log_line(
        log_fh,
        f"# --- prebuild build: {' '.join(shlex.quote(c) for c in build_cmd)} ---",
        worker_idx,
        monitor,
    )
    bld_rc = _run_streaming_cmd(
        build_cmd,
        cwd=wt_path,
        env=worker_env,
        log_fh=log_fh,
        worker_idx=worker_idx,
        monitor=monitor,
        timeout=timeout,
    )
    if bld_rc != 0:
        return f"prebuild build failed (rc={bld_rc})"
    return None


def run_worker(
    idx: int,
    run_id: str,
    group: TargetGroup,
    args: argparse.Namespace,
    repo_root: Path,
    base_head: str,
    log_dir: Path,
    template_text: str,
    shared_env: Dict[str, str],
    monitor: Optional[HarnessMonitor] = None,
) -> Dict[str, Any]:
    branch = f"decomp/{run_id}-w{idx}-{group.slug}"
    wt_root = Path(args.worktree_root)
    if not wt_root.is_absolute():
        wt_root = repo_root / wt_root
    wt_path = wt_root / f"{run_id}-w{idx}"
    log_path = log_dir / f"w{idx}.log"
    prompt_path = log_dir / f"w{idx}.prompt.md"

    build_dir_name = "cmake-build-match"
    wt_build_dir = wt_path / build_dir_name
    prompt_text = render_prompt(template_text, group, branch, build_dir_name)
    prompt_path.write_text(prompt_text)

    before_diff = group.total_diff_bytes
    result: Dict[str, Any] = {
        "worker": idx,
        "tu": group.tu_key,
        "symbols": [s["name"] for s in group.symbols],
        "branch": branch,
        "worktree": str(wt_path),
        "log": str(log_path),
        "prompt": str(prompt_path),
        "before_diff_bytes": before_diff,
        "after_diff_bytes": before_diff,
        "diff_delta": 0,
        "status": "error",
        "kept_branch": False,
        "message": "",
    }

    wt_created = False
    try:
        if monitor is not None:
            monitor.set_phase(idx, "worktree")
        _create_worktree(repo_root, wt_path, branch)
        wt_created = True

        worker_env = dict(shared_env)
        worker_env["GOF2_BUILD_DIR"] = str(wt_build_dir)
        worker_env["PYTHONUNBUFFERED"] = "1"

        if args.dry_run:
            if monitor is not None:
                monitor.set_phase(idx, "dry-run")
            with log_path.open("a", encoding="utf-8") as log_fh:
                for line in (
                    f"[dry-run] worker={idx} branch={branch} tu={group.tu_key}",
                    f"[dry-run] symbols={', '.join(result['symbols'])}",
                    f"[dry-run] worktree={wt_path}",
                    f"[dry-run] build_dir={wt_build_dir}",
                    f"[dry-run] GOF2_ORIGINAL_SO={worker_env.get('GOF2_ORIGINAL_SO', '')}",
                    f"[dry-run] GOF2_FMOD_LIBDIR={worker_env.get('GOF2_FMOD_LIBDIR', '')}",
                ):
                    _write_log_line(log_fh, line, idx, monitor)
            result["status"] = "dry-run"
            result["message"] = "worktree created and prompt rendered (codex skipped)"
            return result

        cmd = [args.codex_bin, "exec"]
        if args.model:
            cmd.extend(["--model", args.model])
        if args.profile:
            cmd.extend(["--profile", args.profile])
        if args.codex_args:
            cmd.extend(shlex.split(args.codex_args))
        cmd.append(prompt_text)

        with log_path.open("a", encoding="utf-8") as log_fh:
            _write_log_line(
                log_fh,
                f"# worker {idx} | tu={group.tu_key} | branch={branch}",
                idx,
                monitor,
            )

            if not args.skip_prebuild:
                if monitor is not None:
                    monitor.set_phase(idx, "prebuild")
                prebuild_err = prebuild_worktree(
                    wt_path=wt_path,
                    worker_env=worker_env,
                    log_fh=log_fh,
                    cmake_bin=args.cmake_bin,
                    timeout=args.timeout,
                    worker_idx=idx,
                    monitor=monitor,
                )
                if prebuild_err is not None:
                    result["status"] = "error"
                    result["message"] = prebuild_err
                    return result

            if monitor is not None:
                monitor.set_phase(idx, "codex")
            _write_log_line(log_fh, "# --- codex exec ---", idx, monitor)
            codex_rc = _run_streaming_cmd(
                cmd,
                cwd=wt_path,
                env=worker_env,
                log_fh=log_fh,
                worker_idx=idx,
                monitor=monitor,
                timeout=args.timeout,
            )

        if codex_rc != 0:
            result["status"] = "error"
            result["message"] = f"codex exited with code {codex_rc}"
            return result

        has_uncommitted, has_new_commits = _has_repo_changes(wt_path, base_head)
        if not has_uncommitted and not has_new_commits:
            result["status"] = "unchanged"
            result["message"] = "no file edits or commits produced"
            return result

        # Run validate.py check inside the worktree against its own cmake-build-match
        if monitor is not None:
            monitor.set_phase(idx, "validate")
        wt_report = log_dir / f"w{idx}.report.json"
        check_cmd = [
            sys.executable,
            str(wt_path / "tools" / "validate.py"),
            "check",
            "--build-dir",
            str(wt_build_dir),
            "--repo-root",
            str(wt_path),
            "--json",
            str(wt_report),
        ]
        orig_so = worker_env.get("GOF2_ORIGINAL_SO")
        if orig_so:
            check_cmd.extend(["--original", orig_so])
        if args.skip_layout:
            check_cmd.append("--skip-layout")
        if args.no_freshness:
            check_cmd.append("--no-freshness")

        with log_path.open("a", encoding="utf-8") as log_fh:
            _write_log_line(log_fh, "# --- post-worker validate.py check ---", idx, monitor)
            chk_rc = _run_streaming_cmd(
                check_cmd,
                cwd=wt_path,
                env=worker_env,
                log_fh=log_fh,
                worker_idx=idx,
                monitor=monitor,
            )

        if chk_rc != 0:
            result["status"] = "regressed"
            result["message"] = f"validate.py check failed (rc={chk_rc})"
            return result

        after_diff = before_diff
        if wt_report.is_file():
            rep = json.loads(wt_report.read_text())
            ne_map = {r["name"]: int(r["diff_bytes"]) for r in rep.get("non_exact", [])}
            after_diff = sum(ne_map.get(s, 0) for s in result["symbols"])

        result["after_diff_bytes"] = after_diff
        result["diff_delta"] = after_diff - before_diff

        if after_diff == 0 and (before_diff > 0 or has_uncommitted or has_new_commits):
            result["status"] = "matched"
            result["message"] = f"matched target ({before_diff}B -> 0B)"
        elif after_diff < before_diff:
            result["status"] = "improved"
            result["message"] = f"improved ({before_diff}B -> {after_diff}B)"
        else:
            result["status"] = "unchanged"
            result["message"] = f"check passed but target diff unchanged ({after_diff}B)"
            return result

        # Preserve improvements on the worker branch before cleanup
        if has_uncommitted:
            _run_git(["add", "-A"], wt_path)
            _run_git(
                [
                    "commit",
                    "-m",
                    f"{group.tu_key}: {result['status']} ({before_diff}B -> {after_diff}B)",
                ],
                wt_path,
            )
        result["kept_branch"] = True
        return result

    except subprocess.TimeoutExpired:
        result["status"] = "error"
        result["message"] = f"worker timed out after {args.timeout}s"
        return result
    except Exception as exc:
        result["status"] = "error"
        result["message"] = str(exc)
        return result
    finally:
        if monitor is not None:
            monitor.finish_worker(
                idx,
                status=result["status"],
                delta=result["diff_delta"],
                message=result["message"],
            )
        if wt_created and not args.keep_worktrees:
            delete_branch = not result["kept_branch"]
            _cleanup_worktree(repo_root, wt_path, branch, delete_branch=delete_branch)


def load_non_exact_from_status(
    args: argparse.Namespace,
    repo_root: Path,
    log_dir: Path,
    orig_path: Optional[Path] = None,
) -> List[Dict[str, Any]]:
    if args.report:
        rep = json.loads(Path(args.report).read_text())
        return list(rep.get("non_exact", []))

    build_dir = validate.resolve_build_dir(args.build_dir, repo_root)
    report_path = log_dir / "initial_status.json"
    cmd = [
        sys.executable,
        str(SCRIPT_DIR / "validate.py"),
        "status",
        "--build-dir",
        str(build_dir),
        "--json",
        str(report_path),
        "--no-disasm",
    ]
    resolved_orig = orig_path or (Path(args.original).expanduser().resolve() if args.original else None)
    if resolved_orig:
        cmd.extend(["--original", str(resolved_orig)])
    proc = subprocess.run(cmd, cwd=repo_root, capture_output=True, text=True)
    if proc.returncode != 0:
        raise RuntimeError(
            f"validate.py status failed (rc={proc.returncode}): {proc.stderr.strip() or proc.stdout.strip()}"
        )
    rep = json.loads(report_path.read_text())
    return list(rep.get("non_exact", []))


def print_summary(results: Sequence[Dict[str, Any]], summary_path: Path) -> None:
    print("\n=== Harness Run Summary ===")
    print(f"  {'W':>2}  {'STATUS':<10}  {'DELTA':>8}  {'TU':<34}  BRANCH")
    for r in sorted(results, key=lambda x: x["worker"]):
        delta = f"{r['diff_delta']:+d}B"
        branch_col = r["branch"] if r["kept_branch"] else f"({r['branch']})"
        print(
            f"  {r['worker']:>2}  {r['status']:<10}  {delta:>8}  "
            f"{r['tu'][:34]:<34}  {branch_col}"
        )
        if r["message"]:
            print(f"      -> {r['message']}")
    print(f"\nSummary JSON: {summary_path}")


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="harness.py",
        description="Run parallel Codex agents in isolated git worktrees on non-matching functions.",
    )
    p.add_argument("--workers", type=int, default=4, help="Max parallel workers (default: 4)")
    p.add_argument("--max-tasks", type=int, default=16, help="Max TU tasks to schedule (default: 16)")
    p.add_argument("--targets", help="Comma-separated explicit target symbol names or substrings")
    p.add_argument("--model", help="Codex model override (--model)")
    p.add_argument("--profile", help="Codex profile override (--profile)")
    p.add_argument("--codex-bin", default="codex", help="Codex CLI binary (default: codex)")
    p.add_argument("--codex-args", help="Additional quoted flags passed to `codex exec`")
    p.add_argument("--build-dir", help="Main CMake build directory for status & CMakeCache.txt discovery (default: auto-discovered)")
    p.add_argument("--original", help="Path to original libgof2hdaa.so")
    p.add_argument("--fmod-libdir", help="Directory holding libfmodex.so/libfmodevent.so (default: auto-discovered)")
    p.add_argument("--cmake-bin", default="cmake", help="CMake CLI binary for worktree prebuild (default: cmake)")
    p.add_argument("--report", help="Existing validate.py status JSON report (skip initial status run)")
    p.add_argument("--worktree-root", default=".harness/wt", help="Worktree parent directory (default: .harness/wt)")
    p.add_argument("--repo-root", help="Repository root (default: auto)")
    p.add_argument("--timeout", type=int, default=1800, help="Per-worker timeout in seconds (default: 1800)")
    p.add_argument(
        "--view",
        choices=("auto", "dashboard", "stream", "tmux", "quiet"),
        default="auto",
        help="Live worker output mode (default: auto -> dashboard on TTY, stream otherwise)",
    )
    p.add_argument("--dry-run", action="store_true", help="Create worktrees & render prompts without running prebuild or codex")
    p.add_argument("--skip-prebuild", action="store_true", default=False, help="Skip automatic cmake configure + build in fresh worktrees")
    p.add_argument("--keep-worktrees", action="store_true", help="Do not delete worktrees after workers finish")
    p.add_argument("--skip-layout", action="store_true", help="Pass --skip-layout to post-worker validate.py check")
    p.add_argument("--no-freshness", action="store_true", help="Pass --no-freshness to post-worker validate.py check")
    return p


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)

    repo_root = Path(args.repo_root).resolve() if args.repo_root else REPO_ROOT
    run_id = dt.datetime.now(dt.timezone.utc).strftime("%Y%m%d-%H%M%S")
    log_dir = repo_root / ".harness" / "logs" / run_id
    log_dir.mkdir(parents=True, exist_ok=True)

    head_proc = _run_git(["rev-parse", "HEAD"], repo_root)
    if head_proc.returncode != 0:
        print(f"error: not a git repository: {repo_root}", file=sys.stderr)
        return 2
    base_head = head_proc.stdout.strip()

    # Resolve GOF2_ORIGINAL_SO / GOF2_FMOD_LIBDIR as absolute paths from CLI, env, or main CMakeCache.txt
    build_dir = validate.resolve_build_dir(args.build_dir, repo_root)
    shared_env = dict(os.environ)
    orig_path, fmod_path = resolve_external_paths(
        repo_root=repo_root,
        build_dir=build_dir,
        original_arg=args.original,
        fmod_libdir_arg=args.fmod_libdir,
        env=shared_env,
    )
    if orig_path is not None:
        shared_env["GOF2_ORIGINAL_SO"] = str(orig_path)
    if fmod_path is not None:
        shared_env["GOF2_FMOD_LIBDIR"] = str(fmod_path)

    explicit_targets = [t.strip() for t in args.targets.split(",") if t.strip()] if args.targets else None

    if explicit_targets and not args.report and args.dry_run:
        non_exact: List[Dict[str, Any]] = []
    else:
        try:
            non_exact = load_non_exact_from_status(args, repo_root, log_dir, orig_path=orig_path)
        except Exception as exc:
            if explicit_targets:
                non_exact = []
            else:
                print(f"error: {exc}", file=sys.stderr)
                return 2

    groups = group_targets_by_tu(
        non_exact=non_exact,
        repo_root=repo_root,
        explicit_targets=explicit_targets,
        max_tasks=args.max_tasks,
    )
    if not groups:
        print("No target functions to process.")
        return 0

    stdout_is_tty = bool(getattr(sys.stdout, "isatty", lambda: False)())
    stdin_is_tty = bool(getattr(sys.stdin, "isatty", lambda: False)())
    view = resolve_view_mode(args.view, stdout_is_tty)

    tmux_bin: Optional[str] = None
    tmux_session: Optional[str] = None
    if view == "tmux":
        tmux_bin = shutil.which("tmux")
        if not tmux_bin:
            print("error: --view tmux requires 'tmux' in PATH", file=sys.stderr)
            return 2
        tmux_session = f"gof2-{run_id}"
        log_paths = [log_dir / f"w{idx}.log" for idx in range(1, len(groups) + 1)]
        try:
            setup_tmux_session(tmux_bin, tmux_session, log_paths)
        except Exception as exc:
            print(f"error: {exc}", file=sys.stderr)
            return 2

    template_text = PROMPT_TEMPLATE.read_text(encoding="utf-8")
    monitor = HarnessMonitor(
        run_id=run_id,
        groups=groups,
        log_dir=log_dir,
        view=view,
        out=sys.stdout,
        is_tty=stdout_is_tty,
    )

    if view != "dashboard":
        print(
            f"Starting harness run {run_id}: {len(groups)} TU task(s), "
            f"up to {args.workers} parallel worker(s){' [DRY-RUN]' if args.dry_run else ''}"
        )
        if view == "tmux" and tmux_session:
            if not (stdout_is_tty and stdin_is_tty and not os.environ.get("TMUX")):
                print(f"tmux attach -t {tmux_session}")

    results: List[Dict[str, Any]] = []
    interrupted = False
    monitor.start()
    pool = concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.workers))
    try:
        futs = [
            pool.submit(
                run_worker,
                idx,
                run_id,
                group,
                args,
                repo_root,
                base_head,
                log_dir,
                template_text,
                shared_env,
                monitor,
            )
            for idx, group in enumerate(groups, 1)
        ]
        if (
            view == "tmux"
            and tmux_bin
            and tmux_session
            and stdout_is_tty
            and stdin_is_tty
            and not os.environ.get("TMUX")
        ):
            subprocess.run([tmux_bin, "attach", "-t", tmux_session])
        for fut in concurrent.futures.as_completed(futs):
            results.append(fut.result())
    except KeyboardInterrupt:
        interrupted = True
    finally:
        pool.shutdown(wait=not interrupted, cancel_futures=interrupted)
        monitor.stop()

    results.sort(key=lambda r: r["worker"])
    summary = {
        "run_id": run_id,
        "base_head": base_head,
        "dry_run": bool(args.dry_run),
        "view": view,
        "workers": args.workers,
        "tasks": len(results),
        "counts": {
            st: sum(1 for r in results if r["status"] == st)
            for st in ("matched", "improved", "unchanged", "regressed", "error", "dry-run")
        },
        "results": results,
    }
    summary_path = log_dir / "summary.json"
    summary_path.write_text(json.dumps(summary, indent=2) + "\n")
    if interrupted:
        print("\nInterrupted by user (Ctrl-C).", file=sys.stderr)
    print_summary(results, summary_path)

    if interrupted:
        return 130
    has_errors = any(r["status"] in ("error", "regressed") for r in results)
    return 1 if has_errors else 0


if __name__ == "__main__":
    sys.exit(main())
