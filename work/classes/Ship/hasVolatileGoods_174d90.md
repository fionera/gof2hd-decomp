# Ship::hasVolatileGoods
elf 0x174d90 body 36
Sig: undefined __thiscall hasVolatileGoods(Ship * this)

## decompile
```c

/* Ship::hasVolatileGoods() */

undefined4 __thiscall Ship::hasVolatileGoods(Ship *this)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = hasCargo((int)this,0xd1);
  if (iVar1 == 0) {
    uVar2 = (*(code *)(DAT_001ac724 + 0x1ac728))(this,0xcc,1);
    return uVar2;
  }
  return 1;
}

```
## target disasm
```
  00184d90: push {r4,r6,r7,lr}
  00184d92: add r7,sp,#0x8
  00184d94: movs r1,#0xd1
  00184d96: movs r2,#0x1
  00184d98: mov r4,r0
  00184d9a: blx 0x00071a70
  00184d9e: cbz r0,0x00184da4
  00184da0: movs r0,#0x1
  00184da2: pop {r4,r6,r7,pc}
  00184da4: mov r0,r4
  00184da6: movs r1,#0xcc
  00184da8: movs r2,#0x1
  00184daa: pop.w {r4,r6,r7,lr}
  00184dae: b.w 0x001ac718
  001ac718: bx pc
```
