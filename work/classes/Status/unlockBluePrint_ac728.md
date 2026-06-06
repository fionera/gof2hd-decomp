# Status::unlockBluePrint
elf 0xac728 body 58
Sig: undefined __thiscall unlockBluePrint(Status * this, int param_1)

## decompile
```c

/* Status::unlockBluePrint(int) */

void __thiscall Status::unlockBluePrint(Status *this,int param_1)

{
  int iVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < **(uint **)(this + 0x18); uVar2 = uVar2 + 1) {
    iVar1 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar2 * 4));
    if (iVar1 == param_1) {
      BluePrint::unlock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar2 * 4));
    }
  }
  return;
}

```
## target disasm
```
  000bc728: push {r4,r5,r6,r7,lr}
  000bc72a: add r7,sp,#0xc
  000bc72c: push.w r11
  000bc730: mov r4,r1
  000bc732: mov r5,r0
  000bc734: movs r6,#0x0
  000bc736: b 0x000bc754
  000bc738: ldr r0,[r0,#0x4]
  000bc73a: ldr.w r0,[r0,r6,lsl #0x2]
  000bc73e: blx 0x000733fc
  000bc742: cmp r0,r4
  000bc744: bne 0x000bc752
  000bc746: ldr r0,[r5,#0x18]
  000bc748: ldr r0,[r0,#0x4]
  000bc74a: ldr.w r0,[r0,r6,lsl #0x2]
  000bc74e: blx 0x000738a0
  000bc752: adds r6,#0x1
  000bc754: ldr r0,[r5,#0x18]
  000bc756: ldr r1,[r0,#0x0]
  000bc758: cmp r6,r1
  000bc75a: bcc 0x000bc738
  000bc75c: pop.w r11
  000bc760: pop {r4,r5,r6,r7,pc}
```
