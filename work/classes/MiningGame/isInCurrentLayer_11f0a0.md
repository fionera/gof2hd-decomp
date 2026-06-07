# MiningGame::isInCurrentLayer
elf 0x11f0a0 body 128
Sig: undefined __thiscall isInCurrentLayer(MiningGame * this)

## decompile
```c

/* MiningGame::isInCurrentLayer() */

bool __thiscall MiningGame::isInCurrentLayer(MiningGame *this)

{
  float extraout_r0;
  int iVar1;
  uint in_fpscr;
  float fVar2;
  float fVar3;
  undefined8 uVar4;
  
  uVar4 = FixedToFP(*(undefined8 *)(this + 0x58),0,0,0,0x20);
  uVar4 = FloatVectorSub(*(undefined8 *)(this + 0x10),uVar4,2,0x20);
  uVar4 = FloatVectorMult(uVar4,uVar4,2,0x20);
  iVar1 = *(int *)(*(int *)(DAT_0012f128 + 0x12f0c8) + (7 - *(int *)(this + 0x7c)) * 0x1c +
                  *(int *)(this + 0x78) * 4);
  fVar2 = (float)((ulonglong)uVar4 >> 0x20);
  fVar3 = *(float *)(**(int **)(DAT_0012f124 + 0x12f0ce) + 0xe8);
  Globals::sqrt((Globals *)**(undefined4 **)(DAT_0012f120 + 0x12f0be),
                (double)CONCAT44(fVar2,(float)uVar4 + fVar2));
  fVar2 = (float)VectorSignedToFloat(iVar1 / 2,(byte)(in_fpscr >> 0x16) & 3);
  return (int)((uint)(extraout_r0 < fVar3 * fVar2) << 0x1f) < 0;
}

```

## target disasm
```
  0012f0a0: push {r4,r6,r7,lr}
  0012f0a2: add r7,sp,#0x8
  0012f0a4: vpush {d8}
  0012f0a8: vldr.64 d17,[r0,#0x58]
  0012f0ac: vcvt.f32.s32 d17,d17
  0012f0b0: vldr.64 d16,[r0,#0x10]
  0012f0b4: ldr r2,[0x0012f120]
  0012f0b6: ldr r3,[0x0012f124]
  0012f0b8: ldr r4,[0x0012f128]
  0012f0ba: add r2,pc
  0012f0bc: vsub.f32 d16,d16,d17
  0012f0c0: ldrd r12,r0,[r0,#0x78]
  0012f0c4: add r4,pc
  0012f0c6: rsb.w r0,r0,#0x7
  0012f0ca: add r3,pc
  0012f0cc: ldr r2,[r2,#0x0]
  0012f0ce: ldr r4,[r4,#0x0]
  0012f0d0: rsb r0,r0,r0, lsl #0x3
  0012f0d4: ldr r3,[r3,#0x0]
  0012f0d6: vmul.f32 d0,d16,d16
  0012f0da: add.w r0,r4,r0, lsl #0x2
  0012f0de: ldr r3,[r3,#0x0]
  0012f0e0: ldr.w r4,[r0,r12,lsl #0x2]
  0012f0e4: ldr r0,[r2,#0x0]
  0012f0e6: vadd.f32 s0,s0,s1
  0012f0ea: vldr.32 s16,[r3,#0xe8]
  0012f0ee: vmov r1,s0
  0012f0f2: blx 0x000760e4
  0012f0f6: add.w r1,r4,r4, lsr #0x1f
  0012f0fa: vmov s2,r0
  0012f0fe: movs r0,#0x0
  0012f100: asrs r1,r1,#0x1
  0012f102: vmov s0,r1
  0012f106: vcvt.f32.s32 s0,s0
  0012f10a: vmul.f32 s0,s16,s0
  0012f10e: vcmpe.f32 s2,s0
  0012f112: vmrs apsr,fpscr
  0012f116: it mi
  0012f118: mov.mi r0,#0x1
  0012f11a: vpop {d8}
  0012f11e: pop {r4,r6,r7,pc}
```
