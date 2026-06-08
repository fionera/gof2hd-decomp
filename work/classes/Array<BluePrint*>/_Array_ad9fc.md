# Array<BluePrint*>::~Array
elf 0xad9fc body 22
Sig: undefined __thiscall ~Array(Array<BluePrint*> * this)

## decompile
```c

/* Array<BluePrint*>::~Array() */

Array<BluePrint*> * __thiscall Array<BluePrint*>::~Array(Array<BluePrint*> *this)

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
  000bd9fc: push {r4,r6,r7,lr}
  000bd9fe: add r7,sp,#0x8
  000bda00: mov r4,r0
  000bda02: ldr r0,[r0,#0x4]
  000bda04: cbz r0,0x000bda0a
  000bda06: blx 0x0006ebfc
  000bda0a: movs r0,#0x0
  000bda0c: str r0,[r4,#0x4]
  000bda0e: mov r0,r4
  000bda10: pop {r4,r6,r7,pc}
```
