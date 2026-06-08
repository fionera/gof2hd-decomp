# Array<RadioMessage*>::~Array
elf 0xadf6c body 22
Sig: undefined __thiscall ~Array(Array<RadioMessage*> * this)

## decompile
```c

/* Array<RadioMessage*>::~Array() */

Array<RadioMessage*> * __thiscall Array<RadioMessage*>::~Array(Array<RadioMessage*> *this)

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
  000bdf6c: push {r4,r6,r7,lr}
  000bdf6e: add r7,sp,#0x8
  000bdf70: mov r4,r0
  000bdf72: ldr r0,[r0,#0x4]
  000bdf74: cbz r0,0x000bdf7a
  000bdf76: blx 0x0006ebfc
  000bdf7a: movs r0,#0x0
  000bdf7c: str r0,[r4,#0x4]
  000bdf7e: mov r0,r4
  000bdf80: pop {r4,r6,r7,pc}
```
