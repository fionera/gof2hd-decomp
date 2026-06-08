# Array<Array<ImagePart*>*>::Array
elf 0x16aa24 body 28
Sig: undefined __thiscall Array(Array<Array<ImagePart*>*> * this)

## decompile
```c

/* Array<Array<ImagePart*>*>::Array() */

Array<Array<ImagePart*>*> * __thiscall
Array<Array<ImagePart*>*>::Array(Array<Array<ImagePart*>*> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  0017aa24: push {r4,r6,r7,lr}
  0017aa26: add r7,sp,#0x8
  0017aa28: mov r4,r0
  0017aa2a: movs r0,#0x4
  0017aa2c: blx 0x0006ec08
  0017aa30: movs r2,#0x0
  0017aa32: movs r1,#0x1
  0017aa34: str r2,[r0,#0x0]
  0017aa36: strd r2,r0,[r4,#0x0]
  0017aa3a: mov r0,r4
  0017aa3c: str r1,[r4,#0x8]
  0017aa3e: pop {r4,r6,r7,pc}
```
