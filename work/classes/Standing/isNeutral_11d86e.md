# Standing::isNeutral
elf 0x11d86e body 32
Sig: undefined __thiscall isNeutral(Standing * this, int param_1)

## decompile
```c

/* Standing::isNeutral(int) */

uint __thiscall Standing::isNeutral(Standing *this,int param_1)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = isEnemy((int)this);
  if (iVar1 != 0) {
    return 0;
  }
  uVar2 = isFriend(this,param_1);
  return uVar2 ^ 1;
}

```

## target disasm
```
  0012d86e: push {r4,r5,r7,lr}
  0012d870: add r7,sp,#0x8
  0012d872: mov r4,r1
  0012d874: mov r5,r0
  0012d876: blx 0x00071b90
  0012d87a: cbz r0,0x0012d880
  0012d87c: movs r0,#0x0
  0012d87e: pop {r4,r5,r7,pc}
  0012d880: mov r0,r5
  0012d882: mov r1,r4
  0012d884: blx 0x00075808
  0012d888: eor r0,r0,#0x1
  0012d88c: pop {r4,r5,r7,pc}
```
