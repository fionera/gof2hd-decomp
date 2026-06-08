# Array<float>::Array
elf 0xa7250 body 28
Sig: undefined __thiscall Array(Array<float> * this)

## decompile
```c

/* Array<float>::Array() */

Array<float> * __thiscall Array<float>::Array(Array<float> *this)

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
  000b7250: push {r4,r6,r7,lr}
  000b7252: add r7,sp,#0x8
  000b7254: mov r4,r0
  000b7256: movs r0,#0x4
  000b7258: blx 0x0006ec08
  000b725c: movs r2,#0x0
  000b725e: movs r1,#0x1
  000b7260: str r2,[r0,#0x0]
  000b7262: strd r2,r0,[r4,#0x0]
  000b7266: mov r0,r4
  000b7268: str r1,[r4,#0x8]
  000b726a: pop {r4,r6,r7,pc}
```
