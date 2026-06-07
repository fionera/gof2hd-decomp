# TargetFollowCamera::enableFirstPersonCam
elf 0x15b2d8 body 72
Sig: undefined __thiscall enableFirstPersonCam(TargetFollowCamera * this, bool param_1)

## decompile
```c

/* TargetFollowCamera::enableFirstPersonCam(bool) */

void __thiscall TargetFollowCamera::enableFirstPersonCam(TargetFollowCamera *this,bool param_1)

{
  int *piVar1;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  int local_1c;
  
  local_20 = DAT_0016b324;
  local_24 = DAT_0016b320;
  piVar1 = *(int **)(DAT_0016b328 + 0x16b2ee);
  local_1c = *piVar1;
  *(bool *)&this->field_F0 = param_1;
  local_28 = 0;
  AbyssEngine::AEMath::Vector::operator=((Vector *)&this->field_F4,(Vector *)&local_28);
  this->field_104 = 0;
  this->field_108 = 0;
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016b2d8: push {r4,r5,r6,r7,lr}
  0016b2da: add r7,sp,#0xc
  0016b2dc: push.w r11
  0016b2e0: sub sp,#0x10
  0016b2e2: mov r4,r0
  0016b2e4: ldr r0,[0x0016b328]
  0016b2e6: ldr r2,[0x0016b320]
  0016b2e8: movs r6,#0x0
  0016b2ea: add r0,pc
  0016b2ec: ldr r3,[0x0016b324]
  0016b2ee: ldr r5,[r0,#0x0]
  0016b2f0: ldr r0,[r5,#0x0]
  0016b2f2: str r0,[sp,#0xc]
  0016b2f4: add.w r0,r4,#0xf4
  0016b2f8: strb.w r1,[r4,#0xf0]
  0016b2fc: mov r1,sp
  0016b2fe: str r2,[sp,#0x4]
  0016b300: str r6,[sp,#0x0]
  0016b302: str r3,[sp,#0x8]
  0016b304: blx 0x0006eb3c
  0016b308: strd r6,r6,[r4,#0x104]
  0016b30c: ldr r0,[sp,#0xc]
  0016b30e: ldr r1,[r5,#0x0]
  0016b310: subs r0,r1,r0
  0016b312: ittt eq
  0016b314: add.eq sp,#0x10
  0016b316: pop.eq.w r11
  0016b31a: pop.eq {r4,r5,r6,r7,pc}
  0016b31c: blx 0x0006e824
```
