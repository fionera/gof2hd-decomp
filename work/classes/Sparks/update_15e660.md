# Sparks::update
elf 0x15e660 body 110
Sig: undefined __thiscall update(Sparks * this, int param_1)

## decompile
```c

/* Sparks::update(int) */

void __thiscall Sparks::update(Sparks *this,int param_1)

{
  int iVar1;
  uint uVar2;
  uint *puVar3;
  
  if (this[0x10] != (Sparks)0x0) {
    *(int *)(this + 0xc) = *(int *)(this + 0xc) + param_1;
    puVar3 = *(uint **)(DAT_0016e6d0 + 0x16e67c);
    for (uVar2 = 0; uVar2 < *(uint *)(this + 0x18); uVar2 = uVar2 + 1) {
      if (*(int *)(*(int *)this + uVar2 * 4) < *(int *)(this + 0xc)) {
        AbyssEngine::PaintCanvas::SpriteSystemSetSize
                  (*puVar3,(ushort)*(undefined4 *)(this + 4),(short)uVar2);
      }
    }
    iVar1 = *(int *)(this + 0xc);
    if (*(int *)(this + 0x14) == 1) {
      if ((iVar1 < 0x1f5) &&
         (iVar1 * 2 - *(int *)(this + 0x1c) == 0 || iVar1 * 2 < *(int *)(this + 0x1c))) {
        return;
      }
    }
    else if (iVar1 < 0x1f5) {
      return;
    }
    this[0x10] = (Sparks)0x0;
    *(undefined4 *)(this + 0xc) = 0;
  }
  return;
}

```

## target disasm
```
  0016e660: push {r4,r5,r6,r7,lr}
  0016e662: add r7,sp,#0xc
  0016e664: push.w r11
  0016e668: mov r4,r0
  0016e66a: ldrb r0,[r0,#0x10]
  0016e66c: cbz r0,0x0016e6c8
  0016e66e: ldr r0,[r4,#0xc]
  0016e670: movs r5,#0x0
  0016e672: add r0,r1
  0016e674: str r0,[r4,#0xc]
  0016e676: ldr r0,[0x0016e6d0]
  0016e678: add r0,pc
  0016e67a: ldr r6,[r0,#0x0]
  0016e67c: b 0x0016e69e
  0016e67e: ldr r1,[r4,#0x0]
  0016e680: ldr r0,[r4,#0xc]
  0016e682: ldr.w r1,[r1,r5,lsl #0x2]
  0016e686: cmp r0,r1
  0016e688: ble 0x0016e69c
  0016e68a: ldr r2,[r4,#0x1c]
  0016e68c: ldr r1,[r4,#0x4]
  0016e68e: sub.w r0,r2,r0, lsl #0x1
  0016e692: uxth r2,r5
  0016e694: sxth r3,r0
  0016e696: ldr r0,[r6,#0x0]
  0016e698: blx 0x000778cc
  0016e69c: adds r5,#0x1
  0016e69e: ldr r0,[r4,#0x18]
  0016e6a0: cmp r5,r0
  0016e6a2: bcc 0x0016e67e
  0016e6a4: ldr r1,[r4,#0x14]
  0016e6a6: ldr r0,[r4,#0xc]
  0016e6a8: cmp r1,#0x1
  0016e6aa: bne 0x0016e6bc
  0016e6ac: cmp.w r0,#0x1f4
  0016e6b0: bgt 0x0016e6c2
  0016e6b2: ldr r1,[r4,#0x1c]
  0016e6b4: lsls r0,r0,#0x1
  0016e6b6: cmp r0,r1
  0016e6b8: ble 0x0016e6c8
  0016e6ba: b 0x0016e6c2
  0016e6bc: cmp.w r0,#0x1f4
  0016e6c0: ble 0x0016e6c8
  0016e6c2: movs r0,#0x0
  0016e6c4: strb r0,[r4,#0x10]
  0016e6c6: str r0,[r4,#0xc]
  0016e6c8: pop.w r11
  0016e6cc: pop {r4,r5,r6,r7,pc}
```
