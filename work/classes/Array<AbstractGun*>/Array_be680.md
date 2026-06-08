# Array<AbstractGun*>::Array
elf 0xbe680 body 28
Sig: undefined __thiscall Array(Array<AbstractGun*> * this)

## decompile
```c

/* Array<AbstractGun*>::Array() */

Array<AbstractGun*> * __thiscall Array<AbstractGun*>::Array(Array<AbstractGun*> *this)

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
  000ce680: push {r4,r6,r7,lr}
  000ce682: add r7,sp,#0x8
  000ce684: mov r4,r0
  000ce686: movs r0,#0x4
  000ce688: blx 0x0006ec08
  000ce68c: movs r2,#0x0
  000ce68e: movs r1,#0x1
  000ce690: str r2,[r0,#0x0]
  000ce692: strd r2,r0,[r4,#0x0]
  000ce696: mov r0,r4
  000ce698: str r1,[r4,#0x8]
  000ce69a: pop {r4,r6,r7,pc}
```
