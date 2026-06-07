# Station::setItems
elf 0xa6d5e body 98
Sig: undefined __thiscall setItems(Station * this, Array * param_1, bool param_2)

## decompile
```c

/* Station::setItems(Array<Item*>*, bool) */

void __thiscall Station::setItems(Station *this,Array *param_1,bool param_2)

{
  void *pvVar1;
  Array<Item*> *this_00;
  int iVar2;
  __fn *extraout_r1;
  __fn *__fn;
  void *extraout_r2;
  void *extraout_r2_00;
  int extraout_r3;
  int extraout_r3_00;
  uint uVar3;
  void *unaff_r11;
  
  if (*(Array<Item*> **)(this + 0x28) != (Array<Item*> *)0x0) {
    pvVar1 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x28));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x28) = 0;
  if ((param_1 == (Array *)0x0) || (!param_2)) {
    *(Array **)(this + 0x28) = param_1;
  }
  else {
    this_00 = operator_new(0xc);
    Array<Item*>::Array(this_00);
    *(Array<Item*> **)(this + 0x28) = this_00;
    ArraySetLength<Item*>(*(uint *)param_1,(Array *)this_00);
    __fn = extraout_r1;
    pvVar1 = extraout_r2;
    iVar2 = extraout_r3;
    for (uVar3 = 0; uVar3 < *(uint *)param_1; uVar3 = uVar3 + 1) {
      iVar2 = Item::clone(*(Item **)(*(int *)(param_1 + 4) + uVar3 * 4),__fn,pvVar1,iVar2,unaff_r11)
      ;
      __fn = *(__fn **)(*(int *)(this + 0x28) + 4);
      *(int *)(__fn + uVar3 * 4) = iVar2;
      pvVar1 = extraout_r2_00;
      iVar2 = extraout_r3_00;
    }
  }
  return;
}

```

## target disasm
```
  000b6d5e: push {r4,r5,r6,r7,lr}
  000b6d60: add r7,sp,#0xc
  000b6d62: push.w r11
  000b6d66: mov r5,r0
  000b6d68: ldr r0,[r0,#0x28]
  000b6d6a: mov r6,r2
  000b6d6c: mov r4,r1
  000b6d6e: cbz r0,0x000b6d78
  000b6d70: blx 0x0007342c
  000b6d74: blx 0x0006eb48
  000b6d78: movs r0,#0x0
  000b6d7a: cmp r4,#0x0
  000b6d7c: str r0,[r5,#0x28]
  000b6d7e: beq 0x000b6db8
  000b6d80: cbz r6,0x000b6db8
  000b6d82: movs r0,#0xc
  000b6d84: blx 0x0006eb24
  000b6d88: mov r6,r0
  000b6d8a: blx 0x00071860
  000b6d8e: str r6,[r5,#0x28]
  000b6d90: mov r1,r6
  000b6d92: ldr r0,[r4,#0x0]
  000b6d94: blx 0x0007186c
  000b6d98: movs r6,#0x0
  000b6d9a: b 0x000b6db0
  000b6d9c: ldr r0,[r4,#0x4]
  000b6d9e: ldr.w r0,[r0,r6,lsl #0x2]
  000b6da2: blx 0x000734c8
  000b6da6: ldr r1,[r5,#0x28]
  000b6da8: ldr r1,[r1,#0x4]
  000b6daa: str.w r0,[r1,r6,lsl #0x2]
  000b6dae: adds r6,#0x1
  000b6db0: ldr r0,[r4,#0x0]
  000b6db2: cmp r6,r0
  000b6db4: bcc 0x000b6d9c
  000b6db6: b 0x000b6dba
  000b6db8: str r4,[r5,#0x28]
  000b6dba: pop.w r11
  000b6dbe: pop {r4,r5,r6,r7,pc}
```
