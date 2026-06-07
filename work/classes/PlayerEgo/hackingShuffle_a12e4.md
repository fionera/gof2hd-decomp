# PlayerEgo::hackingShuffle
elf 0xa12e4 body 16
Sig: undefined __thiscall hackingShuffle(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::hackingShuffle() */

void __thiscall PlayerEgo::hackingShuffle(PlayerEgo *this)

{
  if (this[0xd].field_14 == (int *)0x0) {
    return;
  }
  (*(code *)(DAT_001abbd4 + 0x1abbd8))();
  return;
}

```

## target disasm
```
  000b12e4: ldr.w r0,[r0,#0x1e8]
  000b12e8: cmp r0,#0x0
  000b12ea: it ne
  000b12ec: b.ne.w 0x001abbc8
  000b12f0: bx lr
  001abbc8: bx pc
```
