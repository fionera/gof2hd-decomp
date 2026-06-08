# ImageFont*>::~Array
elf 0x77a2e body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::ImageFont*> * this)

## decompile
```c

/* Array<AbyssEngine::ImageFont*>::~Array() */

Array<AbyssEngine::ImageFont*> * __thiscall
Array<AbyssEngine::ImageFont*>::~Array(Array<AbyssEngine::ImageFont*> *this)

{
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  00087a2e: push {r4,r6,r7,lr}
  00087a30: add r7,sp,#0x8
  00087a32: mov r4,r0
  00087a34: ldr r0,[r0,#0x4]
  00087a36: cbz r0,0x00087a3c
  00087a38: blx 0x0006ebfc
  00087a3c: movs r0,#0x0
  00087a3e: str r0,[r4,#0x4]
  00087a40: mov r0,r4
  00087a42: pop {r4,r6,r7,pc}
```
