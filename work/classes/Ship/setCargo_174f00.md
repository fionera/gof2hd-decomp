# Ship::setCargo
elf 0x174f00 body 84
Sig: undefined __thiscall setCargo(Ship * this, Array * param_1)

## decompile
```c

/* Ship::setCargo(Array<Item*>*) */

void __thiscall Ship::setCargo(Ship *this,Array *param_1)

{
  int iVar1;
  uint uVar2;
  
  *(undefined4 *)(this + 0x10) = 0;
  *(Array **)(this + 0x70) = param_1;
  if (param_1 != (Array *)0x0) {
    for (uVar2 = 0; uVar2 < *(uint *)param_1; uVar2 = uVar2 + 1) {
      iVar1 = Item::getAmount();
      param_1 = *(Array **)(this + 0x70);
      *(int *)(this + 0x10) = iVar1 + *(int *)(this + 0x10);
    }
  }
  refreshValue(this);
  iVar1 = (*(int *)(this + 0xc) + *(int *)(this + 0x28)) - *(int *)(this + 0x10);
  if (*(int *)(**(int **)(DAT_00184f54 + 0x184f3e) + 0xdc) < iVar1) {
    *(int *)(**(int **)(DAT_00184f54 + 0x184f3e) + 0xdc) = iVar1;
  }
  return;
}

```
## target disasm
```
  00184f00: push {r4,r5,r7,lr}
  00184f02: add r7,sp,#0x8
  00184f04: mov r4,r0
  00184f06: movs r0,#0x0
  00184f08: cmp r1,#0x0
  00184f0a: str r0,[r4,#0x10]
  00184f0c: str r1,[r4,#0x70]
  00184f0e: beq 0x00184f2e
  00184f10: movs r5,#0x0
  00184f12: b 0x00184f28
  00184f14: ldr r0,[r1,#0x4]
  00184f16: ldr.w r0,[r0,r5,lsl #0x2]
  00184f1a: blx 0x0007183c
  00184f1e: ldr r2,[r4,#0x10]
  00184f20: adds r5,#0x1
  00184f22: ldr r1,[r4,#0x70]
  00184f24: add r0,r2
  00184f26: str r0,[r4,#0x10]
  00184f28: ldr r0,[r1,#0x0]
  00184f2a: cmp r5,r0
  00184f2c: bcc 0x00184f14
  00184f2e: mov r0,r4
  00184f30: blx 0x00073768
  00184f34: ldr r0,[0x00184f54]
  00184f36: ldrd r1,r2,[r4,#0xc]
  00184f3a: add r0,pc
  00184f3c: ldr r3,[r4,#0x28]
  00184f3e: ldr r0,[r0,#0x0]
  00184f40: add r1,r3
  00184f42: subs r1,r1,r2
  00184f44: ldr r0,[r0,#0x0]
  00184f46: ldr.w r3,[r0,#0xdc]
  00184f4a: cmp r3,r1
  00184f4c: it lt
  00184f4e: str.w.lt r1,[r0,#0xdc]
  00184f52: pop {r4,r5,r7,pc}
```
