# PlayerEgo::getHackingGameDockIndex
elf 0xa1142 body 18
Sig: undefined __thiscall getHackingGameDockIndex(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getHackingGameDockIndex() */

undefined4 __thiscall PlayerEgo::getHackingGameDockIndex(PlayerEgo *this)

{
  undefined4 uVar1;
  
  if (this[0xd].field_14 == (int *)0x0) {
    return 0xffffffff;
  }
  uVar1 = (*(code *)(DAT_001abb64 + 0x1abb68))();
  return uVar1;
}

```

## target disasm
```
  000b1142: ldr.w r0,[r0,#0x1e8]
  000b1146: cbz r0,0x000b114c
  000b1148: b.w 0x001abb58
  000b114c: mov.w r0,#0xffffffff
  000b1150: bx lr
  001abb58: bx pc
```
