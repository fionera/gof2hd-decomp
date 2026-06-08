# Array<Gun*>::Array
elf 0xa2808 body 28
Sig: undefined __thiscall Array(Array<Gun*> * this)

## decompile
```c

/* Array<Gun*>::Array() */

Array<Gun*> * __thiscall Array<Gun*>::Array(Array<Gun*> *this)

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
  000b2808: push {r4,r6,r7,lr}
  000b280a: add r7,sp,#0x8
  000b280c: mov r4,r0
  000b280e: movs r0,#0x4
  000b2810: blx 0x0006ec08
  000b2814: movs r2,#0x0
  000b2816: movs r1,#0x1
  000b2818: str r2,[r0,#0x0]
  000b281a: strd r2,r0,[r4,#0x0]
  000b281e: mov r0,r4
  000b2820: str r1,[r4,#0x8]
  000b2822: pop {r4,r6,r7,pc}
```
