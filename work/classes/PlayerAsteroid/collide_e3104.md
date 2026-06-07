# PlayerAsteroid::collide
elf 0xe3104 body 210
Sig: undefined __thiscall collide(PlayerAsteroid * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerAsteroid::collide(float, float, float) */

void __thiscall
PlayerAsteroid::collide(PlayerAsteroid *this,float param_1,float param_2,float param_3)

{
  float fVar1;
  int iVar2;
  undefined4 uVar3;
  float in_r1;
  float in_r2;
  float in_r3;
  code *pcVar4;
  int *piVar5;
  uint in_fpscr;
  float fVar6;
  float fVar7;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined8 local_48;
  float local_40;
  int local_3c;
  
  piVar5 = *(int **)(DAT_000f31d8 + 0xf311c);
  _local_40 = CONCAT44(*piVar5,in_s14);
  local_48 = in_d6;
  iVar2 = Player::getHitpoints();
  if (0 < iVar2) {
    pcVar4 = *(code **)(DAT_000f31dc + 0xf3138);
    (*pcVar4)(&local_48,*(undefined4 *)(this + 8));
    fVar1 = (float)local_48;
    (*pcVar4)(&local_48,*(undefined4 *)(this + 8));
    (*pcVar4)(&local_48,*(undefined4 *)(this + 8));
    fVar6 = (float)VectorSignedToFloat(*(int *)(*(int *)(this + 4) + 0x40),
                                       (byte)(in_fpscr >> 0x16) & 3);
    if ((((in_r1 - fVar1 < fVar6) &&
         (fVar7 = (float)VectorSignedToFloat(-*(int *)(*(int *)(this + 4) + 0x40),
                                             (byte)((in_fpscr & 0xfffffff) >> 0x16) & 3),
         fVar7 < in_r1 - fVar1)) && ((int)((uint)(in_r2 - local_48._4_4_ < fVar6) << 0x1f) < 0)) &&
       (fVar7 < in_r2 - local_48._4_4_)) {
      if (((int)((uint)(in_r3 - local_40 < fVar6) << 0x1f) < 0) && (fVar7 < in_r3 - local_40)) {
        uVar3 = 1;
        goto LAB_000f31c0;
      }
    }
  }
  uVar3 = 0;
LAB_000f31c0:
  if (*piVar5 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

```

## target disasm
```
  000f3104: push {r4,r5,r6,r7,lr}
  000f3106: add r7,sp,#0xc
  000f3108: push {r8,r9,r11}
  000f310c: vpush {d6,d7,d8,d9,d10}
  000f3110: mov r6,r0
  000f3112: ldr r0,[0x000f31d8]
  000f3114: mov r8,r3
  000f3116: mov r9,r2
  000f3118: add r0,pc
  000f311a: mov r4,r1
  000f311c: ldr r5,[r0,#0x0]
  000f311e: ldr r0,[r5,#0x0]
  000f3120: str r0,[sp,#0xc]
  000f3122: ldr r0,[r6,#0x4]
  000f3124: blx 0x000724f0
  000f3128: cmp r0,#0x1
  000f312a: blt 0x000f31be
  000f312c: ldr r0,[0x000f31dc]
  000f312e: vmov s18,r4
  000f3132: ldr r1,[r6,#0x8]
  000f3134: add r0,pc
  000f3136: ldr r4,[r0,#0x0]
  000f3138: mov r0,sp
  000f313a: blx r4
  000f313c: ldr r1,[r6,#0x8]
  000f313e: mov r0,sp
  000f3140: vldr.32 s20,[sp]
  000f3144: blx r4
  000f3146: ldr r1,[r6,#0x8]
  000f3148: mov r0,sp
  000f314a: vldr.32 s16,[sp,#0x4]
  000f314e: blx r4
  000f3150: ldr r0,[r6,#0x4]
  000f3152: vsub.f32 s4,s18,s20
  000f3156: ldr r0,[r0,#0x40]
  000f3158: vmov s0,r0
  000f315c: vcvt.f32.s32 s0,s0
  000f3160: vcmpe.f32 s4,s0
  000f3164: vmrs apsr,fpscr
  000f3168: bpl 0x000f31be
  000f316a: rsbs r0,r0
  000f316c: vmov s2,r0
  000f3170: vcvt.f32.s32 s2,s2
  000f3174: vcmpe.f32 s4,s2
  000f3178: vmrs apsr,fpscr
  000f317c: ble 0x000f31be
  000f317e: vmov s4,r9
  000f3182: vsub.f32 s4,s4,s16
  000f3186: vcmpe.f32 s4,s0
  000f318a: vmrs apsr,fpscr
  000f318e: bpl 0x000f31be
  000f3190: vcmpe.f32 s4,s2
  000f3194: vmrs apsr,fpscr
  000f3198: ble 0x000f31be
  000f319a: vmov s4,r8
  000f319e: vldr.32 s6,[sp,#0x8]
  000f31a2: vsub.f32 s4,s4,s6
  000f31a6: vcmpe.f32 s4,s0
  000f31aa: vmrs apsr,fpscr
  000f31ae: bpl 0x000f31be
  000f31b0: vcmpe.f32 s4,s2
  000f31b4: vmrs apsr,fpscr
  000f31b8: ble 0x000f31be
  000f31ba: movs r0,#0x1
  000f31bc: b 0x000f31c0
  000f31be: movs r0,#0x0
  000f31c0: ldr r1,[sp,#0xc]
  000f31c2: ldr r2,[r5,#0x0]
  000f31c4: subs r1,r2,r1
  000f31c6: ittt eq
  000f31c8: vpop.eq {d6,d7,d8,d9,d10}
  000f31cc: pop.eq.w {r8,r9,r11}
  000f31d0: pop.eq {r4,r5,r6,r7,pc}
  000f31d2: blx 0x0006e824
```
