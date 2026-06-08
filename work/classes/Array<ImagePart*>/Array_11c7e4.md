# Array<ImagePart*>::Array
elf 0x11c7e4 body 28
Sig: undefined __thiscall Array(Array<ImagePart*> * this)

## decompile
```c

/* Array<ImagePart*>::Array() */

Array<ImagePart*> * __thiscall Array<ImagePart*>::Array(Array<ImagePart*> *this)

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
  0012c7e4: push {r4,r6,r7,lr}
  0012c7e6: add r7,sp,#0x8
  0012c7e8: mov r4,r0
  0012c7ea: movs r0,#0x4
  0012c7ec: blx 0x0006ec08
  0012c7f0: movs r2,#0x0
  0012c7f2: movs r1,#0x1
  0012c7f4: str r2,[r0,#0x0]
  0012c7f6: strd r2,r0,[r4,#0x0]
  0012c7fa: mov r0,r4
  0012c7fc: str r1,[r4,#0x8]
  0012c7fe: pop {r4,r6,r7,pc}
```
