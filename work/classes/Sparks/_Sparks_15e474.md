# Sparks::~Sparks
elf 0x15e474 body 22
Sig: undefined __thiscall ~Sparks(Sparks * this)

## decompile
```c

/* Sparks::~Sparks() */

Sparks * __thiscall Sparks::~Sparks(Sparks *this)

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
  0016e474: push {r4,r6,r7,lr}
  0016e476: add r7,sp,#0x8
  0016e478: mov r4,r0
  0016e47a: ldr r0,[r0,#0x0]
  0016e47c: cbz r0,0x0016e482
  0016e47e: blx 0x0006ebfc
  0016e482: movs r0,#0x0
  0016e484: str r0,[r4,#0x0]
  0016e486: mov r0,r4
  0016e488: pop {r4,r6,r7,pc}
```
