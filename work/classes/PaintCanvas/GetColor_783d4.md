# PaintCanvas::GetColor
elf 0x783d4 body 82
Sig: undefined __thiscall GetColor(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetColor() */

int __thiscall AbyssEngine::PaintCanvas::GetColor(PaintCanvas *this)

{
  return (int)(*(float *)(this + 0x200) * DAT_00088428) * 0x10000 +
         (int)(*(float *)(this + 0x1fc) * DAT_00088428) * 0x1000000 +
         (int)(*(float *)(this + 0x204) * DAT_00088428) * 0x100 +
         (int)(*(float *)(this + 0x208) * DAT_00088428);
}

```

## target disasm
```
  000883d4: vldr.32 s0,[pc,#0x50]
  000883d8: vldr.32 s4,[r0,#0x200]
  000883dc: vldr.32 s6,[r0,#0x204]
  000883e0: vldr.32 s8,[r0,#0x208]
  000883e4: vmul.f32 s4,s4,s0
  000883e8: vldr.32 s2,[r0,#0x1fc]
  000883ec: vmul.f32 s6,s6,s0
  000883f0: vmul.f32 s8,s8,s0
  000883f4: vmul.f32 s0,s2,s0
  000883f8: vcvt.s32.f32 s2,s4
  000883fc: vcvt.s32.f32 s4,s6
  00088400: vcvt.s32.f32 s6,s8
  00088404: vcvt.s32.f32 s0,s0
  00088408: vmov r0,s2
  0008840c: vmov r1,s0
  00088410: lsls r0,r0,#0x10
  00088412: add.w r0,r0,r1, lsl #0x18
  00088416: vmov r1,s4
  0008841a: add.w r0,r0,r1, lsl #0x8
  0008841e: vmov r1,s6
  00088422: add r0,r1
  00088424: bx lr
```
