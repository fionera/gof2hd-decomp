# Station::setShips
elf 0xa6ce6 body 106
Sig: undefined __stdcall setShips(Array * param_1, bool param_2)

## decompile
```c

/* Station::setShips(Array<Ship*>*, bool) */

void Station::setShips(Array *param_1,bool param_2)

{
  void *pvVar1;
  Array<Ship*> *this;
  int iVar2;
  uint *puVar3;
  __fn *extraout_r1;
  __fn *__fn;
  int in_r2;
  void *extraout_r2;
  void *extraout_r2_00;
  int extraout_r3;
  int extraout_r3_00;
  uint uVar4;
  void *unaff_r11;
  
  puVar3 = (uint *)(uint)param_2;
  if (*(Array **)(param_1 + 0x2c) != (Array *)0x0) {
    ArrayReleaseClasses<Ship*>(*(Array **)(param_1 + 0x2c));
    if (*(Array<Ship*> **)(param_1 + 0x2c) != (Array<Ship*> *)0x0) {
      pvVar1 = (void *)Array<Ship*>::~Array(*(Array<Ship*> **)(param_1 + 0x2c));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(param_1 + 0x2c) = 0;
  if ((puVar3 == (uint *)0x0) || (in_r2 == 0)) {
    *(uint **)(param_1 + 0x2c) = puVar3;
  }
  else {
    this = operator_new(0xc);
    Array<Ship*>::Array(this);
    *(Array<Ship*> **)(param_1 + 0x2c) = this;
    ArraySetLength<Ship*>(*puVar3,(Array *)this);
    __fn = extraout_r1;
    pvVar1 = extraout_r2;
    iVar2 = extraout_r3;
    for (uVar4 = 0; uVar4 < *puVar3; uVar4 = uVar4 + 1) {
      iVar2 = Ship::clone(*(Ship **)(puVar3[1] + uVar4 * 4),__fn,pvVar1,iVar2,unaff_r11);
      __fn = *(__fn **)(*(int *)(param_1 + 0x2c) + 4);
      *(int *)(__fn + uVar4 * 4) = iVar2;
      pvVar1 = extraout_r2_00;
      iVar2 = extraout_r3_00;
    }
  }
  return;
}

```

## target disasm
```
  000b6ce6: push {r4,r5,r6,r7,lr}
  000b6ce8: add r7,sp,#0xc
  000b6cea: push.w r11
  000b6cee: mov r5,r0
  000b6cf0: ldr r0,[r0,#0x2c]
  000b6cf2: mov r6,r2
  000b6cf4: mov r4,r1
  000b6cf6: cbz r0,0x000b6d08
  000b6cf8: blx 0x00073408
  000b6cfc: ldr r0,[r5,#0x2c]
  000b6cfe: cbz r0,0x000b6d08
  000b6d00: blx 0x00073414
  000b6d04: blx 0x0006eb48
  000b6d08: movs r0,#0x0
  000b6d0a: cmp r4,#0x0
  000b6d0c: str r0,[r5,#0x2c]
  000b6d0e: beq 0x000b6d48
  000b6d10: cbz r6,0x000b6d48
  000b6d12: movs r0,#0xc
  000b6d14: blx 0x0006eb24
  000b6d18: mov r6,r0
  000b6d1a: blx 0x000719b0
  000b6d1e: str r6,[r5,#0x2c]
  000b6d20: mov r1,r6
  000b6d22: ldr r0,[r4,#0x0]
  000b6d24: blx 0x00071a28
  000b6d28: movs r6,#0x0
  000b6d2a: b 0x000b6d40
  000b6d2c: ldr r0,[r4,#0x4]
  000b6d2e: ldr.w r0,[r0,r6,lsl #0x2]
  000b6d32: blx 0x000734bc
  000b6d36: ldr r1,[r5,#0x2c]
  000b6d38: ldr r1,[r1,#0x4]
  000b6d3a: str.w r0,[r1,r6,lsl #0x2]
  000b6d3e: adds r6,#0x1
  000b6d40: ldr r0,[r4,#0x0]
  000b6d42: cmp r6,r0
  000b6d44: bcc 0x000b6d2c
  000b6d46: b 0x000b6d4a
  000b6d48: str r4,[r5,#0x2c]
  000b6d4a: pop.w r11
  000b6d4e: pop {r4,r5,r6,r7,pc}
```
