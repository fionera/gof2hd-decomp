# Array<SpacePoint*>::Array
elf 0x1217d8 body 28
Sig: undefined __thiscall Array(Array<SpacePoint*> * this)

## decompile
```c

/* Array<SpacePoint*>::Array() */

Array<SpacePoint*> * __thiscall Array<SpacePoint*>::Array(Array<SpacePoint*> *this)

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
  001317d8: push {r4,r6,r7,lr}
  001317da: add r7,sp,#0x8
  001317dc: mov r4,r0
  001317de: movs r0,#0x4
  001317e0: blx 0x0006ec08
  001317e4: movs r2,#0x0
  001317e6: movs r1,#0x1
  001317e8: str r2,[r0,#0x0]
  001317ea: strd r2,r0,[r4,#0x0]
  001317ee: mov r0,r4
  001317f0: str r1,[r4,#0x8]
  001317f2: pop {r4,r6,r7,pc}
```
