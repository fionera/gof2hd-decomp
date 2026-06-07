# HangarList::getCurrentTabItems
elf 0x11e772 body 12
Sig: undefined __thiscall getCurrentTabItems(HangarList * this)

## decompile
```c

/* HangarList::getCurrentTabItems() */

undefined4 __thiscall HangarList::getCurrentTabItems(HangarList *this)

{
  return *(undefined4 *)(*(int *)(*(int *)this + 4) + *(int *)(this + 4) * 4);
}

```

## target disasm
```
  0012e772: ldrd r1,r0,[r0,#0x0]
  0012e776: ldr r1,[r1,#0x4]
  0012e778: ldr.w r0,[r1,r0,lsl #0x2]
  0012e77c: bx lr
```
