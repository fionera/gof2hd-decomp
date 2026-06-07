# Station::setAttackedFriends
elf 0xa6bde body 6
Sig: undefined __thiscall setAttackedFriends(Station * this, bool param_1)

## decompile
```c

/* Station::setAttackedFriends(bool) */

void __thiscall Station::setAttackedFriends(Station *this,bool param_1)

{
  this[0x24] = (Station)param_1;
  return;
}

```

## target disasm
```
  000b6bde: strb.w r1,[r0,#0x24]
  000b6be2: bx lr
```
