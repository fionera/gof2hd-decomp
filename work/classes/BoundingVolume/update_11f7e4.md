# BoundingVolume::update
elf 0x11f7e4 body 134
Sig: undefined __thiscall update(BoundingVolume * this, float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingVolume::update(float, float, float) */

void __thiscall
BoundingVolume::update(BoundingVolume *this,float param_1,float param_2,float param_3)

{
  uint *puVar1;
  uint uVar2;
  int *piVar3;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  Vector local_50 [12];
  int local_44;
  
  piVar3 = *(int **)(DAT_0012f86c + 0x12f7fe);
  local_44 = *piVar3;
  puVar1 = *(uint **)(this + 4);
  if (puVar1 != (uint *)0x0) {
    for (uVar2 = 0; uVar2 < *puVar1; uVar2 = uVar2 + 1) {
      (**(code **)(**(int **)(puVar1[1] + uVar2 * 4) + 4))(param_1,param_2,param_3);
      puVar1 = *(uint **)(this + 4);
      param_1 = extraout_s0;
      param_2 = extraout_s1;
      param_3 = extraout_s2;
    }
  }
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 8),local_50);
  if (*piVar3 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012f7e4: push {r4,r5,r6,r7,lr}
  0012f7e6: add r7,sp,#0xc
  0012f7e8: push {r7,r8,r9,r10,r11}
  0012f7ec: vpush {d6,d7,d8,d9,d10}
  0012f7f0: mov r11,r0
  0012f7f2: ldr r0,[0x0012f86c]
  0012f7f4: vmov s16,r3
  0012f7f8: mov r8,r3
  0012f7fa: add r0,pc
  0012f7fc: vmov s18,r2
  0012f800: vmov s20,r1
  0012f804: mov r10,r2
  0012f806: ldr.w r9,[r0,#0x0]
  0012f80a: mov r5,r1
  0012f80c: ldr.w r0,[r9,#0x0]
  0012f810: str r0,[sp,#0xc]
  0012f812: ldr.w r0,[r11,#0x4]
  0012f816: cbz r0,0x0012f83a
  0012f818: movs r6,#0x0
  0012f81a: b 0x0012f834
  0012f81c: ldr r0,[r0,#0x4]
  0012f81e: mov r2,r10
  0012f820: mov r3,r8
  0012f822: ldr.w r0,[r0,r6,lsl #0x2]
  0012f826: ldr r1,[r0,#0x0]
  0012f828: ldr r4,[r1,#0x4]
  0012f82a: mov r1,r5
  0012f82c: blx r4
  0012f82e: ldr.w r0,[r11,#0x4]
  0012f832: adds r6,#0x1
  0012f834: ldr r1,[r0,#0x0]
  0012f836: cmp r6,r1
  0012f838: bcc 0x0012f81c
  0012f83a: add.w r0,r11,#0x8
  0012f83e: mov r1,sp
  0012f840: vstr.32 s18,[sp,#0x4]
  0012f844: vstr.32 s20,[sp]
  0012f848: vstr.32 s16,[sp,#0x8]
  0012f84c: blx 0x0006eb3c
  0012f850: ldr r0,[sp,#0xc]
  0012f852: ldr.w r1,[r9,#0x0]
  0012f856: subs r0,r1,r0
  0012f858: itttt eq
  0012f85a: vpop.eq {d6,d7,d8,d9,d10}
  0012f85e: add.eq sp,#0x4
  0012f860: pop.eq.w {r8,r9,r10,r11}
  0012f864: pop.eq {r4,r5,r6,r7,pc}
  0012f866: blx 0x0006e824
```
