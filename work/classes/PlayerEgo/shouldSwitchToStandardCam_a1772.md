# PlayerEgo::shouldSwitchToStandardCam
elf 0xa1772 body 22
Sig: undefined __thiscall shouldSwitchToStandardCam(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::shouldSwitchToStandardCam() */

undefined4 __thiscall PlayerEgo::shouldSwitchToStandardCam(PlayerEgo *this)

{
  if ((char)this[4].field_20 == '\0') {
    return 0;
  }
  *(undefined1 *)&this[4].field_20 = 0;
  return 1;
}

```

## target disasm
```
  000b1772: ldrb.w r2,[r0,#0xb0]
  000b1776: mov r1,r0
  000b1778: movs r0,#0x0
  000b177a: cmp r2,#0x0
  000b177c: it eq
  000b177e: bx.eq lr
  000b1780: strb.w r0,[r1,#0xb0]
  000b1784: movs r0,#0x1
  000b1786: bx lr
```
