# RecordHandler::readAllRecords
elf 0xcbcdc body 70
Sig: undefined __thiscall readAllRecords(RecordHandler * this)

## decompile
```c

/* RecordHandler::readAllRecords() */

Array<GameRecord*> * __thiscall RecordHandler::readAllRecords(RecordHandler *this)

{
  Array<GameRecord*> *this_00;
  undefined4 uVar1;
  uint *puVar2;
  int iVar3;
  
  this_00 = operator_new(0xc);
  Array<GameRecord*>::Array(this_00);
  puVar2 = *(uint **)(DAT_000dbd30 + 0xdbcfa);
  ArraySetLength<GameRecord*>(*puVar2,(Array *)this_00);
  for (iVar3 = 0; iVar3 < (int)*puVar2; iVar3 = iVar3 + 1) {
    uVar1 = recordStoreRead(this,iVar3);
    *(undefined4 *)(*(int *)(this_00 + 4) + iVar3 * 4) = uVar1;
  }
  return this_00;
}

```

## target disasm
```
  000dbcdc: push {r4,r5,r6,r7,lr}
  000dbcde: add r7,sp,#0xc
  000dbce0: push.w r8
  000dbce4: mov r8,r0
  000dbce6: movs r0,#0xc
  000dbce8: blx 0x0006eb24
  000dbcec: mov r5,r0
  000dbcee: blx 0x000747dc
  000dbcf2: ldr r0,[0x000dbd30]
  000dbcf4: mov r1,r5
  000dbcf6: add r0,pc
  000dbcf8: ldr r4,[r0,#0x0]
  000dbcfa: ldr r0,[r4,#0x0]
  000dbcfc: blx 0x000747e8
  000dbd00: movs r6,#0x0
  000dbd02: b 0x000dbd14
  000dbd04: mov r0,r8
  000dbd06: mov r1,r6
  000dbd08: blx 0x0007483c
  000dbd0c: ldr r1,[r5,#0x4]
  000dbd0e: str.w r0,[r1,r6,lsl #0x2]
  000dbd12: adds r6,#0x1
  000dbd14: ldr r0,[r4,#0x0]
  000dbd16: cmp r6,r0
  000dbd18: blt 0x000dbd04
  000dbd1a: mov r0,r5
  000dbd1c: pop.w r8
  000dbd20: pop {r4,r5,r6,r7,pc}
```
