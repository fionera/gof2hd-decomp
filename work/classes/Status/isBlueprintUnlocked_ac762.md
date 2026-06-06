# Status::isBlueprintUnlocked
elf 0xac762 body 70
Sig: undefined __thiscall isBlueprintUnlocked(Status * this, int param_1)

## decompile
```c

/* Status::isBlueprintUnlocked(int) */

undefined4 __thiscall Status::isBlueprintUnlocked(Status *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  uVar3 = 0;
  while( true ) {
    if (**(uint **)(this + 0x18) <= uVar3) {
      return 0;
    }
    iVar1 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar3 * 4));
    if (iVar1 == param_1) break;
    uVar3 = uVar3 + 1;
  }
  uVar2 = (*(code *)(DAT_001abf94 + 0x1abf98))
                    (*(undefined4 *)(*(int *)(*(int *)(this + 0x18) + 4) + uVar3 * 4));
  return uVar2;
}

```
## target disasm
```
  000bc762: push {r4,r5,r6,r7,lr}
  000bc764: add r7,sp,#0xc
  000bc766: push.w r11
  000bc76a: mov r5,r1
  000bc76c: mov r4,r0
  000bc76e: movs r6,#0x0
  000bc770: b 0x000bc782
  000bc772: ldr r0,[r0,#0x4]
  000bc774: ldr.w r0,[r0,r6,lsl #0x2]
  000bc778: blx 0x000733fc
  000bc77c: cmp r0,r5
  000bc77e: beq 0x000bc792
  000bc780: adds r6,#0x1
  000bc782: ldr r0,[r4,#0x18]
  000bc784: ldr r1,[r0,#0x0]
  000bc786: cmp r6,r1
  000bc788: bcc 0x000bc772
  000bc78a: movs r0,#0x0
  000bc78c: pop.w r11
  000bc790: pop {r4,r5,r6,r7,pc}
  000bc792: ldr r0,[r4,#0x18]
  000bc794: ldr r0,[r0,#0x4]
  000bc796: ldr.w r0,[r0,r6,lsl #0x2]
  000bc79a: pop.w r11
  000bc79e: pop.w {r4,r5,r6,r7,lr}
  000bc7a2: b.w 0x001abf88
  001abf88: bx pc
```
