# Array<KIPlayer*>::Array
elf 0xbde4e body 28
Sig: undefined __thiscall Array(Array<KIPlayer*> * this)

## decompile
```c

/* Array<KIPlayer*>::Array() */

Array<KIPlayer*> * __thiscall Array<KIPlayer*>::Array(Array<KIPlayer*> *this)

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
  000cde4e: push {r4,r6,r7,lr}
  000cde50: add r7,sp,#0x8
  000cde52: mov r4,r0
  000cde54: movs r0,#0x4
  000cde56: blx 0x0006ec08
  000cde5a: movs r2,#0x0
  000cde5c: movs r1,#0x1
  000cde5e: str r2,[r0,#0x0]
  000cde60: strd r2,r0,[r4,#0x0]
  000cde64: mov r0,r4
  000cde66: str r1,[r4,#0x8]
  000cde68: pop {r4,r6,r7,pc}
```
