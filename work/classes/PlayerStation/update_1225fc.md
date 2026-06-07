# PlayerStation::update
elf 0x1225fc body 220
Sig: undefined __thiscall update(PlayerStation * this, int param_1)

## decompile
```c

/* PlayerStation::update(int) */

void __thiscall PlayerStation::update(PlayerStation *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint *puVar3;
  code *pcVar4;
  code *pcVar5;
  bool bVar6;
  longlong lVar7;
  undefined8 uVar8;
  
  bVar6 = *(int *)(*(int *)(this + 0x140) + 0x14) != -1;
  iVar1 = 0;
  if (bVar6) {
    iVar1 = *(int *)(this + 0x148);
  }
  if ((bVar6 && iVar1 != 0x65) && (iVar1 = Status::inAlienOrbit(), iVar1 == 0)) {
    puVar3 = *(uint **)(DAT_001326dc + 0x13263a);
    lVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
    iVar1 = param_1 >> 0x1f;
    AbyssEngine::Transform::Update(lVar7,SUB41(param_1,0));
    if (*(int *)(this + 0x148) == 100) {
      lVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
      AbyssEngine::Transform::Update(lVar7,SUB41(param_1,0));
    }
    else if (*(int *)(this + 0x148) == 0x6c) {
      pcVar4 = *(code **)(DAT_001326e0 + 0x13266c);
      uVar2 = (*pcVar4)(*puVar3,*(undefined4 *)(this + 0x164));
      pcVar5 = *(code **)(DAT_001326e4 + 0x132680);
      (*pcVar5)(uVar2,(undefined4 *)(DAT_001326e4 + 0x132680),param_1,iVar1,0);
      uVar8 = (*pcVar4)(*puVar3,*(undefined4 *)(this + 0x168));
      (*pcVar5)((int)uVar8,(int)((ulonglong)uVar8 >> 0x20),param_1,iVar1,0);
      uVar8 = (*pcVar4)(*puVar3,*(undefined4 *)(this + 0x16c));
      (*pcVar5)((int)uVar8,(int)((ulonglong)uVar8 >> 0x20),param_1,iVar1,0);
      uVar8 = (*pcVar4)(*puVar3,*(undefined4 *)(this + 0x170));
      (*pcVar5)((int)uVar8,(int)((ulonglong)uVar8 >> 0x20),param_1,iVar1,0);
    }
  }
  return;
}

```

## target disasm
```
  001325fc: push {r4,r5,r6,r7,lr}
  001325fe: add r7,sp,#0xc
  00132600: push {r7,r8,r9,r10,r11}
  00132604: mov r5,r0
  00132606: ldr.w r0,[r0,#0x140]
  0013260a: mov r4,r1
  0013260c: ldr r0,[r0,#0x14]
  0013260e: adds r0,#0x1
  00132610: itt ne
  00132612: ldr.ne.w r0,[r5,#0x148]
  00132616: cmp.ne r0,#0x65
  00132618: bne 0x00132620
  0013261a: pop.w {r3,r8,r9,r10,r11}
  0013261e: pop {r4,r5,r6,r7,pc}
  00132620: ldr r0,[0x001326d8]
  00132622: add r0,pc
  00132624: ldr r0,[r0,#0x0]
  00132626: ldr r0,[r0,#0x0]
  00132628: blx 0x000723d0
  0013262c: cmp r0,#0x0
  0013262e: bne 0x0013261a
  00132630: ldr r0,[0x001326dc]
  00132632: ldr.w r1,[r5,#0x140]
  00132636: add r0,pc
  00132638: ldr r6,[r0,#0x0]
  0013263a: ldr r1,[r1,#0x14]
  0013263c: ldr r0,[r6,#0x0]
  0013263e: blx 0x00072088
  00132642: asr.w r8,r4, asr #0x1f
  00132646: mov.w r9,#0x0
  0013264a: mov r2,r4
  0013264c: mov r3,r8
  0013264e: str.w r9,[sp,#0x0]
  00132652: blx 0x0006f7cc
  00132656: ldr.w r0,[r5,#0x148]
  0013265a: cmp r0,#0x64
  0013265c: beq 0x001326c0
  0013265e: cmp r0,#0x6c
  00132660: bne 0x0013261a
  00132662: ldr r2,[0x001326e0]
  00132664: ldr.w r1,[r5,#0x164]
  00132668: add r2,pc
  0013266a: ldr r0,[r6,#0x0]
  0013266c: ldr.w r9,[r2,#0x0]
  00132670: blx r9
  00132672: ldr r1,[0x001326e4]
  00132674: mov.w r11,#0x0
  00132678: mov r2,r4
  0013267a: mov r3,r8
  0013267c: add r1,pc
  0013267e: str.w r11,[sp,#0x0]
  00132682: ldr.w r10,[r1,#0x0]
  00132686: blx r10
  00132688: ldr.w r1,[r5,#0x168]
  0013268c: ldr r0,[r6,#0x0]
  0013268e: blx r9
  00132690: mov r2,r4
  00132692: mov r3,r8
  00132694: str.w r11,[sp,#0x0]
  00132698: blx r10
  0013269a: ldr.w r1,[r5,#0x16c]
  0013269e: ldr r0,[r6,#0x0]
  001326a0: blx r9
  001326a2: mov r2,r4
  001326a4: mov r3,r8
  001326a6: str.w r11,[sp,#0x0]
  001326aa: blx r10
  001326ac: ldr.w r1,[r5,#0x170]
  001326b0: ldr r0,[r6,#0x0]
  001326b2: blx r9
  001326b4: mov r2,r4
  001326b6: mov r3,r8
  001326b8: str.w r11,[sp,#0x0]
  001326bc: blx r10
  001326be: b 0x0013261a
  001326c0: ldr.w r1,[r5,#0x144]
  001326c4: ldr r0,[r6,#0x0]
  001326c6: blx 0x00072088
  001326ca: mov r2,r4
  001326cc: mov r3,r8
  001326ce: str.w r9,[sp,#0x0]
  001326d2: blx 0x0006f7cc
  001326d6: b 0x0013261a
```
