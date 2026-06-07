# Standing::Standing
elf 0x11d6a8 body 32
Sig: undefined __thiscall Standing(Standing * this)

## decompile
```c

/* Standing::Standing() */

Standing * __thiscall Standing::Standing(Standing *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(8);
  *puVar1 = 0x1e;
  puVar1[1] = 0;
  *(undefined4 **)this = puVar1;
  *(undefined4 *)(this + 4) = 0xffffffff;
  return this;
}

```

## target disasm
```
  0012d6a8: push {r4,r6,r7,lr}
  0012d6aa: add r7,sp,#0x8
  0012d6ac: mov r4,r0
  0012d6ae: movs r0,#0x8
  0012d6b0: blx 0x0006ec08
  0012d6b4: movs r1,#0x0
  0012d6b6: movs r2,#0x1e
  0012d6b8: strd r2,r1,[r0,#0x0]
  0012d6bc: mov.w r1,#0xffffffff
  0012d6c0: strd r0,r1,[r4,#0x0]
  0012d6c4: mov r0,r4
  0012d6c6: pop {r4,r6,r7,pc}
```
