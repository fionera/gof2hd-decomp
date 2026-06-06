# Status::inEmptyOrbit
elf 0xabc18 body 126
Sig: undefined __thiscall inEmptyOrbit(Status * this)

## decompile
```c

/* Status::inEmptyOrbit() */

bool __thiscall Status::inEmptyOrbit(Status *this)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = Station::getIndex(*(Station **)(this + 0x19c));
  if ((((iVar1 != 0x4e) || (1 < *(int *)(this + 0x1e8))) &&
      ((iVar2 = inAlienOrbit(), iVar2 == 0 || (0x28 < *(int *)(this + 0x1e8) - 0x2bU)))) &&
     ((iVar2 = inAlienOrbit(), iVar2 == 0 || (*(int *)(this + 0x1e8) < 0x9a)))) {
    uVar3 = iVar1 - 0x66;
    if (uVar3 < 0x20) {
      if ((1 << (uVar3 & 0xff) & DAT_000bbc98) != 0) {
        return true;
      }
      if (uVar3 == 9) {
        if (0x5d < *(int *)(this + 0x1e8)) {
          return true;
        }
        goto LAB_000bbc8a;
      }
    }
    if ((iVar1 != 0x65) || (*(int *)(this + 0x1e8) < 0x54)) {
LAB_000bbc8a:
      return iVar1 == 0x86;
    }
  }
  return true;
}

```
## target disasm
```
  000bbc18: push {r4,r5,r7,lr}
  000bbc1a: add r7,sp,#0x8
  000bbc1c: mov r5,r0
  000bbc1e: ldr.w r0,[r0,#0x19c]
  000bbc22: blx 0x00071824
  000bbc26: mov r4,r0
  000bbc28: cmp r0,#0x4e
  000bbc2a: bne 0x000bbc38
  000bbc2c: ldr.w r0,[r5,#0x1e8]
  000bbc30: cmp r0,#0x2
  000bbc32: bge 0x000bbc38
  000bbc34: movs r0,#0x1
  000bbc36: pop {r4,r5,r7,pc}
  000bbc38: mov r0,r5
  000bbc3a: blx 0x000723d0
  000bbc3e: cbz r0,0x000bbc4a
  000bbc40: ldr.w r0,[r5,#0x1e8]
  000bbc44: subs r0,#0x2b
  000bbc46: cmp r0,#0x29
  000bbc48: bcc 0x000bbc34
  000bbc4a: mov r0,r5
  000bbc4c: blx 0x000723d0
  000bbc50: cbz r0,0x000bbc5a
  000bbc52: ldr.w r0,[r5,#0x1e8]
  000bbc56: cmp r0,#0x99
  000bbc58: bgt 0x000bbc34
  000bbc5a: sub.w r1,r4,#0x66
  000bbc5e: cmp r1,#0x1f
  000bbc60: bhi 0x000bbc7e
  000bbc62: ldr r2,[0x000bbc98]
  000bbc64: movs r0,#0x1
  000bbc66: lsl.w r3,r0,r1
  000bbc6a: tst r3,r2
  000bbc6c: it ne
  000bbc6e: pop.ne {r4,r5,r7,pc}
  000bbc70: cmp r1,#0x9
  000bbc72: bne 0x000bbc7e
  000bbc74: ldr.w r0,[r5,#0x1e8]
  000bbc78: cmp r0,#0x5d
  000bbc7a: bgt 0x000bbc34
  000bbc7c: b 0x000bbc8a
  000bbc7e: cmp r4,#0x65
  000bbc80: bne 0x000bbc8a
  000bbc82: ldr.w r0,[r5,#0x1e8]
  000bbc86: cmp r0,#0x53
  000bbc88: bgt 0x000bbc34
  000bbc8a: sub.w r0,r4,#0x86
  000bbc8e: clz r0,r0
  000bbc92: lsrs r0,r0,#0x5
  000bbc94: pop {r4,r5,r7,pc}
```
