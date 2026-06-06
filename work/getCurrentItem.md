# HangarList::getCurrentItem

ELF vaddr 0x11e744, body 8 bytes
Signature: undefined __thiscall getCurrentItem(HangarList * this)

## Ghidra decompile
```c

/* HangarList::getCurrentItem() */

void __thiscall HangarList::getCurrentItem(HangarList *this)

{
  (*(code *)(DAT_001ac404 + 0x1ac408))(this,*(undefined4 *)(this + 8));
  return;
}

```

## Target disassembly (must match)
```
  0012e744: ldr r1,[r0,#0x8]
  0012e746: b.w 0x001ac3f8
  001ac3f8: bx pc
```

Verify: tools/try.sh work/src/getCurrentItem.cpp getCurrentItem 11e744 32
