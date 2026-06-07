# PlayerCreature::unhook
elf 0x11cd4e body 24
Sig: undefined __thiscall unhook(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::unhook() */

void __thiscall PlayerCreature::unhook(PlayerCreature *this)

{
  this[0x12a] = (PlayerCreature)0x0;
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined2 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 0x138) = *(undefined4 *)(this + 0x134);
  return;
}

```

## target disasm
```
  0012cd4e: ldr.w r1,[r0,#0x134]
  0012cd52: movs r2,#0x0
  0012cd54: strb.w r2,[r0,#0x12a]
  0012cd58: str.w r2,[r0,#0x130]
  0012cd5c: strh.w r2,[r0,#0x128]
  0012cd60: str.w r1,[r0,#0x138]
  0012cd64: bx lr
```
