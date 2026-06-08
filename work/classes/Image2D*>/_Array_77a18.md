# Image2D*>::~Array
elf 0x77a18 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::Image2D*> * this)

## decompile
```c

/* Array<AbyssEngine::Image2D*>::~Array() */

Array<AbyssEngine::Image2D*> * __thiscall
Array<AbyssEngine::Image2D*>::~Array(Array<AbyssEngine::Image2D*> *this)

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
  00087a18: push {r4,r6,r7,lr}
  00087a1a: add r7,sp,#0x8
  00087a1c: mov r4,r0
  00087a1e: ldr r0,[r0,#0x4]
  00087a20: cbz r0,0x00087a26
  00087a22: blx 0x0006ebfc
  00087a26: movs r0,#0x0
  00087a28: str r0,[r4,#0x4]
  00087a2a: mov r0,r4
  00087a2c: pop {r4,r6,r7,pc}
```
