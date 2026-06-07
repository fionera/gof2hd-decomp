# SpaceLounge::OnRenderBG
elf 0x173792 body 14
Sig: undefined __thiscall OnRenderBG(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::OnRenderBG() */

void __thiscall SpaceLounge::OnRenderBG(SpaceLounge *this)

{
  if (*(int *)(this + 0x44) == 0) {
    return;
  }
  (*(code *)(DAT_001ac6c4 + 0x1ac6c8))();
  return;
}

```

## target disasm
```
  00183792: ldr r0,[r0,#0x44]
  00183794: cmp r0,#0x0
  00183796: it ne
  00183798: b.ne.w 0x001ac6b8
  0018379c: bx lr
  001ac6b8: bx pc
```
