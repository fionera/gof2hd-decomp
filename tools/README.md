# GOF2 asset tools

Reverse-engineered from the AbyssEngine loaders in `libgof2hdaa.so`. All formats are shared
across platforms (Windows/iOS/Android) since they come from the same engine.

## `aei.py` — AbyssEngine `.aei` textures
`"AEimage\0"` · `u8 fmt` · `u16 w` · `u16 h` · `u16 n_meta` · skip `n_meta*8` · pixels.
- `fmt` 0x01/0x03/0x40/0x42/0x81 → uncompressed RGBA8888 (`w*h*4`); others → `u32 len`+GPU-compressed blob.
- `aei.py info <dir>` (validated on 1354 files, 0 errors), `aei.py png <f>` (RGBA→PNG), `aei.py raw <f>`.
- TODO: decode the GPU-compressed minority (PVRTC/ETC — codes 0x0d/0x10/0x12/0x24) or pipe to PVRTexToolCLI.

## `gofbin.py` — `data/bin/*.bin` tables + `.lang`
Java `DataInputStream` style: **big-endian** ints, strings = `u16 len + UTF-8` (writeUTF).
- `names <f>`   — name pools: `u32 count` + `count×UTF`. ✓
- `items <f>`   — 233 records (Win build cut to 196), each = 3 int-arrays
                  (blueprint ingredient ids / counts / ItemInfo[30]). Schema from `FileRead::loadItemsBinary`. ✓
- `lang <f>`    — flat `writeUTF` stream (de.lang = 3385 strings). ✓
- `dump <f>`    — generic big-endian int/UTF token stream for not-yet-schema'd tables.

### Adding a schema for the other tables
ships.bin, systems.bin, agents.bin, weapons.bin, stations.bin, shipparts.bin, stationparts.bin,
wanted.bin, ticker.bin, collision*.bin each have a loader `FileRead::load<Name>Binary` in the
binary. Decompile it (it's a sequence of `AEFile::ReadSwitched` (i32) and `readUTF` calls) and
transcribe the per-record field order, exactly as done for `items`. The recovered structs in
`include/gof2_types.h` (SingleSystem, SingleAgent, ShipInfo, SingleStation…) give the field names.

## Matching pipeline (Phase A — the per-function decomp loop)
The loop: **extract context → author C++ → diff → (permute if stuck) → register → CI-gate.**
- `Gof2ExtractCtx.java` — Ghidra script: `Gof2ExtractCtx <ghidra_addr>` dumps `work/<name>.md`
  (decompile + target disasm + callees + ready-to-run diff command).
- `gofdiff.py` — `--so <lib> --vaddr <elf_hex> --n <bytes> --obj <built.o> --sym <substr>`:
  disassembles target vs built (Thumb), normalizes, prints **MATCH %** + unified diff. Validated:
  ArrayAdd/ArrayRemoveAll 100%, ArraySetLength 95.8%.
- `permuter/` — decomp-permuter glue (compile.sh + recipe) for auto-searching source variants on
  near-matches. (cloned at `/tmp/decomp-permuter`; asm-differ at `/tmp/asm-differ`.)
- `Gof2BSim.java` — BSim build/query (same-compiler/within-binary; cross-compiler is low-yield, see ROADMAP).
- `coverage.py` (`make coverage`) — recovered vs target function counts, by subsystem (reads `matches.tsv`).
- `verify.sh` (`make verify`) — rebuilds + re-diffs every `matches.tsv` entry; CI regression gate
  (exact must stay 100%, near >=90%).

Workflow per function: pick a target → `Gof2ExtractCtx` → write `src/<subsystem>/*.cpp` → `make asm`
+ `gofdiff` → iterate (or permuter) to exact → add a line to `matches.tsv` → `make verify`.

## FMOD audio (`*.fev`, `*.fsb`)
These are stock **FMOD Designer / FMOD Ex** containers (the game links FMOD Ex — `libfmodex`,
`fmod_event`). Don't reimplement: use existing tooling.
- `.fsb` (sound banks): `fsbext`, `python-fsb5`, or `vgmstream` to extract/decode the samples.
- `.fev` (event projects): open with the FMOD Designer (matching the linked FMOD Ex version) or
  parse with `libfmodevent`. The event names are also visible as strings in the binary.
Filenames present: FMOD_GOF2.fev, FMOD_GOF2_*.fsb (MUSIC, VOICE, SFX_*, CUTSCENES, DLC_*).
