# MarqueeImage::MarqueeImage
elf 0x15f59c body 78
Sig: undefined __thiscall MarqueeImage(MarqueeImage * this, ushort param_1, int param_2, int param_3, int param_4, float param_5)

## decompile
```c

/* MarqueeImage::MarqueeImage(unsigned short, int, int, int, float) */

MarqueeImage * __thiscall
MarqueeImage::MarqueeImage
          (MarqueeImage *this,ushort param_1,int param_2,int param_3,int param_4,float param_5)

{
  undefined4 uVar1;
  uint *puVar2;
  undefined4 in_stack_00000004;
  
  puVar2 = *(uint **)(DAT_0016f5ec + 0x16f5b0);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar2,param_1,(uint *)this);
  *(int *)(this + 0xc) = param_3;
  *(int *)(this + 0x10) = param_4;
  *(int *)(this + 0x14) = param_2;
  *(undefined4 *)(this + 0x1c) = in_stack_00000004;
  uVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar2);
  *(undefined4 *)(this + 4) = uVar1;
  uVar1 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar2);
  *(undefined4 *)(this + 8) = uVar1;
  *(undefined4 *)(this + 0x18) = 0;
  return this;
}

```

## target disasm
```
  0016f59c: push {r4,r5,r6,r7,lr}
  0016f59e: add r7,sp,#0xc
  0016f5a0: push.w r8
  0016f5a4: mov r4,r0
  0016f5a6: ldr r0,[0x0016f5ec]
  0016f5a8: mov r8,r2
  0016f5aa: mov r2,r4
  0016f5ac: add r0,pc
  0016f5ae: mov r5,r3
  0016f5b0: ldr r6,[r0,#0x0]
  0016f5b2: ldr r0,[r6,#0x0]
  0016f5b4: blx 0x00071cf8
  0016f5b8: ldr r0,[r7,#0x8]
  0016f5ba: vldr.32 s0,[r7,#0xc]
  0016f5be: ldr r1,[r4,#0x0]
  0016f5c0: strd r5,r0,[r4,#0xc]
  0016f5c4: str.w r8,[r4,#0x14]
  0016f5c8: vstr.32 s0,[r4,#0x1c]
  0016f5cc: ldr r0,[r6,#0x0]
  0016f5ce: blx 0x00072d84
  0016f5d2: ldr r1,[r4,#0x0]
  0016f5d4: str r0,[r4,#0x4]
  0016f5d6: ldr r0,[r6,#0x0]
  0016f5d8: blx 0x00072d90
  0016f5dc: str r0,[r4,#0x8]
  0016f5de: movs r0,#0x0
  0016f5e0: str r0,[r4,#0x18]
  0016f5e2: mov r0,r4
  0016f5e4: pop.w r8
  0016f5e8: pop {r4,r5,r6,r7,pc}
```
