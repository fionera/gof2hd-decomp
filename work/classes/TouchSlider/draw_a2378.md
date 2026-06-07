# TouchSlider::draw
elf 0xa2378 body 68
Sig: undefined __thiscall draw(TouchSlider * this)

## decompile
```c

/* TouchSlider::draw() */

void __thiscall TouchSlider::draw(TouchSlider *this)

{
  uint *puVar1;
  
  puVar1 = *(uint **)(DAT_000b23bc + 0xb238c);
  AbyssEngine::PaintCanvas::SetColor(*puVar1);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar1,*(int *)(this + 0x2c),*(int *)this);
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar1,*(int *)(this + 0x30),*(int *)(this + 8),(uchar)*(undefined4 *)(this + 0xc),
             '\x11');
  return;
}

```

## target disasm
```
  000b2378: push {r2,r3,r4,r5,r7,lr}
  000b237a: add r7,sp,#0x10
  000b237c: mov r4,r0
  000b237e: ldr r0,[0x000b23bc]
  000b2380: ldrb.w r2,[r4,#0x35]
  000b2384: mvn r1,#0xd0
  000b2388: add r0,pc
  000b238a: cmp r2,#0x0
  000b238c: ldr r5,[r0,#0x0]
  000b238e: ldr r0,[r5,#0x0]
  000b2390: it eq
  000b2392: mov.eq.w r1,#0xffffffff
  000b2396: blx 0x0006fac0
  000b239a: ldrd r2,r3,[r4,#0x0]
  000b239e: ldr r1,[r4,#0x2c]
  000b23a0: ldr r0,[r5,#0x0]
  000b23a2: blx 0x00072dc0
  000b23a6: ldrd r2,r3,[r4,#0x8]
  000b23aa: ldr r1,[r4,#0x30]
  000b23ac: movs r4,#0x11
  000b23ae: ldr r0,[r5,#0x0]
  000b23b0: movs r5,#0x44
  000b23b2: strd r4,r5,[sp,#0x0]
  000b23b6: blx 0x00071d70
  000b23ba: pop {r2,r3,r4,r5,r7,pc}
```
