# WantedWindow::OnTouchMove
elf 0xe21a0 body 248
Sig: undefined __thiscall OnTouchMove(WantedWindow * this, int param_1, int param_2)

## decompile
```c

/* WantedWindow::OnTouchMove(int, int) */

undefined4 __thiscall WantedWindow::OnTouchMove(WantedWindow *this,int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  int *piVar4;
  
  if (this[0x14] == (WantedWindow)0x0) {
    piVar4 = *(int **)(DAT_000f2298 + 0xf21c4);
    iVar1 = *piVar4;
    if ((((*(int *)(iVar1 + 0xc) < param_2) &&
         (param_2 < **(int **)(DAT_000f229c + 0xf21d8) - *(int *)(iVar1 + 0x10))) &&
        (param_1 < **(int **)(DAT_000f22a0 + 0xf21e6) / 2)) ||
       (**(int **)(DAT_000f22a4 + 0xf21f8) != 0)) {
      *(int *)(this + 0x90) = param_2 - *(int *)(this + 0x88);
      *(undefined4 *)(this + 0x94) = 0x3f800000;
      *(int *)(this + 0x84) = *(int *)(this + 0x84) + (param_2 - *(int *)(this + 0x88));
      *(int *)(this + 0x88) = param_2;
    }
    if (**(int **)(DAT_000f22a8 + 0xf2216) / 2 < param_1) {
      ScrollTouchWindow::OnTouchMove(*(int *)(this + 0x2c),param_1);
    }
    for (uVar3 = 0; uVar3 < **(uint **)(this + 0xc); uVar3 = uVar3 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(this + 0xc))[1] + uVar3 * 4),param_1);
    }
    Layout::OnTouchMove(*piVar4,param_1);
    if ((*(int *)(this + 0x18) != 0) &&
       (iVar1 = Wanted::isActive(*(Wanted **)
                                  (*(int *)(*(int *)(this + 0x38) + 4) + *(int *)(this + 0x30) * 4))
       , iVar1 != 0)) {
      TouchButton::OnTouchMove(*(int *)(this + 0x18),param_1);
    }
    uVar2 = getWantedAtPosition(this,param_1,param_2);
    iVar1 = param_2 - *(int *)(this + 0x9c);
    if (iVar1 < 0) {
      iVar1 = -iVar1;
    }
    if (5 < iVar1) {
      uVar2 = 0xffffffff;
    }
    *(undefined4 *)(this + 0x34) = uVar2;
  }
  else {
    StarMap::OnTouchMove(*(int *)(this + 4),param_1);
  }
  return 0;
}

```

## target disasm
```
  000f21a0: push {r4,r5,r6,r7,lr}
  000f21a2: add r7,sp,#0xc
  000f21a4: push {r8,r9,r11}
  000f21a8: mov r4,r0
  000f21aa: ldrb r0,[r0,#0x14]
  000f21ac: mov r5,r2
  000f21ae: mov r9,r1
  000f21b0: cbz r0,0x000f21be
  000f21b2: ldr r0,[r4,#0x4]
  000f21b4: mov r1,r9
  000f21b6: mov r2,r5
  000f21b8: blx 0x000750e8
  000f21bc: b 0x000f2290
  000f21be: ldr r0,[0x000f2298]
  000f21c0: add r0,pc
  000f21c2: ldr.w r8,[r0,#0x0]
  000f21c6: ldr.w r0,[r8,#0x0]
  000f21ca: ldr r1,[r0,#0xc]
  000f21cc: cmp r1,r5
  000f21ce: bge 0x000f21f2
  000f21d0: ldr r1,[0x000f229c]
  000f21d2: ldr r0,[r0,#0x10]
  000f21d4: add r1,pc
  000f21d6: ldr r1,[r1,#0x0]
  000f21d8: ldr r1,[r1,#0x0]
  000f21da: subs r0,r1,r0
  000f21dc: cmp r0,r5
  000f21de: ble 0x000f21f2
  000f21e0: ldr r0,[0x000f22a0]
  000f21e2: add r0,pc
  000f21e4: ldr r0,[r0,#0x0]
  000f21e6: ldr r0,[r0,#0x0]
  000f21e8: add.w r0,r0,r0, lsr #0x1f
  000f21ec: cmp.w r9,r0, asr #0x1
  000f21f0: blt 0x000f21fc
  000f21f2: ldr r0,[0x000f22a4]
  000f21f4: add r0,pc
  000f21f6: ldr r0,[r0,#0x0]
  000f21f8: ldr r0,[r0,#0x0]
  000f21fa: cbz r0,0x000f2210
  000f21fc: ldrd r0,r1,[r4,#0x84]
  000f2200: mov.w r2,#0x3f800000
  000f2204: subs r1,r5,r1
  000f2206: strd r1,r2,[r4,#0x90]
  000f220a: add r0,r1
  000f220c: strd r0,r5,[r4,#0x84]
  000f2210: ldr r0,[0x000f22a8]
  000f2212: add r0,pc
  000f2214: ldr r0,[r0,#0x0]
  000f2216: ldr r0,[r0,#0x0]
  000f2218: add.w r0,r0,r0, lsr #0x1f
  000f221c: cmp.w r9,r0, asr #0x1
  000f2220: ble 0x000f222c
  000f2222: ldr r0,[r4,#0x2c]
  000f2224: mov r1,r9
  000f2226: mov r2,r5
  000f2228: blx 0x00075bbc
  000f222c: movs r6,#0x0
  000f222e: b 0x000f2240
  000f2230: ldr r0,[r0,#0x4]
  000f2232: mov r1,r9
  000f2234: mov r2,r5
  000f2236: ldr.w r0,[r0,r6,lsl #0x2]
  000f223a: blx 0x00074764
  000f223e: adds r6,#0x1
  000f2240: ldr r0,[r4,#0xc]
  000f2242: ldr r1,[r0,#0x0]
  000f2244: cmp r6,r1
  000f2246: bcc 0x000f2230
  000f2248: ldr.w r0,[r8,#0x0]
  000f224c: mov r1,r9
  000f224e: mov r2,r5
  000f2250: blx 0x00074758
  000f2254: ldr r0,[r4,#0x18]
  000f2256: cbz r0,0x000f2272
  000f2258: ldr r1,[r4,#0x38]
  000f225a: ldr r0,[r4,#0x30]
  000f225c: ldr r1,[r1,#0x4]
  000f225e: ldr.w r0,[r1,r0,lsl #0x2]
  000f2262: blx 0x00073774
  000f2266: cbz r0,0x000f2272
  000f2268: ldr r0,[r4,#0x18]
  000f226a: mov r1,r9
  000f226c: mov r2,r5
  000f226e: blx 0x00074764
  000f2272: mov r0,r4
  000f2274: mov r1,r9
  000f2276: mov r2,r5
  000f2278: blx 0x00075bb0
  000f227c: ldr.w r1,[r4,#0x9c]
  000f2280: subs r1,r5,r1
  000f2282: it mi
  000f2284: rsb.mi r1,r1
  000f2286: cmp r1,#0x5
  000f2288: it gt
  000f228a: mov.gt.w r0,#0xffffffff
  000f228e: str r0,[r4,#0x34]
  000f2290: movs r0,#0x0
  000f2292: pop.w {r8,r9,r11}
  000f2296: pop {r4,r5,r6,r7,pc}
```
