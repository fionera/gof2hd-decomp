# Player::getArmorHP
elf 0xa2dd6 body 6
Sig: undefined __stdcall getArmorHP(void)

## decompile
```c

/* Player::getArmorHP() */

undefined4 Player::getArmorHP(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x8c);
}

```
## target disasm
```
  000b2dd6: ldr.w r0,[r0,#0x8c]
  000b2dda: bx lr
```
