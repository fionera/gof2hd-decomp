# AbyssEngine::AEMath::MatrixSetRotation
elf 0x80cc4 body 118
Sig: undefined __thiscall MatrixSetRotation(AEMath * this, Matrix * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Vector
   const&) */

void __thiscall AbyssEngine::AEMath::MatrixSetRotation(AEMath *this,Matrix *param_1,Vector *param_2)

{
  int *piVar1;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  Vector aVStack_40 [12];
  AEMath aAStack_34 [12];
  int local_28;
  
  piVar1 = *(int **)(DAT_00090d3c + 0x90cda);
  local_28 = *piVar1;
  local_48 = 0x3f800000;
  local_4c = 0;
  local_44 = 0;
  VectorCross(aVStack_40,(Vector *)&local_4c);
  VectorNormalize(aAStack_34,aVStack_40);
  VectorCross((Vector *)&local_4c,param_2);
  VectorNormalize((AEMath *)aVStack_40,(Vector *)&local_4c);
  MatrixSetRotation((Matrix *)this,(Vector *)param_1,(Vector *)aAStack_34,aVStack_40);
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00090cc4: push {r4,r5,r6,r7,lr}
  00090cc6: add r7,sp,#0xc
  00090cc8: push {r8,r9,r10,r11}
  00090ccc: sub sp,#0x2c
  00090cce: mov r9,r0
  00090cd0: ldr r0,[0x00090d3c]
  00090cd2: add r4,sp,#0x10
  00090cd4: mov r8,r1
  00090cd6: add r0,pc
  00090cd8: add r1,sp,#0x4
  00090cda: mov r5,r2
  00090cdc: ldr r6,[r0,#0x0]
  00090cde: ldr r0,[r6,#0x0]
  00090ce0: str r0,[sp,#0x28]
  00090ce2: mov.w r0,#0x3f800000
  00090ce6: str r0,[sp,#0x8]
  00090ce8: movs r0,#0x0
  00090cea: str r0,[sp,#0x4]
  00090cec: str r0,[sp,#0xc]
  00090cee: mov r0,r4
  00090cf0: blx 0x0006ec8c
  00090cf4: add.w r11,sp,#0x1c
  00090cf8: mov r1,r4
  00090cfa: mov r0,r11
  00090cfc: blx 0x0006ec80
  00090d00: add r4,sp,#0x4
  00090d02: mov r1,r5
  00090d04: mov r2,r11
  00090d06: mov r0,r4
  00090d08: blx 0x0006ec8c
  00090d0c: add.w r10,sp,#0x10
  00090d10: mov r1,r4
  00090d12: mov r0,r10
  00090d14: blx 0x0006ec80
  00090d18: mov r0,r9
  00090d1a: mov r1,r8
  00090d1c: mov r2,r11
  00090d1e: mov r3,r10
  00090d20: str r5,[sp,#0x0]
  00090d22: blx 0x000701bc
  00090d26: ldr r0,[sp,#0x28]
  00090d28: ldr r1,[r6,#0x0]
  00090d2a: subs r0,r1,r0
  00090d2c: ittt eq
  00090d2e: add.eq sp,#0x2c
  00090d30: pop.eq.w {r8,r9,r10,r11}
  00090d34: pop.eq {r4,r5,r6,r7,pc}
  00090d36: blx 0x0006e824
```
