# Transform*>::~Array
elf 0x748ea body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::Transform*> * this)

## decompile
```c

/* Array<AbyssEngine::Transform*>::~Array() */

Array<AbyssEngine::Transform*> * __thiscall
Array<AbyssEngine::Transform*>::~Array(Array<AbyssEngine::Transform*> *this)

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
  000848ea: push {r4,r6,r7,lr}
  000848ec: add r7,sp,#0x8
  000848ee: mov r4,r0
  000848f0: ldr r0,[r0,#0x4]
  000848f2: cbz r0,0x000848f8
  000848f4: blx 0x0006ebfc
  000848f8: movs r0,#0x0
  000848fa: str r0,[r4,#0x4]
  000848fc: mov r0,r4
  000848fe: pop {r4,r6,r7,pc}
```
