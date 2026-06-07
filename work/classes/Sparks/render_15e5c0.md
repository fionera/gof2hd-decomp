# Sparks::render
elf 0x15e5c0 body 132
Sig: undefined __thiscall render(Sparks * this)

## decompile
```c

/* Sparks::render() */

void __thiscall Sparks::render(Sparks *this)

{
  Matrix *pMVar1;
  undefined4 *puVar2;
  int *piVar3;
  uint *puVar4;
  undefined4 local_58 [5];
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined4 local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0016e658 + 0x16e5d2);
  local_1c = *piVar3;
  if (this[0x10] != (Sparks)0x0) {
    puVar4 = *(uint **)(DAT_0016e65c + 0x16e5e6);
    AbyssEngine::PaintCanvas::SetTexture(*puVar4,*(uint *)(this + 8));
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar4,2);
    uStack_28 = DAT_0016e650;
    uStack_30 = DAT_0016e648;
    uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_38 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar2 = (undefined4 *)((uint)local_58 | 4);
    pMVar1 = (Matrix *)*puVar4;
    *puVar2 = 0;
    puVar2[1] = uStack_3c;
    puVar2[2] = uStack_38;
    puVar2[3] = uStack_34;
    local_40 = 0;
    local_58[0] = 0x3f800000;
    local_44 = 0x3f800000;
    local_20 = 0x3f800000;
    AbyssEngine::PaintCanvas::SetWorldViewMatrix(pMVar1);
    AbyssEngine::PaintCanvas::DrawSpriteSystem(*puVar4);
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016e5c0: push {r4,r5,r6,r7,lr}
  0016e5c2: add r7,sp,#0xc
  0016e5c4: push.w r11
  0016e5c8: sub sp,#0x40
  0016e5ca: mov r4,r0
  0016e5cc: ldr r0,[0x0016e658]
  0016e5ce: add r0,pc
  0016e5d0: ldr r5,[r0,#0x0]
  0016e5d2: ldr r0,[r5,#0x0]
  0016e5d4: str r0,[sp,#0x3c]
  0016e5d6: ldrb r0,[r4,#0x10]
  0016e5d8: cbz r0,0x0016e630
  0016e5da: ldr r0,[0x0016e65c]
  0016e5dc: mov.w r2,#0xffffffff
  0016e5e0: ldr r1,[r4,#0x8]
  0016e5e2: add r0,pc
  0016e5e4: ldr r6,[r0,#0x0]
  0016e5e6: ldr r0,[r6,#0x0]
  0016e5e8: blx 0x0006fa6c
  0016e5ec: ldr r0,[r6,#0x0]
  0016e5ee: movs r1,#0x2
  0016e5f0: blx 0x0006febc
  0016e5f4: adr r1,[0x16e648]
  0016e5f6: vmov.i32 q8,#0x0
  0016e5fa: vld1.64 {d18,d19},[r1]
  0016e5fe: mov r1,sp
  0016e600: orr r2,r1,#0x4
  0016e604: ldr r0,[r6,#0x0]
  0016e606: vst1.32 {d16,d17},[r2]
  0016e60a: add.w r2,r1,#0x18
  0016e60e: vst1.64 {d16,d17},[r2]
  0016e612: add.w r2,r1,#0x28
  0016e616: vst1.64 {d18,d19},[r2]
  0016e61a: mov.w r2,#0x3f800000
  0016e61e: str r2,[sp,#0x0]
  0016e620: str r2,[sp,#0x14]
  0016e622: str r2,[sp,#0x38]
  0016e624: blx 0x0006f55c
  0016e628: ldr r1,[r4,#0x4]
  0016e62a: ldr r0,[r6,#0x0]
  0016e62c: blx 0x000778f0
  0016e630: ldr r0,[sp,#0x3c]
  0016e632: ldr r1,[r5,#0x0]
  0016e634: subs r0,r1,r0
  0016e636: ittt eq
  0016e638: add.eq sp,#0x40
  0016e63a: pop.eq.w r11
  0016e63e: pop.eq {r4,r5,r6,r7,pc}
  0016e640: blx 0x0006e824
```
