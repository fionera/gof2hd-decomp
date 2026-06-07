# RecordHandler::readAllPreviewRecords
elf 0xcbb48 body 70
Sig: undefined __thiscall readAllPreviewRecords(RecordHandler * this)

## decompile
```c

/* RecordHandler::readAllPreviewRecords() */

Array<GameRecord*> * __thiscall RecordHandler::readAllPreviewRecords(RecordHandler *this)

{
  Array<GameRecord*> *this_00;
  undefined4 uVar1;
  uint *puVar2;
  int iVar3;
  
  this_00 = operator_new(0xc);
  Array<GameRecord*>::Array(this_00);
  puVar2 = *(uint **)(DAT_000dbb9c + 0xdbb66);
  ArraySetLength<GameRecord*>(*puVar2,(Array *)this_00);
  for (iVar3 = 0; iVar3 < (int)*puVar2; iVar3 = iVar3 + 1) {
    uVar1 = recordStoreReadPreview((int)this);
    *(undefined4 *)(*(int *)(this_00 + 4) + iVar3 * 4) = uVar1;
  }
  return this_00;
}

```

## target disasm
```
  000dbb48: push {r4,r5,r6,r7,lr}
  000dbb4a: add r7,sp,#0xc
  000dbb4c: push.w r8
  000dbb50: mov r8,r0
  000dbb52: movs r0,#0xc
  000dbb54: blx 0x0006eb24
  000dbb58: mov r5,r0
  000dbb5a: blx 0x000747dc
  000dbb5e: ldr r0,[0x000dbb9c]
  000dbb60: mov r1,r5
  000dbb62: add r0,pc
  000dbb64: ldr r4,[r0,#0x0]
  000dbb66: ldr r0,[r4,#0x0]
  000dbb68: blx 0x000747e8
  000dbb6c: movs r6,#0x0
  000dbb6e: b 0x000dbb80
  000dbb70: mov r0,r8
  000dbb72: mov r1,r6
  000dbb74: blx 0x000747f4
  000dbb78: ldr r1,[r5,#0x4]
  000dbb7a: str.w r0,[r1,r6,lsl #0x2]
  000dbb7e: adds r6,#0x1
  000dbb80: ldr r0,[r4,#0x0]
  000dbb82: cmp r6,r0
  000dbb84: blt 0x000dbb70
  000dbb86: mov r0,r5
  000dbb88: pop.w r8
  000dbb8c: pop {r4,r5,r6,r7,pc}
```
