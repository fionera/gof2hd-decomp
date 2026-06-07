# Layout::drawFade
elf 0xd5038 body 246
Sig: undefined __thiscall drawFade(Layout * this)

## decompile
```c

/* Layout::drawFade() */

Layout __thiscall Layout::drawFade(Layout *this)

{
  uint *puVar1;
  uint in_fpscr;
  
  if (this[0x400] != (Layout)0x0) {
    puVar1 = *(uint **)(DAT_000e5134 + 0xe5052);
    AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar1);
    VectorSignedToFloat(*(undefined4 *)(this + 0x40c),(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(*(undefined4 *)(this + 0x408),(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::PaintCanvas::SetColor(*puVar1);
    AbyssEngine::PaintCanvas::FillRectangle(*puVar1,0,0,**(int **)(DAT_000e5138 + 0xe50c8));
    AbyssEngine::PaintCanvas::SetColor(*puVar1);
  }
  if (this[0x410] != (Layout)0x0) {
    puVar1 = *(uint **)(DAT_000e5140 + 0xe50ee);
    AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar1);
    AbyssEngine::PaintCanvas::SetColor(*puVar1);
    AbyssEngine::PaintCanvas::FillRectangle(*puVar1,0,0,**(int **)(DAT_000e5144 + 0xe5108));
    AbyssEngine::PaintCanvas::SetColor(*puVar1);
  }
  return this[0x400];
}

```

## target disasm
```
  000e5038: push {r4,r5,r6,r7,lr}
  000e503a: add r7,sp,#0xc
  000e503c: push.w r11
  000e5040: sub sp,#0x8
  000e5042: mov r4,r0
  000e5044: ldrb.w r0,[r0,#0x400]
  000e5048: cmp r0,#0x0
  000e504a: beq 0x000e50e2
  000e504c: ldr r0,[0x000e5134]
  000e504e: add r0,pc
  000e5050: ldr r6,[r0,#0x0]
  000e5052: ldr r0,[r6,#0x0]
  000e5054: blx 0x0006fa90
  000e5058: mov r5,r0
  000e505a: addw r0,r4,#0x40c
  000e505e: vldr.32 s0,[r0]
  000e5062: add.w r0,r4,#0x408
  000e5066: vldr.32 s2,[r0]
  000e506a: vcvt.f32.s32 s0,s0
  000e506e: vcvt.f32.s32 s2,s2
  000e5072: ldrb.w r0,[r4,#0x401]
  000e5076: cmp r0,#0x0
  000e5078: vdiv.f32 s0,s2,s0
  000e507c: vmov.f32 s2,0x3f800000
  000e5080: vsub.f32 s4,s2,s0
  000e5084: it eq
  000e5086: vmov.eq.f32 s0,s4
  000e508a: vcmpe.f32 s0,s2
  000e508e: vmrs apsr,fpscr
  000e5092: it mi
  000e5094: vmov.mi.f32 s2,s0
  000e5098: vldr.32 s0,[pc,#0x94]
  000e509c: ldr.w r1,[r4,#0x404]
  000e50a0: vcmpe.f32 s2,#0
  000e50a4: vmul.f32 s0,s2,s0
  000e50a8: vmrs apsr,fpscr
  000e50ac: vcvt.s32.f32 s0,s0
  000e50b0: ldr r0,[r6,#0x0]
  000e50b2: vmov r2,s0
  000e50b6: it gt
  000e50b8: add.gt r1,r2
  000e50ba: blx 0x0006fac0
  000e50be: ldr r0,[0x000e5138]
  000e50c0: movs r2,#0x0
  000e50c2: ldr r1,[0x000e513c]
  000e50c4: add r0,pc
  000e50c6: add r1,pc
  000e50c8: ldr r0,[r0,#0x0]
  000e50ca: ldr r1,[r1,#0x0]
  000e50cc: ldr r3,[r0,#0x0]
  000e50ce: ldr r0,[r6,#0x0]
  000e50d0: ldr r1,[r1,#0x0]
  000e50d2: str r1,[sp,#0x0]
  000e50d4: movs r1,#0x0
  000e50d6: blx 0x00074de8
  000e50da: ldr r0,[r6,#0x0]
  000e50dc: mov r1,r5
  000e50de: blx 0x0006fac0
  000e50e2: ldrb.w r0,[r4,#0x410]
  000e50e6: cbz r0,0x000e5122
  000e50e8: ldr r0,[0x000e5140]
  000e50ea: add r0,pc
  000e50ec: ldr r6,[r0,#0x0]
  000e50ee: ldr r0,[r6,#0x0]
  000e50f0: blx 0x0006fa90
  000e50f4: mov r5,r0
  000e50f6: ldr r0,[r6,#0x0]
  000e50f8: movs r1,#0xff
  000e50fa: blx 0x0006fac0
  000e50fe: ldr r0,[0x000e5144]
  000e5100: movs r2,#0x0
  000e5102: ldr r1,[0x000e5148]
  000e5104: add r0,pc
  000e5106: add r1,pc
  000e5108: ldr r0,[r0,#0x0]
  000e510a: ldr r1,[r1,#0x0]
  000e510c: ldr r3,[r0,#0x0]
  000e510e: ldr r0,[r6,#0x0]
  000e5110: ldr r1,[r1,#0x0]
  000e5112: str r1,[sp,#0x0]
  000e5114: movs r1,#0x0
  000e5116: blx 0x00074de8
  000e511a: ldr r0,[r6,#0x0]
  000e511c: mov r1,r5
  000e511e: blx 0x0006fac0
  000e5122: ldrb.w r0,[r4,#0x400]
  000e5126: add sp,#0x8
  000e5128: pop.w r11
  000e512c: pop {r4,r5,r6,r7,pc}
```
