# PlayerEgo::getTurretPosition
elf 0x9b2ec body 78
Sig: undefined __stdcall getTurretPosition(void)

## decompile
```c

/* PlayerEgo::getTurretPosition() */

void PlayerEgo::getTurretPosition(void)

{
  Matrix *in_r0;
  Matrix *pMVar1;
  int *piVar2;
  Matrix aMStack_58 [60];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000ab33c + 0xab300);
  local_1c = *piVar2;
  pMVar1 = (Matrix *)AEGeometry::getMatrix();
  AEGeometry::getMatrix();
  AbyssEngine::AEMath::operator*(aMStack_58,pMVar1);
  AbyssEngine::AEMath::MatrixGetPosition(in_r0);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ab2ec: push {r4,r5,r6,r7,lr}
  000ab2ee: add r7,sp,#0xc
  000ab2f0: push.w r8
  000ab2f4: sub sp,#0x40
  000ab2f6: mov r8,r0
  000ab2f8: ldr r0,[0x000ab33c]
  000ab2fa: mov r4,r1
  000ab2fc: add r0,pc
  000ab2fe: ldr r5,[r0,#0x0]
  000ab300: ldr r0,[r5,#0x0]
  000ab302: str r0,[sp,#0x3c]
  000ab304: ldr r0,[r1,#0x8]
  000ab306: blx 0x000721cc
  000ab30a: mov r6,r0
  000ab30c: ldr r0,[r4,#0x28]
  000ab30e: blx 0x000721cc
  000ab312: mov r4,sp
  000ab314: mov r2,r0
  000ab316: mov r0,r4
  000ab318: mov r1,r6
  000ab31a: blx 0x0006f82c
  000ab31e: mov r0,r8
  000ab320: mov r1,r4
  000ab322: blx 0x0006f16c
  000ab326: ldr r0,[sp,#0x3c]
  000ab328: ldr r1,[r5,#0x0]
  000ab32a: subs r0,r1,r0
  000ab32c: ittt eq
  000ab32e: add.eq sp,#0x40
  000ab330: pop.eq.w r8
  000ab334: pop.eq {r4,r5,r6,r7,pc}
  000ab336: blx 0x0006e824
```
