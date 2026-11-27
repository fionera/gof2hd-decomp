#!/usr/bin/env python3
"""Unit and integration tests for tools/elf32.py, tools/validate.py, and tools/harness.py."""

from __future__ import annotations

import json
import os
import stat
import struct
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from typing import Dict, List, Tuple

REPO_ROOT = Path(__file__).resolve().parent.parent
TOOLS_DIR = REPO_ROOT / "tools"
if str(TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(TOOLS_DIR))

import elf32  # noqa: E402
import harness  # noqa: E402
import validate  # noqa: E402


def build_synthetic_arm32_elf(
    functions: List[Tuple[str, bytes, bool]],
    objects: List[Tuple[str, bytes]],
    imports: List[str],
    extra_rodata: bytes = b"",
) -> bytes:
    """Construct a minimal, valid little-endian ARM32 ELF32 shared object.

    functions: [(name, code_bytes, is_thumb)]
    objects:   [(name, data_bytes)]
    imports:   [name]
    """
    # Section layout:
    # 0: SHT_NULL
    # 1: .text     (SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR)
    # 2: .rodata   (SHT_PROGBITS, SHF_ALLOC)
    # 3: .dynstr   (SHT_STRTAB, SHF_ALLOC)
    # 4: .dynsym   (SHT_DYNSYM, SHF_ALLOC, sh_link=3, sh_entsize=16)
    # 5: .shstrtab (SHT_STRTAB)
    text_addr = 0x1000
    text_buf = bytearray()
    fn_entries: List[Tuple[str, int, int]] = []
    for name, code, is_thumb in functions:
        if len(text_buf) % 4:
            text_buf.extend(b"\x00" * (4 - (len(text_buf) % 4)))
        addr = text_addr + len(text_buf)
        val = (addr | 1) if is_thumb else addr
        text_buf.extend(code)
        fn_entries.append((name, val, len(code)))

    rodata_addr = 0x4000
    rodata_buf = bytearray()
    obj_entries: List[Tuple[str, int, int]] = []
    for name, data in objects:
        if len(rodata_buf) % 4:
            rodata_buf.extend(b"\x00" * (4 - (len(rodata_buf) % 4)))
        addr = rodata_addr + len(rodata_buf)
        rodata_buf.extend(data)
        obj_entries.append((name, addr, len(data)))
    rodata_buf.extend(extra_rodata)

    dynstr = bytearray(b"\x00")

    def add_dynstr(s: str) -> int:
        off = len(dynstr)
        dynstr.extend(s.encode("utf-8") + b"\x00")
        return off

    dynsym = bytearray(struct.pack("<IIIBBH", 0, 0, 0, 0, 0, 0))  # STN_UNDEF
    for name, val, size in fn_entries:
        st_name = add_dynstr(name)
        st_info = (elf32.STB_GLOBAL << 4) | elf32.STT_FUNC
        dynsym.extend(struct.pack("<IIIBBH", st_name, val, size, st_info, 0, 1))
    for name, addr, size in obj_entries:
        st_name = add_dynstr(name)
        st_info = (elf32.STB_GLOBAL << 4) | elf32.STT_OBJECT
        dynsym.extend(struct.pack("<IIIBBH", st_name, addr, size, st_info, 0, 2))
    for name in imports:
        st_name = add_dynstr(name)
        st_info = (elf32.STB_GLOBAL << 4) | elf32.STT_FUNC
        dynsym.extend(struct.pack("<IIIBBH", st_name, 0, 0, st_info, 0, elf32.SHN_UNDEF))

    shstrtab = bytearray(b"\x00")
    sh_name_offs: Dict[str, int] = {}
    for sname in (".text", ".rodata", ".dynstr", ".dynsym", ".shstrtab"):
        sh_name_offs[sname] = len(shstrtab)
        shstrtab.extend(sname.encode("ascii") + b"\x00")

    # Assemble file payload after 52-byte ELF32 header
    ehdr_size = 52
    body = bytearray()

    def place(blob: bytes) -> Tuple[int, int]:
        while (ehdr_size + len(body)) % 4:
            body.append(0)
        off = ehdr_size + len(body)
        body.extend(blob)
        return off, len(blob)

    text_off, text_sz = place(bytes(text_buf))
    rodata_off, rodata_sz = place(bytes(rodata_buf))
    dynstr_off, dynstr_sz = place(bytes(dynstr))
    dynsym_off, dynsym_sz = place(bytes(dynsym))
    shstr_off, shstr_sz = place(bytes(shstrtab))

    while (ehdr_size + len(body)) % 4:
        body.append(0)
    shoff = ehdr_size + len(body)

    shdrs = [
        # 0: NULL
        struct.pack("<IIIIIIIIII", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        # 1: .text
        struct.pack(
            "<IIIIIIIIII",
            sh_name_offs[".text"], elf32.SHT_PROGBITS, elf32.SHF_ALLOC | 0x4,
            text_addr, text_off, text_sz, 0, 0, 4, 0,
        ),
        # 2: .rodata
        struct.pack(
            "<IIIIIIIIII",
            sh_name_offs[".rodata"], elf32.SHT_PROGBITS, elf32.SHF_ALLOC,
            rodata_addr, rodata_off, rodata_sz, 0, 0, 4, 0,
        ),
        # 3: .dynstr
        struct.pack(
            "<IIIIIIIIII",
            sh_name_offs[".dynstr"], elf32.SHT_STRTAB, elf32.SHF_ALLOC,
            0x8000, dynstr_off, dynstr_sz, 0, 0, 1, 0,
        ),
        # 4: .dynsym
        struct.pack(
            "<IIIIIIIIII",
            sh_name_offs[".dynsym"], elf32.SHT_DYNSYM, elf32.SHF_ALLOC,
            0x9000, dynsym_off, dynsym_sz, 3, 1, 4, 16,
        ),
        # 5: .shstrtab
        struct.pack(
            "<IIIIIIIIII",
            sh_name_offs[".shstrtab"], elf32.SHT_STRTAB, 0,
            0, shstr_off, shstr_sz, 0, 0, 1, 0,
        ),
    ]

    e_ident = b"\x7fELF" + bytes([1, 1, 1, 0]) + (b"\x00" * 8)
    ehdr = e_ident + struct.pack(
        "<HHIIIIIHHHHHH",
        3,          # ET_DYN
        40,         # EM_ARM
        1,          # EV_CURRENT
        0,          # e_entry
        0,          # e_phoff
        shoff,      # e_shoff
        0x05000000, # e_flags
        ehdr_size,  # e_ehsize
        0,          # e_phentsize
        0,          # e_phnum
        40,         # e_shentsize
        len(shdrs), # e_shnum
        5,          # e_shstrndx
    )
    return ehdr + bytes(body) + b"".join(shdrs)


class TestValidateElf(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.build_dir = self.root / "cmake-build-match"
        self.build_dir.mkdir()
        (self.root / "src").mkdir()
        (self.root / "tools").mkdir()
        (self.root / "CMakeLists.txt").write_text("# clean\n")
        (self.root / "tools" / "source_order.txt").write_text("src/Foo.cpp\n")
        (self.root / "src" / "Foo.cpp").write_text("int ok() { return 1; }\n")

        self.orig_so = self.root / "orig.so"
        self.ours_so = self.build_dir / "libgof2hdaa.so"

        self.orig_so.write_bytes(
            build_synthetic_arm32_elf(
                functions=[
                    ("_ZN3Foo5matchEv", b"\x00\xbf\x70\x47", True),
                    ("_ZN3Foo4diffEv", b"\x01\x20\x70\x47\x00\xbf", True),
                ],
                objects=[("g_table", b"\x01\x02\x03\x04")],
                imports=["malloc", "free"],
            )
        )
        self.ours_so.write_bytes(
            build_synthetic_arm32_elf(
                functions=[
                    ("_ZN3Foo5matchEv", b"\x00\xbf\x70\x47", True),
                    ("_ZN3Foo4diffEv", b"\x02\x20\x70\x47\x00\xbf", True),
                ],
                objects=[("g_table", b"\x01\x02\x03\x04")],
                imports=["malloc", "free"],
            )
        )
        # Ensure built .so is newer than source files
        future = time.time() + 5
        os.utime(self.ours_so, (future, future))

        self.baseline = self.root / "tools" / "baseline.json"
        self.baseline.write_text(
            json.dumps(
                {
                    "functions": {
                        "compared": 2,
                        "byte_exact": 1,
                        "linked_exact": 1,
                        "stub_zero_size": 0,
                        "missing": 0,
                        "wrong_type": 0,
                        "extra": 0,
                    },
                    "data": {"exact": 1, "common": 1},
                    "allowed_extra": [],
                    "imports": ["free", "malloc"],
                }
            )
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_status_never_invokes_objdump_for_byte_exact(self) -> None:
        spy_log = self.root / "objdump_calls.log"
        spy_bin = self.root / "fake_objdump.py"
        spy_bin.write_text(
            "#!/usr/bin/env python3\n"
            "import sys\n"
            f"with open({str(spy_log)!r}, 'a') as f:\n"
            "    f.write(' '.join(sys.argv[1:]) + '\\n')\n"
            "print('  1004: 2001 movs r0, #1')\n"
        )
        spy_bin.chmod(spy_bin.stat().st_mode | stat.S_IXUSR)

        report_path = self.build_dir / "report.json"
        rc = validate.main(
            [
                "status",
                "--original", str(self.orig_so),
                "--ours", str(self.ours_so),
                "--build-dir", str(self.build_dir),
                "--objdump", str(spy_bin),
                "--json", str(report_path),
            ]
        )
        self.assertEqual(rc, 0)
        self.assertTrue(report_path.is_file())
        self.assertFalse((self.root / "report.json").exists())

        rep = json.loads(report_path.read_text())
        self.assertEqual(rep["functions"]["byte_exact"], 1)
        self.assertEqual(rep["functions"]["compared"], 2)
        self.assertEqual(len(rep["non_exact"]), 1)
        self.assertEqual(rep["non_exact"][0]["name"], "_ZN3Foo4diffEv")
        self.assertEqual(rep["non_exact"][0]["diff_bytes"], 1)

        # Spy objdump must ONLY be called for the non-exact function at 0x1004,
        # NEVER for the byte-exact function at 0x1000!
        calls = spy_log.read_text().splitlines()
        self.assertEqual(len(calls), 2)  # orig + ours for _ZN3Foo4diffEv only
        for line in calls:
            self.assertIn("--start-address=0x1004", line)
            self.assertNotIn("--start-address=0x1000", line)

    def test_diff_hex_fallback_and_substring_match(self) -> None:
        rc = validate.main(
            [
                "diff",
                "--original", str(self.orig_so),
                "--ours", str(self.ours_so),
                "--hex-only",
                "4diff",
            ]
        )
        self.assertEqual(rc, 0)

    def test_check_pass_regression_and_ratchet(self) -> None:
        # 1. Clean pass
        rc = validate.main(
            [
                "check",
                "--original", str(self.orig_so),
                "--ours", str(self.ours_so),
                "--build-dir", str(self.build_dir),
                "--baseline", str(self.baseline),
                "--repo-root", str(self.root),
                "--skip-layout",
                "--no-disasm",
            ]
        )
        self.assertEqual(rc, 0)

        # 2. Regression: extra unexpected import fails check and refuses --update-baseline
        self.ours_so.write_bytes(
            build_synthetic_arm32_elf(
                functions=[
                    ("_ZN3Foo5matchEv", b"\x00\xbf\x70\x47", True),
                    ("_ZN3Foo4diffEv", b"\x02\x20\x70\x47\x00\xbf", True),
                ],
                objects=[("g_table", b"\x01\x02\x03\x04")],
                imports=["malloc", "free", "bogus_typo_sym"],
            )
        )
        future = time.time() + 10
        os.utime(self.ours_so, (future, future))
        rc_reg = validate.main(
            [
                "check",
                "--original", str(self.orig_so),
                "--ours", str(self.ours_so),
                "--build-dir", str(self.build_dir),
                "--baseline", str(self.baseline),
                "--repo-root", str(self.root),
                "--skip-layout",
                "--no-disasm",
                "--update-baseline",
            ]
        )
        self.assertEqual(rc_reg, 1)
        # Baseline must remain unchanged after failed check
        self.assertEqual(json.loads(self.baseline.read_text())["functions"]["byte_exact"], 1)

        # 3. Improvement: both functions now match -> --update-baseline ratchets to 2
        self.ours_so.write_bytes(self.orig_so.read_bytes())
        os.utime(self.ours_so, (future + 5, future + 5))
        rc_up = validate.main(
            [
                "check",
                "--original", str(self.orig_so),
                "--ours", str(self.ours_so),
                "--build-dir", str(self.build_dir),
                "--baseline", str(self.baseline),
                "--repo-root", str(self.root),
                "--skip-layout",
                "--no-disasm",
                "--update-baseline",
            ]
        )
        self.assertEqual(rc_up, 0)
        updated = json.loads(self.baseline.read_text())
        self.assertEqual(updated["functions"]["byte_exact"], 2)
        self.assertEqual(updated["functions"]["linked_exact"], 2)


class TestLint(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.src = self.root / "src"
        self.src.mkdir()
        self.cmake = self.root / "CMakeLists.txt"

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_clean_authentic_code_passes(self) -> None:
        (self.src / "Good.cpp").write_text(
            '// comment mentioning asm volatile("nop") and goto *tbl;\n'
            'const char* s = "__asm__ union extern \\"C\\"";\n'
            'extern "C" void Java_ok() {} // lint: extern_c\n'
            "int add(int a, int b) { return (a && b) ? a + b : 0; }\n"
        )
        self.cmake.write_text("add_library(gof2 OBJECT src/Good.cpp)\n")
        hard, audit = validate.run_lint(self.src, self.cmake)
        self.assertEqual(hard, [])
        self.assertEqual(len(audit["cmake_per_file_flags"]), 0)

    def test_banned_patterns_trigger_hard_errors(self) -> None:
        (self.src / "Bad.cpp").write_text(
            'void f() { asm volatile("nop"); }\n'
            "void g(int x) { void* tbl[] = {&&L1}; goto *tbl[x]; L1: return; }\n"
            'extern "C" void sneaky();\n'
            "union U { int a; float b; };\n"
            "int h(void* p) { return *((char*)(void*)p); }\n"
        )
        self.cmake.write_text(
            'set_source_files_properties(src/Bad.cpp PROPERTIES COMPILE_OPTIONS "-mllvm;-foo")\n'
        )
        hard, audit = validate.run_lint(self.src, self.cmake)
        rules = {e["rule"] for e in hard}
        self.assertIn("inline_asm", rules)
        self.assertIn("computed_goto", rules)
        self.assertIn("extern_c", rules)
        self.assertIn("union_decl", rules)
        self.assertIn("void_ptr_arith", rules)
        self.assertEqual(len(audit["cmake_per_file_flags"]), 1)


class TestHarness(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.repo = Path(self.tmp.name)
        subprocess.run(["git", "init", "-b", "master"], cwd=self.repo, check=True, capture_output=True)
        subprocess.run(["git", "config", "user.email", "test@example.com"], cwd=self.repo, check=True)
        subprocess.run(["git", "config", "user.name", "Test"], cwd=self.repo, check=True)

        (self.repo / "src").mkdir()
        (self.repo / "tools").mkdir()
        (self.repo / "src" / "Alpha.cpp").write_text("int alpha() { return 1; }\n")
        (self.repo / "src" / "Beta.cpp").write_text("int beta() { return 2; }\n")
        (self.repo / "CMakeLists.txt").write_text("# test\n")
        (self.repo / "tools" / "source_order.txt").write_text("src/Alpha.cpp\nsrc/Beta.cpp\n")

        # Copy validate.py and elf32.py into temp repo so post-worker check works inside worktrees
        for name in ("elf32.py", "validate.py", "agent_prompt.md"):
            (self.repo / "tools" / name).write_text((TOOLS_DIR / name).read_text())

        self.orig_so = self.repo / "orig.so"
        self.orig_so.write_bytes(
            build_synthetic_arm32_elf(
                functions=[
                    ("_ZN5Alpha3runEv", b"\x01\x20\x70\x47", True),
                    ("_ZN5Alpha4drawEv", b"\x02\x20\x70\x47", True),
                    ("_ZN4Beta3runEv", b"\x03\x20\x70\x47", True),
                ],
                objects=[],
                imports=[],
            )
        )
        (self.repo / "tools" / "baseline.json").write_text(
            json.dumps(
                {
                    "functions": {
                        "compared": 3,
                        "byte_exact": 0,
                        "linked_exact": 0,
                        "stub_zero_size": 0,
                        "missing": 0,
                        "wrong_type": 0,
                        "extra": 0,
                    },
                    "data": {"exact": 0, "common": 0},
                    "allowed_extra": [],
                    "imports": [],
                }
            )
        )
        subprocess.run(["git", "add", "."], cwd=self.repo, check=True)
        subprocess.run(["git", "commit", "-m", "initial"], cwd=self.repo, check=True, capture_output=True)

        self.report_path = self.repo / "initial_report.json"
        self.report_path.write_text(
            json.dumps(
                {
                    "non_exact": [
                        {"name": "_ZN5Alpha3runEv", "demangled": "Alpha::run()", "diff_bytes": 4, "orig_size": 4, "size_delta": 0, "linked_exact": False},
                        {"name": "_ZN5Alpha4drawEv", "demangled": "Alpha::draw()", "diff_bytes": 2, "orig_size": 4, "size_delta": 0, "linked_exact": False},
                        {"name": "_ZN4Beta3runEv", "demangled": "Beta::run()", "diff_bytes": 4, "orig_size": 4, "size_delta": 0, "linked_exact": False},
                    ]
                }
            )
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_tu_grouping_avoids_worker_file_collisions(self) -> None:
        rep = json.loads(self.report_path.read_text())
        groups = harness.group_targets_by_tu(rep["non_exact"], self.repo, max_tasks=8)
        self.assertEqual(len(groups), 2)
        self.assertEqual(groups[0].tu_key, "src/Alpha.cpp")
        self.assertEqual(len(groups[0].symbols), 2)
        self.assertEqual(groups[1].tu_key, "src/Beta.cpp")
        self.assertEqual(len(groups[1].symbols), 1)

    def test_dry_run_lifecycle_and_cleanup(self) -> None:
        rc = harness.main(
            [
                "--repo-root", str(self.repo),
                "--report", str(self.report_path),
                "--workers", "2",
                "--max-tasks", "2",
                "--dry-run",
            ]
        )
        self.assertEqual(rc, 0)
        logs_root = self.repo / ".harness" / "logs"
        summaries = list(logs_root.rglob("summary.json"))
        self.assertEqual(len(summaries), 1)
        summary = json.loads(summaries[0].read_text())
        self.assertEqual(summary["counts"]["dry-run"], 2)
        for item in summary["results"]:
            self.assertTrue(Path(item["prompt"]).is_file())
            self.assertTrue(Path(item["log"]).is_file())
            self.assertFalse(Path(item["worktree"]).exists())

        # Temporary dry-run branches must be cleaned up
        branches = subprocess.run(
            ["git", "branch", "--list", "decomp/*"],
            cwd=self.repo, capture_output=True, text=True, check=True,
        ).stdout.strip()
        self.assertEqual(branches, "")

    def test_mock_codex_worker_matches_and_keeps_branch(self) -> None:
        mock_codex = self.repo / "mock_codex.py"
        mock_codex.write_text(
            "#!/usr/bin/env python3\n"
            "import os, shutil\n"
            "from pathlib import Path\n"
            "# Simulate agent editing source and building a byte-exact .so\n"
            "p = Path('src/Alpha.cpp')\n"
            "p.write_text(p.read_text() + '// matched\\n')\n"
            "bdir = Path(os.environ['GOF2_BUILD_DIR'])\n"
            "bdir.mkdir(parents=True, exist_ok=True)\n"
            "shutil.copyfile(os.environ['GOF2_ORIGINAL_SO'], bdir / 'libgof2hdaa.so')\n"
        )
        mock_codex.chmod(mock_codex.stat().st_mode | stat.S_IXUSR)

        rc = harness.main(
            [
                "--repo-root", str(self.repo),
                "--report", str(self.report_path),
                "--original", str(self.orig_so),
                "--targets", "_ZN5Alpha3runEv,_ZN5Alpha4drawEv",
                "--workers", "1",
                "--codex-bin", str(mock_codex),
                "--skip-layout",
                "--no-freshness",
            ]
        )
        self.assertEqual(rc, 0)
        summaries = sorted((self.repo / ".harness" / "logs").rglob("summary.json"))
        summary = json.loads(summaries[-1].read_text())
        self.assertEqual(summary["counts"]["matched"], 1)
        res = summary["results"][0]
        self.assertTrue(res["kept_branch"])
        self.assertEqual(res["after_diff_bytes"], 0)
        self.assertEqual(res["diff_delta"], -6)
        # Worktree removed, but improved branch preserved in git
        self.assertFalse(Path(res["worktree"]).exists())
        branches = subprocess.run(
            ["git", "branch", "--list", res["branch"]],
            cwd=self.repo, capture_output=True, text=True, check=True,
        ).stdout.strip()
        self.assertIn(res["branch"], branches)


if __name__ == "__main__":
    unittest.main()
