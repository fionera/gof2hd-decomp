# Ship::replaceEquipment
elf 0x175000 body 52
Sig: undefined __thiscall replaceEquipment(Ship * this, Array * param_1)

## decompile
```c

/* Ship::replaceEquipment(Array<Item*>*) */

void __thiscall Ship::replaceEquipment(Ship *this,Array *param_1)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  
  if (param_1 != (Array *)0x0) {
    piVar1 = *(int **)(this + 0x68);
    uVar2 = *piVar1 + piVar1[3] + piVar1[1] + piVar1[2];
    if (uVar2 < *(uint *)param_1) {
      iVar3 = *(uint *)param_1 - uVar2;
      *(int *)(this + 0x7c) = iVar3;
      piVar1[3] = iVar3 + piVar1[3];
    }
  }
  *(Array **)(this + 0x6c) = param_1;
  (*(code *)(DAT_001abf14 + 0x1abf18))();
  return;
}

```
## target disasm
```
  00185000: push {r4,r5,r7,lr}
  00185002: add r7,sp,#0x8
  00185004: cbz r1,0x0018502a
  00185006: ldr r2,[r0,#0x68]
  00185008: ldrd r3,lr,[r2,#0x0]
  0018500c: ldrd r4,r5,[r2,#0x8]
  00185010: add r3,r5
  00185012: ldr.w r12,[r1,#0x0]
  00185016: add r3,lr
  00185018: add r3,r4
  0018501a: cmp r12,r3
  0018501c: bls 0x0018502a
  0018501e: sub.w r3,r12,r3
  00185022: str r3,[r0,#0x7c]
  00185024: ldr r5,[r2,#0xc]
  00185026: add r3,r5
  00185028: str r3,[r2,#0xc]
  0018502a: str r1,[r0,#0x6c]
  0018502c: pop.w {r4,r5,r7,lr}
  00185030: b.w 0x001abf08
```
