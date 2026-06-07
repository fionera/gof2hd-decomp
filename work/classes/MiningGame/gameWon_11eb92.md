# MiningGame::gameWon
elf 0x11eb92 body 6
Sig: undefined __thiscall gameWon(MiningGame * this)

## decompile
```c

/* MiningGame::gameWon() */

MiningGame __thiscall MiningGame::gameWon(MiningGame *this)

{
  return this[0x81];
}

```

## target disasm
```
  0012eb92: ldrb.w r0,[r0,#0x81]
  0012eb96: bx lr
```
