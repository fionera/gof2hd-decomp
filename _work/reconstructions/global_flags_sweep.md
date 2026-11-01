# Global compile/link flag A/B sweep (w3414)

Status: research note (2026-11-01), worktree `wt/w3414`, branch `wave87-w3414`,
base `103bcc42` (`docs: lock byte_exact 4115`). No flag or source change was landed:
**no tested global compile or link flag is a strict improvement at this tree state.**
The worktree is clean; this note is the only commit.

## 0. Baseline (exact gate)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Measured at the start and again after all experiments:

```
verify: byte 4115  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` from `cmake-build-match/verify/report.json` is **4524 / 4524**
(`sum(osize == usize)`). `lint` is `CLEAN ✓`; `drift` is `0 field(s) across 0 class(es)`.

Section sizes/bases are exact except the known `.aezdata` artifact:

| section | original | baseline |
|---|---|---|
| `.text` | 0x69440 / 0x1337c8 | 0x69440 / 0x1337c8 |
| `.ARM.exidx` | 0x19cc08 / 0x6f70 | same |
| `.ARM.extab` | 0x1a3b78 / 0x26298 | same |
| `.rodata` | 0x1c9e10 / 0x41b67 | same |
| `.data.rel.ro` | 0x20cbb0 / 0x2fe8 | same |
| `.data` | 0x214000 / 0x296c | same |
| `.got` | 0x20fcf4 / 0x430c | same |
| `.dynamic` | 0x20fba4 / 0x148 | same |
| `.bss` | 0x216970 / 0x69ac | **0x216980 / 0x69ac** |
| `.aezdata` | absent | **0x21696c / 8** |

The `.bss` base is `+0x10` solely because our link still contains the 8-byte
`.aezdata` section from `src/engine/file/AEFile.cpp` plus its alignment. This is a
source artifact, not a link-flag error (see `--gc-sections` below).

## 1. Link-flag A/B method

The link A/B used the exact object set and link order of the current build. The gold
command is the last command printed by Ninja for `libgof2hdaa.so`; the driver retargets
`-o`, inserts the candidate flags after `--build-id`, and strips the result:

```bash
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 \
  > /tmp/w3414/linkcmd.txt

python3 - <<'PY'
from pathlib import Path
cmd = Path('/tmp/w3414/linkcmd.txt').read_text().strip()
cmd = cmd.split(' && ', 1)[1]                    # drop "cd ... &&"
cmd = cmd.replace('-o /home/debian/GalaxyOnFire2/wt/w3414/cmake-build-match/libgof2hdaa.so',
                  '-o "$OUT"')
cmd = cmd.replace('--build-id ', '--build-id "$@" ', 1)
cmd = cmd.replace(
  '/home/debian/GalaxyOnFire2/wt/w3414/.cache/ndk/android-ndk-r18b/toolchains/'
  'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip '
  '--strip-all /home/debian/GalaxyOnFire2/wt/w3414/cmake-build-match/libgof2hdaa.so',
  '/home/debian/GalaxyOnFire2/wt/w3414/.cache/ndk/android-ndk-r18b/toolchains/'
  'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip '
  '--strip-all "$OUT"')
Path('/tmp/w3414/relink.sh').write_text(
  '#!/bin/bash\nset -e\nOUT="$1"; shift\nexport OUT\nexec bash -c '
  + repr(cmd) + ' _ "$@"\n')
PY
chmod +x /tmp/w3414/relink.sh

# example: /tmp/w3414/relink.sh /tmp/w3414/gc.so --gc-sections
```

Each variant was verified with:

```bash
python3 tools/verify/verify.py \
  --build-dir /tmp/w3414/verify_NAME \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3414/NAME.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

## 2. Link-flag results

Baseline is `4115 / 4495 / 4524` (byte / linked / size-exact). “identical” means the
stripped output was byte-identical to the baseline (`sha256` `8c66902c…`), so the gate
counts are necessarily unchanged.

| extra gold flag(s) | byte | linked | size | `.text` | `.rodata` | `.bss` | verdict |
|---|---:|---:|---:|---:|---:|---:|---|
| *(baseline)* | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | — |
| `-z relro` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical (already default with `-z now`) |
| `--no-undefined` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--fix-cortex-a8` / `--no-fix-cortex-a8` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--hash-style=sysv` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--build-id=sha1` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `-z max-page-size=4096` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `-z common-page-size=4096` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `-z lazy` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `-z noexecstack` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--sort-common` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--warn-shared-textrel` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--fatal-warnings` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--allow-multiple-definition` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical (not proposed; AGENTS.md forbids it) |
| `--no-keep-memory` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--threads` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--no-as-needed` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--no-copy-dt-needed-entries` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | identical |
| `--gc-sections` | **1617** | 4495 | 4524 | 0x1337b8 | 0x41b57 | **0x216970** | removes `.aezdata` and fixes `.bss` base, but costs −2498 byte_exact |
| `--icf=all` | 1385 | 4494 | 4524 | 0x12f9c0 | 0x41b67 | 0x216980 | over-folds |
| `--icf=safe` | 1385 | 4495 | 4524 | 0x130988 | 0x41b67 | 0x216980 | over-folds |
| `--sort-section=name` | 1342 | 4493 | 4524 | 0x13383c | 0x41b40 | 0x216980 | worse |
| `--rosegment` | 1674 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | extra LOAD; worse |
| `-z norelro` | 2766 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216960 | wrong RELRO layout; worse |
| `-z execstack` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | code-neutral, but GNU_STACK becomes RWE (wrong) |
| `--hash-style=gnu` | 1677 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | wrong hash section; worse |
| `--build-id=md5` | 1868 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | wrong note size; worse |
| `--build-id=none` | 1687 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | removes note; worse |
| `--spare-dynamic-tags=0` | 4111 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | dynamic shrinks to 0x120; worse |
| `--spare-dynamic-tags=10` | 4111 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | dynamic grows to 0x170; worse |
| `--as-needed` | 1680 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | drops NEEDED entries; worse |

Link failures with the current `-Tdata 0x20cb80` pin:

| flag | failure |
|---|---|
| `--no-merge-exidx-entries` | `load segment overlap [0x0 -> 0x20dfd7] and [0x20cba0 -> 0x21d32c]` |
| `--long-plt` | `load segment overlap` (PLT grows past the pinned RW segment) |
| `--hash-style=both` | `load segment overlap` |
| `--sort-section=alignment` | gold: `must take one of ... none, name` |
| `--no-sort-common`, `--no-rosegment`, `--no-eh-frame-hdr`, `-z separate-code`, `-z noseparate-code` | gold does not accept the option |
| `--copy-dt-needed-entries` | gold: not supported but required for `libstdc++.so` in `libfmodex.so` |
| `--emit-relocs` | gold internal error in `relocate_relocs` |

**Link conclusion:** the only link flag that changes the `.aezdata`/`.bss` base is
`--gc-sections`, and it costs 2498 byte-exact functions. No link flag improves the gate.
`-z now` is already in the tree; `-z relro`, `--fix-cortex-a8`, `--no-undefined`,
`--hash-style=sysv`, `--build-id=sha1`, and `-z max-page-size=4096` are exact no-ops.

## 3. Compile-flag A/B method

`GOF2_EXTRA_FLAGS` is not a native knob in `cmake/toolchain.cmake`; a wrapper toolchain
appends it to both C and C++ flags after the canonical set:

```bash
cat > /tmp/w3414/toolchain_extra.cmake <<'EOF'
include("/home/debian/GalaxyOnFire2/wt/w3414/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
EOF

cmake -S /home/debian/GalaxyOnFire2/wt/w3414 -B /tmp/w3414/build-NAME -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3414/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3414/build-NAME --target link_so -j8
```

Then the same `verify.py` command as §1. **Compile variants must be built sequentially**
in this tree: `third_party/match_libs.cmake` writes the vendored zlib/libzip/libcrypto
archives into the source tree (`third_party/...`), and concurrent scratch builds race on
`_obj_gof2`. A concurrent run produced `sha256-armv4.S: #else without #if`; the table below
was re-run sequentially after deleting the generated archives/objects and rebuilding the
baseline. The baseline returned to `4115 / 4495 / 4524`.

## 4. Compile-flag results (sequential, current tree)

Baseline `4115 / 4495 / 4524`; `.text` 0x1337c8, `.rodata` 0x41b67, `.bss` 0x216980.

| extra flag | byte | linked | size | `.text` | `.rodata` | `.bss` | verdict |
|---|---:|---:|---:|---:|---:|---:|---|
| `-DNDEBUG` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | neutral |
| `-fno-plt` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | neutral (clang 7 ARM emits the same code) |
| `-fno-use-init-array` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | neutral |
| `-mno-restrict-it` | 4115 | 4495 | 4524 | 0x1337c8 | 0x41b67 | 0x216980 | neutral |
| `-fdata-sections` | **4108** | 4495 | 4524 | 0x1337c8 | 0x41a17 | 0x216980 | −7 byte_exact; authentic ndk-build flag, but current data layout is not faithful enough |
| `-fno-slp-vectorize` | 1411 | 4381 | 4408 | 0x132e28 | 0x41b67 | 0x216980 | much worse |
| `-fno-zero-initialized-in-bss` | 1454 | 4485 | 4516 | 0x133768 | 0x41b67 | 0x2e8 | much worse |
| `-fno-rtti` | 1678 | 4495 | 4524 | 0x1337c8 | 0x4104e | 0x216980 | much worse |
| `-fno-unroll-loops` | 2840 | 4488 | 4520 | 0x1337a8 | 0x41b67 | 0x216980 | worse |
| `-fno-strict-aliasing` | 1408 | 4347 | 4410 | 0x133cc8 | 0x41b67 | 0x216980 | much worse |
| `-fno-builtin` | 1365 | 3983 | 4121 | 0x134838 | 0x41b57 | 0x216980 | much worse; `extra=1` |
| `-fno-optimize-sibling-calls` | 1316 | 3585 | 3686 | 0x1315b8 | 0x41b67 | 0x216980 | much worse |
| `-fno-jump-tables` | 1352 | 4407 | 4437 | 0x134808 | 0x41937 | 0x216980 | much worse |

Additional screened-neutral candidates produced the baseline gate counts (and, where
checked, `.text`/`.rodata`/`.data` section hashes identical to the baseline):
`-fexceptions`, `-fpic`, `-fno-asynchronous-unwind-tables`, `-fno-omit-frame-pointer`,
`-mimplicit-it=always`, `-mtune=cortex-a8`, `-mno-long-calls`,
`-mno-omit-leaf-frame-pointer`, `-mabi=aapcs-linux`. (`-fno-asynchronous-unwind-tables`
is overridden by the later `-funwind-tables` target option, so it is a no-op here.) `-fno-common` **fails to link** at this tree state:

```
ld.gold: multiple definition of 'g_android_back_button_pressed'
  GOF2HD2012.c.obj: previous definition here
  BackButton.c.obj: previous definition here
```

(`-fno-common` was reported neutral in the older `flag_matrix_2.md`; the current tree has
duplicate tentative C definitions, so it cannot be enabled without a source fix.)

Historical alternatives from `flag_matrix_2.md` (older 1415 baseline) remain worse and
were not all re-run here: `-Os` 1252/2649, `-O2` 1157/2161, `-O1` 1031/1783,
`-O3` 1156/2121, `-fomit-frame-pointer` 1273/1921, `-fno-stack-protector` 1343/3780,
`-fstack-protector` 1343/3784, `-fno-threadsafe-statics` 1347/4489,
`-mcpu=cortex-a8` 1304/4223, `-marm` 55/235. Two current-tree re-verifies
(`-fomit-frame-pointer`, `-fno-stack-protector`) did not finish within ~8 minutes because
the all-functions-changed diffs are pathologically slow in `verify.py`; their older
counts are directionally unambiguous and neither is a strict-improvement candidate.

## 5. What is proven / what remains hypothesis

**Proven by controlled rebuilds at `103bcc42`:**

1. No tested global compile flag raises `byte_exact` above 4115. The best accepted
   candidates are exactly neutral (`-DNDEBUG`, `-fno-plt`, `-fno-use-init-array`,
   `-mno-restrict-it`, and the screened-neutral list above).
2. No tested link flag raises `byte_exact` above 4115. The best link candidates are
   exact no-ops (`-z relro`, `--no-undefined`, `--fix-cortex-a8`, `--hash-style=sysv`,
   `--build-id=sha1`, `-z max-page-size=4096`).
3. `-fdata-sections` is authentic ndk-build behavior but currently costs 7 byte_exact
   and shrinks `.rodata` by 0x150; it should be re-tested only after the TU/data layout
   is faithful.
4. `-fno-common` cannot link the current tree because of duplicate tentative C
   definitions (`g_android_back_button_pressed` in `BackButton.c` and `GOF2HD2012.c`).
5. `--gc-sections` removes `.aezdata`, makes `.bss` land at the original `0x216970`, and
   keeps `.text`/`.rodata`/`.exidx` almost intact, but it drops byte_exact to 1617.
   Therefore the `.bss` `+0x10` is not worth a link flag; it is a source artifact.
6. `-z now` is already present and is not a candidate. `-z relro` is implied by it.

**Still hypothesis / not settled by flags:**

1. The remaining 409 non-byte-exact functions are source/TU/data-order gaps, not a
   missing global flag. This is consistent with the existing `literal_pool_audit.md`
   classification (`.rodata` order, PLT/dynsym order, `.bss`/`.got` address residue).
2. The original almost certainly used ndk-build defaults `-DNDEBUG` and
   `-fdata-sections` (see `build_system_evidence.md`), but at this tree state
   `-fdata-sections` costs 7 and `-DNDEBUG` is neutral. Adding either is an authenticity
   choice, not a gate improvement.
3. The exact original LLVM 7 defaults are not fully enumerable from the stripped `.so`;
   this sweep covers the candidate flags that can plausibly change codegen at `-Oz`.
   `-mcpu=cortex-a9` was built but its `verify.py` run did not complete in 7 minutes;
   it is not a strict-improvement candidate (it changes the CPU model away from the
   original's plain `armv7-a`).
4. `-fno-common` becoming viable after the duplicate C definitions are collapsed is a
   source cleanup, not a flag experiment.

## 6. Reproduction summary

```bash
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# link A/B (see §1 for the exact relink.sh generator)
/tmp/w3414/relink.sh /tmp/w3414/NAME.so <gold flags>
python3 tools/verify/verify.py --build-dir /tmp/w3414/verify_NAME \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/w3414/NAME.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# compile A/B (see §3; run sequentially, one scratch build at a time)
cmake -S . -B /tmp/w3414/build-NAME -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3414/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3414/build-NAME --target link_so -j8
```

No `--update-baseline` was run; `tools/verify_baseline.json` is unchanged.
