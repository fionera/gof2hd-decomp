# Array<AbstractGun*>::~Array
elf 0xadea6 body 22
Sig: undefined __thiscall ~Array(Array<AbstractGun*> * this)

## decompile
```c

/* Array<AbstractGun*>::~Array() */

Array<AbstractGun*> * __thiscall Array<AbstractGun*>::~Array(Array<AbstractGun*> *this)

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
  000bdea6: push {r4,r6,r7,lr}
  000bdea8: add r7,sp,#0x8
  000bdeaa: mov r4,r0
  000bdeac: ldr r0,[r0,#0x4]
  000bdeae: cbz r0,0x000bdeb4
  000bdeb0: blx 0x0006ebfc
  000bdeb4: movs r0,#0x0
  000bdeb6: str r0,[r4,#0x4]
  000bdeb8: mov r0,r4
  000bdeba: pop {r4,r6,r7,pc}
```
