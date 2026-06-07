# Agent::setStationName
elf 0x177718 body 6
Sig: undefined __stdcall setStationName(String param_1)

## decompile
```c

/* Agent::setStationName(AbyssEngine::String) */

void Agent::setStationName(int param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x78);
  return;
}

```

## target disasm
```
  00187718: adds r0,#0x78
  0018771a: b.w 0x001ac548
```
