# Standing::isEnemyWithAnyone
elf 0x11d780 body 34
Sig: undefined __stdcall isEnemyWithAnyone(void)

## decompile
```c

/* Standing::isEnemyWithAnyone() */

bool Standing::isEnemyWithAnyone(void)

{
  int *in_r0;
  
  return 0x8c < ((int *)*in_r0)[1] + 0x46U || 0x8c < *(int *)*in_r0 + 0x46U;
}

```

## target disasm
```
  0012d780: ldr r0,[r0,#0x0]
  0012d782: movs r2,#0x0
  0012d784: ldrd r1,r0,[r0,#0x0]
  0012d788: adds r0,#0x46
  0012d78a: cmp r0,#0x8c
  0012d78c: mov.w r0,#0x0
  0012d790: add.w r1,r1,#0x46
  0012d794: it hi
  0012d796: mov.hi r0,#0x1
  0012d798: cmp r1,#0x8c
  0012d79a: it hi
  0012d79c: mov.hi r2,#0x1
  0012d79e: orrs r0,r2
  0012d7a0: bx lr
```
