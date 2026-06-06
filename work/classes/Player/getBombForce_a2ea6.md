# Player::getBombForce
elf 0xa2ea6 body 6
Sig: undefined __stdcall getBombForce(void)

## decompile
```c

/* Player::getBombForce() */

undefined4 Player::getBombForce(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xd4);
}

```
## target disasm
```
  000b2ea6: ldr.w r0,[r0,#0xd4]
  000b2eaa: bx lr
```
