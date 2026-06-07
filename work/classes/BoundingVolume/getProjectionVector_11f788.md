# BoundingVolume::getProjectionVector
elf 0x11f788 body 74
Sig: undefined __stdcall getProjectionVector(Vector * param_1)

## decompile
```c

/* BoundingVolume::getProjectionVector(AbyssEngine::AEMath::Vector const&) */

void BoundingVolume::getProjectionVector(Vector *param_1)

{
  Vector *in_r2;
  int *piVar1;
  AEMath aAStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_0012f7d4 + 0x12f7a0);
  local_1c = *piVar1;
  AbyssEngine::AEMath::operator-(param_1,in_r2);
  AbyssEngine::AEMath::VectorNormalize(aAStack_28,param_1);
  AbyssEngine::AEMath::Vector::operator=(param_1,(Vector *)aAStack_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012f788: push {r4,r5,r6,r7,lr}
  0012f78a: add r7,sp,#0xc
  0012f78c: push.w r11
  0012f790: sub sp,#0x10
  0012f792: mov r4,r0
  0012f794: ldr r0,[0x0012f7d4]
  0012f796: mov r3,r2
  0012f798: add.w r2,r1,#0x8
  0012f79c: add r0,pc
  0012f79e: mov r1,r3
  0012f7a0: ldr r6,[r0,#0x0]
  0012f7a2: ldr r0,[r6,#0x0]
  0012f7a4: str r0,[sp,#0xc]
  0012f7a6: mov r0,r4
  0012f7a8: blx 0x0006ec38
  0012f7ac: mov r5,sp
  0012f7ae: mov r1,r4
  0012f7b0: mov r0,r5
  0012f7b2: blx 0x0006ec80
  0012f7b6: mov r0,r4
  0012f7b8: mov r1,r5
  0012f7ba: blx 0x0006eb3c
  0012f7be: ldr r0,[sp,#0xc]
  0012f7c0: ldr r1,[r6,#0x0]
  0012f7c2: subs r0,r1,r0
  0012f7c4: ittt eq
  0012f7c6: add.eq sp,#0x10
  0012f7c8: pop.eq.w r11
  0012f7cc: pop.eq {r4,r5,r6,r7,pc}
  0012f7ce: blx 0x0006e824
```
