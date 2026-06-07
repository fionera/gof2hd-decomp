# MTitle::OnRender2D
elf 0x97a8c body 296
Sig: undefined __thiscall OnRender2D(MTitle * this)

## decompile
```c

/* MTitle::OnRender2D() */

void __thiscall MTitle::OnRender2D(MTitle *this)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  uint *puVar4;
  uint in_fpscr;
  
  AbyssEngine::PaintCanvas::Begin2d();
  puVar4 = *(uint **)(DAT_000a7bb8 + 0xa7aa8);
  AbyssEngine::PaintCanvas::SetColor(*puVar4);
  puVar3 = *(undefined4 **)(DAT_000a7bbc + 0xa7ab4);
  Layout::drawBG();
  Layout::drawHeader();
  Layout::drawEmptyFooter(SUB41(*puVar3,0));
  iVar1 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
  if (iVar1 < 0x33) {
    iVar1 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
  }
  else {
    iVar1 = 0x32;
  }
  iVar1 = iVar1 + *(int *)(this + 0x1c);
  *(int *)(this + 0x1c) = iVar1;
  if (iVar1 < 0xfa1) {
    iVar2 = 0x10;
    if (*(int *)(this + 0x18) == 0) {
      iVar2 = 0x14;
    }
    iVar2 = *(int *)(this + iVar2);
    if (999 < iVar1) {
      if (3000 < iVar1) {
        VectorSignedToFloat(iVar1 + -3000,(byte)(in_fpscr >> 0x16) & 3);
      }
      goto LAB_000a7b68;
    }
  }
  else {
    iVar1 = *(int *)(this + 0x18) + 1;
    *(int *)(this + 0x18) = iVar1;
    *(undefined4 *)(this + 0x1c) = 0;
    if (iVar1 == 2) {
      (*(code *)(DAT_001ab904 + 0x1ab908))(**(undefined4 **)(DAT_000a7bc0 + 0xa7afe),1);
      return;
    }
    iVar2 = 0x10;
    if (iVar1 == 0) {
      iVar2 = 0x14;
    }
    iVar1 = 0;
    iVar2 = *(int *)(this + iVar2);
  }
  VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
LAB_000a7b68:
  AbyssEngine::PaintCanvas::SetColor(*(uint *)(this + 4));
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar4,iVar2,0,'\0','D');
  AbyssEngine::PaintCanvas::End2d();
  (*(code *)(DAT_001ab914 + 0x1ab918))(*(undefined4 *)(this + 4));
  return;
}

```

## target disasm
```
  000a7a8c: push {r4,r5,r6,r7,lr}
  000a7a8e: add r7,sp,#0xc
  000a7a90: push.w r11
  000a7a94: sub sp,#0x8
  000a7a96: mov r4,r0
  000a7a98: ldr r0,[r0,#0x4]
  000a7a9a: blx 0x00071d34
  000a7a9e: ldr r0,[0x000a7bb8]
  000a7aa0: mov.w r1,#0xffffffff
  000a7aa4: add r0,pc
  000a7aa6: ldr r6,[r0,#0x0]
  000a7aa8: ldr r0,[r6,#0x0]
  000a7aaa: blx 0x0006fac0
  000a7aae: ldr r0,[0x000a7bbc]
  000a7ab0: add r0,pc
  000a7ab2: ldr r5,[r0,#0x0]
  000a7ab4: ldr r0,[r5,#0x0]
  000a7ab6: blx 0x00071d40
  000a7aba: ldr r0,[r5,#0x0]
  000a7abc: blx 0x00071d4c
  000a7ac0: ldr r0,[r5,#0x0]
  000a7ac2: movs r1,#0x0
  000a7ac4: blx 0x00071d58
  000a7ac8: ldr r0,[r4,#0x8]
  000a7aca: blx 0x00070e4c
  000a7ace: cmp r0,#0x32
  000a7ad0: ble 0x000a7ad6
  000a7ad2: movs r0,#0x32
  000a7ad4: b 0x000a7adc
  000a7ad6: ldr r0,[r4,#0x8]
  000a7ad8: blx 0x00070e4c
  000a7adc: ldr r1,[r4,#0x1c]
  000a7ade: add r0,r1
  000a7ae0: str r0,[r4,#0x1c]
  000a7ae2: cmp.w r0,#0xfa0
  000a7ae6: ble 0x000a7b0e
  000a7ae8: ldr r0,[r4,#0x18]
  000a7aea: movs r1,#0x0
  000a7aec: adds r0,#0x1
  000a7aee: strd r0,r1,[r4,#0x18]
  000a7af2: cmp r0,#0x2
  000a7af4: bne 0x000a7b46
  000a7af6: ldr r0,[0x000a7bc0]
  000a7af8: movs r1,#0x1
  000a7afa: add r0,pc
  000a7afc: ldr r0,[r0,#0x0]
  000a7afe: ldr r0,[r0,#0x0]
  000a7b00: add sp,#0x8
  000a7b02: pop.w r11
  000a7b06: pop.w {r4,r5,r6,r7,lr}
  000a7b0a: b.w 0x001ab8f8
  000a7b0e: ldr r1,[r4,#0x18]
  000a7b10: movs r2,#0x10
  000a7b12: cmp r1,#0x0
  000a7b14: it eq
  000a7b16: mov.eq r2,#0x14
  000a7b18: ldr r5,[r4,r2]
  000a7b1a: cmp.w r0,#0x3e8
  000a7b1e: blt 0x000a7b52
  000a7b20: movw r1,#0xbb9
  000a7b24: cmp r0,r1
  000a7b26: blt 0x000a7b64
  000a7b28: subw r0,r0,#0xbb8
  000a7b2c: vldr.32 s2,[pc,#0x7c]
  000a7b30: vmov s0,r0
  000a7b34: vcvt.f32.s32 s0,s0
  000a7b38: vdiv.f32 s0,s0,s2
  000a7b3c: vmov.f32 s2,0x3f800000
  000a7b40: vadd.f32 s0,s0,s2
  000a7b44: b 0x000a7b68
  000a7b46: movs r1,#0x10
  000a7b48: cmp r0,#0x0
  000a7b4a: it eq
  000a7b4c: mov.eq r1,#0x14
  000a7b4e: movs r0,#0x0
  000a7b50: ldr r5,[r4,r1]
  000a7b52: vmov s0,r0
  000a7b56: vldr.32 s2,[pc,#0x58]
  000a7b5a: vcvt.f32.s32 s0,s0
  000a7b5e: vdiv.f32 s0,s0,s2
  000a7b62: b 0x000a7b68
  000a7b64: vmov.f32 s0,0x3f800000
  000a7b68: vldr.32 s2,[pc,#0x48]
  000a7b6c: vmul.f32 s0,s0,s2
  000a7b70: vcvt.s32.f32 s0,s0
  000a7b74: ldr r0,[r4,#0x4]
  000a7b76: vmov r1,s0
  000a7b7a: sub.w r1,r1,#0x100
  000a7b7e: blx 0x0006fac0
  000a7b82: ldr r0,[r6,#0x0]
  000a7b84: movs r1,#0x44
  000a7b86: strd r1,r1,[sp,#0x0]
  000a7b8a: mov r1,r5
  000a7b8c: movs r2,#0x0
  000a7b8e: movs r3,#0x0
  000a7b90: blx 0x00071d70
  000a7b94: ldr r0,[r4,#0x4]
  000a7b96: blx 0x00071d7c
  000a7b9a: ldr r0,[r4,#0x4]
  000a7b9c: add sp,#0x8
  000a7b9e: pop.w r11
  000a7ba2: pop.w {r4,r5,r6,r7,lr}
  000a7ba6: b.w 0x001ab908
  001ab908: bx pc
  001ab90c: ldr r12,[0x1ab914]
  001ab910: add pc,r12,pc
```
