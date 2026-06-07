# MiningGame::getOreAmount
elf 0x11eb9e body 14
Sig: undefined __thiscall getOreAmount(MiningGame * this)

## decompile
```c

/* MiningGame::getOreAmount() */

int __thiscall MiningGame::getOreAmount(MiningGame *this)

{
  return (int)*(float *)(this + 0x24);
}

```

## target disasm
```
  0012eb9e: vldr.32 s0,[r0,#0x24]
  0012eba2: vcvt.s32.f32 s0,s0
  0012eba6: vmov r0,s0
  0012ebaa: bx lr
```
