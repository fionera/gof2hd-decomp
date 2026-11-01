# Data-layout audit 2 — why 1848 linked-exact functions are not byte-exact

Worker `w3357`, branch `wave87-w3357`, base `e8587e75` (gate: byte_exact 2646,
linked_exact 4494). Target: the 1848 functions whose instruction stream and address are
identical to the original but whose literal-pool words differ. All work is reproducible with
the standard `cmake-build-match` tree; scratch files live under `/tmp`.

**Result of this pass:** `byte_exact 2646 -> 2734` (+88), `linked_exact 4494 -> 4494`,
`avg 99.89 -> 99.9`, `.rodata` base restored to the original `0x1c9e10`. Two source fixes:
(1) drop ten empty-`.text` CANTUNWIND sentinels that inflated `.ARM.exidx` by 0x60
(+86 functions, all of them `.rodata`+0x60-only), and (2) restore the original string-literal
emission order in three shader TUs (+2). The remaining ranked causes are below.

---

## 1. Baseline and final section table

Measured with `readelf -SW` on the stripped link output:

| section | original addr / size | ours (base) addr / size | ours (final) addr / size | final delta |
|---|---|---|---|---|
| `.text` | `0x0069440` / `0x1337c8` | `0x0069440` / `0x1337ec` | `0x0069440` / `0x1337ec` | **+0x24** |
| `.ARM.exidx` | `0x019cc08` / `0x6f70` | `0x019cc2c` / `0x6fb8` | `0x019cc2c` / `0x6f58` | **-0x18** |
| `.ARM.extab` | `0x01a3b78` / `0x26298` | `0x01a3b84` / `0x2628c` | `0x01a3b84` / `0x2628c` | **-0xc** |
| `.rodata` | `0x01c9e10` / `0x41b67` | `0x01c9e50` / `0x41b97` | `0x01c9e10` / `0x41b97` | **+0x30** |
| `.data.rel.ro` | `0x020cbb0` / `0x2fe8` | exact | exact | **0** |
| `.data` | `0x0214000` / `0x296c` | exact | exact | **0** |
| `.got` | `0x020fcf4` / `0x430c` | exact | exact | **0** |
| `.dynamic` | `0x020fba4` / `0x148` | exact | exact | **0** |
| `.plt` | `0x005e7ec` / `0xac4c` | exact | exact | **0** |
| `.bss` | `0x0216970` / `0x69ac` | `0x0216980` / `0x69c8` | `0x0216980` / `0x69c8` | **+0x1c** |

The read-only bases are a chain: `rodata = text_end + exidx_size + extab_size` (all three
sections are contiguous and 4-byte aligned). At the base, `.text` was +0x24 and `.exidx` was
+0x60, so `.rodata` sat at `0x1c9e50` (+0x40). Fix 1 removes exactly 0x60 from `.exidx`, which
puts `.rodata` back at `0x1c9e10` while leaving `.text` untouched.

## 2. Method

```sh
# build + report
cmake --preset default
cmake --build cmake-build-match --target verify -j8     # regenerates report.json
cmake --build cmake-build-match --target check  -j8     # ratchet gate

# exact gold relink with the map + local symbols (the shipped .so is stripped)
bash /tmp/relink_map_w3357.sh                            # -> /tmp/w3357_unstripped.so

# per-line classification of every linked-but-not-byte function
python3 /tmp/classify2_w3357.py                          # -> /tmp/classify2.out
# raw 4-byte word deltas
python3 /tmp/raw_word_delta_w3357.py
# PIC pool words grouped by target section + delta
python3 /tmp/pool_target_delta_w3357.py
# per-input-section .rodata placement vs the original
python3 /tmp/rodata_map_w3357.py
# first byte divergence of .rodata
python3 - <<'PY'  # (inline; see section 5)
PY
```

`tools/verify` decodes each function from a copy with literal-pool bytes NOPed and compares
words as data, so a differing pool word is reported as a `pool` line, not as an instruction.

## 3. Classification of the 1848 (base build)

`/tmp/classify2_w3357.py` walks the disassembly of both links line by line:

| class | count | meaning |
|---|---|---|
| branch -> branch, PLT -> PLT, same symbol | 8898 | PLT **entry order** differs; `.plt` bytes/size are exact |
| pool -> pool, `.text` -> `.text`, different target | 5328 | literal word into `.text` (local code or a PC-relative string offset) |
| branch -> branch, `.text` -> `.text`, same | 951 | intra-`.text` local branch (target address differs) |
| pool -> pool, `.rel.plt` | 110 | GOT slot for an import |
| pool -> pool, `.plt` | 100 | PLT address in a pool |
| branch -> branch, `.plt` -> `.plt`, different symbol | 69 | PLT reorder |
| branch -> branch, `.text` -> `.text`, different symbol | 57 | local callee differs |
| pool -> pool, `.ARM.extab` | 27 | LSDA/personality pointer |
| pool -> pool, `.rel.dyn` | 32 | dynamic relocation word |
| pool -> pool, `.dynstr` / `.hash` / `.dynsym` / `.note.android.ident` | 7 / 5 / 2 / 2 | dynamic-table words |

Raw 4-byte word delta histogram (top): `-8` 2730, `-0x80000` 2646, `-0x10` 1126,
`+0x60000` 782, `+6` 768, `+4` 645, `+0x120000` 463, `-6423` 307. The `+0x60` group
(3199 words at the base) is gone after Fix 1. The remaining dominant deltas are **instruction
encodings** (Thumb `blx`/`b` to PLT or local code, and `add pc` immediates), not data-object
offsets.

Pool words grouped by *target section* (base build, after Fix 1):

| target | words | top deltas |
|---|---|---|
| `.got` | 1571 | 962 `0`, 542 `+4`, 20 `-556`, 19 `+12` |
| `.rodata` | 1033 | 307 `-6423`, 229 `0`, 94 `+1200`, 93 `+3104`, 30 `+48`, 25 `-109` |
| `.bss` | 48 | 11 `+108`, 10 `+4304`, 6 `-3980`, 6 `+4256`, 5 `+4224` |
| `.data` | 12 | 4 `-1204`, 4 `0`, 3 `-4`, 1 `-16` |
| `.data.rel.ro` | 7 | 6 `0`, 1 `-4` |
| `.text` | 4 | 2 `-4`, 2 `-8` |

## 4. Fix 1 — empty-`.text` CANTUNWIND sentinels (the `.rodata` +0x60)

`tools/gen_exidx_markers.py` renames every empty plain `.text` section to a unique
`.text.tuend.*` marker so gold emits one CANTUNWIND `.ARM.exidx` entry per empty TU, matching
the original's per-object sentinels. Ten app objects have an empty `.text` but the original
emits **no** CANTUNWIND entry for them. Renaming them added 12 entries (0x60) to `.exidx`,
which pushed `.rodata` by +0x40.

Fix: drop the section entirely for those ten objects instead of renaming it:

```python
DROP_EMPTY_TEXT = {
    "BackButton.c.obj", "NFC.cpp.obj", "VirtualInput.cpp.obj",
    "BlowfishSubkeyFile.cpp.obj", "main.c.obj", "GOF2HD2012.c.obj",
    "Mesh.cpp.obj", "GLES1Compat.cpp.obj", "FileInterfaceAndroid.cpp.obj",
    "EaseInOutMatrix.cpp.obj",
}
# rename_empty_text(..., drop=True) -> objcopy --remove-section .text
```

Measured: `.ARM.exidx 0x6fb8 -> 0x6f58` (-0x60), `.rodata` base `0x1c9e50 -> 0x1c9e10`,
`byte_exact 2646 -> 2732` (+86). All 86 are functions whose only remaining diff was a
`.rodata` pool word at `+0x60`; `/tmp/count_rodata_fixable_w3357.py` counted 93 such functions
before the fix (the other 7 also had another diff).

## 5. Fix 2 — shader string-literal emission order (+2)

The first `.rodata` content divergence (after Fix 1) is at offset `0xc2f`:

```
orig: ...TextureAlphaTestShader\0BloomShader\0BloomShader fboLuma\0...fboBlack\0texSize\0s_texture_bloom\0...
ours: ...TextureAlphaTestShader\0texSize\0s_texture_bloom\0BloomShader\0BloomShader fboLuma\0...
```

Clang emits string literals in function-definition order within a TU. The original defines the
constructor and `InternalInit` before `Init`; we defined `Init` first. Moving the definitions
(no code change) fixes the order. The same pattern was found in `GlowPPShader.cpp` and
`SandboxShader.cpp`:

| TU | original literal order | our order (before) |
|---|---|---|
| `BloomShader.cpp` | `BloomShader`, `fboLuma..fboBlack`, `texSize`, `s_texture_bloom` | `texSize`, `s_texture_bloom`, `BloomShader`, ... |
| `GlowPPShader.cpp` | `GlowPPShader`, `fboMasked..fboBlack`, `s_texture_glow` | `fboMasked..fboBlack`, `s_texture_glow`, `GlowPPShader` |
| `SandboxShader.cpp` | `SandboxShader`, `u_light_dir`, `u_eye_pos`, `s_texture_norm` | `u_light_dir`, `u_eye_pos`, `s_texture_norm`, `SandboxShader` |

Measured: `byte_exact 2732 -> 2734` (+2). The first `.rodata` divergence moves from `0xc2f`
to `0x1193` (the FModSound block, see below).

## 6. Attempted — FModSound string order (correct data, net -11 byte)

The next divergence is the FModSound block:

```
orig: purge\0pause\0FMOD_GOF2\0master\0music\0sfx\0voice\0english\0deutsch\0
ours: purge\0pause\0master\0music\0sfx\0voice\0english\0deutsch\0FMOD_GOF2\0
```

`FMOD_GOF2` is the literal in `FModSound::freeAllEvents`; `master..voice` are the static
`kCats` array in `init`; `english/deutsch` are `kLangs` in `setAudioLanguage`. The original
therefore emitted `freeAllEvents` before `init`. Moving `freeAllEvents` above `init` makes the
whole 0x3d-byte FModSound `.rodata.str1.1` section byte-identical and removes ~3000
`.text`-target pool diffs (diff lines 15622 -> 12664), **but** gold's PLT order follows the
object's section order, so it reorders `getProject` before `FMOD_EventSystem_Create` and costs
11 byte-exact functions (all FModSound callers plus `GameRecord`, `RecordHandler`,
`__dynamic_cast`). The original has *both* the freeAll-first string order and the init-first
PLT order; reproducing that needs the PLT-order campaign (section 8), so this change is
documented but **not committed**. The patch is `/tmp/FModSound_moved.cpp`.

## 7. Ranked remaining data-object offsets

Fresh numbers after Fixes 1–2 (`/tmp/pool_target_delta2.out`). "Functions" is the number of
pool words in the group; a fix is only worth byte_exact when every pool word in a function
matches.

1. **`.rodata` `-6423` (307 words) — `loadPortraits`.** The whole `GameResource.cpp` block
   (`loadPortraits`, `loadLowTexturesAndMaterials`, `ResourceTexture` ctor) is placed ~6.4 KB
   later in our `.rodata` than in the original. `_work/original_layout/tu_mapping.md` says the
   original TU is `Classes/Game/GameResource.cpp` = `BuildResourceList + loadPortraits +
   loadLowTexturesAndMaterials + ResourceTexture ctor`; our tree splits it across
   `engine/render/BuildResourceList.cpp` and other TUs. This is a TU-membership/order fix.
2. **`.rodata` `+1200` (94 words) — `BlurShader::Init`.** The `BlurShader` string block is
   1200 bytes too early. Compare `src/engine/render/shaders/BlurShader.cpp` literal order
   with the original (same class of bug as Fix 2).
3. **`.rodata` `+3104` (93 words) — `loadLowTexturesAndMaterials`.** Same `GameResource.cpp`
   block as (1); fixing the TU order should move both.
4. **`.rodata` `+48` (30 words).** The remaining `.rodata` size delta (+0x30) is concentrated
   here; the 12 input sections with no exact byte match are
   `VertexColorAlphaTextureShader` (0x56f), `Generator` (0x248), `Status` (0x154),
   `Level` (0x1414), `LevelCreateScene` (0x480), `StarMap` (0x22c), `ModStation` (0x1b9),
   `ImageFactory` (0x320), `LensFlare` (0x3c), `MenuTouchWindow` (0xa4),
   `AutoPilotList` (5), `DialogueWindow` (0x2904). Each is a per-TU literal/table difference.
5. **`.bss` (48 words, size +0x1c).** The original `.bss` is **not** in object order:
   `g_pActivity` @0, `stackX` @0x1010, `ndk_APK_Path` @0x1034, `gi_iap_*` @0x1044,
   `FileInterfaceAndroid` @0x106c, `AEFile` @0x1090, ... `ParticleSettingsRef::cur/init`
   @0x1a78/0x1e78, `S` @0x5908, `P` @0x6908, then the **COMMON block last** @0x6950.
   Ours puts `** common` first @0x216980, then `g_pActivity` @0x2169dc, `stackX` @0x2169e0,
   `P` @0x218a10, `S` @0x217a10. Two independent causes:
   - the trailing `*` wildcard in `cmake-build-match/section_order.txt` makes gold place
     `** common` before the wildcard-matched `.bss` sections (verified with a minimal gold
     link: `.data` entry alone -> COMMON last; `*` or a `.bss` entry -> COMMON first);
   - the original `.bss` order follows a link order in which `BlowfishSubkeyFile` is near the
     end and `NdkHooks`/`gi_iap_*` is near the start, unlike our `o001..o008` order.
   Removing `*` alone restores `g_pActivity @0` and COMMON-last but shifts `.text` by -12 B
   and `.bss` to +0x28, so it is not a standalone win.
6. **`.data` `-1204` (4 words) — `ModStation::OnKeyPress`/`checkHints`.** Both load a
   `.data` pointer 1204 bytes too early. `GameText::currentLang` is also at orig `0x10c4`
   but ours is ~0x184c later. This is `GameText.cpp` `.data` ordering.
7. **`.data.rel.ro` `-4` (1 word).** 15 `_ZTI/_ZTV` objects (ModStation, PlayerStatic,
   PlayerFighter, PlayerStation, BoundingVolume, PlayerAsteroid, PlayerCreature,
   PlayerStaticFar) sit 4 bytes before the original; a 4-byte alignment gap is missing before
   the ModStation typeinfo.
8. **`.got` `+4` (542 words) and `-556` (20 words).** GOT slot order follows the dynsym/PLT
   order; this is the same campaign as (9).
9. **PLT entry order (8898 branch words, 717 of 3674 slots differ).** `.plt` content and size
   are exact, but the entry order follows the object section order (gold scans relocations per
   input object). Reordering function definitions to the original first-reference order is the
   w3362/w3363 technique; it is the single largest remaining block.

## 8. Reproduction / verification

```sh
cmake --build cmake-build-match --target verify -j8
cmake --build cmake-build-match --target check  -j8
# expected:
#   verify: byte 2734  linked 4494  stubs 0  missing 0  extra 0  avg 99.9
#   data:   exact 843 of 843 exported objects
#   sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
#   RATCHET PASS
```

The two committed fixes are `tools/gen_exidx_markers.py` (DROP_EMPTY_TEXT) and the three
shader `.cpp` definition reorders. The FModSound reorder is staged in
`/tmp/FModSound_moved.cpp` for the PLT-order campaign.

## Coordinator note (wave 93, byte_exact 4105)

First `.rodata` divergence is at **0x1cafa0**:

```
orig:  ... "se\0FMOD_GOF2\0master\0music\0sfx\0voice\0english\0deutsch\0" ...
ours:  ... "se\0master\0music\0sfx\0voice\0english\0deutsch\0FMOD_GOF2\0" ...
```

`FMOD_GOF2` is `src/engine/audio/FModSound.cpp:155` (inside `FModSound::freeAllEvents`),
`master/music/sfx/voice` is `kCats` at line 93 and `english/deutsch` is `kLangs` at line 127
(both inside `FModSound::init`). So the original TU defines `freeAllEvents` **before** `init`.

Moving `freeAllEvents` above `init` does fix that string order but costs 10 byte_exact because it
changes the `.dynsym`/PLT first-reference order (PLT diff 14 -> 20 lines). The fix must therefore
be done together with the PLT order (move the other FModSound functions so the PLT sequence is
preserved), or by finding a different function that emits `FMOD_GOF2` earlier.

Second divergence block starts at **0x1e3740**: a large run of `data/assets/...` path strings is
reordered (`.aei` texture paths). Map those to their TUs with the section-ordering file.
