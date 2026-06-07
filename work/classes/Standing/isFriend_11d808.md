# Standing::isFriend
elf 0x11d808 body 102
Sig: undefined __thiscall isFriend(Standing * this, int param_1)

## decompile
```c

/* Standing::isFriend(int) */

bool __thiscall Standing::isFriend(Standing *this,int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 4);
  if (-1 < iVar1) {
    if (param_1 == 1) {
      iVar1 = iVar1 + -1;
    }
    else if (param_1 == 3) {
      iVar1 = iVar1 + -3;
    }
    else if (param_1 == 2) {
      iVar1 = iVar1 + -2;
    }
    else if (param_1 != 0) {
      return false;
    }
    return iVar1 == 0;
  }
  if (param_1 == 1) {
    iVar1 = **(int **)this;
  }
  else {
    if (param_1 != 3) {
      if (param_1 == 2) {
        iVar1 = *(int *)(*(int *)this + 4);
      }
      else {
        if (param_1 != 0) {
          return false;
        }
        iVar1 = **(int **)this;
      }
      return 0x46 < iVar1;
    }
    iVar1 = *(int *)(*(int *)this + 4);
  }
  return iVar1 < -0x46;
}

```

## target disasm
```
  0012d808: mov r2,r0
  0012d80a: ldr r0,[r0,#0x4]
  0012d80c: cmp r0,#0x0
  0012d80e: blt 0x0012d818
  0012d810: cmp r1,#0x1
  0012d812: bne 0x0012d824
  0012d814: subs r0,#0x1
  0012d816: b 0x0012d84a
  0012d818: cmp r1,#0x1
  0012d81a: bne 0x0012d832
  0012d81c: ldr r0,[r2,#0x0]
  0012d81e: ldr r1,[r0,#0x0]
  0012d820: movs r0,#0x0
  0012d822: b 0x0012d856
  0012d824: cmp r1,#0x3
  0012d826: beq 0x0012d844
  0012d828: cmp r1,#0x2
  0012d82a: beq 0x0012d848
  0012d82c: cbz r1,0x0012d84a
  0012d82e: movs r0,#0x0
  0012d830: bx lr
  0012d832: movs r0,#0x0
  0012d834: cmp r1,#0x3
  0012d836: beq 0x0012d852
  0012d838: cmp r1,#0x2
  0012d83a: beq 0x0012d860
  0012d83c: cbnz r1,0x0012d86c
  0012d83e: ldr r0,[r2,#0x0]
  0012d840: ldr r1,[r0,#0x0]
  0012d842: b 0x0012d864
  0012d844: subs r0,#0x3
  0012d846: b 0x0012d84a
  0012d848: subs r0,#0x2
  0012d84a: clz r0,r0
  0012d84e: lsrs r0,r0,#0x5
  0012d850: bx lr
  0012d852: ldr r1,[r2,#0x0]
  0012d854: ldr r1,[r1,#0x4]
  0012d856: cmn r1,#0x46
  0012d85a: it lt
  0012d85c: mov.lt r0,#0x1
  0012d85e: bx lr
  0012d860: ldr r0,[r2,#0x0]
  0012d862: ldr r1,[r0,#0x4]
  0012d864: movs r0,#0x0
  0012d866: cmp r1,#0x46
  0012d868: it gt
  0012d86a: mov.gt r0,#0x1
  0012d86c: bx lr
```
