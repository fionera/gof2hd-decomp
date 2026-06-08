# String::~String
elf 0x72594 body 32
Sig: undefined __thiscall ~String(String * this)

## decompile
```c

/* AbyssEngine::String::~String() */

String * __thiscall AbyssEngine::String::~String(String *this)

{
  *(int *)this = *(int *)(DAT_000825b4 + 0x825a0) + 8;
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  00082594: push {r4,r6,r7,lr}
  00082596: add r7,sp,#0x8
  00082598: mov r4,r0
  0008259a: ldr r0,[0x000825b4]
  0008259c: add r0,pc
  0008259e: ldr r1,[r0,#0x0]
  000825a0: ldr r0,[r4,#0x4]
  000825a2: adds r1,#0x8
  000825a4: str r1,[r4,#0x0]
  000825a6: cbz r0,0x000825ac
  000825a8: blx 0x0006ebfc
  000825ac: movs r0,#0x0
  000825ae: str r0,[r4,#0x4]
  000825b0: mov r0,r4
  000825b2: pop {r4,r6,r7,pc}
```
