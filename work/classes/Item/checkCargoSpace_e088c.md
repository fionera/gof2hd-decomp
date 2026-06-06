# Item::checkCargoSpace
elf 0xe088c body 50
Sig: undefined __thiscall checkCargoSpace(Item * this)

## decompile
```c

/* Item::checkCargoSpace() */

bool __thiscall Item::checkCargoSpace(Item *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  Status::getShip();
  iVar1 = Ship::getCurrentLoad();
  iVar3 = *(int *)(this + 0x34);
  Status::getShip();
  iVar2 = Ship::getMaxLoad();
  return iVar3 + iVar1 <= iVar2;
}

```
## target disasm
```
  000f088c: push {r4,r5,r7,lr}
  000f088e: add r7,sp,#0x8
  000f0890: mov r4,r0
  000f0892: ldr r0,[0x000f08c0]
  000f0894: add r0,pc
  000f0896: ldr r5,[r0,#0x0]
  000f0898: ldr r0,[r5,#0x0]
  000f089a: blx 0x00071a58
  000f089e: blx 0x00072bf8
  000f08a2: ldr r1,[r5,#0x0]
  000f08a4: ldr r2,[r4,#0x34]
  000f08a6: adds r4,r2,r0
  000f08a8: mov r0,r1
  000f08aa: blx 0x00071a58
  000f08ae: blx 0x00072c04
  000f08b2: movs r1,#0x0
  000f08b4: cmp r4,r0
  000f08b6: it le
  000f08b8: mov.le r1,#0x1
  000f08ba: mov r0,r1
  000f08bc: pop {r4,r5,r7,pc}
```
