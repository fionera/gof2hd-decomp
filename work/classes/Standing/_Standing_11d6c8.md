# Standing::~Standing
elf 0x11d6c8 body 22
Sig: undefined __thiscall ~Standing(Standing * this)

## decompile
```c

/* Standing::~Standing() */

Standing * __thiscall Standing::~Standing(Standing *this)

{
  if (*(void **)this != (void *)0x0) {
    operator_delete__(*(void **)this);
  }
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  0012d6c8: push {r4,r6,r7,lr}
  0012d6ca: add r7,sp,#0x8
  0012d6cc: mov r4,r0
  0012d6ce: ldr r0,[r0,#0x0]
  0012d6d0: cbz r0,0x0012d6d6
  0012d6d2: blx 0x0006ebfc
  0012d6d6: movs r0,#0x0
  0012d6d8: str r0,[r4,#0x0]
  0012d6da: mov r0,r4
  0012d6dc: pop {r4,r6,r7,pc}
```
