# HangarList::HangarList
elf 0x11d9f4 body 10
Sig: undefined __thiscall HangarList(HangarList * this)

## decompile
```c

/* HangarList::HangarList() */

void __thiscall HangarList::HangarList(HangarList *this)

{
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  return;
}

```

## target disasm
```
  0012d9f4: movs r1,#0x0
  0012d9f6: strd r1,r1,[r0,#0x0]
  0012d9fa: str r1,[r0,#0x8]
  0012d9fc: bx lr
```
