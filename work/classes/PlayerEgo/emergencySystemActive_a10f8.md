# PlayerEgo::emergencySystemActive
elf 0xa10f8 body 14
Sig: undefined __thiscall emergencySystemActive(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::emergencySystemActive() */

bool __thiscall PlayerEgo::emergencySystemActive(PlayerEgo *this)

{
  return 0 < (int)this[0x15].field_18;
}

```

## target disasm
```
  000b10f8: ldr.w r1,[r0,#0x30c]
  000b10fc: movs r0,#0x0
  000b10fe: cmp r1,#0x0
  000b1100: it gt
  000b1102: mov.gt r0,#0x1
  000b1104: bx lr
```
