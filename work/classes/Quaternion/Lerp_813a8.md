# AbyssEngine::Quaternion::Lerp
elf 0x813a8 body 98
Sig: undefined __thiscall Lerp(Quaternion * this, Quaternion * param_1, Quaternion * param_2, float param_3)

## decompile
```c

/* AbyssEngine::Quaternion::Lerp(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&,
   float) */

void __thiscall
AbyssEngine::Quaternion::Lerp
          (Quaternion *this,Quaternion *param_1,Quaternion *param_2,float param_3)

{
  undefined4 in_r3;
  int *piVar1;
  undefined1 auVar2 [16];
  AbyssEngine local_50 [16];
  undefined8 local_40;
  undefined8 uStack_38;
  int local_24;
  
  piVar1 = *(int **)(DAT_0009140c + 0x913c6);
  local_24 = *piVar1;
  operator-(local_50,param_2,param_1);
  auVar2._8_8_ = CONCAT44(in_r3,in_r3);
  auVar2._0_8_ = CONCAT44(in_r3,in_r3);
  auVar2 = FloatVectorMultiplyAccumulate(auVar2,(undefined1  [16])local_50,2,0x10);
  local_40 = auVar2._0_8_;
  uStack_38 = auVar2._8_8_;
  Normalized();
  if (*piVar1 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000913a8: push {r4,r5,r6,r7,lr}
  000913aa: add r7,sp,#0xc
  000913ac: push {r8,r9,r11}
  000913b0: sub sp,#0x30
  000913b2: mov r4,sp
  000913b4: bfc r4,#0x0,#0x4
  000913b8: mov sp,r4
  000913ba: mov r8,r0
  000913bc: ldr r0,[0x0009140c]
  000913be: mov r5,r1
  000913c0: mov r9,sp
  000913c2: add r0,pc
  000913c4: mov r1,r2
  000913c6: mov r2,r5
  000913c8: mov r4,r3
  000913ca: ldr r6,[r0,#0x0]
  000913cc: ldr r0,[r6,#0x0]
  000913ce: str r0,[sp,#0x2c]
  000913d0: mov r0,r9
  000913d2: blx 0x0006f184
  000913d6: vld1.64 {d16,d17},[r9:128]
  000913da: add r1,sp,#0x10
  000913dc: mov r0,r8
  000913de: vdup.32 q9,r4
  000913e2: vld1.32 {d20,d21},[r5]
  000913e6: vmla.f32 q10,q9,q8
  000913ea: vst1.64 {d20,d21},[r1:128]
  000913ee: blx 0x000701e0
  000913f2: ldr r0,[sp,#0x2c]
  000913f4: ldr r1,[r6,#0x0]
  000913f6: subs r0,r1,r0
  000913f8: itttt eq
  000913fa: sub.eq.w r4,r7,#0x18
  000913fe: mov.eq sp,r4
  00091400: pop.eq.w {r8,r9,r11}
  00091404: pop.eq {r4,r5,r6,r7,pc}
  00091406: blx 0x0006e824
```
