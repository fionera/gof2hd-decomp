# IParticleSystem::resetEmitterVelocity
elf 0x181b34 body 86
Sig: undefined __thiscall resetEmitterVelocity(IParticleSystem * this)

## decompile
```c

/* IParticleSystem::resetEmitterVelocity() */

void __thiscall IParticleSystem::resetEmitterVelocity(IParticleSystem *this)

{
  int *piVar1;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 local_20;
  int local_1c;
  
  piVar1 = *(int **)(DAT_00191b8c + 0x191b4a);
  local_1c = *piVar1;
  local_28 = 0;
  uStack_24 = 0;
  local_20 = 0;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x1c),(Vector *)&local_28);
  this[5] = (IParticleSystem)0x1;
  AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_28);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x28),(Vector *)&local_28);
  this[4] = (IParticleSystem)0x0;
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00191b34: push {r4,r5,r6,r7,lr}
  00191b36: add r7,sp,#0xc
  00191b38: push.w r8
  00191b3c: sub sp,#0x10
  00191b3e: mov r4,r0
  00191b40: ldr r0,[0x00191b8c]
  00191b42: movs r5,#0x0
  00191b44: mov r1,sp
  00191b46: add r0,pc
  00191b48: ldr r6,[r0,#0x0]
  00191b4a: ldr r0,[r6,#0x0]
  00191b4c: str r0,[sp,#0xc]
  00191b4e: add.w r0,r4,#0x1c
  00191b52: strd r5,r5,[sp,#0x0]
  00191b56: str r5,[sp,#0x8]
  00191b58: blx 0x0006eb3c
  00191b5c: movs r0,#0x1
  00191b5e: mov r8,sp
  00191b60: strb r0,[r4,#0x5]
  00191b62: mov r0,r8
  00191b64: ldr r1,[r4,#0x18]
  00191b66: blx 0x0006f16c
  00191b6a: add.w r0,r4,#0x28
  00191b6e: mov r1,r8
  00191b70: blx 0x0006eb3c
  00191b74: strb r5,[r4,#0x4]
  00191b76: ldr r0,[sp,#0xc]
  00191b78: ldr r1,[r6,#0x0]
  00191b7a: subs r0,r1,r0
  00191b7c: ittt eq
  00191b7e: add.eq sp,#0x10
  00191b80: pop.eq.w r8
  00191b84: pop.eq {r4,r5,r6,r7,pc}
  00191b86: blx 0x0006e824
```
