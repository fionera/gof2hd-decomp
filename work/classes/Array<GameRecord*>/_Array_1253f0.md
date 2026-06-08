# Array<GameRecord*>::~Array
elf 0x1253f0 body 22
Sig: undefined __thiscall ~Array(Array<GameRecord*> * this)

## decompile
```c

/* Array<GameRecord*>::~Array() */

Array<GameRecord*> * __thiscall Array<GameRecord*>::~Array(Array<GameRecord*> *this)

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
  001353f0: push {r4,r6,r7,lr}
  001353f2: add r7,sp,#0x8
  001353f4: mov r4,r0
  001353f6: ldr r0,[r0,#0x4]
  001353f8: cbz r0,0x001353fe
  001353fa: blx 0x0006ebfc
  001353fe: movs r0,#0x0
  00135400: str r0,[r4,#0x4]
  00135402: mov r0,r4
  00135404: pop {r4,r6,r7,pc}
```
