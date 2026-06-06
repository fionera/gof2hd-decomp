# Player::Player
elf 0xa2570 body 416
Sig: undefined __thiscall Player(Player * this, int param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* Player::Player(int, int, int, int, int) */

void __thiscall
Player::Player(Player *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  Array<Array<Gun*>*> *this_00;
  Array<Gun*> *pAVar4;
  
  piVar2 = *(int **)(DAT_000b272c + 0xb2586);
  iVar3 = *piVar2;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 4));
  uVar1 = DAT_000b2728;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x90) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x94) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 200) = 0;
  *(undefined4 *)(this + 0xcc) = 0;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x104) = 0;
  *(int *)(this + 0x40) = param_1;
  *(int *)(this + 0x9c) = param_3;
  *(int *)(this + 0xa0) = param_4;
  *(int *)(this + 0xa4) = param_5;
  *(undefined4 *)(this + 0xb8) = uVar1;
  *(int *)(this + 0x78) = param_2;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(int *)(this + 0x84) = param_2;
  *(undefined4 *)(this + 0x6c) = 0;
  this[0x5e] = (Player)0x0;
  this[0xc3] = (Player)0x1;
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 0xd4) = 0;
  *(undefined4 *)(this + 0xd8) = 0;
  updateDamageRate(this);
  *(undefined4 *)(this + 0x58) = 0xffffffff;
  this_00 = operator_new(0xc);
  Array<Array<Gun*>*>::Array(this_00);
  *(Array<Array<Gun*>*> **)this = this_00;
  ArraySetLength<Array<Gun*>*>(3,(Array *)this_00);
  if (param_3 < 1) {
    **(undefined4 **)(*(int *)this + 4) = 0;
  }
  else {
    pAVar4 = operator_new(0xc);
    Array<Gun*>::Array(pAVar4);
    **(undefined4 **)(*(int *)this + 4) = pAVar4;
    ArraySetLength<Gun*>(param_3,(Array *)**(undefined4 **)(*(int *)this + 4));
  }
  if (param_4 < 1) {
    *(undefined4 *)(*(int *)(*(int *)this + 4) + 4) = 0;
  }
  else {
    pAVar4 = operator_new(0xc);
    Array<Gun*>::Array(pAVar4);
    *(Array<Gun*> **)(*(int *)(*(int *)this + 4) + 4) = pAVar4;
    ArraySetLength<Gun*>(param_4,*(Array **)(*(int *)(*(int *)this + 4) + 4));
  }
  if (param_5 < 1) {
    *(undefined4 *)(*(int *)(*(int *)this + 4) + 8) = 0;
  }
  else {
    pAVar4 = operator_new(0xc);
    Array<Gun*>::Array(pAVar4);
    *(Array<Gun*> **)(*(int *)(*(int *)this + 4) + 8) = pAVar4;
    ArraySetLength<Gun*>(param_5,*(Array **)(*(int *)(*(int *)this + 4) + 8));
  }
  this[0x70] = (Player)0x1;
  *(undefined4 *)(this + 0x10c) = 1;
  *(undefined2 *)(this + 0x44) = 0;
  *(undefined2 *)(this + 0xc0) = 1;
  *(undefined4 *)(this + 0xb4) = 0;
  this[0xc2] = (Player)0x1;
  *(undefined4 *)(this + 0xd0) = 0;
  *(undefined4 *)(this + 0xd4) = 0;
  *(undefined2 *)(this + 0x5c) = 0;
  this[0xe0] = (Player)0x0;
  *(undefined2 *)(this + 0xec) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined2 *)(this + 0x54) = 0;
  *(undefined2 *)(this + 0x68) = 0;
  this[0xee] = (Player)0x0;
  *(undefined4 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0xf0) = 0;
  this[0x108] = (Player)0x0;
  this[0xf8] = (Player)0x0;
  *(undefined4 *)(this + 100) = 0;
  AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&stack0xffffffcc);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0xfc),(Vector *)&stack0xffffffcc);
  *(undefined4 *)(this + 0xf4) = 0xffffffff;
  iVar3 = *piVar2 - iVar3;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```
## target disasm
```
  000b2570: push {r4,r5,r6,r7,lr}
  000b2572: add r7,sp,#0xc
  000b2574: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000b2578: mov r4,r0
  000b257a: ldr r0,[0x000b272c]
  000b257c: add.w r10,r4,#0x4
  000b2580: mov r8,r3
  000b2582: add r0,pc
  000b2584: mov r6,r2
  000b2586: mov r5,r1
  000b2588: ldr r0,[r0,#0x0]
  000b258a: str r0,[sp,#0x0]
  000b258c: ldr r0,[r0,#0x0]
  000b258e: str r0,[sp,#0x10]
  000b2590: mov r0,r10
  000b2592: blx 0x0006f118
  000b2596: vmov.i32 q8,#0x0
  000b259a: add.w r1,r4,#0x88
  000b259e: ldr.w r9,[r7,#0x8]
  000b25a2: mov r11,r4
  000b25a4: ldr r0,[0x000b2728]
  000b25a6: vst1.32 {d16,d17},[r1]
  000b25aa: movs r1,#0x0
  000b25ac: ldr r2,[r7,#0xc]
  000b25ae: strd r1,r1,[r4,#0xc4]
  000b25b2: str.w r1,[r4,#0xcc]
  000b25b6: strd r1,r1,[r4,#0x100]
  000b25ba: str r5,[r4,#0x40]
  000b25bc: strd r8,r9,[r4,#0x9c]
  000b25c0: str.w r2,[r4,#0xa4]
  000b25c4: str.w r0,[r4,#0xb8]
  000b25c8: movs r0,#0x1
  000b25ca: strd r6,r1,[r4,#0x78]
  000b25ce: strd r1,r6,[r4,#0x80]
  000b25d2: str r1,[r4,#0x6c]
  000b25d4: strb.w r1,[r4,#0x5e]
  000b25d8: strb.w r0,[r4,#0xc3]
  000b25dc: mov r0,r4
  000b25de: str.w r1,[r11,#0xfc]!
  000b25e2: str r1,[r4,#0x60]
  000b25e4: strd r1,r1,[r4,#0xd4]
  000b25e8: blx 0x00072df0
  000b25ec: mov.w r0,#0xffffffff
  000b25f0: str r0,[r4,#0x58]
  000b25f2: movs r0,#0xc
  000b25f4: blx 0x0006eb24
  000b25f8: mov r6,r0
  000b25fa: blx 0x00072dfc
  000b25fe: movs r0,#0x3
  000b2600: mov r1,r6
  000b2602: str r6,[r4,#0x0]
  000b2604: blx 0x00072e08
  000b2608: cmp.w r8,#0x1
  000b260c: blt 0x000b262e
  000b260e: movs r0,#0xc
  000b2610: blx 0x0006eb24
  000b2614: mov r6,r0
  000b2616: blx 0x00072e14
  000b261a: ldr r0,[r4,#0x0]
  000b261c: ldr r0,[r0,#0x4]
  000b261e: str r6,[r0,#0x0]
  000b2620: ldr r0,[r4,#0x0]
  000b2622: ldr r0,[r0,#0x4]
  000b2624: ldr r1,[r0,#0x0]
  000b2626: mov r0,r8
  000b2628: blx 0x00072e20
  000b262c: b 0x000b2636
  000b262e: ldr r0,[r4,#0x0]
  000b2630: movs r1,#0x0
  000b2632: ldr r0,[r0,#0x4]
  000b2634: str r1,[r0,#0x0]
  000b2636: cmp.w r9,#0x1
  000b263a: blt 0x000b265c
  000b263c: movs r0,#0xc
  000b263e: blx 0x0006eb24
  000b2642: mov r5,r0
  000b2644: blx 0x00072e14
  000b2648: ldr r0,[r4,#0x0]
  000b264a: ldr r0,[r0,#0x4]
  000b264c: str r5,[r0,#0x4]
  000b264e: ldr r0,[r4,#0x0]
  000b2650: ldr r0,[r0,#0x4]
  000b2652: ldr r1,[r0,#0x4]
  000b2654: mov r0,r9
  000b2656: blx 0x00072e20
  000b265a: b 0x000b2664
  000b265c: ldr r0,[r4,#0x0]
  000b265e: movs r1,#0x0
  000b2660: ldr r0,[r0,#0x4]
  000b2662: str r1,[r0,#0x4]
  000b2664: ldr r6,[r7,#0xc]
  000b2666: cmp r6,#0x1
  000b2668: blt 0x000b268a
  000b266a: movs r0,#0xc
  000b266c: blx 0x0006eb24
  000b2670: mov r5,r0
  000b2672: blx 0x00072e14
  000b2676: ldr r0,[r4,#0x0]
  000b2678: ldr r0,[r0,#0x4]
  000b267a: str r5,[r0,#0x8]
  000b267c: ldr r0,[r4,#0x0]
  000b267e: ldr r0,[r0,#0x4]
  000b2680: ldr r1,[r0,#0x8]
  000b2682: mov r0,r6
  000b2684: blx 0x00072e20
  000b2688: b 0x000b2692
  000b268a: ldr r0,[r4,#0x0]
  000b268c: movs r1,#0x0
  000b268e: ldr r0,[r0,#0x4]
  000b2690: str r1,[r0,#0x8]
  000b2692: add r5,sp,#0x4
  000b2694: movs r0,#0x1
  000b2696: movs r1,#0x0
  000b2698: strb.w r0,[r4,#0x70]
  000b269c: str.w r0,[r4,#0x10c]
  000b26a0: strh.w r1,[r4,#0x44]
  000b26a4: strh.w r0,[r4,#0xc0]
  000b26a8: str.w r1,[r4,#0xb4]
  000b26ac: strb.w r0,[r4,#0xc2]
  000b26b0: mov r0,r5
  000b26b2: strd r1,r1,[r4,#0xd0]
  000b26b6: strh.w r1,[r4,#0x5c]
  000b26ba: strb.w r1,[r4,#0xe0]
  000b26be: strh.w r1,[r4,#0xec]
  000b26c2: str r1,[r4,#0x74]
  000b26c4: strh.w r1,[r4,#0x54]
  000b26c8: strh.w r1,[r4,#0x68]
  000b26cc: strb.w r1,[r4,#0xee]
  000b26d0: str.w r1,[r4,#0x110]
  000b26d4: str.w r1,[r4,#0xf0]
  000b26d8: strb.w r1,[r4,#0x108]
  000b26dc: strb.w r1,[r4,#0xf8]
  000b26e0: str r1,[r4,#0x64]
  000b26e2: mov r1,r10
  000b26e4: blx 0x0006f16c
  000b26e8: mov r0,r11
  000b26ea: mov r1,r5
  000b26ec: blx 0x0006eb3c
  000b26f0: mov.w r0,#0xffffffff
  000b26f4: str.w r0,[r4,#0xf4]
  000b26f8: ldr r0,[sp,#0x10]
  000b26fa: ldr r1,[sp,#0x0]
  000b26fc: ldr r1,[r1,#0x0]
  000b26fe: subs r0,r1,r0
  000b2700: itttt eq
  000b2702: mov.eq r0,r4
  000b2704: add.eq sp,#0x14
  000b2706: pop.eq.w {r8,r9,r10,r11}
  000b270a: pop.eq {r4,r5,r6,r7,pc}
  000b270c: blx 0x0006e824
```
