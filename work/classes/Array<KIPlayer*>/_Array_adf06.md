# Array<KIPlayer*>::~Array
elf 0xadf06 body 22
Sig: undefined __thiscall ~Array(Array<KIPlayer*> * this)

## decompile
```c

/* Array<KIPlayer*>::~Array() */

Array<KIPlayer*> * __thiscall Array<KIPlayer*>::~Array(Array<KIPlayer*> *this)

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
  000bdf06: push {r4,r6,r7,lr}
  000bdf08: add r7,sp,#0x8
  000bdf0a: mov r4,r0
  000bdf0c: ldr r0,[r0,#0x4]
  000bdf0e: cbz r0,0x000bdf14
  000bdf10: blx 0x0006ebfc
  000bdf14: movs r0,#0x0
  000bdf16: str r0,[r4,#0x4]
  000bdf18: mov r0,r4
  000bdf1a: pop {r4,r6,r7,pc}
```
