# Layout::drawHeader
elf 0xd4100 body 246
Sig: undefined __stdcall drawHeader(String param_1, bool param_2)

## decompile
```c

/* Layout::drawHeader(AbyssEngine::String, bool) */

void Layout::drawHeader(Layout *param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  
  puVar3 = *(uint **)(DAT_000e41f4 + 0xe4118);
  AbyssEngine::PaintCanvas::SetColor(*puVar3);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar3);
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar3);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar3,*(int *)(param_1 + 0x330),*(int *)(param_1 + 0x2dc))
  ;
  drawBGPattern(param_1,*(uint *)(param_1 + 0x328),*(int *)(param_1 + 0x2dc) + iVar1,
                *(int *)(param_1 + 0x2e0),*(int *)(param_1 + 0x2e4) + iVar1 * -2,iVar2);
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)*puVar3,*(uint *)(param_1 + 0x330),
             *(int *)(param_1 + 0x2e4) + *(int *)(param_1 + 0x2dc),*(int *)(param_1 + 0x2e0),iVar1,
             iVar2,'\x11','\x12','\x01');
  if (*(int *)(param_2 + 8) != 0) {
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar3,*(int *)(param_1 + 0x32c),*(int *)(param_1 + 0x2dc));
    AbyssEngine::PaintCanvas::DrawString
              (*puVar3,(String *)**(undefined4 **)(DAT_000e41fc + 0xe41ac),param_2,
               *(int *)(**(int **)(DAT_000e41f8 + 0xe41aa) + 0x28) +
               *(int *)(**(int **)(DAT_000e41f8 + 0xe41aa) + 0x44) + *(int *)(param_1 + 0x2dc),
               (bool)((char)*(undefined4 *)(param_1 + 0x18) + (char)*(undefined4 *)(param_1 + 0x2e0)
                     ));
  }
  param_1[0x3cc] = SUB41(param_3,0);
  if ((param_3 != 0) && (*param_1 == (Layout)0x0)) {
    (*(code *)(DAT_001ac0b4 + 0x1ac0b8))(*(undefined4 *)(param_1 + 0x3bc));
    return;
  }
  return;
}

```

## target disasm
```
  000e4100: push {r4,r5,r6,r7,lr}
  000e4102: add r7,sp,#0xc
  000e4104: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000e4108: str r2,[sp,#0x18]
  000e410a: mov r4,r0
  000e410c: ldr r0,[0x000e41f4]
  000e410e: mov r9,r1
  000e4110: ldr.w r1,[r4,#0x3b0]
  000e4114: add r0,pc
  000e4116: ldr r5,[r0,#0x0]
  000e4118: ldr r0,[r5,#0x0]
  000e411a: blx 0x0006fac0
  000e411e: ldr.w r1,[r4,#0x330]
  000e4122: ldr r0,[r5,#0x0]
  000e4124: blx 0x00072d84
  000e4128: mov r6,r0
  000e412a: ldr r0,[r5,#0x0]
  000e412c: ldr.w r1,[r4,#0x330]
  000e4130: blx 0x00072d90
  000e4134: ldrd r2,r3,[r4,#0x2dc]
  000e4138: mov r10,r0
  000e413a: ldr.w r1,[r4,#0x330]
  000e413e: ldr r0,[r5,#0x0]
  000e4140: blx 0x00072dc0
  000e4144: ldrd r0,r3,[r4,#0x2dc]
  000e4148: ldr.w r2,[r4,#0x2e4]
  000e414c: ldr.w r1,[r4,#0x328]
  000e4150: sub.w r2,r2,r6, lsl #0x1
  000e4154: strd r2,r10,[sp,#0x0]
  000e4158: adds r2,r0,r6
  000e415a: mov r0,r4
  000e415c: blx 0x00074e00
  000e4160: ldrd r12,r3,[r4,#0x2dc]
  000e4164: mov.w lr,#0x1
  000e4168: ldr.w r2,[r4,#0x2e4]
  000e416c: mov.w r11,#0x12
  000e4170: ldr.w r1,[r4,#0x330]
  000e4174: mov.w r8,#0x11
  000e4178: ldr r0,[r5,#0x0]
  000e417a: add r2,r12
  000e417c: strd r6,r10,[sp,#0x0]
  000e4180: add r6,sp,#0x8
  000e4182: stm r6,{r8,r11,lr}
  000e4186: blx 0x00073564
  000e418a: ldr.w r0,[r9,#0x8]
  000e418e: cbz r0,0x000e41ce
  000e4190: ldrd r2,r3,[r4,#0x2dc]
  000e4194: ldr.w r1,[r4,#0x32c]
  000e4198: ldr r0,[r5,#0x0]
  000e419a: blx 0x00072dc0
  000e419e: ldr r0,[0x000e41f8]
  000e41a0: mov.w lr,#0x0
  000e41a4: ldr r1,[0x000e41fc]
  000e41a6: add r0,pc
  000e41a8: add r1,pc
  000e41aa: ldr r0,[r0,#0x0]
  000e41ac: ldr r1,[r1,#0x0]
  000e41ae: ldr r2,[r0,#0x0]
  000e41b0: ldr r1,[r1,#0x0]
  000e41b2: ldr r0,[r5,#0x0]
  000e41b4: ldrd r12,r6,[r4,#0x2dc]
  000e41b8: ldr r3,[r2,#0x28]
  000e41ba: ldr r2,[r2,#0x44]
  000e41bc: ldr r5,[r4,#0x18]
  000e41be: add r2,r12
  000e41c0: add r3,r2
  000e41c2: mov r2,r9
  000e41c4: add r5,r6
  000e41c6: strd r5,lr,[sp,#0x0]
  000e41ca: blx 0x0006fe14
  000e41ce: ldr r0,[sp,#0x18]
  000e41d0: strb.w r0,[r4,#0x3cc]
  000e41d4: cbz r0,0x000e41da
  000e41d6: ldrb r0,[r4,#0x0]
  000e41d8: cbz r0,0x000e41e2
  000e41da: add sp,#0x1c
  000e41dc: pop.w {r8,r9,r10,r11}
  000e41e0: pop {r4,r5,r6,r7,pc}
  000e41e2: ldr.w r0,[r4,#0x3bc]
  000e41e6: add sp,#0x1c
  000e41e8: pop.w {r8,r9,r10,r11}
  000e41ec: pop.w {r4,r5,r6,r7,lr}
  000e41f0: b.w 0x001ac0a8
  001ac0a8: bx pc
```
