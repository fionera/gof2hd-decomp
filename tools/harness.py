#!/usr/bin/env python3
"""Parallel Codex decompilation harness for GOF2HD.

Spawns N isolated git-worktree workers targeting non-overlapping TUs/symbols,
runs `codex exec`, validates each worker with `tools/validate.py check`,
keeps improved branches, and cleans up unimproved worktrees automatically.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import datetime as dt
import json
import os
import re
import shlex
import subprocess
import sys
import threading
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


@dataclass
class TargetGroup:
    tu_key: str
    slug: str
    symbols: List[Dict[str, Any]] = field(default_factory=list)

    @property
    def total_diff_bytes(self) -> int:
        return sum(int(s.get("diff_bytes", 0)) for s in self.symbols)


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
) -> Optional[str]:
    """Configure and build <wt_path>/cmake-build-match before invoking codex.

    Returns None on success or an error message string on failure.
    """
    configure_cmd = [cmake_bin, "--preset", "default"]
    if worker_env.get("GOF2_ORIGINAL_SO"):
        configure_cmd.append(f"-DGOF2_ORIGINAL_SO={worker_env['GOF2_ORIGINAL_SO']}")
    if worker_env.get("GOF2_FMOD_LIBDIR"):
        configure_cmd.append(f"-DGOF2_FMOD_LIBDIR={worker_env['GOF2_FMOD_LIBDIR']}")

    log_fh.write(f"\n# --- prebuild configure: {' '.join(shlex.quote(c) for c in configure_cmd)} ---\n")
    log_fh.flush()
    cfg = subprocess.run(
        configure_cmd,
        cwd=wt_path,
        env=worker_env,
        stdout=log_fh,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=timeout,
    )
    if cfg.returncode != 0:
        return f"prebuild configure failed (rc={cfg.returncode})"

    build_cmd = [cmake_bin, "--build", "--preset", "link"]
    log_fh.write(f"\n# --- prebuild build: {' '.join(shlex.quote(c) for c in build_cmd)} ---\n")
    log_fh.flush()
    bld = subprocess.run(
        build_cmd,
        cwd=wt_path,
        env=worker_env,
        stdout=log_fh,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=timeout,
    )
    if bld.returncode != 0:
        return f"prebuild build failed (rc={bld.returncode})"
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
        _create_worktree(repo_root, wt_path, branch)
        wt_created = True

        worker_env = dict(shared_env)
        worker_env["GOF2_BUILD_DIR"] = str(wt_build_dir)

        if args.dry_run:
            log_path.write_text(
                f"[dry-run] worker={idx} branch={branch} tu={group.tu_key}\n"
                f"[dry-run] symbols={', '.join(result['symbols'])}\n"
                f"[dry-run] worktree={wt_path}\n"
                f"[dry-run] build_dir={wt_build_dir}\n"
                f"[dry-run] GOF2_ORIGINAL_SO={worker_env.get('GOF2_ORIGINAL_SO', '')}\n"
                f"[dry-run] GOF2_FMOD_LIBDIR={worker_env.get('GOF2_FMOD_LIBDIR', '')}\n"
            )
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

        with log_path.open("w", encoding="utf-8") as log_fh:
            log_fh.write(f"# worker {idx} | tu={group.tu_key} | branch={branch}\n")
            log_fh.flush()

            if not args.skip_prebuild:
                prebuild_err = prebuild_worktree(
                    wt_path=wt_path,
                    worker_env=worker_env,
                    log_fh=log_fh,
                    cmake_bin=args.cmake_bin,
                    timeout=args.timeout,
                )
                if prebuild_err is not None:
                    result["status"] = "error"
                    result["message"] = prebuild_err
                    return result

            log_fh.write("\n# --- codex exec ---\n")
            log_fh.flush()
            proc = subprocess.run(
                cmd,
                cwd=wt_path,
                env=worker_env,
                stdout=log_fh,
                stderr=subprocess.STDOUT,
                text=True,
                timeout=args.timeout,
            )

        if proc.returncode != 0:
            result["status"] = "error"
            result["message"] = f"codex exited with code {proc.returncode}"
            return result

        has_uncommitted, has_new_commits = _has_repo_changes(wt_path, base_head)
        if not has_uncommitted and not has_new_commits:
            result["status"] = "unchanged"
            result["message"] = "no file edits or commits produced"
            return result

        # Run validate.py check inside the worktree against its own cmake-build-match
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
            log_fh.write("\n# --- post-worker validate.py check ---\n")
            log_fh.flush()
            chk = subprocess.run(
                check_cmd,
                cwd=wt_path,
                env=worker_env,
                stdout=log_fh,
                stderr=subprocess.STDOUT,
                text=True,
            )

        if chk.returncode != 0:
            result["status"] = "regressed"
            result["message"] = f"validate.py check failed (rc={chk.returncode})"
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

    template_text = PROMPT_TEMPLATE.read_text(encoding="utf-8")

    print(
        f"Starting harness run {run_id}: {len(groups)} TU task(s), "
        f"up to {args.workers} parallel worker(s){' [DRY-RUN]' if args.dry_run else ''}"
    )

    results: List[Dict[str, Any]] = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.workers)) as pool:
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
            )
            for idx, group in enumerate(groups, 1)
        ]
        for fut in concurrent.futures.as_completed(futs):
            results.append(fut.result())

    results.sort(key=lambda r: r["worker"])
    summary = {
        "run_id": run_id,
        "base_head": base_head,
        "dry_run": bool(args.dry_run),
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
    print_summary(results, summary_path)

    has_errors = any(r["status"] in ("error", "regressed") for r in results)
    return 1 if has_errors else 0


if __name__ == "__main__":
    sys.exit(main())
