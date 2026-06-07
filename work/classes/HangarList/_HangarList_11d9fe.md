# HangarList::~HangarList
elf 0x11d9fe body 14
Sig: undefined __thiscall ~HangarList(HangarList * this)

## decompile
```c

/* HangarList::~HangarList() */

HangarList * __thiscall HangarList::~HangarList(HangarList *this)

{
  release(this);
  return this;
}

```

## target disasm
```
  0012d9fe: push {r4,r6,r7,lr}
  0012da00: add r7,sp,#0x8
  0012da02: mov r4,r0
  0012da04: blx 0x000760fc
  0012da08: mov r0,r4
  0012da0a: pop {r4,r6,r7,pc}
```
