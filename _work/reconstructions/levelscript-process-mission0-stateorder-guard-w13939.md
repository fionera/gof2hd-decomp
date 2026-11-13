# LevelScript::process — wave 192 / w13939: mission-0 state-3/4 order and state-5 guard

Target `_ZN11LevelScript7processEi` (`LevelScript::process`, original
`0x137d18`), **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w13939`, base master
`0ff8ef0a5` (wave 192).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13939`.

## Baseline (pristine HEAD, rebuilt this session)

```
process raw diffbytes    53,209
process aligned insns    18,127
orig / ours size         57,232 / 57,232   (size-exact)
orig / ours insns        20,830 / 20,723
```

## Test 1 — move the `m_nState == 3` block above `m_nState == 4` only

Bodies verbatim; state-3 block (camera drift + radio #8 handback) placed
before the state-4 cutscene-exit teardown, state-5 block left in place after
state 4.

```
usize                    56,920  (-312)  -> SIZE-FATAL
raw diffbytes            55,367
aligned insns            20,281
uinsn                    20,612
```

## Test 2 — split the state-5 guard into a nested `if`

`else if (m_nState == 5 && ((RadioMessage *) ((*messages)[12]))->isOver())`
became `else if (m_nState == 5) { if (((RadioMessage *) ((*messages)[12]))->isOver()) { ... } }`,
state-3/4 order untouched.

```
usize                    57,224  (-8)    -> SIZE-FATAL
raw diffbytes            55,094
aligned insns            20,188
uinsn                    20,721
```

## Conclusion

Neither edit is size-exact; both shrink the function by 312 / 8 bytes and
raise aligned bytes substantially (18,127 -> 20,281 / 20,188).  Both were
reverted; tree is pristine HEAD.  The original's state-3/4/5 block order and
the single-compound state-5 guard as currently written are the size-correct
shapes.
