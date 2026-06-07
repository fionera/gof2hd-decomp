# Standing::getEnemyRace
elf 0x11d890 body 18
Sig: undefined __thiscall getEnemyRace(Standing * this, int param_1)

## decompile
```c

/* Standing::getEnemyRace(int) */

undefined4 __thiscall Standing::getEnemyRace(Standing *this,int param_1)

{
  if ((uint)param_1 < 4) {
    return *(undefined4 *)(DAT_0012d8a4 + 0x12d89e + param_1 * 4);
  }
  return 8;
}

```

## target disasm
```
  0012d890: cmp r1,#0x3
  0012d892: itt hi
  0012d894: mov.hi r0,#0x8
  0012d896: bx.hi lr
  0012d898: ldr r0,[0x0012d8a4]
  0012d89a: add r0,pc
  0012d89c: ldr.w r0,[r0,r1,lsl #0x2]
  0012d8a0: bx lr
```
