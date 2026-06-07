# MovingStars::translate
elf 0x14675c body 134
Sig: undefined __thiscall translate(MovingStars * this, Vector * param_1)

## decompile
```c

/* MovingStars::translate(AbyssEngine::AEMath::Vector const&) */

void __thiscall MovingStars::translate(MovingStars *this,Vector *param_1)

{
  int iVar1;
  uint *puVar2;
  int *piVar3;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_70 [60];
  Matrix local_34 [8];
  float local_2c;
  int local_28;
  
  piVar3 = *(int **)(DAT_001567e4 + 0x156776);
  local_28 = *piVar3;
  puVar2 = *(uint **)(DAT_001567e8 + 0x156788);
  for (iVar1 = 0; iVar1 != 0x32; iVar1 = iVar1 + 1) {
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar2);
    AbyssEngine::AEMath::MatrixGetPosition(local_34);
    AbyssEngine::AEMath::Vector::operator+=((Vector *)local_34,param_1);
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar2);
    AbyssEngine::AEMath::MatrixSetTranslation(aMStack_70,local_2c,extraout_s1,extraout_s2);
  }
  if (*piVar3 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0015675c: push {r4,r5,r6,r7,lr}
  0015675e: add r7,sp,#0xc
  00156760: push {r8,r9,r10,r11}
  00156764: sub sp,#0x54
  00156766: mov r5,r0
  00156768: ldr r0,[0x001567e4]
  0015676a: add.w r11,sp,#0x44
  0015676e: add.w r8,sp,#0x8
  00156772: add r0,pc
  00156774: mov r10,r1
  00156776: movs r4,#0x0
  00156778: ldr.w r9,[r0,#0x0]
  0015677c: ldr.w r0,[r9,#0x0]
  00156780: str r0,[sp,#0x50]
  00156782: ldr r0,[0x001567e8]
  00156784: add r0,pc
  00156786: ldr r6,[r0,#0x0]
  00156788: b 0x001567c8
  0015678a: ldr r0,[r5,#0x4]
  0015678c: ldr.w r1,[r0,r4,lsl #0x2]
  00156790: ldr r0,[r6,#0x0]
  00156792: blx 0x000720c4
  00156796: mov r1,r0
  00156798: mov r0,r11
  0015679a: blx 0x0006f16c
  0015679e: mov r0,r11
  001567a0: mov r1,r10
  001567a2: blx 0x00073534
  001567a6: ldr r1,[r5,#0x4]
  001567a8: ldr r0,[r6,#0x0]
  001567aa: ldr.w r1,[r1,r4,lsl #0x2]
  001567ae: blx 0x000720c4
  001567b2: ldrd r2,r3,[sp,#0x44]
  001567b6: mov r1,r0
  001567b8: vldr.32 s0,[sp,#0x4c]
  001567bc: mov r0,r8
  001567be: vstr.32 s0,[sp]
  001567c2: blx 0x0006f820
  001567c6: adds r4,#0x1
  001567c8: cmp r4,#0x32
  001567ca: bne 0x0015678a
  001567cc: ldr r0,[sp,#0x50]
  001567ce: ldr.w r1,[r9,#0x0]
  001567d2: subs r0,r1,r0
  001567d4: ittt eq
  001567d6: add.eq sp,#0x54
  001567d8: pop.eq.w {r8,r9,r10,r11}
  001567dc: pop.eq {r4,r5,r6,r7,pc}
  001567de: blx 0x0006e824
```
