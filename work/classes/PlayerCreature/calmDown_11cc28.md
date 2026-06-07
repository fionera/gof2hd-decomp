# PlayerCreature::calmDown
elf 0x11cc28 body 20
Sig: undefined __thiscall calmDown(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::calmDown() */

void __thiscall PlayerCreature::calmDown(PlayerCreature *this)

{
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined2 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 0x138) = *(undefined4 *)(this + 0x134);
  return;
}

```

## target disasm
```
  0012cc28: ldr.w r1,[r0,#0x134]
  0012cc2c: movs r2,#0x0
  0012cc2e: str.w r2,[r0,#0x130]
  0012cc32: strh.w r2,[r0,#0x128]
  0012cc36: str.w r1,[r0,#0x138]
  0012cc3a: bx lr
```
