# SpaceLounge::OnTouchMove
elf 0x171928 body 284
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* SpaceLounge::OnTouchMove(int, int) */

undefined4 SpaceLounge::OnTouchMove(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 in_r2;
  float *pfVar3;
  int iVar4;
  float *pfVar5;
  uint uVar6;
  uint in_fpscr;
  float fVar7;
  float fVar8;
  
  *(int *)(param_1 + 0xb4) = param_2;
  *(undefined4 *)(param_1 + 0xb8) = in_r2;
  if ((*(char *)(param_1 + 0x1b) == '\0') && (*(char *)(param_1 + 0x19) == '\0')) {
    if (*(char *)(param_1 + 0x34) == '\0') {
      iVar1 = *(int *)(param_1 + 0x14);
      if (iVar1 == 0) {
        if (*(int **)(param_1 + 0x24) != (int *)0x0) {
          fVar7 = (float)VectorSignedToFloat(in_r2,(byte)(in_fpscr >> 0x16) & 3);
          fVar8 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
          *(undefined4 *)(param_1 + 0x88) = 0xffffffff;
          *(undefined4 *)(param_1 + 0x20) = 0xffffffff;
          iVar1 = **(int **)(param_1 + 0x24);
          for (iVar2 = 0; iVar1 != iVar2; iVar2 = iVar2 + 1) {
            iVar4 = *(int *)(*(int *)(param_1 + 0x40) + 4);
            pfVar3 = *(float **)(iVar4 + iVar2 * 8);
            if (((((int)((uint)(*pfVar3 < fVar8) << 0x1f) < 0) &&
                 (pfVar5 = *(float **)(iVar4 + iVar2 * 8 + 4), fVar8 < *pfVar5)) &&
                (fVar7 < pfVar3[1])) && ((int)((uint)(pfVar5[1] < fVar7) << 0x1f) < 0)) {
              *(int *)(param_1 + 0x88) = iVar2;
            }
          }
        }
      }
      else if (iVar1 == 3) {
        TouchButton::OnTouchMove(**(int **)(*(int *)(param_1 + 0x5c) + 4),param_2);
      }
      else if (iVar1 == 2) {
        for (uVar6 = 0; uVar6 < **(uint **)(param_1 + 0x5c); uVar6 = uVar6 + 1) {
          TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0x5c))[1] + uVar6 * 4),param_2);
        }
      }
      Layout::OnTouchMove(**(int **)(DAT_00181a44 + 0x181a22),param_2);
      if (*(char *)(param_1 + 0x1c) == '\0') {
        ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 0x60),param_2);
      }
      else {
        ListItemWindow::OnTouchMove(*(int *)(param_1 + 0xc),param_2);
      }
    }
    else {
      StarMap::OnTouchMove(*(int *)(param_1 + 4),param_2);
    }
  }
  else {
    ChoiceWindow::OnTouchMove(*(int *)(param_1 + 8),param_2);
  }
  return 0;
}

```

## target disasm
```
  00181928: push {r4,r5,r6,r7,lr}
  0018192a: add r7,sp,#0xc
  0018192c: push.w r8
  00181930: mov r6,r0
  00181932: ldrb r0,[r0,#0x1b]
  00181934: mov r8,r2
  00181936: mov r5,r1
  00181938: cmp r0,#0x0
  0018193a: strd r1,r2,[r6,#0xb4]
  0018193e: bne 0x00181944
  00181940: ldrb r0,[r6,#0x19]
  00181942: cbz r0,0x00181956
  00181944: ldr r0,[r6,#0x8]
  00181946: mov r1,r5
  00181948: mov r2,r8
  0018194a: blx 0x0007474c
  0018194e: movs r0,#0x0
  00181950: pop.w r8
  00181954: pop {r4,r5,r6,r7,pc}
  00181956: ldrb.w r0,[r6,#0x34]
  0018195a: cbz r0,0x00181968
  0018195c: ldr r0,[r6,#0x4]
  0018195e: mov r1,r5
  00181960: mov r2,r8
  00181962: blx 0x000750e8
  00181966: b 0x0018194e
  00181968: ldr r0,[r6,#0x14]
  0018196a: cbz r0,0x00181992
  0018196c: cmp r0,#0x3
  0018196e: beq 0x00181a0a
  00181970: cmp r0,#0x2
  00181972: bne 0x00181a18
  00181974: movs r4,#0x0
  00181976: b 0x00181988
  00181978: ldr r0,[r0,#0x4]
  0018197a: mov r1,r5
  0018197c: mov r2,r8
  0018197e: ldr.w r0,[r0,r4,lsl #0x2]
  00181982: blx 0x00074764
  00181986: adds r4,#0x1
  00181988: ldr r0,[r6,#0x5c]
  0018198a: ldr r1,[r0,#0x0]
  0018198c: cmp r4,r1
  0018198e: bcc 0x00181978
  00181990: b 0x00181a18
  00181992: ldr r0,[r6,#0x24]
  00181994: cmp r0,#0x0
  00181996: beq 0x00181a18
  00181998: vmov s0,r8
  0018199c: mov.w r1,#0xffffffff
  001819a0: vmov s2,r5
  001819a4: vcvt.f32.s32 s0,s0
  001819a8: vcvt.f32.s32 s2,s2
  001819ac: str.w r1,[r6,#0x88]
  001819b0: str r1,[r6,#0x20]
  001819b2: movs r1,#0x0
  001819b4: ldr r0,[r0,#0x0]
  001819b6: b 0x00181a04
  001819b8: ldr r2,[r6,#0x40]
  001819ba: ldr r3,[r2,#0x4]
  001819bc: ldr.w r2,[r3,r1,lsl #0x3]
  001819c0: vldr.32 s4,[r2]
  001819c4: vcmpe.f32 s4,s2
  001819c8: vmrs apsr,fpscr
  001819cc: bpl 0x00181a02
  001819ce: add.w r3,r3,r1, lsl #0x3
  001819d2: ldr r3,[r3,#0x4]
  001819d4: vldr.32 s4,[r3]
  001819d8: vcmpe.f32 s4,s2
  001819dc: vmrs apsr,fpscr
  001819e0: ble 0x00181a02
  001819e2: vldr.32 s4,[r2,#0x4]
  001819e6: vcmpe.f32 s4,s0
  001819ea: vmrs apsr,fpscr
  001819ee: ble 0x00181a02
  001819f0: vldr.32 s4,[r3,#0x4]
  001819f4: vcmpe.f32 s4,s0
  001819f8: vmrs apsr,fpscr
  001819fc: it mi
  001819fe: str.w.mi r1,[r6,#0x88]
  00181a02: adds r1,#0x1
  00181a04: cmp r0,r1
  00181a06: bne 0x001819b8
  00181a08: b 0x00181a18
  00181a0a: ldr r0,[r6,#0x5c]
  00181a0c: mov r1,r5
  00181a0e: mov r2,r8
  00181a10: ldr r0,[r0,#0x4]
  00181a12: ldr r0,[r0,#0x0]
  00181a14: blx 0x00074764
  00181a18: ldr r0,[0x00181a44]
  00181a1a: mov r1,r5
  00181a1c: mov r2,r8
  00181a1e: add r0,pc
  00181a20: ldr r0,[r0,#0x0]
  00181a22: ldr r0,[r0,#0x0]
  00181a24: blx 0x00074758
  00181a28: ldrb r0,[r6,#0x1c]
  00181a2a: cbz r0,0x00181a38
  00181a2c: ldr r0,[r6,#0xc]
  00181a2e: mov r1,r5
  00181a30: mov r2,r8
  00181a32: blx 0x000770f8
  00181a36: b 0x0018194e
  00181a38: ldr r0,[r6,#0x60]
  00181a3a: mov r1,r5
  00181a3c: mov r2,r8
  00181a3e: blx 0x00075bbc
  00181a42: b 0x0018194e
```
