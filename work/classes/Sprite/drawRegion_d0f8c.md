# Sprite::drawRegion
elf 0xd0f8c body 136
Sig: undefined __thiscall drawRegion(Sprite * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* Sprite::drawRegion(int, int, int, int) */

void __thiscall Sprite::drawRegion(Sprite *this,int param_1,int param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  float in_s0;
  
  if (*(int *)this == 0) {
    uVar1 = *(uint *)(this + 4);
    iVar2 = (*(int *)(this + 0x10) + param_1) - *(int *)(this + 8);
    param_2 = *(int *)(this + 0x2c) + param_2;
    param_1 = *(int *)(this + 0x28) + param_1;
  }
  else {
    uVar1 = *(uint *)(*(int *)this + *(int *)(this + 0x38) * 4);
    iVar2 = (*(int *)(this + 0x10) + param_1) - *(int *)(this + 8);
  }
  AbyssEngine::PaintCanvas::DrawRegion2D
            ((PaintCanvas *)**(undefined4 **)(DAT_000e1014 + 0xe0fa2),uVar1,param_1,param_2,param_3,
             param_4,in_s0,0,0,0,iVar2);
  return;
}

```

## target disasm
```
  000e0f8c: push {r4,r5,r6,r7,lr}
  000e0f8e: add r7,sp,#0xc
  000e0f90: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10}
  000e0f94: mov r12,r2
  000e0f96: mov r2,r0
  000e0f98: ldr r0,[0x000e1014]
  000e0f9a: mov lr,r1
  000e0f9c: ldr r1,[r2,#0x0]
  000e0f9e: add r0,pc
  000e0fa0: ldr.w r8,[r7,#0x8]
  000e0fa4: cmp r1,#0x0
  000e0fa6: ldr r0,[r0,#0x0]
  000e0fa8: ldr r0,[r0,#0x0]
  000e0faa: beq 0x000e0fda
  000e0fac: ldr r5,[r2,#0x8]
  000e0fae: ldrd r9,r4,[r2,#0x10]
  000e0fb2: ldr r2,[r2,#0x38]
  000e0fb4: sub.w r6,r12,r5
  000e0fb8: ldr.w r1,[r1,r2,lsl #0x2]
  000e0fbc: adds r2,r6,r4
  000e0fbe: add.w r4,r9,lr
  000e0fc2: subs r4,r4,r5
  000e0fc4: movs r5,#0x0
  000e0fc6: strd r3,r8,[sp,#0x0]
  000e0fca: mov r3,r12
  000e0fcc: strd r5,r5,[sp,#0x8]
  000e0fd0: strd r5,r4,[sp,#0x10]
  000e0fd4: str r2,[sp,#0x18]
  000e0fd6: mov r2,lr
  000e0fd8: b 0x000e1008
  000e0fda: ldrd r1,r4,[r2,#0x4]
  000e0fde: ldrd r5,r6,[r2,#0x10]
  000e0fe2: ldrd r9,r10,[r2,#0x28]
  000e0fe6: sub.w r2,r12,r4
  000e0fea: add r5,lr
  000e0fec: add r2,r6
  000e0fee: subs r4,r5,r4
  000e0ff0: movs r5,#0x0
  000e0ff2: strd r3,r8,[sp,#0x0]
  000e0ff6: add.w r3,r10,r12
  000e0ffa: strd r5,r5,[sp,#0x8]
  000e0ffe: strd r5,r4,[sp,#0x10]
  000e1002: str r2,[sp,#0x18]
  000e1004: add.w r2,r9,lr
  000e1008: blx 0x00072d9c
  000e100c: add sp,#0x20
  000e100e: pop.w {r8,r9,r10}
  000e1012: pop {r4,r5,r6,r7,pc}
```
