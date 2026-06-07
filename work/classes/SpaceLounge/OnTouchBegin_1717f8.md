# SpaceLounge::OnTouchBegin
elf 0x1717f8 body 298
Sig: undefined __thiscall OnTouchBegin(SpaceLounge * this, int param_1, int param_2)

## decompile
```c

/* SpaceLounge::OnTouchBegin(int, int) */

undefined4 __thiscall SpaceLounge::OnTouchBegin(SpaceLounge *this,int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  float *pfVar3;
  float *pfVar4;
  uint uVar5;
  uint in_fpscr;
  float fVar6;
  float fVar7;
  
  this[0xb2] = (SpaceLounge)0x1;
  *(int *)(this + 0xb4) = param_1;
  *(int *)(this + 0xb8) = param_2;
  *(undefined4 *)(this + 0x88) = 0xffffffff;
  if ((this[0x1b] == (SpaceLounge)0x0) && (this[0x19] == (SpaceLounge)0x0)) {
    if (this[0x34] == (SpaceLounge)0x0) {
      if (this[0x1c] == (SpaceLounge)0x0) {
        iVar1 = *(int *)(this + 0x14);
        if (iVar1 == 0) {
          if (this[0xbd] == (SpaceLounge)0x0) {
            return 0;
          }
          if (*(int **)(this + 0x24) != (int *)0x0) {
            iVar2 = 0;
            fVar6 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
            fVar7 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
            iVar1 = **(int **)(this + 0x24);
            for (; iVar1 != iVar2; iVar2 = iVar2 + 1) {
              pfVar3 = *(float **)(*(int *)(*(int *)(this + 0x40) + 4) + iVar2 * 8);
              if (((((int)((uint)(*pfVar3 < fVar7) << 0x1f) < 0) &&
                   (pfVar4 = *(float **)(*(int *)(*(int *)(this + 0x40) + 4) + iVar2 * 8 + 4),
                   fVar7 < *pfVar4)) && (fVar6 < pfVar3[1])) &&
                 ((int)((uint)(pfVar4[1] < fVar6) << 0x1f) < 0)) {
                *(int *)(this + 0x88) = iVar2;
              }
            }
          }
        }
        else if (iVar1 == 3) {
          TouchButton::OnTouchBegin(**(int **)(*(int *)(this + 0x5c) + 4),param_1);
        }
        else if (iVar1 == 2) {
          for (uVar5 = 0; uVar5 < **(uint **)(this + 0x5c); uVar5 = uVar5 + 1) {
            TouchButton::OnTouchBegin(*(int *)((*(uint **)(this + 0x5c))[1] + uVar5 * 4),param_1);
          }
        }
      }
      else {
        ListItemWindow::OnTouchBegin(*(ListItemWindow **)(this + 0xc),param_1,param_2);
      }
      Layout::OnTouchBegin(**(int **)(DAT_00181924 + 0x18185c),param_1);
      ScrollTouchWindow::OnTouchBegin(*(int *)(this + 0x60),param_1);
    }
    else {
      StarMap::OnTouchBegin(*(int *)(this + 4),param_1);
    }
  }
  else {
    ChoiceWindow::OnTouchBegin(*(int *)(this + 8),param_1);
  }
  return 0;
}

```

## target disasm
```
  001817f8: push {r4,r5,r6,r7,lr}
  001817fa: add r7,sp,#0xc
  001817fc: push.w r8
  00181800: mov r6,r0
  00181802: movs r0,#0x1
  00181804: strb.w r0,[r6,#0xb2]
  00181808: mov r4,r2
  0018180a: strd r1,r2,[r6,#0xb4]
  0018180e: mov r5,r1
  00181810: ldrb r0,[r6,#0x1b]
  00181812: mov.w r1,#0xffffffff
  00181816: str.w r1,[r6,#0x88]
  0018181a: cbnz r0,0x00181820
  0018181c: ldrb r0,[r6,#0x19]
  0018181e: cbz r0,0x00181832
  00181820: ldr r0,[r6,#0x8]
  00181822: mov r1,r5
  00181824: mov r2,r4
  00181826: blx 0x0007471c
  0018182a: movs r0,#0x0
  0018182c: pop.w r8
  00181830: pop {r4,r5,r6,r7,pc}
  00181832: ldrb.w r0,[r6,#0x34]
  00181836: cbz r0,0x00181844
  00181838: ldr r0,[r6,#0x4]
  0018183a: mov r1,r5
  0018183c: mov r2,r4
  0018183e: blx 0x00074704
  00181842: b 0x0018182a
  00181844: ldrb r0,[r6,#0x1c]
  00181846: cbz r0,0x0018186e
  00181848: ldr r0,[r6,#0xc]
  0018184a: mov r1,r5
  0018184c: mov r2,r4
  0018184e: blx 0x00077098
  00181852: ldr r0,[0x00181924]
  00181854: mov r1,r5
  00181856: mov r2,r4
  00181858: add r0,pc
  0018185a: ldr r0,[r0,#0x0]
  0018185c: ldr r0,[r0,#0x0]
  0018185e: blx 0x00074728
  00181862: ldr r0,[r6,#0x60]
  00181864: mov r1,r5
  00181866: mov r2,r4
  00181868: blx 0x00075ba4
  0018186c: b 0x0018182a
  0018186e: ldr r0,[r6,#0x14]
  00181870: cbz r0,0x0018189c
  00181872: cmp r0,#0x3
  00181874: beq 0x00181912
  00181876: cmp r0,#0x2
  00181878: bne 0x00181852
  0018187a: mov.w r8,#0x0
  0018187e: b 0x00181892
  00181880: ldr r0,[r0,#0x4]
  00181882: mov r1,r5
  00181884: mov r2,r4
  00181886: ldr.w r0,[r0,r8,lsl #0x2]
  0018188a: blx 0x00074734
  0018188e: add.w r8,r8,#0x1
  00181892: ldr r0,[r6,#0x5c]
  00181894: ldr r1,[r0,#0x0]
  00181896: cmp r8,r1
  00181898: bcc 0x00181880
  0018189a: b 0x00181852
  0018189c: ldrb.w r0,[r6,#0xbd]
  001818a0: cmp r0,#0x0
  001818a2: beq 0x0018182a
  001818a4: ldr r0,[r6,#0x24]
  001818a6: cmp r0,#0x0
  001818a8: beq 0x00181852
  001818aa: vmov s0,r4
  001818ae: movs r1,#0x0
  001818b0: vmov s2,r5
  001818b4: vcvt.f32.s32 s0,s0
  001818b8: vcvt.f32.s32 s2,s2
  001818bc: ldr r0,[r0,#0x0]
  001818be: b 0x0018190c
  001818c0: ldr r2,[r6,#0x40]
  001818c2: ldr r3,[r2,#0x4]
  001818c4: ldr.w r2,[r3,r1,lsl #0x3]
  001818c8: vldr.32 s4,[r2]
  001818cc: vcmpe.f32 s4,s2
  001818d0: vmrs apsr,fpscr
  001818d4: bpl 0x0018190a
  001818d6: add.w r3,r3,r1, lsl #0x3
  001818da: ldr r3,[r3,#0x4]
  001818dc: vldr.32 s4,[r3]
  001818e0: vcmpe.f32 s4,s2
  001818e4: vmrs apsr,fpscr
  001818e8: ble 0x0018190a
  001818ea: vldr.32 s4,[r2,#0x4]
  001818ee: vcmpe.f32 s4,s0
  001818f2: vmrs apsr,fpscr
  001818f6: ble 0x0018190a
  001818f8: vldr.32 s4,[r3,#0x4]
  001818fc: vcmpe.f32 s4,s0
  00181900: vmrs apsr,fpscr
  00181904: it mi
  00181906: str.w.mi r1,[r6,#0x88]
  0018190a: adds r1,#0x1
  0018190c: cmp r0,r1
  0018190e: bne 0x001818c0
  00181910: b 0x00181852
  00181912: ldr r0,[r6,#0x5c]
  00181914: mov r1,r5
  00181916: mov r2,r4
  00181918: ldr r0,[r0,#0x4]
  0018191a: ldr r0,[r0,#0x0]
  0018191c: blx 0x00074734
  00181920: b 0x00181852
```
