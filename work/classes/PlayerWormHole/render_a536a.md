# PlayerWormHole::render
elf 0xa536a body 14
Sig: undefined __thiscall render(PlayerWormHole * this)

## decompile
```c

/* PlayerWormHole::render() */

void __thiscall PlayerWormHole::render(PlayerWormHole *this)

{
  if (this[0xf5] == (PlayerWormHole)0x0) {
    return;
  }
  (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(this + 8));
  return;
}

```

## target disasm
```
  000b536a: ldrb.w r1,[r0,#0xf5]
  000b536e: cbz r1,0x000b5376
  000b5370: ldr r0,[r0,#0x8]
  000b5372: b.w 0x001abdc8
  000b5376: bx lr
```
