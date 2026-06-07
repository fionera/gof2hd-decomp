# Transform::InitAnimationRangeInTime
elf 0x74c62 body 102
Sig: undefined __thiscall InitAnimationRangeInTime(Transform * this)

## decompile
```c

/* AbyssEngine::Transform::InitAnimationRangeInTime() */

void __thiscall AbyssEngine::Transform::InitAnimationRangeInTime(Transform *this)

{
  Transform *this_00;
  int iVar1;
  int extraout_r1;
  int extraout_r1_00;
  int iVar2;
  uint uVar3;
  
  iVar1 = *(int *)(this + 0xf8);
  if (iVar1 != 0 || *(int *)(this + 0xfc) != 0) {
    iVar2 = *(int *)(this + 0xe8);
    *(int *)(this + 0x100) = iVar2;
    *(int *)(this + 0x104) = iVar2 >> 0x1f;
    *(int *)(this + 0x108) = iVar1;
    *(int *)(this + 0x10c) = *(int *)(this + 0xfc);
    *(int *)(this + 0x110) = iVar2;
    *(int *)(this + 0x114) = iVar2 >> 0x1f;
    for (uVar3 = 0; uVar3 < *(uint *)(this + 0x3c); uVar3 = uVar3 + 1) {
      iVar2 = *(int *)(*(int *)(this + 0x40) + uVar3 * 4);
      if ((iVar2 != 0) && (this_00 = *(Transform **)(iVar2 + 0x34), this_00 != (Transform *)0x0)) {
        InitAnimationRangeInTime(this_00);
        iVar1 = extraout_r1;
      }
    }
    for (uVar3 = 0; uVar3 < *(uint *)(this + 0x4c); uVar3 = uVar3 + 1) {
      InitAnimationRangeInTime(*(Transform **)(*(int *)(this + 0x50) + uVar3 * 4));
      iVar1 = extraout_r1_00;
    }
    Update(CONCAT44(iVar1,this),false);
    return;
  }
  return;
}

```

## target disasm
```
  00084c62: push {r2,r3,r4,r5,r7,lr}
  00084c64: add r7,sp,#0x10
  00084c66: mov r4,r0
  00084c68: ldrd r1,r0,[r0,#0xf8]
  00084c6c: orrs.w r2,r1,r0
  00084c70: beq 0x00084cc6
  00084c72: ldr.w r2,[r4,#0xe8]
  00084c76: movs r5,#0x0
  00084c78: asrs r3,r2,#0x1f
  00084c7a: strd r2,r3,[r4,#0x100]
  00084c7e: strd r1,r0,[r4,#0x108]
  00084c82: strd r2,r3,[r4,#0x110]
  00084c86: b 0x00084c9a
  00084c88: ldr r0,[r4,#0x40]
  00084c8a: ldr.w r0,[r0,r5,lsl #0x2]
  00084c8e: cbz r0,0x00084c98
  00084c90: ldr r0,[r0,#0x34]
  00084c92: cbz r0,0x00084c98
  00084c94: blx 0x0006f7c0
  00084c98: adds r5,#0x1
  00084c9a: ldr r0,[r4,#0x3c]
  00084c9c: cmp r5,r0
  00084c9e: bcc 0x00084c88
  00084ca0: movs r5,#0x0
  00084ca2: b 0x00084cb0
  00084ca4: ldr r0,[r4,#0x50]
  00084ca6: ldr.w r0,[r0,r5,lsl #0x2]
  00084caa: blx 0x0006f7c0
  00084cae: adds r5,#0x1
  00084cb0: ldr r0,[r4,#0x4c]
  00084cb2: cmp r5,r0
  00084cb4: bcc 0x00084ca4
  00084cb6: movs r0,#0x0
  00084cb8: movs r2,#0x0
  00084cba: str r0,[sp,#0x0]
  00084cbc: mov r0,r4
  00084cbe: movs r3,#0x0
  00084cc0: blx 0x0006f7cc
  00084cc4: pop {r2,r3,r4,r5,r7,pc}
  00084cc6: pop {r2,r3,r4,r5,r7,pc}
```
