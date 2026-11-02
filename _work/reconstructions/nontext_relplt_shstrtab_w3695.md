# Non-.text section deltas: `.rel.plt` and `.shstrtab` (w3695)

Status: research note (2026-11-02), worktree `wt/w3695`, branch `wave87-w3695`, based on
master `0e966b1c`. Every claim below is backed by a command against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or by a controlled relink of this tree.

Baseline (before any change):

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)   .text 0x1337c8
```

## TL;DR

1. **`.shstrtab` is a strip-tool artifact, not a link-layout artifact.** The original's
   `.shstrtab` is in **gold's Stringpool order** (the raw link order with debug-section names
   removed). Our build linked with gold and then ran `arm-linux-androideabi-strip --strip-all`,
   and GNU strip 2.27 **rewrites** `.shstrtab` into section-header order. Passing **`-s`**
   (`--strip-all`) to gold itself and dropping the post-link GNU strip reproduces the original
   `.shstrtab` **byte-for-byte** (md5 `f5c1dbee0f4b19a0b24ceb49f0963a49`, 279 bytes, 0 differing
   bytes; before: 259 differing bytes). Gate counts are unchanged (byte 4496 / linked 4496).
   This is committed with the note.

2. **`.rel.plt` is a 6-entry rotation caused by `LevelScript::process`'s body layout**, not by a
   link flag. The only moved symbol is `PlayerEgo::deleteHackingGame`; its call site is at
   `.text` offset `0x543e` in our object but `0x61bc` in the original, so gold assigns its PLT
   slot first instead of last. All six symbols are referenced from the *same* function, so the
   slot order is the call-site address order. `--sort-section=name`, `-z nocombreloc`,
   `--gc-sections` all leave the same 12-byte delta. No independent link knob exists.

## 1. `.shstrtab`: gold `-s` vs post-link GNU strip

### Symptom

Both files have 27 sections and `.shstrtab` is 0x117 bytes, but the bytes differ:

| file | `.shstrtab` size | differing bytes |
|---|---:|---:|
| original | 0x117 (279) | — |
| ours, gold link + GNU `strip --strip-all` | 0x117 (279) | **259** |

The section **set and order** are identical; only the order of the strings inside `.shstrtab`
differs. Original (first strings):

```
.init_array .fini_array .ARM.exidx .text .got .comment .note.android.ident .rel.plt .bss
.ARM.attributes .dynstr .gnu.version_r .data.rel.ro .rel.dyn .gnu.version ...
```

Ours (GNU-stripped) is exactly the final section-header order:

```
.shstrtab .note.android.ident .note.gnu.build-id .dynsym .dynstr .hash .gnu.version ...
```

### Root cause

Gold's raw (unstripped) output already has the original's order, with the debug-section names
interleaved:

```
/tmp/raw.so (gold, no strip):
.init_array .fini_array .ARM.exidx .debug_abbrev .text .got .comment .note.android.ident
.rel.plt .bss .ARM.attributes .debug_pubtypes ... .dynstr ... .rodata .data
```

Removing the debug names from that sequence gives the original's `.shstrtab` exactly. GNU
`strip --strip-all` (binutils 2.27, the NDK r18b strip) rebuilds `.shstrtab` in section-header
order, which is what our build was doing.

Gold's own strip (`-s` / `--strip-all`) emits the same 27-section set (no
`.symtab`/`.strtab`/`.debug_*`, keeps `.comment` and `.note.gnu.gold-version`) and preserves its
Stringpool order. `-s` and `--strip-all` are byte-identical:

```
cmp /tmp/gold_s.so /tmp/gold_stripall.so   # identical
```

`.shstrtab` md5:

| file | md5 |
|---|---|
| original | `f5c1dbee0f4b19a0b24ceb49f0963a49` |
| gold `-s` | `f5c1dbee0f4b19a0b24ceb49f0963a49` |
| gold link + GNU strip (before) | different (259 bytes differ) |

### Reproduction

The exact link command is the `COMMAND` of the `libgof2hdaa.so` rule in
`cmake-build-match/build.ninja`. The controlled relink used here (extract the gold part, replace
`-o`, append `-s`, do **not** run the post-link strip):

```bash
python3 - <<'PY'
lines = open('cmake-build-match/build.ninja').read().splitlines()
for ln in lines:
    if 'ld.gold -shared' in ln:
        cmd = ln.split('COMMAND = ', 1)[1]; break
gold = cmd.split(' && ')[0] + ' && ' + cmd.split(' && ')[1]
open('/tmp/goldcmd.txt','w').write(gold)
PY
# then run the gold command with -o /tmp/gold_s.so and an extra -s
```

Section-by-section diff (the helper is the same logic used for every table below):

```python
import struct
def sec(path, name):
    d = open(path,'rb').read()
    shoff = struct.unpack_from('<I', d, 0x20)[0]
    shentsize = struct.unpack_from('<H', d, 0x2e)[0]
    shnum = struct.unpack_from('<H', d, 0x30)[0]
    shstrndx = struct.unpack_from('<H', d, 0x32)[0]
    sh = []
    for i in range(shnum):
        off = shoff + i*shentsize
        nm, typ, fl, addr, offset, size, link, info, al, es = struct.unpack_from('<IIIIIIIIII', d, off)
        sh.append((nm, typ, offset, size))
    off = shoff + shstrndx*shentsize
    nm, typ, fl, addr, offset, size, link, info, al, es = struct.unpack_from('<IIIIIIIIII', d, off)
    st = d[offset:offset+size]
    for nm, typ, offset, size in sh:
        if nm == 0: continue
        n = st[nm:st.index(b'\0', nm)].decode()
        if n == name:
            return d[offset:offset+size]
```

### Measured section diff before / after

| section | size orig | size before | diff bytes before | diff bytes after `-s` |
|---|---:|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | 114,529 | 114,529 |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | 2,707 | 2,707 |
| `.ARM.extab` | 0x26298 | 0x26290 | 121,218 (+ size −8) | 121,218 (+ size −8) |
| `.rel.plt` | 0x72d0 | 0x72d0 | 12 | 12 |
| `.note.gnu.build-id` | 0x24 | 0x24 | 20 | 20 |
| **`.shstrtab`** | **0x117** | **0x117** | **259** | **0** |
| `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_d`, `.gnu.version_r`, `.rel.dyn`, `.plt`, `.rodata`, `.data.rel.ro`, `.data`, `.got`, `.dynamic`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes` | — | — | 0 | 0 |

The `.note.gnu.build-id` value changes when the strip method changes (gold hashes its own
pre-strip output; GNU strip recomputes it), but it already differs from the original because the
content differs. The original's build-id is `c011810d...`; gold `-s` gives `c3d071ba...`.

### Change committed

`CMakeLists.txt`: add `-s` to the gold command and remove
`COMMAND ${GOF2_NDK_STRIP} --strip-all "${_so}"`. Gate after the change:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)   .text 0x1337c8
```

This is a strict improvement in ELF identity (`.shstrtab` 259 -> 0 differing bytes) with no gate
regression. It is also the parsimonious explanation of the original: the original was linked with
NDK r18b gold 1.12 and its `.shstrtab` is exactly gold's Stringpool order; the NDK r18b GNU strip
does not preserve that order, and `llvm-strip` drops `.comment`/`.note.gnu.gold-version` (24
sections, not 27).

## 2. `.rel.plt`: the 6-entry `deleteHackingGame` rotation

### Symptom

Both `.rel.plt` sections are 0x72d0 bytes (3,674 `R_ARM_JUMP_SLOT` entries, same
`r_offset` sequence), but 12 bytes differ. Those 12 bytes are exactly 6 entries:

| entry | `r_offset` | original sym (index) | ours sym (index) |
|---:|---:|---|---|
| 2831 | 0x2132d4 | `_ZN10StarSystem20switchPlanetForIntroEv` (4367) | `_ZN9PlayerEgo17deleteHackingGameEv` (2594) |
| 2832 | 0x2132d8 | `_ZN5Level20switchSkyboxForIntroEv` (3370) | `_ZN10StarSystem20switchPlanetForIntroEv` (4367) |
| 2833 | 0x2132dc | `_ZN21ParticleSystemManager5resetEv` (4420) | `_ZN5Level20switchSkyboxForIntroEv` (3370) |
| 2834 | 0x2132e0 | `_ZN9PlayerEgo18startSmokeEmissionEv` (2610) | `_ZN21ParticleSystemManager5resetEv` (4420) |
| 2835 | 0x2132e4 | `_ZN13PlayerStation10setVisibleEb` (4238) | `_ZN9PlayerEgo18startSmokeEmissionEv` (2610) |
| 2836 | 0x2132e8 | `_ZN9PlayerEgo17deleteHackingGameEv` (2594) | `_ZN13PlayerStation10setVisibleEb` (4238) |

It is a single cyclic shift: `deleteHackingGame` is first in ours, last in the original; the other
five keep their relative order. The PLT addresses are the same six slots in both files:

```
orig: 0x66cb4 switchPlanetForIntro, 0x66cc0 switchSkyboxForIntro,
      0x66ccc ParticleSystemManager::reset, 0x66cd8 startSmokeEmission,
      0x66ce4 PlayerStation::setVisible, 0x66cf0 deleteHackingGame
ours: 0x66cb4 deleteHackingGame, 0x66cc0 switchPlanetForIntro,
      0x66ccc switchSkyboxForIntro, 0x66cd8 ParticleSystemManager::reset,
      0x66ce4 startSmokeEmission, 0x66cf0 PlayerStation::setVisible
```

### Root cause: call-site order inside `LevelScript::process`

All six symbols are referenced only from `LevelScript::process` (`src/game/world/LevelScript.cpp`;
`deleteHackingGame` only at line 3617). Gold assigns PLT slots in input-section/relocation order;
for a single `-ffunction-sections` function that is increasing `r_offset` in the object's
`.rel.text._ZN11LevelScript7processEi`. The object relocation offsets are:

```
0000543e  R_ARM_THM_CALL  _ZN9PlayerEgo17deleteHackingGameEv
000061ba  R_ARM_THM_CALL  _ZN10StarSystem20switchPlanetForIntroEv
000061c2  R_ARM_THM_CALL  _ZN5Level20switchSkyboxForIntroEv
000061e0  R_ARM_THM_CALL  _ZN21ParticleSystemManager5resetEv
000061ea  R_ARM_THM_CALL  _ZN21ParticleSystemManager5resetEv
0000646c  R_ARM_THM_CALL  _ZN9PlayerEgo18startSmokeEmissionEv
00006786  R_ARM_THM_CALL  _ZN13PlayerStation10setVisibleEb
```

`deleteHackingGame` is at 0x543e, before the other five (0x61ba+), which is why it gets the
first slot. The final `.text` call sites confirm the same in both binaries (function start
0x137d18, size 0xdf90 = 57,232 in both):

| symbol | original call | ours call |
|---|---:|---:|
| `deleteHackingGame` | 0x13ded4 (offset 0x61bc) | **0x13d156 (offset 0x543e)** |
| `switchPlanetForIntro` | 0x13d1ec (0x54d4) | 0x13ded2 (0x61ba) |
| `switchSkyboxForIntro` | 0x13d1f4 (0x54dc) | 0x13deda (0x61c2) |
| `ParticleSystemManager::reset` | 0x13d212 / 0x13d21c | 0x13def8 / 0x13df02 |
| `startSmokeEmission` | 0x13d396 (0x567e) | 0x13e184 (0x646c) |
| `PlayerStation::setVisible` | 0x13da1e (0x5d06) | 0x13e49e (0x6786) |

The original emits the `deleteHackingGame` block (inside `case 125`'s `m_nState >= 4 && ...`
branch) **after** the other five calls; our `LevelScript::process` emits it **before** them
(`0x543e` vs `0x61bc`). This is a body/control-flow-layout mismatch in the same function that the
literal-pool audit already lists as the worst non-exact body (`LevelScript::process`, 54.4% in
`docs/IDENTICALITY.md`). It is not a link-order or TU-membership issue: all six relocations live
in one section, so their relative order is fixed by the emitted code.

### Controlled A/B: no link flag changes it

Relinked the current objects with extra gold flags (same command, `-o /tmp/v_*.so`) and counted
`.rel.plt` differing bytes vs the original:

| variant | `.rel.plt` size | differing bytes |
|---|---:|---:|
| current (`-s`, no extra flag) | 0x72d0 | **12** |
| `--sort-section=name` | 0x72d0 | 12 |
| `-z nocombreloc` | 0x72d0 | 12 |
| `--gc-sections` | 0x72d0 | 12 |
| `--icf=all` | 0x72d0 | 7,360 (whole PLT reordered) |

`--icf=all` is not a candidate (it also shrinks `.text` to 0x1305a8 and lowers `byte_exact`).
The three non-ICF variants leave the same 6-entry rotation, so `.rel.plt` cannot be fixed by a
link flag. The fix is the `LevelScript::process` body layout; when that body becomes exact, the
call-site order (and therefore the PLT slot order) follows automatically.

## 3. Reproduction commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w3695
cmake --preset default
cmake --build cmake-build-match --target check -j8        # baseline / after-change gate
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift

# section-by-section byte diff (original vs ours)
python3 /tmp/sectdiff.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so

# .rel.plt entry names
python3 /tmp/relplt.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so

# call sites in LevelScript::process (function starts at 0x137d18)
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x137d18 --stop-address=0x185c88 \
    cmake-build-match/libgof2hdaa.so | grep -E 'bl.*(66cb4|66cc0|66ccc|66cd8|66ce4|66cf0)'

# gold -s vs GNU strip
readelf -SW cmake-build-match/libgof2hdaa.so      # 27 sections, .shstrtab 0x117
readelf -p .note.gnu.gold-version cmake-build-match/libgof2hdaa.so
```

## 4. Proven vs hypothesis

**Proven by measurement**

* The original `.shstrtab` is byte-identical to gold `-s` output (`md5
  f5c1dbee0f4b19a0b24ceb49f0963a49`) and to the debug-name-free subsequence of gold's raw
  Stringpool order.
* GNU `strip --strip-all` 2.27 rewrites `.shstrtab` into section-header order (259 differing
  bytes); gold `-s` does not (0 differing bytes).
* `.rel.plt` has exactly one 6-entry rotation; the moved symbol is
  `PlayerEgo::deleteHackingGame`; its call site is at object offset 0x543e ours vs 0x61bc
  original, inside `LevelScript::process`.
* `--sort-section=name`, `-z nocombreloc`, `--gc-sections` leave the 12-byte `.rel.plt` delta
  unchanged.
* The `-s` change keeps the gate at byte 4496 / linked 4496, lint CLEAN, drift 0.

**Hypothesis (not directly witnessed)**

* The original build passed `-s`/`--strip-all` to gold rather than running a post-link strip.
  The evidence is that gold `-s` reproduces the original `.shstrtab` exactly while the NDK r18b
  GNU strip does not, and `llvm-strip` produces a different section set (drops `.comment` and
  `.note.gnu.gold-version`). A hypothetical strip tool that preserves gold's Stringpool order
  would be observationally equivalent, but no such tool is in NDK r18b.
* `.rel.plt` will become byte-identical exactly when `LevelScript::process`'s emitted block order
  matches the original (the same fix that would raise its 54.4% body match). No separate action
  is needed.
