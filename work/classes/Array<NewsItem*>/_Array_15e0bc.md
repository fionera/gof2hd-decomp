# Array<NewsItem*>::~Array
elf 0x15e0bc body 22
Sig: undefined __thiscall ~Array(Array<NewsItem*> * this)

## decompile
```c

/* Array<NewsItem*>::~Array() */

Array<NewsItem*> * __thiscall Array<NewsItem*>::~Array(Array<NewsItem*> *this)

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
  0016e0bc: push {r4,r6,r7,lr}
  0016e0be: add r7,sp,#0x8
  0016e0c0: mov r4,r0
  0016e0c2: ldr r0,[r0,#0x4]
  0016e0c4: cbz r0,0x0016e0ca
  0016e0c6: blx 0x0006ebfc
  0016e0ca: movs r0,#0x0
  0016e0cc: str r0,[r4,#0x4]
  0016e0ce: mov r0,r4
  0016e0d0: pop {r4,r6,r7,pc}
```
