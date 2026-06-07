# PlayerFixedObject::moveForward
elf 0x154134 body 166
Sig: undefined __thiscall moveForward(PlayerFixedObject * this, int param_1)

## decompile
```c

/* PlayerFixedObject::moveForward(int) */

void __thiscall PlayerFixedObject::moveForward(PlayerFixedObject *this,int param_1)

{
  Matrix *pMVar1;
  uint *puVar2;
  int *piVar3;
  uint uVar4;
  int *piVar5;
  uint in_fpscr;
  float fVar6;
  Vector aVStack_28 [12];
  int local_1c;
  
  fVar6 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  piVar5 = *(int **)(DAT_001641dc + 0x16414e);
  local_1c = *piVar5;
  *(int *)(this + 0x180) = param_1 + *(int *)(this + 0x180);
  AEGeometry::moveForward(fVar6);
  pMVar1 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),pMVar1);
  AEGeometry::getPosition();
  fVar6 = (float)AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x2c),aVStack_28);
  if (*(int *)(this + 0x124) != 0) {
    AEGeometry::moveForward(fVar6);
  }
  puVar2 = *(uint **)(this + 0x128);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      piVar3 = *(int **)(puVar2[1] + uVar4 * 4);
      (**(code **)(*piVar3 + 4))
                (piVar3,*(undefined4 *)(this + 0x2c),*(undefined4 *)(this + 0x30),
                 *(undefined4 *)(this + 0x34));
      puVar2 = *(uint **)(this + 0x128);
    }
  }
  if (*piVar5 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00164134: push {r4,r5,r6,r7,lr}
  00164136: add r7,sp,#0xc
  00164138: push.w r8
  0016413c: sub sp,#0x10
  0016413e: vmov s0,r1
  00164142: mov r4,r0
  00164144: vcvt.f32.s32 s0,s0
  00164148: ldr r0,[0x001641dc]
  0016414a: add r0,pc
  0016414c: ldr.w r8,[r0,#0x0]
  00164150: vmov r5,s0
  00164154: ldr.w r0,[r8,#0x0]
  00164158: str r0,[sp,#0xc]
  0016415a: ldr.w r2,[r4,#0x180]
  0016415e: ldr r0,[r4,#0x8]
  00164160: add r1,r2
  00164162: str.w r1,[r4,#0x180]
  00164166: mov r1,r5
  00164168: blx 0x000725c8
  0016416c: ldr r0,[r4,#0x8]
  0016416e: blx 0x000721cc
  00164172: mov r1,r0
  00164174: ldr r0,[r4,#0x4]
  00164176: adds r0,#0x4
  00164178: blx 0x0006f148
  0016417c: ldr r1,[r4,#0x8]
  0016417e: mov r6,sp
  00164180: mov r0,r6
  00164182: blx 0x000720b8
  00164186: add.w r0,r4,#0x2c
  0016418a: mov r1,r6
  0016418c: blx 0x0006eb3c
  00164190: ldr.w r0,[r4,#0x124]
  00164194: cbz r0,0x0016419c
  00164196: mov r1,r5
  00164198: blx 0x000725c8
  0016419c: ldr.w r0,[r4,#0x128]
  001641a0: cbz r0,0x001641c4
  001641a2: movs r5,#0x0
  001641a4: b 0x001641be
  001641a6: ldr r0,[r0,#0x4]
  001641a8: add.w r3,r4,#0x2c
  001641ac: ldr.w r0,[r0,r5,lsl #0x2]
  001641b0: ldr r6,[r0,#0x0]
  001641b2: ldmia r3,{r1,r2,r3}
  001641b4: ldr r6,[r6,#0x4]
  001641b6: blx r6
  001641b8: ldr.w r0,[r4,#0x128]
  001641bc: adds r5,#0x1
  001641be: ldr r1,[r0,#0x0]
  001641c0: cmp r5,r1
  001641c2: bcc 0x001641a6
  001641c4: ldr r0,[sp,#0xc]
  001641c6: ldr.w r1,[r8,#0x0]
  001641ca: subs r0,r1,r0
  001641cc: ittt eq
  001641ce: add.eq sp,#0x10
  001641d0: pop.eq.w r8
  001641d4: pop.eq {r4,r5,r6,r7,pc}
  001641d6: blx 0x0006e824
```
