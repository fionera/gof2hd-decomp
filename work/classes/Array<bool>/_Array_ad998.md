# Array<bool>::~Array
elf 0xad998 body 20
Sig: undefined __thiscall ~Array(Array<bool> * this)

## decompile
```c

/* Array<bool>::~Array() */

Array<bool> * __thiscall Array<bool>::~Array(Array<bool> *this)

{
  operator_delete__(*(void **)(this + 4));
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  000bd998: push {r4,r6,r7,lr}
  000bd99a: add r7,sp,#0x8
  000bd99c: mov r4,r0
  000bd99e: ldr r0,[r0,#0x4]
  000bd9a0: blx 0x0006ebfc
  000bd9a4: movs r0,#0x0
  000bd9a6: str r0,[r4,#0x4]
  000bd9a8: mov r0,r4
  000bd9aa: pop {r4,r6,r7,pc}
```
