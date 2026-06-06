# Ship::getEquipment
elf 0x1750cc body 128
Sig: undefined __thiscall getEquipment(Ship * this, int param_1)

## decompile
```c

/* Ship::getEquipment(int) */

Array<Item*> * __thiscall Ship::getEquipment(Ship *this,int param_1)

{
  Array<Item*> *this_00;
  int iVar1;
  uint uVar2;
  int iVar3;
  
  if ((param_1 < 4) && (*(int *)(*(int *)(this + 0x68) + param_1 * 4) != 0)) {
    this_00 = operator_new(0xc);
    Array<Item*>::Array(this_00);
    ArraySetLength<Item*>(*(uint *)(*(int *)(this + 0x68) + param_1 * 4),(Array *)this_00);
    iVar3 = 0;
    for (iVar1 = 0; iVar1 < param_1; iVar1 = iVar1 + 1) {
      iVar3 = iVar3 + *(int *)(*(int *)(this + 0x68) + iVar1 * 4);
    }
    uVar2 = 0;
    for (iVar1 = iVar3; iVar1 < *(int *)(*(int *)(this + 0x68) + param_1 * 4) + iVar3;
        iVar1 = iVar1 + 1) {
      if (uVar2 < *(uint *)this_00) {
        *(undefined4 *)(*(int *)(this_00 + 4) + uVar2 * 4) =
             *(undefined4 *)(*(int *)(*(int *)(this + 0x6c) + 4) + iVar1 * 4);
        uVar2 = uVar2 + 1;
      }
    }
  }
  else {
    this_00 = (Array<Item*> *)0x0;
  }
  return this_00;
}

```
## target disasm
```
  001850cc: push {r4,r5,r6,r7,lr}
  001850ce: add r7,sp,#0xc
  001850d0: push.w r8
  001850d4: mov r5,r1
  001850d6: mov r6,r0
  001850d8: cmp r1,#0x3
  001850da: bgt 0x00185140
  001850dc: ldr r0,[r6,#0x68]
  001850de: ldr.w r0,[r0,r5,lsl #0x2]
  001850e2: cbz r0,0x00185140
  001850e4: movs r0,#0xc
  001850e6: blx 0x0006eb24
  001850ea: mov r8,r0
  001850ec: blx 0x00071860
  001850f0: ldr r0,[r6,#0x68]
  001850f2: mov r1,r8
  001850f4: ldr.w r0,[r0,r5,lsl #0x2]
  001850f8: blx 0x0007186c
  001850fc: movs r2,#0x0
  001850fe: movs r0,#0x0
  00185100: b 0x0018510c
  00185102: ldr r1,[r6,#0x68]
  00185104: ldr.w r1,[r1,r0,lsl #0x2]
  00185108: adds r0,#0x1
  0018510a: add r2,r1
  0018510c: cmp r0,r5
  0018510e: blt 0x00185102
  00185110: movs r0,#0x0
  00185112: mov r1,r2
  00185114: b 0x00185132
  00185116: ldr.w r3,[r8,#0x0]
  0018511a: cmp r0,r3
  0018511c: bcs 0x00185130
  0018511e: ldr r3,[r6,#0x6c]
  00185120: ldr.w r4,[r8,#0x4]
  00185124: ldr r3,[r3,#0x4]
  00185126: ldr.w r3,[r3,r1,lsl #0x2]
  0018512a: str.w r3,[r4,r0,lsl #0x2]
  0018512e: adds r0,#0x1
  00185130: adds r1,#0x1
  00185132: ldr r3,[r6,#0x68]
  00185134: ldr.w r3,[r3,r5,lsl #0x2]
  00185138: add r3,r2
  0018513a: cmp r1,r3
  0018513c: blt 0x00185116
  0018513e: b 0x00185144
  00185140: mov.w r8,#0x0
  00185144: mov r0,r8
  00185146: pop.w r8
  0018514a: pop {r4,r5,r6,r7,pc}
```
