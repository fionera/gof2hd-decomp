# HackingGame::getRewardAmount
elf 0x14ef48 body 6
Sig: undefined __thiscall getRewardAmount(HackingGame * this)

## decompile
```c

/* HackingGame::getRewardAmount() */

undefined4 __thiscall HackingGame::getRewardAmount(HackingGame *this)

{
  return *(undefined4 *)(this + 0x138);
}

```

## target disasm
```
  0015ef48: ldr.w r0,[r0,#0x138]
  0015ef4c: bx lr
```
