# MiningGame::gameLost
elf 0x11eb98 body 6
Sig: undefined __thiscall gameLost(MiningGame * this)

## decompile
```c

/* MiningGame::gameLost() */

MiningGame __thiscall MiningGame::gameLost(MiningGame *this)

{
  return this[0x82];
}

```

## target disasm
```
  0012eb98: ldrb.w r0,[r0,#0x82]
  0012eb9c: bx lr
```
