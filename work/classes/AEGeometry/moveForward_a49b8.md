# AEGeometry::moveForward
elf 0xa49b8 body 132
Sig: undefined __stdcall moveForward(float param_1)

## decompile
```c

/* AEGeometry::moveForward(float) */

void AEGeometry::moveForward(float param_1)

{
  AEGeometry *in_r0;
  float in_r1;
  int *piVar1;
  float local_34;
  float local_30;
  float local_2c;
  float local_28;
  float local_24;
  float local_20;
  int local_1c;
  
  piVar1 = *(int **)(DAT_000b4a3c + 0xb49ce);
  local_1c = *piVar1;
  getDirection();
  AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_28,(Vector *)&local_34);
  getPosition();
  local_34 = local_34 + local_28 * in_r1;
  local_30 = local_30 + local_24 * in_r1;
  local_2c = local_2c + local_20 * in_r1;
  setPosition(in_r0,(Vector *)&local_34);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b49b8: push {r4,r5,r6,r7,lr}
  000b49ba: add r7,sp,#0xc
  000b49bc: push.w r8
  000b49c0: sub sp,#0x20
  000b49c2: mov r4,r0
  000b49c4: ldr r0,[0x000b4a3c]
  000b49c6: add r6,sp,#0x4
  000b49c8: mov r8,r1
  000b49ca: add r0,pc
  000b49cc: mov r1,r4
  000b49ce: ldr r5,[r0,#0x0]
  000b49d0: ldr r0,[r5,#0x0]
  000b49d2: str r0,[sp,#0x1c]
  000b49d4: mov r0,r6
  000b49d6: blx 0x00072574
  000b49da: add r0,sp,#0x10
  000b49dc: mov r1,r6
  000b49de: blx 0x0006ec80
  000b49e2: add r6,sp,#0x4
  000b49e4: mov r1,r4
  000b49e6: mov r0,r6
  000b49e8: blx 0x000720b8
  000b49ec: vmov s0,r8
  000b49f0: vldr.32 s2,[sp,#0x10]
  000b49f4: vldr.32 s4,[sp,#0x14]
  000b49f8: mov r0,r4
  000b49fa: vldr.32 s6,[sp,#0x18]
  000b49fe: mov r1,r6
  000b4a00: vldr.32 s8,[sp,#0x4]
  000b4a04: vldr.32 s10,[sp,#0x8]
  000b4a08: vmla.f32 s8,s2,s0
  000b4a0c: vldr.32 s12,[sp,#0xc]
  000b4a10: vmla.f32 s10,s4,s0
  000b4a14: vmla.f32 s12,s6,s0
  000b4a18: vstr.32 s8,[sp,#0x4]
  000b4a1c: vstr.32 s10,[sp,#0x8]
  000b4a20: vstr.32 s12,[sp,#0xc]
  000b4a24: blx 0x00072148
  000b4a28: ldr r0,[sp,#0x1c]
  000b4a2a: ldr r1,[r5,#0x0]
  000b4a2c: subs r0,r1,r0
  000b4a2e: ittt eq
  000b4a30: add.eq sp,#0x20
  000b4a32: pop.eq.w r8
  000b4a36: pop.eq {r4,r5,r6,r7,pc}
  000b4a38: blx 0x0006e824
```
