# ParticleSystemMesh::finishCurrentTrailParticle
elf 0x1855fe body 76
Sig: undefined __stdcall finishCurrentTrailParticle(ParticleSet param_1, int param_2, Vector * param_3, Vector * param_4)

## decompile
```c

/* ParticleSystemMesh::finishCurrentTrailParticle(ParticleSettings::ParticleSet, int,
   AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) */

void ParticleSystemMesh::finishCurrentTrailParticle
               (int param_1,undefined1 param_2,int param_3,Vector *param_4,Vector *param_5)

{
  uint uVar1;
  Vector *this;
  
  *(undefined1 *)(*(int *)(param_1 + 0x6c) + param_3) = param_2;
  *(undefined4 *)(*(int *)(param_1 + 0x68) + param_3 * 4) = 0;
  uVar1 = *(uint *)(param_1 + 0x34);
  this = (Vector *)(*(int *)(param_1 + 100) + (*(int *)(param_1 + 0x98) * param_3 * 2 | 1U) * 0xc);
  if ((uVar1 & 0x1000) != 0) {
    AbyssEngine::AEMath::Vector::operator=(this,param_4);
    uVar1 = *(uint *)(param_1 + 0x34);
    this = this + 0x18;
  }
  if (-1 < (int)(uVar1 << 0x12)) {
    return;
  }
  AbyssEngine::AEMath::Vector::operator=(this,param_5);
  return;
}

```

## target disasm
```
  001955fe: push {r4,r5,r7,lr}
  00195600: add r7,sp,#0x8
  00195602: mov r5,r0
  00195604: ldr r0,[r0,#0x6c]
  00195606: strb r1,[r0,r2]
  00195608: movs r1,#0x0
  0019560a: ldr r0,[r5,#0x68]
  0019560c: str.w r1,[r0,r2,lsl #0x2]
  00195610: ldr.w r4,[r5,#0x98]
  00195614: ldr r1,[r5,#0x64]
  00195616: ldr r0,[r5,#0x34]
  00195618: muls r2,r4
  0019561a: movs r4,#0x1
  0019561c: tst r0,#0x1000
  00195620: orr.w r2,r4,r2, lsl #0x1
  00195624: add.w r2,r2,r2, lsl #0x1
  00195628: add.w r4,r1,r2, lsl #0x2
  0019562c: beq 0x0019563a
  0019562e: mov r0,r4
  00195630: mov r1,r3
  00195632: blx 0x0006eb3c
  00195636: ldr r0,[r5,#0x34]
  00195638: adds r4,#0x18
  0019563a: lsls r0,r0,#0x12
  0019563c: it pl
  0019563e: pop.pl {r4,r5,r7,pc}
  00195640: ldr r1,[r7,#0x8]
  00195642: mov r0,r4
  00195644: blx 0x0006eb3c
  00195648: pop {r4,r5,r7,pc}
```
