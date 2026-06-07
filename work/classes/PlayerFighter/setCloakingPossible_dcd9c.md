# PlayerFighter::setCloakingPossible
elf 0xdcd9c body 32
Sig: undefined __thiscall setCloakingPossible(PlayerFighter * this, bool param_1)

## decompile
```c

/* PlayerFighter::setCloakingPossible(bool) */

void __thiscall PlayerFighter::setCloakingPossible(PlayerFighter *this,bool param_1)

{
  this[0x2d8] = (PlayerFighter)param_1;
  if ((!param_1) && (this[0x13c] != (PlayerFighter)0x0)) {
    *(int *)(this + 0x2c8) = *(int *)(this + 0x2cc) + 1;
    (*(code *)(DAT_001ac1e4 + 0x1ac1e8))();
    return;
  }
  return;
}

```

## target disasm
```
  000ecd9c: cmp r1,#0x0
  000ecd9e: strb.w r1,[r0,#0x2d8]
  000ecda2: bne 0x000ecdb8
  000ecda4: ldrb.w r1,[r0,#0x13c]
  000ecda8: cbz r1,0x000ecdb8
  000ecdaa: ldr.w r1,[r0,#0x2cc]
  000ecdae: adds r1,#0x1
  000ecdb0: str.w r1,[r0,#0x2c8]
  000ecdb4: b.w 0x001ac1d8
  000ecdb8: bx lr
  001ac1d8: bx pc
```
