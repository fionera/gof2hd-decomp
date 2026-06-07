# Trail::update
elf 0x15c228 body 98
Sig: undefined __thiscall update(Trail * this, Matrix * param_1, Matrix * param_2, Vector * param_3)

## decompile
```c

/* Trail::update(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Vector const&) */

void __thiscall Trail::update(Trail *this,Matrix *param_1,Matrix *param_2,Vector *param_3)

{
  int *piVar1;
  float extraout_s1;
  float extraout_s3;
  float extraout_s5;
  float local_34;
  float local_30;
  float local_2c;
  Matrix local_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_0016c28c + 0x16c23e);
  local_1c = *piVar1;
  AbyssEngine::AEMath::MatrixTransformVector(local_28,(Vector *)param_1);
  AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_34,(Vector *)param_2);
  update(local_34,extraout_s1,local_30,extraout_s3,local_2c,extraout_s5);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016c228: push {r4,r5,r6,r7,lr}
  0016c22a: add r7,sp,#0xc
  0016c22c: push.w r8
  0016c230: sub sp,#0x28
  0016c232: mov r8,r0
  0016c234: ldr r0,[0x0016c28c]
  0016c236: mov r5,r2
  0016c238: mov r2,r3
  0016c23a: add r0,pc
  0016c23c: mov r4,r3
  0016c23e: ldr r6,[r0,#0x0]
  0016c240: ldr r0,[r6,#0x0]
  0016c242: str r0,[sp,#0x24]
  0016c244: add r0,sp,#0x18
  0016c246: blx 0x0006f688
  0016c24a: add r0,sp,#0xc
  0016c24c: mov r1,r5
  0016c24e: mov r2,r4
  0016c250: blx 0x0006f688
  0016c254: add r3,sp,#0x18
  0016c256: mov r0,r8
  0016c258: ldmia r3,{r1,r2,r3}
  0016c25a: vldr.32 s0,[sp,#0xc]
  0016c25e: vldr.32 s2,[sp,#0x10]
  0016c262: vldr.32 s4,[sp,#0x14]
  0016c266: vstr.32 s0,[sp]
  0016c26a: vstr.32 s2,[sp,#0x4]
  0016c26e: vstr.32 s4,[sp,#0x8]
  0016c272: blx 0x00077800
  0016c276: ldr r0,[sp,#0x24]
  0016c278: ldr r1,[r6,#0x0]
  0016c27a: subs r0,r1,r0
  0016c27c: ittt eq
  0016c27e: add.eq sp,#0x28
  0016c280: pop.eq.w r8
  0016c284: pop.eq {r4,r5,r6,r7,pc}
  0016c286: blx 0x0006e824
```
