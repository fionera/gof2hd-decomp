# Array<Agent*>::Array
elf 0x96e3c body 28
Sig: undefined __thiscall Array(Array<Agent*> * this)

## decompile
```c

/* Array<Agent*>::Array() */

Array<Agent*> * __thiscall Array<Agent*>::Array(Array<Agent*> *this)

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
  000a6e3c: push {r4,r6,r7,lr}
  000a6e3e: add r7,sp,#0x8
  000a6e40: mov r4,r0
  000a6e42: movs r0,#0x4
  000a6e44: blx 0x0006ec08
  000a6e48: movs r2,#0x0
  000a6e4a: movs r1,#0x1
  000a6e4c: str r2,[r0,#0x0]
  000a6e4e: strd r2,r0,[r4,#0x0]
  000a6e52: mov r0,r4
  000a6e54: str r1,[r4,#0x8]
  000a6e56: pop {r4,r6,r7,pc}
```
