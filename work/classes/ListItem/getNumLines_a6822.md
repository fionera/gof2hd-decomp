# ListItem::getNumLines
elf 0xa6822 body 16
Sig: undefined __thiscall getNumLines(ListItem * this)

## decompile
```c

/* ListItem::getNumLines() */

undefined4 __thiscall ListItem::getNumLines(ListItem *this)

{
  undefined4 uVar1;
  
  if (this[0x44] == (ListItem)0x0) {
    uVar1 = 0;
  }
  else {
    uVar1 = **(undefined4 **)this;
  }
  return uVar1;
}

```

## target disasm
```
  000b6822: ldrb.w r1,[r0,#0x44]
  000b6826: cmp r1,#0x0
  000b6828: itee eq
  000b682a: mov.eq r0,#0x0
  000b682c: ldr.ne r0,[r0,#0x0]
  000b682e: ldr.ne r0,[r0,#0x0]
  000b6830: bx lr
```
