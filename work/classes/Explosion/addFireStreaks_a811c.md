# Explosion::addFireStreaks
elf 0xa811c body 346
Sig: undefined __thiscall addFireStreaks(Explosion * this)

## decompile
```c

/* Explosion::addFireStreaks() */

void __thiscall Explosion::addFireStreaks(Explosion *this)

{
  undefined4 uVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  Array<AEGeometry*> *this_00;
  int iVar5;
  AEGeometry *pAVar6;
  undefined4 uVar7;
  uint uVar8;
  int *piVar9;
  uint *puVar10;
  code *pcVar11;
  uint in_fpscr;
  float fVar12;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float fVar13;
  
  if (*(int *)(this + 0xc) == 0) {
    this_00 = operator_new(0xc);
    Array<AEGeometry*>::Array(this_00);
    iVar5 = DAT_000b8298;
    *(Array<AEGeometry*> **)(this + 0xc) = this_00;
    piVar9 = *(int **)(iVar5 + 0xb8148);
    iVar5 = AbyssEngine::AERandom::nextInt(*piVar9);
    ArraySetLength<AEGeometry*>(iVar5 + 3,*(Array **)(this + 0xc));
    fVar4 = DAT_000b8294;
    fVar3 = DAT_000b8290;
    fVar2 = DAT_000b828c;
    uVar1 = DAT_000b8288;
    puVar10 = *(uint **)(DAT_000b829c + 0xb8166);
    pcVar11 = *(code **)(DAT_000b82a0 + 0xb817a);
    for (uVar8 = 0; uVar8 < **(uint **)(this + 0xc); uVar8 = uVar8 + 1) {
      pAVar6 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar6,0x37d4,(PaintCanvas *)*puVar10,false);
      *(AEGeometry **)(*(int *)(*(int *)(this + 0xc) + 4) + uVar8 * 4) = pAVar6;
      uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
      AbyssEngine::Transform::SetAnimationState(uVar7,1,0);
      iVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
      *(undefined4 *)(iVar5 + 0xe0) = uVar1;
      pAVar6 = *(AEGeometry **)(*(int *)(*(int *)(this + 0xc) + 4) + uVar8 * 4);
      uVar7 = (*pcVar11)(*piVar9,0x168);
      fVar13 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
      uVar7 = (*pcVar11)(*piVar9,0x168);
      VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
      uVar7 = (*pcVar11)(uVar7,*piVar9,0x168);
      fVar12 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
      AEGeometry::setRotation(pAVar6,(fVar12 / fVar2) * fVar3,extraout_s1,(fVar13 / fVar2) * fVar3);
      iVar5 = (*pcVar11)(*piVar9,0x32);
      fVar12 = (float)VectorSignedToFloat(iVar5 + 0x32,(byte)(in_fpscr >> 0x16) & 3);
      AEGeometry::setScaling(fVar12 / fVar4,extraout_s1_00,extraout_s2);
    }
  }
  return;
}

```

## target disasm
```
  000b811c: push {r4,r5,r6,r7,lr}
  000b811e: add r7,sp,#0xc
  000b8120: push {r7,r8,r9,r10,r11}
  000b8124: vpush {d8,d9,d10,d11,d12}
  000b8128: mov r4,r0
  000b812a: ldr r0,[r0,#0xc]
  000b812c: cmp r0,#0x0
  000b812e: bne.w 0x000b826a
  000b8132: movs r0,#0xc
  000b8134: blx 0x0006eb24
  000b8138: mov r5,r0
  000b813a: blx 0x000717b8
  000b813e: ldr r0,[0x000b8298]
  000b8140: movs r1,#0x7
  000b8142: str r5,[r4,#0xc]
  000b8144: add r0,pc
  000b8146: ldr.w r8,[r0,#0x0]
  000b814a: ldr.w r0,[r8,#0x0]
  000b814e: blx 0x00071848
  000b8152: ldr r1,[r4,#0xc]
  000b8154: adds r0,#0x3
  000b8156: blx 0x00073504
  000b815a: ldr r0,[0x000b829c]
  000b815c: movs r6,#0x0
  000b815e: vldr.32 s16,[pc,#0x12c]
  000b8162: add r0,pc
  000b8164: vldr.32 s18,[pc,#0x128]
  000b8168: vldr.32 s20,[pc,#0x128]
  000b816c: ldr.w r9,[r0,#0x0]
  000b8170: ldr r0,[0x000b8288]
  000b8172: mov r11,r0
  000b8174: ldr r0,[0x000b82a0]
  000b8176: add r0,pc
  000b8178: ldr.w r10,[r0,#0x0]
  000b817c: b 0x000b8262
  000b817e: movs r0,#0xc0
  000b8180: blx 0x0006eb24
  000b8184: mov r5,r0
  000b8186: ldr.w r2,[r9,#0x0]
  000b818a: movw r1,#0x37d4
  000b818e: movs r3,#0x0
  000b8190: blx 0x0007207c
  000b8194: ldr r0,[r4,#0xc]
  000b8196: ldr r0,[r0,#0x4]
  000b8198: str.w r5,[r0,r6,lsl #0x2]
  000b819c: ldr r0,[r4,#0xc]
  000b819e: ldr r0,[r0,#0x4]
  000b81a0: ldr.w r1,[r0,r6,lsl #0x2]
  000b81a4: ldr.w r0,[r9,#0x0]
  000b81a8: ldr r1,[r1,#0xc]
  000b81aa: blx 0x00072088
  000b81ae: movs r1,#0x1
  000b81b0: movs r2,#0x0
  000b81b2: blx 0x0006fd18
  000b81b6: ldr r0,[r4,#0xc]
  000b81b8: ldr r0,[r0,#0x4]
  000b81ba: ldr.w r1,[r0,r6,lsl #0x2]
  000b81be: ldr.w r0,[r9,#0x0]
  000b81c2: ldr r1,[r1,#0xc]
  000b81c4: blx 0x00072088
  000b81c8: str.w r11,[r0,#0xe0]
  000b81cc: ldr r0,[r4,#0xc]
  000b81ce: ldr r1,[r0,#0x4]
  000b81d0: ldr.w r0,[r8,#0x0]
  000b81d4: ldr.w r5,[r1,r6,lsl #0x2]
  000b81d8: mov.w r1,#0x168
  000b81dc: blx r10
  000b81de: vmov s0,r0
  000b81e2: mov.w r1,#0x168
  000b81e6: vcvt.f32.s32 s22,s0
  000b81ea: ldr.w r0,[r8,#0x0]
  000b81ee: blx r10
  000b81f0: ldr.w r1,[r8,#0x0]
  000b81f4: vmov s0,r0
  000b81f8: vcvt.f32.s32 s24,s0
  000b81fc: mov r0,r1
  000b81fe: mov.w r1,#0x168
  000b8202: blx r10
  000b8204: vmov s0,r0
  000b8208: mov r0,r5
  000b820a: vdiv.f32 s2,s22,s16
  000b820e: vcvt.f32.s32 s0,s0
  000b8212: vdiv.f32 s4,s24,s16
  000b8216: vdiv.f32 s0,s0,s16
  000b821a: vmul.f32 s2,s2,s18
  000b821e: vmul.f32 s4,s4,s18
  000b8222: vmul.f32 s0,s0,s18
  000b8226: vmov r1,s2
  000b822a: vmov r2,s4
  000b822e: vmov r3,s0
  000b8232: blx 0x00073054
  000b8236: ldr.w r0,[r8,#0x0]
  000b823a: movs r1,#0x32
  000b823c: blx r10
  000b823e: adds r0,#0x32
  000b8240: vmov s0,r0
  000b8244: vcvt.f32.s32 s0,s0
  000b8248: ldr r0,[r4,#0xc]
  000b824a: ldr r0,[r0,#0x4]
  000b824c: ldr.w r0,[r0,r6,lsl #0x2]
  000b8250: vdiv.f32 s0,s0,s20
  000b8254: vmov r1,s0
  000b8258: mov r2,r1
  000b825a: mov r3,r1
  000b825c: blx 0x000727b4
  000b8260: adds r6,#0x1
  000b8262: ldr r0,[r4,#0xc]
  000b8264: ldr r0,[r0,#0x0]
  000b8266: cmp r6,r0
  000b8268: bcc 0x000b817e
  000b826a: vpop {d8,d9,d10,d11,d12}
  000b826e: add sp,#0x4
  000b8270: pop.w {r8,r9,r10,r11}
  000b8274: pop {r4,r5,r6,r7,pc}
```
