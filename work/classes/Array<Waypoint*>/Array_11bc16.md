# Array<Waypoint*>::Array
elf 0x11bc16 body 28
Sig: undefined __thiscall Array(Array<Waypoint*> * this)

## decompile
```c

/* Array<Waypoint*>::Array() */

Array<Waypoint*> * __thiscall Array<Waypoint*>::Array(Array<Waypoint*> *this)

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
  0012bc16: push {r4,r6,r7,lr}
  0012bc18: add r7,sp,#0x8
  0012bc1a: mov r4,r0
  0012bc1c: movs r0,#0x4
  0012bc1e: blx 0x0006ec08
  0012bc22: movs r2,#0x0
  0012bc24: movs r1,#0x1
  0012bc26: str r2,[r0,#0x0]
  0012bc28: strd r2,r0,[r4,#0x0]
  0012bc2c: mov r0,r4
  0012bc2e: str r1,[r4,#0x8]
  0012bc30: pop {r4,r6,r7,pc}
```
