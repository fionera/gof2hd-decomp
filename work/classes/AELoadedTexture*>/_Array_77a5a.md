# AELoadedTexture*>::~Array
elf 0x77a5a body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::AELoadedTexture*> * this)

## decompile
```c

/* Array<AbyssEngine::AELoadedTexture*>::~Array() */

Array<AbyssEngine::AELoadedTexture*> * __thiscall
Array<AbyssEngine::AELoadedTexture*>::~Array(Array<AbyssEngine::AELoadedTexture*> *this)

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
  00087a5a: push {r4,r6,r7,lr}
  00087a5c: add r7,sp,#0x8
  00087a5e: mov r4,r0
  00087a60: ldr r0,[r0,#0x4]
  00087a62: cbz r0,0x00087a68
  00087a64: blx 0x0006ebfc
  00087a68: movs r0,#0x0
  00087a6a: str r0,[r4,#0x4]
  00087a6c: mov r0,r4
  00087a6e: pop {r4,r6,r7,pc}
```
