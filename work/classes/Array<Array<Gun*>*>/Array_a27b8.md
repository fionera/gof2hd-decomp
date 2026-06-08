# Array<Array<Gun*>*>::Array
elf 0xa27b8 body 28
Sig: undefined __thiscall Array(Array<Array<Gun*>*> * this)

## decompile
```c

/* Array<Array<Gun*>*>::Array() */

Array<Array<Gun*>*> * __thiscall Array<Array<Gun*>*>::Array(Array<Array<Gun*>*> *this)

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
  000b27b8: push {r4,r6,r7,lr}
  000b27ba: add r7,sp,#0x8
  000b27bc: mov r4,r0
  000b27be: movs r0,#0x4
  000b27c0: blx 0x0006ec08
  000b27c4: movs r2,#0x0
  000b27c6: movs r1,#0x1
  000b27c8: str r2,[r0,#0x0]
  000b27ca: strd r2,r0,[r4,#0x0]
  000b27ce: mov r0,r4
  000b27d0: str r1,[r4,#0x8]
  000b27d2: pop {r4,r6,r7,pc}
```
