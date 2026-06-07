# PlayerFighter::setExhaustVisible
elf 0xdcd24 body 48
Sig: undefined __stdcall setExhaustVisible(bool param_1)

## decompile
```c

/* PlayerFighter::setExhaustVisible(bool) */

void PlayerFighter::setExhaustVisible(bool param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 8);
  if (iVar2 != 0) {
    iVar1 = *(int *)(param_1 + 0xc);
    if (iVar1 == 0) {
      iVar2 = *(int *)(iVar2 + 0x14);
    }
    else {
      iVar2 = *(int *)(iVar1 + 0x14);
    }
    if (iVar2 != -1) {
      AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000ecd54 + 0xecd42));
      (*(code *)(DAT_001ac1d4 + 0x1ac1d8))();
      return;
    }
  }
  return;
}

```

## target disasm
```
  000ecd24: push {r4,r6,r7,lr}
  000ecd26: add r7,sp,#0x8
  000ecd28: mov r4,r1
  000ecd2a: ldr r1,[r0,#0x8]
  000ecd2c: cbz r1,0x000ecd52
  000ecd2e: ldr r0,[r0,#0xc]
  000ecd30: cmp r0,#0x0
  000ecd32: ite ne
  000ecd34: ldr.ne r1,[r0,#0x14]
  000ecd36: ldr.eq r1,[r1,#0x14]
  000ecd38: adds r0,r1,#0x1
  000ecd3a: beq 0x000ecd52
  000ecd3c: ldr r0,[0x000ecd54]
  000ecd3e: add r0,pc
  000ecd40: ldr r0,[r0,#0x0]
  000ecd42: ldr r0,[r0,#0x0]
  000ecd44: blx 0x00072088
  000ecd48: mov r1,r4
  000ecd4a: pop.w {r4,r6,r7,lr}
  000ecd4e: b.w 0x001ac1c8
  000ecd52: pop {r4,r6,r7,pc}
```
