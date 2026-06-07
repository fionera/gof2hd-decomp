# PlayerEgo::explosionEnded
elf 0xa15d4 body 26
Sig: undefined __thiscall explosionEnded(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::explosionEnded() */

bool __thiscall PlayerEgo::explosionEnded(PlayerEgo *this)

{
  if (this[3].field_20 != 0) {
    return 8000 < (int)this[0x15].field_4;
  }
  return true;
}

```

## target disasm
```
  000b15d4: ldr.w r1,[r0,#0x8c]
  000b15d8: cbz r1,0x000b15ea
  000b15da: ldr.w r1,[r0,#0x2f8]
  000b15de: movs r0,#0x0
  000b15e0: cmp.w r1,#0x1f40
  000b15e4: it gt
  000b15e6: mov.gt r0,#0x1
  000b15e8: bx lr
  000b15ea: movs r0,#0x1
  000b15ec: bx lr
```
