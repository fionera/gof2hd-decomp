# Explosion::update
elf 0xa8820 body 220
Sig: undefined __thiscall update(Explosion * this, int param_1, Vector * param_2)

## decompile
```c

/* Explosion::update(int, AbyssEngine::AEMath::Vector const&) */

void __thiscall Explosion::update(Explosion *this,int param_1,Vector *param_2)

{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  bool bVar4;
  int iVar5;
  undefined4 unaff_r6;
  uint *puVar6;
  longlong lVar7;
  
  if (this[0x20] != (Explosion)0x0) {
    puVar6 = *(uint **)(DAT_000b88fc + 0xb883e);
    lVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
    bVar4 = SUB41(param_1,0);
    AbyssEngine::Transform::Update(lVar7,bVar4);
    if (*(int *)(*(int *)(this + 4) + 0x14) != -1) {
      lVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
      AbyssEngine::Transform::Update(lVar7,bVar4);
    }
    if (*(int *)(this + 8) != 0) {
      uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
      AbyssEngine::Transform::Update((ulonglong)uVar1,bVar4);
    }
    puVar2 = *(uint **)(this + 0xc);
    if (puVar2 != (uint *)0x0) {
      for (uVar1 = 0; uVar1 < *puVar2; uVar1 = uVar1 + 1) {
        AEGeometry::setPosition(*(AEGeometry **)(puVar2[1] + uVar1 * 4),param_2);
        lVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
        AbyssEngine::Transform::Update(lVar7,bVar4);
        puVar2 = *(uint **)(this + 0xc);
      }
    }
    iVar3 = *(int *)(this + 0x14);
    uVar1 = *(uint *)(this + 0x18) + param_1;
    iVar5 = *(int *)(this + 0x1c) + (param_1 >> 0x1f) + (uint)CARRY4(*(uint *)(this + 0x18),param_1)
    ;
    *(uint *)(this + 0x18) = uVar1;
    *(int *)(this + 0x1c) = iVar5;
    if ((int)((iVar3 - iVar5) - (uint)(*(uint *)(this + 0x10) < uVar1)) < 0 !=
        (SBORROW4(iVar3,iVar5) != SBORROW4(iVar3 - iVar5,(uint)(*(uint *)(this + 0x10) < uVar1)))) {
      (*(code *)(DAT_001abbf4 + 0x1abbf8))(this,0,unaff_r6,param_2);
      return;
    }
  }
  return;
}

```

## target disasm
```
  000b8820: push {r4,r5,r6,r7,lr}
  000b8822: add r7,sp,#0xc
  000b8824: push {r5,r6,r7,r8,r9,r10,r11}
  000b8828: mov r4,r0
  000b882a: ldrb.w r0,[r0,#0x20]
  000b882e: mov r9,r1
  000b8830: str r2,[sp,#0x8]
  000b8832: cmp r0,#0x0
  000b8834: beq 0x000b88f4
  000b8836: ldr r0,[0x000b88fc]
  000b8838: ldr r1,[r4,#0x4]
  000b883a: add r0,pc
  000b883c: ldr.w r10,[r0,#0x0]
  000b8840: ldr r1,[r1,#0xc]
  000b8842: ldr.w r0,[r10,#0x0]
  000b8846: blx 0x00072088
  000b884a: asr.w r6,r9, asr #0x1f
  000b884e: movs r5,#0x0
  000b8850: mov r2,r9
  000b8852: mov r3,r6
  000b8854: str r5,[sp,#0x0]
  000b8856: blx 0x0006f7cc
  000b885a: ldr r0,[r4,#0x4]
  000b885c: ldr r1,[r0,#0x14]
  000b885e: adds r0,r1,#0x1
  000b8860: beq 0x000b8874
  000b8862: ldr.w r0,[r10,#0x0]
  000b8866: blx 0x00072088
  000b886a: mov r2,r9
  000b886c: mov r3,r6
  000b886e: str r5,[sp,#0x0]
  000b8870: blx 0x0006f7cc
  000b8874: ldr r0,[r4,#0x8]
  000b8876: cbz r0,0x000b888e
  000b8878: ldr r1,[r0,#0xc]
  000b887a: ldr.w r0,[r10,#0x0]
  000b887e: blx 0x00072088
  000b8882: movs r1,#0x0
  000b8884: mov r2,r9
  000b8886: mov r3,r6
  000b8888: str r1,[sp,#0x0]
  000b888a: blx 0x0006f7cc
  000b888e: ldr r0,[r4,#0xc]
  000b8890: cbz r0,0x000b88cc
  000b8892: mov.w r11,#0x0
  000b8896: mov.w r8,#0x0
  000b889a: b 0x000b88c6
  000b889c: ldr r0,[r0,#0x4]
  000b889e: ldr r1,[sp,#0x8]
  000b88a0: ldr.w r5,[r0,r8,lsl #0x2]
  000b88a4: mov r0,r5
  000b88a6: blx 0x00072148
  000b88aa: ldr.w r0,[r10,#0x0]
  000b88ae: ldr r1,[r5,#0xc]
  000b88b0: blx 0x00072088
  000b88b4: mov r2,r9
  000b88b6: mov r3,r6
  000b88b8: str.w r11,[sp,#0x0]
  000b88bc: blx 0x0006f7cc
  000b88c0: ldr r0,[r4,#0xc]
  000b88c2: add.w r8,r8,#0x1
  000b88c6: ldr r1,[r0,#0x0]
  000b88c8: cmp r8,r1
  000b88ca: bcc 0x000b889c
  000b88cc: add.w r3,r4,#0x10
  000b88d0: ldmia r3,{r0,r1,r2,r3}
  000b88d2: adds.w r2,r2,r9
  000b88d6: adc.w r3,r3,r9, asr #0x1f
  000b88da: subs r0,r0,r2
  000b88dc: sbcs.w r0,r1,r3
  000b88e0: strd r2,r3,[r4,#0x18]
  000b88e4: bge 0x000b88f4
  000b88e6: mov r0,r4
  000b88e8: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000b88ec: pop.w {r4,r5,r6,r7,lr}
  000b88f0: b.w 0x001abbe8
  000b88f4: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000b88f8: pop {r4,r5,r6,r7,pc}
  001abbe8: bx pc
```
