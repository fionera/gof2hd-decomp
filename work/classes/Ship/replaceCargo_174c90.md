# Ship::replaceCargo
elf 0x174c90 body 78
Sig: undefined __thiscall replaceCargo(Ship * this, Array * param_1)

## decompile
```c

/* Ship::replaceCargo(Array<Item*>*) */

void __thiscall Ship::replaceCargo(Ship *this,Array *param_1)

{
  int iVar1;
  uint uVar2;
  
  *(undefined4 *)(this + 0x10) = 0;
  *(Array **)(this + 0x70) = param_1;
  for (uVar2 = 0; uVar2 < *(uint *)param_1; uVar2 = uVar2 + 1) {
    iVar1 = Item::getAmount();
    param_1 = *(Array **)(this + 0x70);
    *(int *)(this + 0x10) = iVar1 + *(int *)(this + 0x10);
  }
  refreshValue(this);
  iVar1 = (*(int *)(this + 0xc) + *(int *)(this + 0x28)) - *(int *)(this + 0x10);
  if (*(int *)(**(int **)(DAT_00184ce0 + 0x184cc8) + 0xdc) < iVar1) {
    *(int *)(**(int **)(DAT_00184ce0 + 0x184cc8) + 0xdc) = iVar1;
  }
  return;
}

```
## target disasm
```
  00184c90: push {r4,r5,r7,lr}
  00184c92: add r7,sp,#0x8
  00184c94: movs r5,#0x0
  00184c96: mov r4,r0
  00184c98: str r5,[r0,#0x10]
  00184c9a: str r1,[r0,#0x70]
  00184c9c: b 0x00184cb2
  00184c9e: ldr r0,[r1,#0x4]
  00184ca0: ldr.w r0,[r0,r5,lsl #0x2]
  00184ca4: blx 0x0007183c
  00184ca8: ldr r2,[r4,#0x10]
  00184caa: adds r5,#0x1
  00184cac: ldr r1,[r4,#0x70]
  00184cae: add r0,r2
  00184cb0: str r0,[r4,#0x10]
  00184cb2: ldr r0,[r1,#0x0]
  00184cb4: cmp r5,r0
  00184cb6: bcc 0x00184c9e
  00184cb8: mov r0,r4
  00184cba: blx 0x00073768
  00184cbe: ldr r0,[0x00184ce0]
  00184cc0: ldrd r1,r2,[r4,#0xc]
  00184cc4: add r0,pc
  00184cc6: ldr r3,[r4,#0x28]
  00184cc8: ldr r0,[r0,#0x0]
  00184cca: add r1,r3
  00184ccc: subs r1,r1,r2
  00184cce: ldr r0,[r0,#0x0]
  00184cd0: ldr.w r3,[r0,#0xdc]
  00184cd4: cmp r3,r1
  00184cd6: it lt
  00184cd8: str.w.lt r1,[r0,#0xdc]
  00184cdc: pop {r4,r5,r7,pc}
```
