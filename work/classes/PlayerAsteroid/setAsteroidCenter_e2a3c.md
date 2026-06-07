# PlayerAsteroid::setAsteroidCenter
elf 0xe2a3c body 72
Sig: undefined __stdcall setAsteroidCenter(Vector param_1)

## decompile
```c

/* PlayerAsteroid::setAsteroidCenter(AbyssEngine::AEMath::Vector) */

undefined8
PlayerAsteroid::setAsteroidCenter
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  float fVar1;
  int *piVar2;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_000f2a84 + 0xf2a48);
  local_14 = *piVar2;
  local_20 = param_2;
  local_1c = param_3;
  local_18 = param_4;
  AbyssEngine::AEMath::Vector::operator=(*(Vector **)(DAT_000f2a88 + 0xf2a4c),(Vector *)&local_20);
  fVar1 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_20);
  **(int **)(DAT_000f2a8c + 0xf2a72) = (int)fVar1;
  if (*piVar2 == local_14) {
    return CONCAT44(local_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000f2a3c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000f2a3e: add r7,sp,#0x18
  000f2a40: ldr r0,[0x000f2a84]
  000f2a42: ldr r4,[0x000f2a88]
  000f2a44: add r0,pc
  000f2a46: str r1,[sp,#0x0]
  000f2a48: add r4,pc
  000f2a4a: str r2,[sp,#0x4]
  000f2a4c: ldr r5,[r0,#0x0]
  000f2a4e: ldr r0,[r4,#0x0]
  000f2a50: mov r4,sp
  000f2a52: ldr r1,[r5,#0x0]
  000f2a54: str r3,[sp,#0x8]
  000f2a56: str r1,[sp,#0xc]
  000f2a58: mov r1,r4
  000f2a5a: blx 0x0006eb3c
  000f2a5e: mov r0,r4
  000f2a60: blx 0x0006ec44
  000f2a64: vmov s0,r0
  000f2a68: ldr r0,[0x000f2a8c]
  000f2a6a: vcvt.s32.f32 s0,s0
  000f2a6e: add r0,pc
  000f2a70: ldr r0,[r0,#0x0]
  000f2a72: vstr.32 s0,[r0]
  000f2a76: ldr r0,[sp,#0xc]
  000f2a78: ldr r1,[r5,#0x0]
  000f2a7a: subs r0,r1,r0
  000f2a7c: it eq
  000f2a7e: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  000f2a80: blx 0x0006e824
```
