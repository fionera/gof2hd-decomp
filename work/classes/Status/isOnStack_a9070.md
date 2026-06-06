# Status::isOnStack
elf 0xa9070 body 62
Sig: undefined __thiscall isOnStack(Status * this, Station * param_1)

## decompile
```c

/* Status::isOnStack(Station*) */

undefined4 __thiscall Status::isOnStack(Status *this,Station *param_1)

{
  Station *this_00;
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  while( true ) {
    if (2 < uVar2) {
      return 0;
    }
    this_00 = *(Station **)(*(int *)(*(int *)(this + 0x1a0) + 4) + uVar2 * 4);
    if ((this_00 != (Station *)0x0) && (iVar1 = Station::equals(this_00,param_1), iVar1 != 0))
    break;
    uVar2 = uVar2 + 1;
  }
  return *(undefined4 *)(*(int *)(*(int *)(this + 0x1a0) + 4) + uVar2 * 4);
}

```
## target disasm
```
  000b9070: push {r4,r5,r6,r7,lr}
  000b9072: add r7,sp,#0xc
  000b9074: push.w r11
  000b9078: mov r5,r1
  000b907a: mov r4,r0
  000b907c: movs r6,#0x0
  000b907e: b 0x000b9096
  000b9080: ldr.w r0,[r4,#0x1a0]
  000b9084: ldr r0,[r0,#0x4]
  000b9086: ldr.w r0,[r0,r6,lsl #0x2]
  000b908a: cbz r0,0x000b9094
  000b908c: mov r1,r5
  000b908e: blx 0x00073678
  000b9092: cbnz r0,0x000b909e
  000b9094: adds r6,#0x1
  000b9096: cmp r6,#0x2
  000b9098: bls 0x000b9080
  000b909a: movs r0,#0x0
  000b909c: b 0x000b90a8
  000b909e: ldr.w r0,[r4,#0x1a0]
  000b90a2: ldr r0,[r0,#0x4]
  000b90a4: ldr.w r0,[r0,r6,lsl #0x2]
  000b90a8: pop.w r11
  000b90ac: pop {r4,r5,r6,r7,pc}
```
