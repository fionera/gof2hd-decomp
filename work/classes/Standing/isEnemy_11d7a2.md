# Standing::isEnemy
elf 0x11d7a2 body 102
Sig: undefined __stdcall isEnemy(int param_1)

## decompile
```c

/* Standing::isEnemy(int) */

bool Standing::isEnemy(int param_1)

{
  int iVar1;
  int in_r1;
  
  iVar1 = *(int *)(param_1 + 4);
  if (-1 < iVar1) {
    if (in_r1 != 1) {
      if (in_r1 == 3) {
        iVar1 = iVar1 + -2;
      }
      else if (in_r1 == 2) {
        iVar1 = iVar1 + -3;
      }
      else {
        if (in_r1 != 0) {
          return false;
        }
        iVar1 = iVar1 + -1;
      }
    }
    return iVar1 == 0;
  }
  if (in_r1 == 1) {
    iVar1 = **(int **)param_1;
  }
  else {
    if (in_r1 != 3) {
      if (in_r1 == 2) {
        iVar1 = *(int *)(*(int *)param_1 + 4);
      }
      else {
        if (in_r1 != 0) {
          return false;
        }
        iVar1 = **(int **)param_1;
      }
      return iVar1 < -0x46;
    }
    iVar1 = *(int *)(*(int *)param_1 + 4);
  }
  return 0x46 < iVar1;
}

```

## target disasm
```
  0012d7a2: mov r2,r0
  0012d7a4: ldr r0,[r0,#0x4]
  0012d7a6: cmp r0,#0x0
  0012d7a8: blt 0x0012d7bc
  0012d7aa: cmp r1,#0x1
  0012d7ac: beq 0x0012d7e0
  0012d7ae: cmp r1,#0x3
  0012d7b0: beq 0x0012d7da
  0012d7b2: cmp r1,#0x2
  0012d7b4: beq 0x0012d7de
  0012d7b6: cbnz r1,0x0012d7e8
  0012d7b8: subs r0,#0x1
  0012d7ba: b 0x0012d7e0
  0012d7bc: cmp r1,#0x1
  0012d7be: bne 0x0012d7c8
  0012d7c0: ldr r0,[r2,#0x0]
  0012d7c2: ldr r1,[r0,#0x0]
  0012d7c4: movs r0,#0x0
  0012d7c6: b 0x0012d7f0
  0012d7c8: movs r0,#0x0
  0012d7ca: cmp r1,#0x3
  0012d7cc: beq 0x0012d7ec
  0012d7ce: cmp r1,#0x2
  0012d7d0: beq 0x0012d7f8
  0012d7d2: cbnz r1,0x0012d806
  0012d7d4: ldr r0,[r2,#0x0]
  0012d7d6: ldr r1,[r0,#0x0]
  0012d7d8: b 0x0012d7fc
  0012d7da: subs r0,#0x2
  0012d7dc: b 0x0012d7e0
  0012d7de: subs r0,#0x3
  0012d7e0: clz r0,r0
  0012d7e4: lsrs r0,r0,#0x5
  0012d7e6: bx lr
  0012d7e8: movs r0,#0x0
  0012d7ea: bx lr
  0012d7ec: ldr r1,[r2,#0x0]
  0012d7ee: ldr r1,[r1,#0x4]
  0012d7f0: cmp r1,#0x46
  0012d7f2: it gt
  0012d7f4: mov.gt r0,#0x1
  0012d7f6: bx lr
  0012d7f8: ldr r0,[r2,#0x0]
  0012d7fa: ldr r1,[r0,#0x4]
  0012d7fc: movs r0,#0x0
  0012d7fe: cmn r1,#0x46
  0012d802: it lt
  0012d804: mov.lt r0,#0x1
  0012d806: bx lr
```
