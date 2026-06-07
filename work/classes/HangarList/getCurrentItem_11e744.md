# HangarList::getCurrentItem
elf 0x11e744 body 8
Sig: undefined __thiscall getCurrentItem(HangarList * this)

## decompile
```c

/* HangarList::getCurrentItem() */

void __thiscall HangarList::getCurrentItem(HangarList *this)

{
  (*(code *)(DAT_001ac404 + 0x1ac408))(this,*(undefined4 *)(this + 8));
  return;
}

```

## target disasm
```
  0012e744: ldr r1,[r0,#0x8]
  0012e746: b.w 0x001ac3f8
  001ac3f8: bx pc
```
