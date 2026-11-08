# Radar::draw / `.text` tail veneers — wave 151 / worker w9701

Worker w9701 was killed at the 55-minute budget while exploring how to remove
one gold Thumb->ARM interworking veneer (the wave-150 w9605 lead: one fewer
veneer is exactly -16 B of stub table, the clean shift-free lever for the
Radar String-labelCopy 9,908 B shape).

## Recovered evidence (scratch `/dev/shm/w9701`, now removed)

- `veneers.json` / `ven_orig.txt` / `ven_ours.txt`: the 467 veneer entries in
  the original and our link are identical (address, target, and order).
- `sites.json`: all `b.w` tail-call sites that reference a veneer, with the
  source function and callee symbol.
- `callsites.py`, `ven.py`, `ven2.py`: the extraction/measure scripts.
- `dynsyms.txt`, `und.txt`: the dynamic symbol and undefined-import tables.

## Conclusion (also measured independently by w9702)

Veneer removal is **not byte_exact-neutral**: the minimum cost is 2 (possibly
4 because of the D1/D2 alias rows) `byte_exact` functions, because removing a
middle veneer shifts every later `.text` address and the `b.w` relaxation.
The `.text` tail pad is Android gold's `stub_group_auto_padding`
(`pad = min(veneer_bytes * 0.02, 0x40)`), not a fixed 0x40 blob; w9702's
linker instrumentation confirms the 0x40 pad and shows that forcing the cap
to 0x30 lands the String shape at `.text 0x1337b8` with byte_exact 4507.

No source change was committed by w9701.
