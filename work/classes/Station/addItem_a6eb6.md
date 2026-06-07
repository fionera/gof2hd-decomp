# Station::addItem
elf 0xa6eb6 body 118
Sig: undefined __stdcall addItem(Item * param_1)

## decompile
```c

/* Station::addItem(Item*) */

void Station::addItem(Item *param_1)

{
  uint uVar1;
  int iVar2;
  Array<Item*> *this;
  undefined4 uVar3;
  Item *in_r1;
  uint uVar4;
  uint *puVar5;
  undefined4 uVar6;
  
  puVar5 = *(uint **)(param_1 + 0x28);
  if (puVar5 == (uint *)0x0) {
    this = operator_new(0xc);
    Array<Item*>::Array(this);
    *(Array<Item*> **)(param_1 + 0x28) = this;
  }
  else {
    uVar1 = *puVar5;
    if (uVar1 != 0) {
      for (uVar4 = 0; uVar4 < uVar1; uVar4 = uVar4 + 1) {
        iVar2 = Item::equals(*(Item **)(puVar5[1] + uVar4 * 4),in_r1);
        if (iVar2 != 0) {
          if (-1 < (int)uVar4) {
            uVar6 = *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x28) + 4) + uVar4 * 4);
            uVar3 = Item::getAmount();
            (*(code *)(DAT_001abed4 + 0x1abed8))(uVar6,uVar3);
            return;
          }
          break;
        }
        puVar5 = *(uint **)(param_1 + 0x28);
        uVar1 = *puVar5;
      }
    }
  }
  (*(code *)(DAT_001abec4 + 0x1abec8))();
  return;
}

```

## target disasm
```
  000b6eb6: push {r4,r5,r6,r7,lr}
  000b6eb8: add r7,sp,#0xc
  000b6eba: push.w r8
  000b6ebe: ldr r5,[r0,#0x28]
  000b6ec0: mov r8,r1
  000b6ec2: mov r6,r0
  000b6ec4: cbz r5,0x000b6ee8
  000b6ec6: ldr r0,[r5,#0x0]
  000b6ec8: cbz r0,0x000b6ef6
  000b6eca: movs r4,#0x0
  000b6ecc: b 0x000b6ee2
  000b6ece: ldr r0,[r5,#0x4]
  000b6ed0: mov r1,r8
  000b6ed2: ldr.w r0,[r0,r4,lsl #0x2]
  000b6ed6: blx 0x000734e0
  000b6eda: cbnz r0,0x000b6f06
  000b6edc: ldr r5,[r6,#0x28]
  000b6ede: adds r4,#0x1
  000b6ee0: ldr r0,[r5,#0x0]
  000b6ee2: cmp r4,r0
  000b6ee4: bcc 0x000b6ece
  000b6ee6: b 0x000b6ef6
  000b6ee8: movs r0,#0xc
  000b6eea: blx 0x0006eb24
  000b6eee: mov r5,r0
  000b6ef0: blx 0x00071860
  000b6ef4: str r5,[r6,#0x28]
  000b6ef6: mov r0,r8
  000b6ef8: mov r1,r5
  000b6efa: pop.w r8
  000b6efe: pop.w {r4,r5,r6,r7,lr}
  000b6f02: b.w 0x001abeb8
  000b6f06: ldr r5,[r6,#0x28]
  000b6f08: cmp r4,#0x0
  000b6f0a: blt 0x000b6ef6
  000b6f0c: ldr r0,[r5,#0x4]
  000b6f0e: ldr.w r5,[r0,r4,lsl #0x2]
  000b6f12: mov r0,r8
  000b6f14: blx 0x0007183c
  000b6f18: mov r1,r0
  000b6f1a: mov r0,r5
  000b6f1c: pop.w r8
  000b6f20: pop.w {r4,r5,r6,r7,lr}
  000b6f24: b.w 0x001abec8
  001abeb8: bx pc
  001abec8: bx pc
```
