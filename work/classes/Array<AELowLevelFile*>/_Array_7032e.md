# Array<AELowLevelFile*>::~Array
elf 0x7032e body 22
Sig: undefined __thiscall ~Array(Array<AELowLevelFile*> * this)

## decompile
```c

/* Array<AELowLevelFile*>::~Array() */

Array<AELowLevelFile*> * __thiscall Array<AELowLevelFile*>::~Array(Array<AELowLevelFile*> *this)

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
  0008032e: push {r4,r6,r7,lr}
  00080330: add r7,sp,#0x8
  00080332: mov r4,r0
  00080334: ldr r0,[r0,#0x4]
  00080336: cbz r0,0x0008033c
  00080338: blx 0x0006ebfc
  0008033c: movs r0,#0x0
  0008033e: str r0,[r4,#0x4]
  00080340: mov r0,r4
  00080342: pop {r4,r6,r7,pc}
```
