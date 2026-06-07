# Achievements::applyNewMedals
elf 0x156fb0 body 90
Sig: undefined __thiscall applyNewMedals(Achievements * this)

## decompile
```c

/* Achievements::applyNewMedals() */

void __thiscall Achievements::applyNewMedals(Achievements *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = *(int *)(this + 4);
  for (iVar2 = 0; iVar2 != 0x2d; iVar2 = iVar2 + 1) {
    iVar3 = *(int *)(iVar1 + iVar2 * 4);
    if (0 < iVar3) {
      iVar4 = *(int *)this;
      iVar5 = *(int *)(iVar4 + iVar2 * 4);
      if (iVar3 < iVar5) {
        *(int *)(iVar4 + iVar2 * 4) = iVar3;
      }
      else if (iVar5 == 0) {
        *(int *)(iVar4 + iVar2 * 4) = iVar3;
      }
    }
  }
  countMedals(this);
  if (*(int *)(this + 0x24) == 0x23) {
    *(undefined4 *)(*(int *)(this + 4) + 0x8c) = 1;
    *(undefined4 *)(*(int *)this + 0x8c) = 1;
    (*(code *)(DAT_001ac5a4 + 0x1ac5a8))(this);
    return;
  }
  return;
}

```

## target disasm
```
  00166fb0: push {r4,r5,r7,lr}
  00166fb2: add r7,sp,#0x8
  00166fb4: mov r5,r0
  00166fb6: ldr r0,[r0,#0x4]
  00166fb8: movs r1,#0x0
  00166fba: b 0x00166fde
  00166fbc: ldr.w r2,[r0,r1,lsl #0x2]
  00166fc0: cmp r2,#0x1
  00166fc2: blt 0x00166fdc
  00166fc4: ldr r3,[r5,#0x0]
  00166fc6: ldr.w r4,[r3,r1,lsl #0x2]
  00166fca: cmp r2,r4
  00166fcc: blt 0x00166fd8
  00166fce: cmp r4,#0x0
  00166fd0: it eq
  00166fd2: str.eq.w r2,[r3,r1,lsl #0x2]
  00166fd6: b 0x00166fdc
  00166fd8: str.w r2,[r3,r1,lsl #0x2]
  00166fdc: adds r1,#0x1
  00166fde: cmp r1,#0x2d
  00166fe0: bne 0x00166fbc
  00166fe2: mov r0,r5
  00166fe4: blx 0x00077680
  00166fe8: ldr r0,[r5,#0x24]
  00166fea: cmp r0,#0x23
  00166fec: it ne
  00166fee: pop.ne {r4,r5,r7,pc}
  00166ff0: ldr r0,[r5,#0x4]
  00166ff2: movs r1,#0x1
  00166ff4: str.w r1,[r0,#0x8c]
  00166ff8: ldr r0,[r5,#0x0]
  00166ffa: str.w r1,[r0,#0x8c]
  00166ffe: mov r0,r5
  00167000: pop.w {r4,r5,r7,lr}
  00167004: b.w 0x001ac598
  001ac598: bx pc
```
