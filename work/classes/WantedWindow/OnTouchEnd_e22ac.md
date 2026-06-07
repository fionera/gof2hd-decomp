# WantedWindow::OnTouchEnd
elf 0xe22ac body 830
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* WantedWindow::OnTouchEnd(int, int) */

void WantedWindow::OnTouchEnd(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  Station *this;
  int *piVar4;
  Mission *pMVar5;
  String *pSVar6;
  undefined4 uVar7;
  void *pvVar8;
  uint uVar9;
  int iVar10;
  undefined4 *puVar11;
  int in_r2;
  uint uVar12;
  StarMap *pSVar13;
  int *piVar14;
  uint *puVar15;
  uint in_fpscr;
  undefined4 uVar16;
  
  piVar14 = *(int **)(DAT_000f2614 + 0xf22c0);
  iVar1 = *piVar14;
  if (*(char *)(param_1 + 0x14) == '\0') {
    iVar2 = *(int *)(param_1 + 0x90);
    uVar3 = 0;
    iVar10 = *(int *)(param_1 + 0x84) + iVar2;
    uVar7 = VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    uVar16 = DAT_000f260c;
    if (3 < iVar2) {
      uVar16 = uVar7;
    }
    *(undefined4 *)(param_1 + 0x94) = DAT_000f2610;
    *(undefined1 *)(param_1 + 0xa0) = 0;
    *(int *)(param_1 + 0x84) = iVar10;
    *(int *)(param_1 + 0x8c) = iVar10;
    *(undefined4 *)(param_1 + 0x98) = uVar16;
    ScrollTouchWindow::OnTouchEnd(*(int *)(param_1 + 0x2c),param_2);
    for (; uVar3 < **(uint **)(param_1 + 0xc); uVar3 = uVar3 + 1) {
      iVar2 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(param_1 + 0xc))[1] + uVar3 * 4),param_2);
      if (iVar2 != 0) {
        *(uint *)param_1 = uVar3;
      }
    }
    if (-1 < *(int *)(param_1 + 0x34)) {
      iVar2 = getWantedAtPosition((WantedWindow *)param_1,param_2,in_r2);
      *(int *)(param_1 + 0x30) = iVar2;
      *(int *)(param_1 + 0x34) = iVar2;
      selectWanted((WantedWindow *)param_1,iVar2);
    }
    if (((*(int *)(param_1 + 0x18) == 0) ||
        (iVar2 = Wanted::isActive(*(Wanted **)
                                   (*(int *)(*(int *)(param_1 + 0x38) + 4) +
                                   *(int *)(param_1 + 0x30) * 4)), iVar2 == 0)) ||
       (iVar2 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x18),param_2), iVar2 == 0)) {
      puVar11 = *(undefined4 **)(DAT_000f2640 + 0xf246a);
      iVar2 = Layout::OnTouchEnd((Layout *)*puVar11,param_2,in_r2);
      if (iVar2 != 0) {
        Layout::resetWindowDimensions((Layout *)*puVar11);
        uVar7 = 1;
        goto LAB_000f25d4;
      }
      iVar2 = Layout::helpPressed((Layout *)*puVar11);
      if (iVar2 != 0) {
        uVar7 = *puVar11;
        pSVar6 = (String *)GameText::getText(**(int **)(DAT_000f2644 + 0xf2494));
        AbyssEngine::String::String((String *)&stack0xffffffcc,pSVar6,false);
        Layout::initHelpWindow(uVar7,(String *)&stack0xffffffcc);
        AbyssEngine::String::~String((String *)&stack0xffffffcc);
      }
    }
    else {
      puVar15 = *(uint **)(DAT_000f2634 + 0xf23cc);
      iVar2 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar15);
      puVar11 = (undefined4 *)(DAT_000f2638 + 0xf23e2);
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(iVar2 + 0x10);
      iVar2 = *(int *)*puVar11;
      Wanted::getLastSeen(*(Wanted **)
                           (*(int *)(*(int *)(param_1 + 0x38) + 4) + *(int *)(param_1 + 0x30) * 4));
      this = (Station *)Galaxy::getStation(iVar2);
      piVar4 = *(int **)(param_1 + 0xb0);
      if (*(int *)(param_1 + 4) == 0) {
        if (piVar4 != (int *)0x0) {
          (**(code **)(*piVar4 + 4))();
        }
        *(undefined4 *)(param_1 + 0xb0) = 0;
        pMVar5 = operator_new(0x78);
        iVar2 = Wanted::getTravelsTo
                          (*(Wanted **)
                            (*(int *)(*(int *)(param_1 + 0x38) + 4) + *(int *)(param_1 + 0x30) * 4))
        ;
        Mission::Mission(pMVar5,0,0,iVar2);
        *(Mission **)(param_1 + 0xb0) = pMVar5;
        pSVar13 = operator_new(0x1e8);
        StarMap::StarMap(pSVar13,true,pMVar5,false,-1);
        iVar2 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar15);
        *(StarMap **)(iVar2 + 0x10) = pSVar13;
        iVar2 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar15);
        pSVar13 = *(StarMap **)(iVar2 + 0x10);
        *(StarMap **)(param_1 + 4) = pSVar13;
      }
      else {
        if (piVar4 != (int *)0x0) {
          (**(code **)(*piVar4 + 4))();
        }
        *(undefined4 *)(param_1 + 0xb0) = 0;
        pMVar5 = operator_new(0x78);
        iVar2 = Wanted::getTravelsTo
                          (*(Wanted **)
                            (*(int *)(*(int *)(param_1 + 0x38) + 4) + *(int *)(param_1 + 0x30) * 4))
        ;
        Mission::Mission(pMVar5,0,0,iVar2);
        *(Mission **)(param_1 + 0xb0) = pMVar5;
        StarMap::init(*(EVP_PKEY_CTX **)(param_1 + 4));
        pSVar13 = *(StarMap **)(param_1 + 4);
      }
      iVar2 = Station::getSystem(this);
      iVar10 = Wanted::getLastSeen(*(Wanted **)
                                    (*(int *)(*(int *)(param_1 + 0x38) + 4) +
                                    *(int *)(param_1 + 0x30) * 4));
      StarMap::setStart(pSVar13,iVar2,iVar10);
      if (this != (Station *)0x0) {
        pvVar8 = (void *)Station::~Station(this);
        operator_delete(pvVar8);
      }
      iVar2 = DAT_000f263c;
      *(undefined1 *)(param_1 + 0x14) = 1;
      Layout::resetWindowDimensions((Layout *)**(undefined4 **)(iVar2 + 0xf25cc));
    }
  }
  else {
    iVar2 = StarMap::OnTouchEnd(*(int *)(param_1 + 4),param_2);
    if (iVar2 != 0) {
      if (**(char **)(DAT_000f2618 + 0xf22e6) == '\0') {
        uVar3 = **(uint **)(DAT_000f262c + 0xf24c0);
        uVar9 = **(uint **)(DAT_000f2630 + 0xf24c2);
        *(undefined4 *)(param_1 + 0x1c) = 0;
        *(undefined4 *)(param_1 + 0x20) = 0;
      }
      else {
        if (**(char **)(DAT_000f261c + 0xf22f4) == '\0') {
          uVar3 = 1000;
          uVar9 = 0x514;
          if (**(char **)(DAT_000f2620 + 0xf24d4) == '\0') {
            uVar3 = 500;
            uVar9 = 0x28a;
          }
          uVar12 = uVar9 >> 1;
        }
        else {
          uVar3 = 0x2bf;
          uVar12 = 0x1c9;
          uVar9 = 0x392;
        }
        iVar2 = **(int **)(DAT_000f2628 + 0xf24f8);
        *(uint *)(param_1 + 0x1c) = (**(int **)(DAT_000f2624 + 0xf24f6) >> 1) - uVar12;
        *(uint *)(param_1 + 0x20) = (iVar2 >> 1) - (uVar3 >> 1);
      }
      *(uint *)(param_1 + 0x24) = uVar9;
      *(uint *)(param_1 + 0x28) = uVar3;
      uVar7 = 0;
      *(undefined1 *)(param_1 + 0x14) = 0;
      goto LAB_000f25d4;
    }
  }
  uVar7 = 0;
LAB_000f25d4:
  if (*piVar14 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

```

## target disasm
```
  000f22ac: push {r4,r5,r6,r7,lr}
  000f22ae: add r7,sp,#0xc
  000f22b0: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000f22b4: mov r11,r0
  000f22b6: ldr r0,[0x000f2614]
  000f22b8: mov r5,r2
  000f22ba: mov r6,r1
  000f22bc: add r0,pc
  000f22be: ldr.w r9,[r0,#0x0]
  000f22c2: ldr.w r0,[r9,#0x0]
  000f22c6: str r0,[sp,#0x10]
  000f22c8: ldrb.w r0,[r11,#0x14]
  000f22cc: cbz r0,0x000f230a
  000f22ce: ldr.w r0,[r11,#0x4]
  000f22d2: mov r1,r6
  000f22d4: mov r2,r5
  000f22d6: blx 0x000751e4
  000f22da: cmp r0,#0x0
  000f22dc: beq.w 0x000f25d2
  000f22e0: ldr r0,[0x000f2618]
  000f22e2: add r0,pc
  000f22e4: ldr r0,[r0,#0x0]
  000f22e6: ldrb r0,[r0,#0x0]
  000f22e8: cmp r0,#0x0
  000f22ea: beq.w 0x000f24b6
  000f22ee: ldr r0,[0x000f261c]
  000f22f0: add r0,pc
  000f22f2: ldr r0,[r0,#0x0]
  000f22f4: ldrb r0,[r0,#0x0]
  000f22f6: cmp r0,#0x0
  000f22f8: beq.w 0x000f24ce
  000f22fc: movw r0,#0x2bf
  000f2300: movw r2,#0x1c9
  000f2304: movw r1,#0x392
  000f2308: b 0x000f24ee
  000f230a: ldr.w r2,[r11,#0x90]
  000f230e: movs r4,#0x0
  000f2310: ldr.w r1,[r11,#0x84]
  000f2314: ldr.w r0,[r11,#0x2c]
  000f2318: cmp r2,#0x0
  000f231a: vmov s2,r2
  000f231e: mov r3,r2
  000f2320: vldr.32 s0,[pc,#0x2e8]
  000f2324: add r1,r2
  000f2326: vcvt.f32.s32 s2,s2
  000f232a: it mi
  000f232c: rsb.mi r3,r2
  000f232e: cmp r3,#0x3
  000f2330: ldr r3,[0x000f2610]
  000f2332: mov r2,r5
  000f2334: it gt
  000f2336: vmov.gt.f32 s0,s2
  000f233a: str.w r3,[r11,#0x94]
  000f233e: strb.w r4,[r11,#0xa0]
  000f2342: str.w r1,[r11,#0x84]
  000f2346: str.w r1,[r11,#0x8c]
  000f234a: mov r1,r6
  000f234c: vstr.32 s0,[r11,#0x98]
  000f2350: blx 0x00075bc8
  000f2354: b 0x000f236c
  000f2356: ldr r0,[r0,#0x4]
  000f2358: mov r1,r6
  000f235a: mov r2,r5
  000f235c: ldr.w r0,[r0,r4,lsl #0x2]
  000f2360: blx 0x00074788
  000f2364: cbz r0,0x000f236a
  000f2366: str.w r4,[r11,#0x0]
  000f236a: adds r4,#0x1
  000f236c: ldr.w r0,[r11,#0xc]
  000f2370: ldr r1,[r0,#0x0]
  000f2372: cmp r4,r1
  000f2374: bcc 0x000f2356
  000f2376: ldr.w r0,[r11,#0x34]
  000f237a: cmp.w r0,#0xffffffff
  000f237e: ble 0x000f2396
  000f2380: mov r0,r11
  000f2382: mov r1,r6
  000f2384: mov r2,r5
  000f2386: blx 0x00075bb0
  000f238a: mov r1,r0
  000f238c: strd r0,r0,[r11,#0x30]
  000f2390: mov r0,r11
  000f2392: blx 0x00075ab4
  000f2396: ldr.w r0,[r11,#0x18]
  000f239a: cmp r0,#0x0
  000f239c: beq 0x000f2460
  000f239e: ldr.w r1,[r11,#0x38]
  000f23a2: ldr.w r0,[r11,#0x30]
  000f23a6: ldr r1,[r1,#0x4]
  000f23a8: ldr.w r0,[r1,r0,lsl #0x2]
  000f23ac: blx 0x00073774
  000f23b0: cmp r0,#0x0
  000f23b2: beq 0x000f2460
  000f23b4: ldr.w r0,[r11,#0x18]
  000f23b8: mov r1,r6
  000f23ba: mov r2,r5
  000f23bc: blx 0x00074788
  000f23c0: cmp r0,#0x0
  000f23c2: beq 0x000f2460
  000f23c4: ldr r0,[0x000f2634]
  000f23c6: movs r1,#0x5
  000f23c8: add r0,pc
  000f23ca: ldr.w r10,[r0,#0x0]
  000f23ce: ldr.w r0,[r10,#0x0]
  000f23d2: blx 0x00075bd4
  000f23d6: ldr r1,[0x000f2638]
  000f23d8: ldr.w r3,[r11,#0x38]
  000f23dc: ldr r0,[r0,#0x10]
  000f23de: add r1,pc
  000f23e0: ldr.w r2,[r11,#0x30]
  000f23e4: str.w r0,[r11,#0x4]
  000f23e8: ldr r0,[r3,#0x4]
  000f23ea: ldr r1,[r1,#0x0]
  000f23ec: ldr.w r0,[r0,r2,lsl #0x2]
  000f23f0: ldr r5,[r1,#0x0]
  000f23f2: blx 0x00074c2c
  000f23f6: mov r1,r0
  000f23f8: mov r0,r5
  000f23fa: blx 0x00071c44
  000f23fe: mov r8,r0
  000f2400: ldr.w r1,[r11,#0x4]
  000f2404: ldr.w r0,[r11,#0xb0]
  000f2408: cmp r1,#0x0
  000f240a: beq.w 0x000f251c
  000f240e: cbz r0,0x000f2416
  000f2410: ldr r1,[r0,#0x0]
  000f2412: ldr r1,[r1,#0x4]
  000f2414: blx r1
  000f2416: movs r0,#0x0
  000f2418: str.w r0,[r11,#0xb0]
  000f241c: movs r0,#0x78
  000f241e: blx 0x0006eb24
  000f2422: ldr.w r1,[r11,#0x38]
  000f2426: mov r6,r0
  000f2428: ldr.w r0,[r11,#0x30]
  000f242c: ldr r1,[r1,#0x4]
  000f242e: ldr.w r0,[r1,r0,lsl #0x2]
  000f2432: blx 0x000737b0
  000f2436: mov r3,r0
  000f2438: mov r0,r6
  000f243a: movs r1,#0x0
  000f243c: movs r2,#0x0
  000f243e: blx 0x00073834
  000f2442: ldr.w r0,[r11,#0x4]
  000f2446: mov.w r1,#0xffffffff
  000f244a: str.w r6,[r11,#0xb0]
  000f244e: mov r2,r6
  000f2450: str r1,[sp,#0x0]
  000f2452: movs r1,#0x1
  000f2454: movs r3,#0x0
  000f2456: blx 0x00074554
  000f245a: ldr.w r5,[r11,#0x4]
  000f245e: b 0x000f258c
  000f2460: ldr r0,[0x000f2640]
  000f2462: mov r1,r6
  000f2464: mov r2,r5
  000f2466: add r0,pc
  000f2468: ldr r4,[r0,#0x0]
  000f246a: ldr r0,[r4,#0x0]
  000f246c: blx 0x0007477c
  000f2470: mov r1,r0
  000f2472: ldr r0,[r4,#0x0]
  000f2474: cbz r1,0x000f247e
  000f2476: blx 0x00074dc4
  000f247a: movs r0,#0x1
  000f247c: b 0x000f25d4
  000f247e: blx 0x000747b8
  000f2482: cmp r0,#0x0
  000f2484: beq.w 0x000f25d2
  000f2488: ldr r0,[0x000f2644]
  000f248a: movw r1,#0x27b
  000f248e: ldr r4,[r4,#0x0]
  000f2490: add r0,pc
  000f2492: ldr r0,[r0,#0x0]
  000f2494: ldr r0,[r0,#0x0]
  000f2496: blx 0x00072f70
  000f249a: add r5,sp,#0x4
  000f249c: mov r1,r0
  000f249e: movs r2,#0x0
  000f24a0: mov r0,r5
  000f24a2: blx 0x0006f028
  000f24a6: mov r0,r4
  000f24a8: mov r1,r5
  000f24aa: blx 0x000747c4
  000f24ae: add r0,sp,#0x4
  000f24b0: blx 0x0006ee30
  000f24b4: b 0x000f25d2
  000f24b6: ldr r0,[0x000f262c]
  000f24b8: movs r2,#0x0
  000f24ba: ldr r1,[0x000f2630]
  000f24bc: add r0,pc
  000f24be: add r1,pc
  000f24c0: ldr r0,[r0,#0x0]
  000f24c2: ldr r1,[r1,#0x0]
  000f24c4: ldr r0,[r0,#0x0]
  000f24c6: ldr r1,[r1,#0x0]
  000f24c8: strd r2,r2,[r11,#0x1c]
  000f24cc: b 0x000f250c
  000f24ce: ldr r0,[0x000f2620]
  000f24d0: add r0,pc
  000f24d2: ldr r0,[r0,#0x0]
  000f24d4: ldrb r1,[r0,#0x0]
  000f24d6: mov.w r0,#0x3e8
  000f24da: cmp r1,#0x0
  000f24dc: movw r1,#0x514
  000f24e0: it eq
  000f24e2: mov.eq.w r0,#0x1f4
  000f24e6: it eq
  000f24e8: movw.eq r1,#0x28a
  000f24ec: lsrs r2,r1,#0x1
  000f24ee: ldr r3,[0x000f2624]
  000f24f0: ldr r4,[0x000f2628]
  000f24f2: add r3,pc
  000f24f4: add r4,pc
  000f24f6: ldr r3,[r3,#0x0]
  000f24f8: ldr r6,[r4,#0x0]
  000f24fa: ldr r3,[r3,#0x0]
  000f24fc: rsb r2,r2,r3, asr #0x1
  000f2500: ldr r3,[r6,#0x0]
  000f2502: asrs r3,r3,#0x1
  000f2504: sub.w r3,r3,r0, lsr #0x1
  000f2508: strd r2,r3,[r11,#0x1c]
  000f250c: str.w r1,[r11,#0x24]
  000f2510: str.w r0,[r11,#0x28]
  000f2514: movs r0,#0x0
  000f2516: strb.w r0,[r11,#0x14]
  000f251a: b 0x000f25d4
  000f251c: cbz r0,0x000f2524
  000f251e: ldr r1,[r0,#0x0]
  000f2520: ldr r1,[r1,#0x4]
  000f2522: blx r1
  000f2524: movs r0,#0x0
  000f2526: str.w r0,[r11,#0xb0]
  000f252a: movs r0,#0x78
  000f252c: blx 0x0006eb24
  000f2530: ldr.w r1,[r11,#0x38]
  000f2534: mov r6,r0
  000f2536: ldr.w r0,[r11,#0x30]
  000f253a: ldr r1,[r1,#0x4]
  000f253c: ldr.w r0,[r1,r0,lsl #0x2]
  000f2540: blx 0x000737b0
  000f2544: mov r3,r0
  000f2546: mov r0,r6
  000f2548: movs r1,#0x0
  000f254a: movs r2,#0x0
  000f254c: blx 0x00073834
  000f2550: mov.w r0,#0x1e8
  000f2554: str.w r6,[r11,#0xb0]
  000f2558: blx 0x0006eb24
  000f255c: mov r5,r0
  000f255e: mov.w r0,#0xffffffff
  000f2562: movs r1,#0x1
  000f2564: str r0,[sp,#0x0]
  000f2566: mov r0,r5
  000f2568: mov r2,r6
  000f256a: movs r3,#0x0
  000f256c: blx 0x00075424
  000f2570: ldr.w r0,[r10,#0x0]
  000f2574: movs r1,#0x5
  000f2576: blx 0x00075bd4
  000f257a: str r5,[r0,#0x10]
  000f257c: movs r1,#0x5
  000f257e: ldr.w r0,[r10,#0x0]
  000f2582: blx 0x00075bd4
  000f2586: ldr r5,[r0,#0x10]
  000f2588: str.w r5,[r11,#0x4]
  000f258c: mov r0,r8
  000f258e: blx 0x0007189c
  000f2592: ldr.w r1,[r11,#0x38]
  000f2596: mov r6,r0
  000f2598: ldr.w r0,[r11,#0x30]
  000f259c: ldr r1,[r1,#0x4]
  000f259e: ldr.w r0,[r1,r0,lsl #0x2]
  000f25a2: blx 0x00074c2c
  000f25a6: mov r2,r0
  000f25a8: mov r0,r5
  000f25aa: mov r1,r6
  000f25ac: blx 0x00075be0
  000f25b0: cmp.w r8,#0x0
  000f25b4: beq 0x000f25c0
  000f25b6: mov r0,r8
  000f25b8: blx 0x0007360c
  000f25bc: blx 0x0006eb48
  000f25c0: ldr r0,[0x000f263c]
  000f25c2: movs r1,#0x1
  000f25c4: strb.w r1,[r11,#0x14]
  000f25c8: add r0,pc
  000f25ca: ldr r0,[r0,#0x0]
  000f25cc: ldr r0,[r0,#0x0]
  000f25ce: blx 0x00074dc4
  000f25d2: movs r0,#0x0
  000f25d4: ldr r1,[sp,#0x10]
  000f25d6: ldr.w r2,[r9,#0x0]
  000f25da: subs r1,r2,r1
  000f25dc: ittt eq
  000f25de: add.eq sp,#0x14
  000f25e0: pop.eq.w {r8,r9,r10,r11}
  000f25e4: pop.eq {r4,r5,r6,r7,pc}
  000f25e6: blx 0x0006e824
```
