# Globals::Globals
elf 0xe31e0 body 472
Sig: undefined __thiscall Globals(Globals * this)

## decompile
```c

/* Globals::Globals() */

Globals * __thiscall Globals::Globals(Globals *this)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 uVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  float fVar13;
  
  puVar7 = (undefined4 *)(DAT_000f33d0 + 0xf3200);
  puVar9 = (undefined4 *)(DAT_000f33d8 + 0xf3208);
  puVar8 = (undefined4 *)(DAT_000f33dc + 0xf320e);
  puVar11 = *(undefined4 **)(DAT_000f33cc + 0xf31fc);
  puVar5 = (undefined4 *)(DAT_000f33e0 + 0xf3216);
  puVar12 = *(undefined4 **)(DAT_000f33d4 + 0xf3204);
  **(undefined4 **)(DAT_000f33c8 + 0xf31f6) = 0;
  uVar2 = DAT_000f33c0;
  uVar1 = DAT_000f33b8;
  puVar5 = (undefined4 *)*puVar5;
  puVar8 = (undefined4 *)*puVar8;
  puVar7 = (undefined4 *)*puVar7;
  puVar9 = (undefined4 *)*puVar9;
  uVar10 = 0x247;
  **(undefined4 **)(DAT_000f33e4 + 0xf322a) = 0;
  **(undefined4 **)(DAT_000f33e8 + 0xf3242) = 0;
  **(undefined4 **)(DAT_000f33ec + 0xf324a) = 0;
  **(undefined4 **)(DAT_000f33f0 + 0xf3252) = 0;
  **(undefined4 **)(DAT_000f33f4 + 0xf325a) = 0;
  **(undefined4 **)(DAT_000f33f8 + 0xf3262) = 0;
  **(undefined4 **)(DAT_000f33fc + 0xf326a) = 0;
  **(undefined4 **)(DAT_000f3400 + 0xf3272) = 0;
  **(undefined4 **)(DAT_000f3404 + 0xf327a) = 0;
  iVar3 = DAT_000f3408;
  *puVar5 = 0;
  **(undefined4 **)(iVar3 + 0xf3288) = 0;
  *(undefined4 *)this = 0;
  *(undefined8 *)(puVar8 + 5) = uVar1;
  *(undefined8 *)(puVar8 + 7) = uVar2;
  *(undefined2 *)(puVar8 + 3) = 0x101;
  *puVar8 = 0x3f000000;
  puVar8[1] = 0x3f000000;
  puVar8[2] = 0x3f000000;
  puVar8[9] = 0x3f000000;
  puVar8[10] = 0x3f800000;
  puVar8[0xb] = 0x3f000000;
  *(undefined1 *)((int)puVar8 + 0xe) = 1;
  *(undefined1 *)(puVar8 + 0xc) = 1;
  iVar3 = DAT_000f340c;
  *(undefined2 *)(puVar8 + 4) = 0;
  *(undefined2 *)(puVar8 + 0x10) = 0;
  puVar8[0x14] = 0;
  puVar5 = *(undefined4 **)(iVar3 + 0xf32bc);
  *(undefined1 *)((int)puVar8 + 0x39) = 0;
  *(undefined4 *)((int)puVar8 + 0x35) = 0;
  *(undefined4 *)((int)puVar8 + 0x31) = 0;
  puVar5[1] = 0;
  *(undefined1 *)((int)puVar5 + 0x13) = 0;
  *(undefined1 *)((int)puVar8 + 0x3f) = 0;
  *(undefined4 *)((int)puVar8 + 0x3b) = 0;
  *(undefined1 *)((int)puVar8 + 0x4e) = 0;
  *(undefined2 *)(puVar8 + 0x13) = 0;
  puVar8[0x12] = 0;
  *puVar5 = 0;
  **(undefined1 **)(DAT_000f3410 + 0xf32ec) = 0;
  **(undefined1 **)(DAT_000f3414 + 0xf32f4) = 0;
  iVar3 = DAT_000f3418;
  *puVar11 = 0xffffffff;
  **(undefined1 **)(iVar3 + 0xf3300) = 0;
  iVar3 = DAT_000f341c;
  *puVar7 = 0xffffffff;
  iVar4 = DAT_000f3420;
  fVar13 = (float)puVar8[0x11];
  *puVar9 = 0;
  *puVar12 = 0xffffffff;
  puVar7 = *(undefined4 **)(iVar3 + 0xf330c);
  uVar6 = 0x201;
  puVar5 = *(undefined4 **)(iVar4 + 0xf3316);
  if (1.0 <= fVar13) {
    uVar10 = 0x33e;
  }
  if (fVar13 <= 0.0) {
    uVar10 = 0x19f;
  }
  puVar8[0x15] = uVar10;
  iVar3 = DAT_000f3424;
  if (1.0 <= fVar13) {
    uVar6 = 0x2da;
  }
  if (fVar13 <= 0.0) {
    uVar6 = 0x16d;
  }
  puVar8[0x16] = uVar6;
  puVar11 = *(undefined4 **)(iVar3 + 0xf3366);
  **(undefined4 **)(DAT_000f3428 + 0xf3368) = 0;
  **(undefined4 **)(DAT_000f342c + 0xf3372) = 0;
  puVar9 = *(undefined4 **)(DAT_000f3430 + 0xf337c);
  **(undefined4 **)(DAT_000f3434 + 0xf337e) = 0xffffffff;
  **(undefined4 **)(DAT_000f3438 + 0xf3388) = 0;
  **(undefined4 **)(DAT_000f343c + 0xf3390) = 0;
  iVar3 = DAT_000f3440;
  *puVar9 = 0;
  *(undefined2 *)(puVar8 + 0x18) = 0x100;
  *puVar11 = 0;
  puVar8 = *(undefined4 **)(iVar3 + 0xf339e);
  *puVar5 = 0;
  *puVar7 = 0;
  *puVar8 = 0;
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  000f31e0: push {r4,r5,r6,r7,lr}
  000f31e2: add r7,sp,#0xc
  000f31e4: push {r8,r9,r10}
  000f31e8: ldr r1,[0x000f33c8]
  000f31ea: mov r9,r0
  000f31ec: ldr r2,[0x000f33cc]
  000f31ee: vmov.f32 s0,0x3f800000
  000f31f2: add r1,pc
  000f31f4: ldr r3,[0x000f33d0]
  000f31f6: ldr r5,[0x000f33d4]
  000f31f8: add r2,pc
  000f31fa: ldr r6,[0x000f33d8]
  000f31fc: add r3,pc
  000f31fe: ldr r4,[0x000f33dc]
  000f3200: add r5,pc
  000f3202: ldr r0,[0x000f33e0]
  000f3204: add r6,pc
  000f3206: ldr.w r10,[r1,#0x0]
  000f320a: add r4,pc
  000f320c: ldr.w r8,[r2,#0x0]
  000f3210: movs r2,#0x0
  000f3212: add r0,pc
  000f3214: ldr.w lr,[r5,#0x0]
  000f3218: str.w r2,[r10,#0x0]
  000f321c: adr r1,[0xf33b8]
  000f321e: ldr r5,[r0,#0x0]
  000f3220: ldr r0,[0x000f33e4]
  000f3222: vld1.64 {d16,d17},[r1]
  000f3226: add r0,pc
  000f3228: ldr r1,[r4,#0x0]
  000f322a: mov.w r4,#0x3f000000
  000f322e: ldr r0,[r0,#0x0]
  000f3230: ldr r3,[r3,#0x0]
  000f3232: ldr.w r12,[r6,#0x0]
  000f3236: movw r6,#0x247
  000f323a: str r2,[r0,#0x0]
  000f323c: ldr r0,[0x000f33e8]
  000f323e: add r0,pc
  000f3240: ldr r0,[r0,#0x0]
  000f3242: str r2,[r0,#0x0]
  000f3244: ldr r0,[0x000f33ec]
  000f3246: add r0,pc
  000f3248: ldr r0,[r0,#0x0]
  000f324a: str r2,[r0,#0x0]
  000f324c: ldr r0,[0x000f33f0]
  000f324e: add r0,pc
  000f3250: ldr r0,[r0,#0x0]
  000f3252: str r2,[r0,#0x0]
  000f3254: ldr r0,[0x000f33f4]
  000f3256: add r0,pc
  000f3258: ldr r0,[r0,#0x0]
  000f325a: str r2,[r0,#0x0]
  000f325c: ldr r0,[0x000f33f8]
  000f325e: add r0,pc
  000f3260: ldr r0,[r0,#0x0]
  000f3262: str r2,[r0,#0x0]
  000f3264: ldr r0,[0x000f33fc]
  000f3266: add r0,pc
  000f3268: ldr r0,[r0,#0x0]
  000f326a: str r2,[r0,#0x0]
  000f326c: ldr r0,[0x000f3400]
  000f326e: add r0,pc
  000f3270: ldr r0,[r0,#0x0]
  000f3272: str r2,[r0,#0x0]
  000f3274: ldr r0,[0x000f3404]
  000f3276: add r0,pc
  000f3278: ldr r0,[r0,#0x0]
  000f327a: str r2,[r0,#0x0]
  000f327c: ldr r0,[0x000f3408]
  000f327e: str r2,[r5,#0x0]
  000f3280: mov.w r5,#0xffffffff
  000f3284: add r0,pc
  000f3286: ldr r0,[r0,#0x0]
  000f3288: str r2,[r0,#0x0]
  000f328a: add.w r0,r1,#0x14
  000f328e: str.w r2,[r9,#0x0]
  000f3292: vst1.32 {d16,d17},[r0]
  000f3296: movw r0,#0x101
  000f329a: strh r0,[r1,#0xc]
  000f329c: mov.w r0,#0x3f800000
  000f32a0: strd r4,r4,[r1,#0x0]
  000f32a4: str r4,[r1,#0x8]
  000f32a6: strd r4,r0,[r1,#0x24]
  000f32aa: movs r0,#0x1
  000f32ac: str r4,[r1,#0x2c]
  000f32ae: strb r0,[r1,#0xe]
  000f32b0: strb.w r0,[r1,#0x30]
  000f32b4: ldr r0,[0x000f340c]
  000f32b6: strh r2,[r1,#0x10]
  000f32b8: add r0,pc
  000f32ba: strh.w r2,[r1,#0x40]
  000f32be: str r2,[r1,#0x50]
  000f32c0: ldr r0,[r0,#0x0]
  000f32c2: strb.w r2,[r1,#0x39]
  000f32c6: str.w r2,[r1,#0x35]
  000f32ca: str.w r2,[r1,#0x31]
  000f32ce: str r2,[r0,#0x4]
  000f32d0: strb r2,[r0,#0x13]
  000f32d2: strb.w r2,[r1,#0x3f]
  000f32d6: str.w r2,[r1,#0x3b]
  000f32da: strb.w r2,[r1,#0x4e]
  000f32de: strh.w r2,[r1,#0x4c]
  000f32e2: str r2,[r1,#0x48]
  000f32e4: str r2,[r0,#0x0]
  000f32e6: ldr r0,[0x000f3410]
  000f32e8: add r0,pc
  000f32ea: ldr r0,[r0,#0x0]
  000f32ec: strb r2,[r0,#0x0]
  000f32ee: ldr r0,[0x000f3414]
  000f32f0: add r0,pc
  000f32f2: ldr r0,[r0,#0x0]
  000f32f4: strb r2,[r0,#0x0]
  000f32f6: ldr r0,[0x000f3418]
  000f32f8: str.w r5,[r8,#0x0]
  000f32fc: add r0,pc
  000f32fe: ldr r0,[r0,#0x0]
  000f3300: strb r2,[r0,#0x0]
  000f3302: ldr r0,[0x000f341c]
  000f3304: str r5,[r3,#0x0]
  000f3306: ldr r4,[0x000f3420]
  000f3308: add r0,pc
  000f330a: vldr.32 s2,[r1,#0x44]
  000f330e: str.w r2,[r12,#0x0]
  000f3312: add r4,pc
  000f3314: vcmpe.f32 s2,s0
  000f3318: str.w r5,[lr,#0x0]
  000f331c: vmrs apsr,fpscr
  000f3320: vcmpe.f32 s2,#0
  000f3324: ldr.w r12,[r0,#0x0]
  000f3328: movw r0,#0x201
  000f332c: ldr r4,[r4,#0x0]
  000f332e: it ge
  000f3330: movw.ge r6,#0x33e
  000f3334: vmrs apsr,fpscr
  000f3338: vcmpe.f32 s2,s0
  000f333c: it ls
  000f333e: movw.ls r6,#0x19f
  000f3342: vmrs apsr,fpscr
  000f3346: vcmpe.f32 s2,#0
  000f334a: str r6,[r1,#0x54]
  000f334c: it ge
  000f334e: movw.ge r0,#0x2da
  000f3352: vmrs apsr,fpscr
  000f3356: it ls
  000f3358: movw.ls r0,#0x16d
  000f335c: ldr r6,[0x000f3424]
  000f335e: str r0,[r1,#0x58]
  000f3360: ldr r0,[0x000f3428]
  000f3362: add r6,pc
  000f3364: add r0,pc
  000f3366: ldr r6,[r6,#0x0]
  000f3368: ldr r0,[r0,#0x0]
  000f336a: str r2,[r0,#0x0]
  000f336c: ldr r0,[0x000f342c]
  000f336e: add r0,pc
  000f3370: ldr r0,[r0,#0x0]
  000f3372: str r2,[r0,#0x0]
  000f3374: ldr r0,[0x000f3430]
  000f3376: ldr r3,[0x000f3434]
  000f3378: add r0,pc
  000f337a: add r3,pc
  000f337c: ldr r0,[r0,#0x0]
  000f337e: ldr r3,[r3,#0x0]
  000f3380: str r5,[r3,#0x0]
  000f3382: ldr r3,[0x000f3438]
  000f3384: add r3,pc
  000f3386: ldr r3,[r3,#0x0]
  000f3388: str r2,[r3,#0x0]
  000f338a: ldr r3,[0x000f343c]
  000f338c: add r3,pc
  000f338e: ldr r3,[r3,#0x0]
  000f3390: str r2,[r3,#0x0]
  000f3392: ldr r3,[0x000f3440]
  000f3394: str r2,[r0,#0x0]
  000f3396: mov.w r0,#0x100
  000f339a: add r3,pc
  000f339c: strh.w r0,[r1,#0x60]
  000f33a0: str r2,[r6,#0x0]
  000f33a2: mov r0,r9
  000f33a4: ldr r3,[r3,#0x0]
  000f33a6: str r2,[r4,#0x0]
  000f33a8: str.w r2,[r12,#0x0]
  000f33ac: str r2,[r3,#0x0]
  000f33ae: str.w r2,[r9,#0x4]
  000f33b2: pop.w {r8,r9,r10}
  000f33b6: pop {r4,r5,r6,r7,pc}
```
