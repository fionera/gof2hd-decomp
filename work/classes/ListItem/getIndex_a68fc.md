# ListItem::getIndex
elf 0xa68fc body 48
Sig: undefined __thiscall getIndex(ListItem * this)

## decompile
```c

/* ListItem::getIndex() */

undefined4 __thiscall ListItem::getIndex(ListItem *this)

{
  undefined4 uVar1;
  
  if (*(int *)(this + 0xc) != 0) {
    uVar1 = (*(code *)(DAT_001abe64 + 0x1abe68))(*(int *)(this + 0xc));
    return uVar1;
  }
  if (*(int *)(this + 0x10) != 0) {
    uVar1 = (*(code *)(DAT_001abe74 + 0x1abe78))(*(int *)(this + 0x10));
    return uVar1;
  }
  if (*(int *)(this + 8) != 0) {
    uVar1 = (*(code *)(DAT_001abe84 + 0x1abe88))(*(int *)(this + 8));
    return uVar1;
  }
  uVar1 = DAT_000b6928;
  if (*(int *)(this + 0x18) != 0) {
    uVar1 = *(undefined4 *)(*(int *)(this + 0x18) + 0x14);
  }
  return uVar1;
}

```

## target disasm
```
  000b68fc: ldr r1,[r0,#0xc]
  000b68fe: cbz r1,0x000b6906
  000b6900: mov r0,r1
  000b6902: b.w 0x001abe58
  000b6906: ldr r1,[r0,#0x10]
  000b6908: cbz r1,0x000b6910
  000b690a: mov r0,r1
  000b690c: b.w 0x001abe68
  000b6910: ldr r1,[r0,#0x8]
  000b6912: cbz r1,0x000b691a
  000b6914: mov r0,r1
  000b6916: b.w 0x001abe78
  000b691a: ldr r0,[r0,#0x18]
  000b691c: cmp r0,#0x0
  000b691e: ite eq
  000b6920: ldr.eq r0,[0x000b6928]
  000b6922: ldr.ne r0,[r0,#0x14]
  000b6924: bx lr
  001abe58: bx pc
  001abe68: bx pc
  001abe78: bx pc
```
