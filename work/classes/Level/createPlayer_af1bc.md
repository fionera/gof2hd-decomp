# Level::createPlayer
elf 0xaf1bc body 12
Sig: undefined __stdcall createPlayer(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* Level::createPlayer() */

void Level::createPlayer(void)

{
  char in_NG;
  bool in_ZR;
  bool in_CY;
  char in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0x80e92d);
  }
  if (in_CY && !in_ZR) {
    FUN_001fa680();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  000bf1bc: swige 0x3b5f0
  000bf1c0: swieq 0x80e92d
  000bf1c4: blhi 0x001fa680
```
