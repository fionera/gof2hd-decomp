# PlayerEgo::isInDockingProcedure
elf 0xa1152 body 22
Sig: undefined __thiscall isInDockingProcedure(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isInDockingProcedure() */

bool __thiscall PlayerEgo::isInDockingProcedure(PlayerEgo *this)

{
  if (*(char *)&this[0xc].field_10 != '\0') {
    return true;
  }
  return *(char *)((int)&this[0x17].field_18 + 2) != '\0';
}

```

## target disasm
```
  000b1152: ldrb.w r1,[r0,#0x1c0]
  000b1156: cbz r1,0x000b115c
  000b1158: movs r0,#0x1
  000b115a: bx lr
  000b115c: ldrb.w r0,[r0,#0x356]
  000b1160: cmp r0,#0x0
  000b1162: it ne
  000b1164: mov.ne r0,#0x1
  000b1166: bx lr
```
