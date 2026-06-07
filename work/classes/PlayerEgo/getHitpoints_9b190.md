# PlayerEgo::getHitpoints
elf 0x9b190 body 8
Sig: undefined __stdcall getHitpoints(void)

## decompile
```c

/* PlayerEgo::getHitpoints() */

void PlayerEgo::getHitpoints(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001aba94 + 0x1aba98))(*in_r0);
  return;
}

```

## target disasm
```
  000ab190: ldr r0,[r0,#0x0]
  000ab192: b.w 0x001aba88
  001aba88: bx pc
```
