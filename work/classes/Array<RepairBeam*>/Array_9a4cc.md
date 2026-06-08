# Array<RepairBeam*>::Array
elf 0x9a4cc body 28
Sig: undefined __thiscall Array(Array<RepairBeam*> * this)

## decompile
```c

/* Array<RepairBeam*>::Array() */

Array<RepairBeam*> * __thiscall Array<RepairBeam*>::Array(Array<RepairBeam*> *this)

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
  000aa4cc: push {r4,r6,r7,lr}
  000aa4ce: add r7,sp,#0x8
  000aa4d0: mov r4,r0
  000aa4d2: movs r0,#0x4
  000aa4d4: blx 0x0006ec08
  000aa4d8: movs r2,#0x0
  000aa4da: movs r1,#0x1
  000aa4dc: str r2,[r0,#0x0]
  000aa4de: strd r2,r0,[r4,#0x0]
  000aa4e2: mov r0,r4
  000aa4e4: str r1,[r4,#0x8]
  000aa4e6: pop {r4,r6,r7,pc}
```
