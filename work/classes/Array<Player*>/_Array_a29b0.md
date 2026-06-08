# Array<Player*>::~Array
elf 0xa29b0 body 22
Sig: undefined __thiscall ~Array(Array<Player*> * this)

## decompile
```c

/* Array<Player*>::~Array() */

Array<Player*> * __thiscall Array<Player*>::~Array(Array<Player*> *this)

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
  000b29b0: push {r4,r6,r7,lr}
  000b29b2: add r7,sp,#0x8
  000b29b4: mov r4,r0
  000b29b6: ldr r0,[r0,#0x4]
  000b29b8: cbz r0,0x000b29be
  000b29ba: blx 0x0006ebfc
  000b29be: movs r0,#0x0
  000b29c0: str r0,[r4,#0x4]
  000b29c2: mov r0,r4
  000b29c4: pop {r4,r6,r7,pc}
```
