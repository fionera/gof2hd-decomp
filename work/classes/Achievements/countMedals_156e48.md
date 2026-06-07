# Achievements::countMedals
elf 0x156e48 body 126
Sig: undefined __thiscall countMedals(Achievements * this)

## decompile
```c

/* Achievements::countMedals() */

void __thiscall Achievements::countMedals(Achievements *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = 0;
  iVar4 = 0;
  *(undefined4 *)(this + 0x24) = 0;
  for (iVar2 = 0; iVar2 != 0x24; iVar2 = iVar2 + 1) {
    iVar3 = *(int *)(*(int *)this + iVar2 * 4);
    if (iVar3 != 0) {
      iVar5 = iVar5 + 1;
      if (iVar3 == 1) {
        *(int *)(this + 0x24) = iVar5;
        iVar4 = iVar4 + 1;
      }
      else {
        *(int *)(this + 0x24) = iVar5;
      }
    }
  }
  iVar3 = 0;
  iVar2 = 0;
  while (iVar3 != 9) {
    iVar1 = iVar3 * 4;
    iVar3 = iVar3 + 1;
    if (*(int *)(*(int *)this + 0x90 + iVar1) != 0) {
      iVar2 = iVar2 + 1;
    }
  }
  this[0x22] = (Achievements)(iVar2 == 9 && iVar4 == 0x24);
  this[0x21] = (Achievements)(iVar4 == 0x24);
  this[0x20] = (Achievements)(iVar5 == 0x24);
  return;
}

```

## target disasm
```
  00166e48: push {r4,r6,r7,lr}
  00166e4a: add r7,sp,#0x8
  00166e4c: ldr r3,[r0,#0x0]
  00166e4e: mov.w lr,#0x0
  00166e52: mov.w r12,#0x0
  00166e56: movs r2,#0x0
  00166e58: str.w lr,[r0,#0x24]
  00166e5c: b 0x00166e7c
  00166e5e: ldr.w r1,[r3,r2,lsl #0x2]
  00166e62: cbz r1,0x00166e7a
  00166e64: add.w lr,lr,#0x1
  00166e68: cmp r1,#0x1
  00166e6a: bne 0x00166e76
  00166e6c: str.w lr,[r0,#0x24]
  00166e70: add.w r12,r12,#0x1
  00166e74: b 0x00166e7a
  00166e76: str.w lr,[r0,#0x24]
  00166e7a: adds r2,#0x1
  00166e7c: cmp r2,#0x24
  00166e7e: bne 0x00166e5e
  00166e80: add.w r2,r3,#0x90
  00166e84: movs r3,#0x0
  00166e86: movs r1,#0x0
  00166e88: b 0x00166e96
  00166e8a: ldr.w r4,[r2,r3,lsl #0x2]
  00166e8e: adds r3,#0x1
  00166e90: cmp r4,#0x0
  00166e92: it ne
  00166e94: add.ne r1,#0x1
  00166e96: cmp r3,#0x9
  00166e98: bne 0x00166e8a
  00166e9a: subs r1,#0x9
  00166e9c: sub.w r2,r12,#0x24
  00166ea0: clz r1,r1
  00166ea4: clz r2,r2
  00166ea8: lsrs r1,r1,#0x5
  00166eaa: lsrs r2,r2,#0x5
  00166eac: ands r1,r2
  00166eae: strb.w r1,[r0,#0x22]
  00166eb2: sub.w r1,lr,#0x24
  00166eb6: strb.w r2,[r0,#0x21]
  00166eba: clz r1,r1
  00166ebe: lsrs r1,r1,#0x5
  00166ec0: strb.w r1,[r0,#0x20]
  00166ec4: pop {r4,r6,r7,pc}
```
