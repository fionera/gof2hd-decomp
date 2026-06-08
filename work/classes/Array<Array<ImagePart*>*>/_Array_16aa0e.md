# Array<Array<ImagePart*>*>::~Array
elf 0x16aa0e body 22
Sig: undefined __thiscall ~Array(Array<Array<ImagePart*>*> * this)

## decompile
```c

/* Array<Array<ImagePart*>*>::~Array() */

Array<Array<ImagePart*>*> * __thiscall
Array<Array<ImagePart*>*>::~Array(Array<Array<ImagePart*>*> *this)

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
  0017aa0e: push {r4,r6,r7,lr}
  0017aa10: add r7,sp,#0x8
  0017aa12: mov r4,r0
  0017aa14: ldr r0,[r0,#0x4]
  0017aa16: cbz r0,0x0017aa1c
  0017aa18: blx 0x0006ebfc
  0017aa1c: movs r0,#0x0
  0017aa1e: str r0,[r4,#0x4]
  0017aa20: mov r0,r4
  0017aa22: pop {r4,r6,r7,pc}
```
