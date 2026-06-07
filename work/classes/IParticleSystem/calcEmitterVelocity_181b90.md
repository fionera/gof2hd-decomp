# IParticleSystem::calcEmitterVelocity
elf 0x181b90 body 130
Sig: undefined __thiscall calcEmitterVelocity(IParticleSystem * this, int param_1)

## decompile
```c

/* IParticleSystem::calcEmitterVelocity(int) */

void __thiscall IParticleSystem::calcEmitterVelocity(IParticleSystem *this,int param_1)

{
  int *piVar1;
  uint in_fpscr;
  float fVar2;
  Vector aVStack_48 [12];
  Vector aVStack_3c [12];
  Matrix aMStack_30 [12];
  int local_24;
  
  piVar1 = *(int **)(DAT_00191c18 + 0x191ba8);
  local_24 = *piVar1;
  AbyssEngine::AEMath::MatrixGetPosition(aMStack_30);
  AbyssEngine::AEMath::operator-(aVStack_48,(Vector *)aMStack_30);
  fVar2 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::AEMath::operator*(aVStack_3c,DAT_00191c14 / fVar2);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x1c),aVStack_3c);
  this[5] = (IParticleSystem)0x0;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x28),(Vector *)aMStack_30);
  if (*piVar1 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00191b90: push {r4,r5,r6,r7,lr}
  00191b92: add r7,sp,#0xc
  00191b94: push {r8,r9,r10}
  00191b98: sub sp,#0x28
  00191b9a: mov r4,r0
  00191b9c: ldr r0,[0x00191c18]
  00191b9e: mov r6,r1
  00191ba0: add.w r8,sp,#0x18
  00191ba4: add r0,pc
  00191ba6: ldr.w r10,[r0,#0x0]
  00191baa: ldr.w r0,[r10,#0x0]
  00191bae: str r0,[sp,#0x24]
  00191bb0: mov r0,r8
  00191bb2: ldr r1,[r4,#0x18]
  00191bb4: blx 0x0006f16c
  00191bb8: add.w r5,r4,#0x28
  00191bbc: mov r9,sp
  00191bbe: mov r0,r9
  00191bc0: mov r1,r8
  00191bc2: mov r2,r5
  00191bc4: blx 0x0006ec38
  00191bc8: vmov s0,r6
  00191bcc: vldr.32 s2,[pc,#0x44]
  00191bd0: add r6,sp,#0xc
  00191bd2: mov r1,r9
  00191bd4: vcvt.f32.s32 s0,s0
  00191bd8: mov r0,r6
  00191bda: vdiv.f32 s0,s2,s0
  00191bde: vmov r2,s0
  00191be2: blx 0x0006ec74
  00191be6: add.w r0,r4,#0x1c
  00191bea: mov r1,r6
  00191bec: blx 0x0006eb3c
  00191bf0: movs r0,#0x0
  00191bf2: mov r1,r8
  00191bf4: strb r0,[r4,#0x5]
  00191bf6: mov r0,r5
  00191bf8: blx 0x0006eb3c
  00191bfc: ldr r0,[sp,#0x24]
  00191bfe: ldr.w r1,[r10,#0x0]
  00191c02: subs r0,r1,r0
  00191c04: ittt eq
  00191c06: add.eq sp,#0x28
  00191c08: pop.eq.w {r8,r9,r10}
  00191c0c: pop.eq {r4,r5,r6,r7,pc}
  00191c0e: blx 0x0006e824
```
