# PlayerStation::render
elf 0x1226e8 body 16
Sig: undefined __thiscall render(PlayerStation * this)

## decompile
```c

/* PlayerStation::render() */

void __thiscall PlayerStation::render(PlayerStation *this)

{
  if (this[0xf5] == (PlayerStation)0x0) {
    return;
  }
  (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(this + 0x140));
  return;
}

```

## target disasm
```
  001326e8: ldrb.w r1,[r0,#0xf5]
  001326ec: cbz r1,0x001326f6
  001326ee: ldr.w r0,[r0,#0x140]
  001326f2: b.w 0x001abdc8
  001326f6: bx lr
```
