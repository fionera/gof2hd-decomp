# Player::isGasCloud
elf 0xa29e2 body 22
Sig: undefined __thiscall isGasCloud(Player * this)

## decompile
```c

/* Player::isGasCloud() */

bool __thiscall Player::isGasCloud(Player *this)

{
  if (*(int *)(this + 0xd0) != 0) {
    return *(char *)(*(int *)(this + 0xd0) + 0x44) != '\0';
  }
  return false;
}

```
## target disasm
```
  000b29e2: ldr.w r0,[r0,#0xd0]
  000b29e6: cbz r0,0x000b29f4
  000b29e8: ldrb.w r0,[r0,#0x44]
  000b29ec: cmp r0,#0x0
  000b29ee: it ne
  000b29f0: mov.ne r0,#0x1
  000b29f2: bx lr
  000b29f4: movs r0,#0x0
  000b29f6: bx lr
```
