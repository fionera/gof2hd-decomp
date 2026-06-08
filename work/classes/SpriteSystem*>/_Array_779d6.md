# SpriteSystem*>::~Array
elf 0x779d6 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::SpriteSystem*> * this)

## decompile
```c

/* Array<AbyssEngine::SpriteSystem*>::~Array() */

Array<AbyssEngine::SpriteSystem*> * __thiscall
Array<AbyssEngine::SpriteSystem*>::~Array(Array<AbyssEngine::SpriteSystem*> *this)

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
  000879d6: push {r4,r6,r7,lr}
  000879d8: add r7,sp,#0x8
  000879da: mov r4,r0
  000879dc: ldr r0,[r0,#0x4]
  000879de: cbz r0,0x000879e4
  000879e0: blx 0x0006ebfc
  000879e4: movs r0,#0x0
  000879e6: str r0,[r4,#0x4]
  000879e8: mov r0,r4
  000879ea: pop {r4,r6,r7,pc}
```
