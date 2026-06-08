# KeyFrame*>::~Array
elf 0x748d4 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::KeyFrame*> * this)

## decompile
```c

/* Array<AbyssEngine::KeyFrame*>::~Array() */

Array<AbyssEngine::KeyFrame*> * __thiscall
Array<AbyssEngine::KeyFrame*>::~Array(Array<AbyssEngine::KeyFrame*> *this)

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
  000848d4: push {r4,r6,r7,lr}
  000848d6: add r7,sp,#0x8
  000848d8: mov r4,r0
  000848da: ldr r0,[r0,#0x4]
  000848dc: cbz r0,0x000848e2
  000848de: blx 0x0006ebfc
  000848e2: movs r0,#0x0
  000848e4: str r0,[r4,#0x4]
  000848e6: mov r0,r4
  000848e8: pop {r4,r6,r7,pc}
```
