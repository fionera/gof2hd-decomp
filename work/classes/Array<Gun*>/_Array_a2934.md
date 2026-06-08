# Array<Gun*>::~Array
elf 0xa2934 body 22
Sig: undefined __thiscall ~Array(Array<Gun*> * this)

## decompile
```c

/* Array<Gun*>::~Array() */

Array<Gun*> * __thiscall Array<Gun*>::~Array(Array<Gun*> *this)

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
  000b2934: push {r4,r6,r7,lr}
  000b2936: add r7,sp,#0x8
  000b2938: mov r4,r0
  000b293a: ldr r0,[r0,#0x4]
  000b293c: cbz r0,0x000b2942
  000b293e: blx 0x0006ebfc
  000b2942: movs r0,#0x0
  000b2944: str r0,[r4,#0x4]
  000b2946: mov r0,r4
  000b2948: pop {r4,r6,r7,pc}
```
