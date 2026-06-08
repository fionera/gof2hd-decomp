# Array<BluePrint*>::Array
elf 0xada12 body 28
Sig: undefined __thiscall Array(Array<BluePrint*> * this)

## decompile
```c

/* Array<BluePrint*>::Array() */

Array<BluePrint*> * __thiscall Array<BluePrint*>::Array(Array<BluePrint*> *this)

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
  000bda12: push {r4,r6,r7,lr}
  000bda14: add r7,sp,#0x8
  000bda16: mov r4,r0
  000bda18: movs r0,#0x4
  000bda1a: blx 0x0006ec08
  000bda1e: movs r2,#0x0
  000bda20: movs r1,#0x1
  000bda22: str r2,[r0,#0x0]
  000bda24: strd r2,r0,[r4,#0x0]
  000bda28: mov r0,r4
  000bda2a: str r1,[r4,#0x8]
  000bda2c: pop {r4,r6,r7,pc}
```
