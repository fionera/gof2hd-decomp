# AESoundInterface*>::~Array
elf 0x7fb5e body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::AESoundInterface*> * this)

## decompile
```c

/* Array<AbyssEngine::AESoundInterface*>::~Array() */

Array<AbyssEngine::AESoundInterface*> * __thiscall
Array<AbyssEngine::AESoundInterface*>::~Array(Array<AbyssEngine::AESoundInterface*> *this)

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
  0008fb5e: push {r4,r6,r7,lr}
  0008fb60: add r7,sp,#0x8
  0008fb62: mov r4,r0
  0008fb64: ldr r0,[r0,#0x4]
  0008fb66: cbz r0,0x0008fb6c
  0008fb68: blx 0x0006ebfc
  0008fb6c: movs r0,#0x0
  0008fb6e: str r0,[r4,#0x4]
  0008fb70: mov r0,r4
  0008fb72: pop {r4,r6,r7,pc}
```
