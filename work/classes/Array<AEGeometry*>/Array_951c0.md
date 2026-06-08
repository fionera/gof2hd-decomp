# Array<AEGeometry*>::Array
elf 0x951c0 body 28
Sig: undefined __thiscall Array(Array<AEGeometry*> * this)

## decompile
```c

/* Array<AEGeometry*>::Array() */

Array<AEGeometry*> * __thiscall Array<AEGeometry*>::Array(Array<AEGeometry*> *this)

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
  000a51c0: push {r4,r6,r7,lr}
  000a51c2: add r7,sp,#0x8
  000a51c4: mov r4,r0
  000a51c6: movs r0,#0x4
  000a51c8: blx 0x0006ec08
  000a51cc: movs r2,#0x0
  000a51ce: movs r1,#0x1
  000a51d0: str r2,[r0,#0x0]
  000a51d2: strd r2,r0,[r4,#0x0]
  000a51d6: mov r0,r4
  000a51d8: str r1,[r4,#0x8]
  000a51da: pop {r4,r6,r7,pc}
```
