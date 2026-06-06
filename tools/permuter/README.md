# decomp-permuter integration (auto source-search to an exact match)

When a function is *close* (e.g. `ArraySetLength` at 95.8% — one `mov`/`cmp` schedule swap),
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter) randomly rewrites the C and
recompiles until the output matches the target — automating the hand-tweaking we did for ArrayAdd.

Installed in the guest at `/tmp/decomp-permuter` (deps: `python3-pycparser python3-toml`).
`compile.sh` here is the permuter's compile step (NDK r18b clang + our verified flags).

## Per-function setup
1. **Target object** — assemble the target function's disassembly into an object the scorer can read:
   ```sh
   arm-linux-gnueabihf-objdump -d --no-show-raw-insn -M force-thumb \
     --start-address=0x<elf_vaddr> --stop-address=0x<end> libgof2hdaa.so \
     | sed -n 's/^ *[0-9a-f]*:\t//p' > target.s          # clean to bare instructions
   echo '.thumb' | cat - target.s | arm-linux-gnueabihf-as -mthumb -o target.o
   ```
   (vaddr = Ghidra addr − 0x10000; end = vaddr + body bytes from the work-item.)
2. **Base source** — the recovered C/C++ for the function (from `work/<name>.md`).
3. **Import**:
   ```sh
   python3 /tmp/decomp-permuter/import.py base.c '<FunctionName>' --target target.o
   ```
   (or hand-create `nonmatchings/<name>/{base.c,target.o,compile.sh}` using this dir's compile.sh)
4. **Run**:
   ```sh
   python3 /tmp/decomp-permuter/permuter.py nonmatchings/<name>/
   ```
   It searches source variants; a score of 0 = byte-identical. Fold the winning source into `src/`.

## When to use it
- Functions stuck at "near match" due to instruction scheduling / register allocation / temp reuse.
- Not needed for functions that match first try (e.g. ArrayAdd) or are trivial.
- Verify the final result with `tools/gofdiff.py` (the project's quick differ) and `make verify`.
