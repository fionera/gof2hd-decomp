# SpaceLounge::listMode
elf 0x1714ea body 12
Sig: undefined __thiscall listMode(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::listMode() */

bool __thiscall SpaceLounge::listMode(SpaceLounge *this)

{
  return this[0x34] == (SpaceLounge)0x0;
}

```

## target disasm
```
  001814ea: ldrb.w r0,[r0,#0x34]
  001814ee: clz r0,r0
  001814f2: lsrs r0,r0,#0x5
  001814f4: bx lr
```
