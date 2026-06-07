# HackingGame::getRewardItem
elf 0x14ef42 body 6
Sig: undefined __thiscall getRewardItem(HackingGame * this)

## decompile
```c

/* HackingGame::getRewardItem() */

undefined4 __thiscall HackingGame::getRewardItem(HackingGame *this)

{
  return *(undefined4 *)(this + 0x134);
}

```

## target disasm
```
  0015ef42: ldr.w r0,[r0,#0x134]
  0015ef46: bx lr
```
