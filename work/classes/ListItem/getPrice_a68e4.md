# ListItem::getPrice
elf 0xa68e4 body 26
Sig: undefined __thiscall getPrice(ListItem * this)

## decompile
```c

/* ListItem::getPrice() */

undefined4 __thiscall ListItem::getPrice(ListItem *this)

{
  undefined4 uVar1;
  
  if (*(int *)(this + 0xc) != 0) {
    uVar1 = (*(code *)(DAT_001abe44 + 0x1abe48))(*(int *)(this + 0xc));
    return uVar1;
  }
  if (*(int *)(this + 0x10) == 0) {
    return 0;
  }
  uVar1 = (*(code *)(DAT_001abe54 + 0x1abe58))();
  return uVar1;
}

```

## target disasm
```
  000b68e4: ldr r1,[r0,#0xc]
  000b68e6: cbz r1,0x000b68ee
  000b68e8: mov r0,r1
  000b68ea: b.w 0x001abe38
  000b68ee: ldr r0,[r0,#0x10]
  000b68f0: cbz r0,0x000b68f6
  000b68f2: b.w 0x001abe48
  000b68f6: movs r0,#0x0
  000b68f8: bx lr
  001abe38: bx pc
  001abe48: bx pc
```
