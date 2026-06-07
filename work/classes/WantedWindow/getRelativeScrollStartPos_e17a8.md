# WantedWindow::getRelativeScrollStartPos
elf 0xe17a8 body 44
Sig: undefined __thiscall getRelativeScrollStartPos(WantedWindow * this)

## decompile
```c

/* WantedWindow::getRelativeScrollStartPos() */

float __thiscall WantedWindow::getRelativeScrollStartPos(WantedWindow *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = DAT_000f17d4;
  if (*(int *)(this + 0x84) < 1) {
    fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x84),(byte)(in_fpscr >> 0x16) & 3);
    fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0xa4),(byte)(in_fpscr >> 0x16) & 3);
    fVar1 = -fVar1 / fVar2;
  }
  return fVar1;
}

```

## target disasm
```
  000f17a8: ldr.w r1,[r0,#0x84]
  000f17ac: cmp r1,#0x0
  000f17ae: ble 0x000f17b6
  000f17b0: vldr.32 s0,[pc,#0x20]
  000f17b4: b 0x000f17ce
  000f17b6: vmov s0,r1
  000f17ba: vcvt.f32.s32 s0,s0
  000f17be: vldr.32 s2,[r0,#0xa4]
  000f17c2: vcvt.f32.s32 s2,s2
  000f17c6: vneg.f32 s0,s0
  000f17ca: vdiv.f32 s0,s0,s2
  000f17ce: vmov r0,s0
  000f17d2: bx lr
```
