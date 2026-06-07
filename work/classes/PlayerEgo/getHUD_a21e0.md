# PlayerEgo::getHUD
elf 0xa21e0 body 6
Sig: undefined __thiscall getHUD(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getHUD() */

int * __thiscall PlayerEgo::getHUD(PlayerEgo *this)

{
  return this[0xf].field_4;
}

```

## target disasm
```
  000b21e0: ldr.w r0,[r0,#0x220]
  000b21e4: bx lr
```
