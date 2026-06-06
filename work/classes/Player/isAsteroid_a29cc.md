# Player::isAsteroid
elf 0xa29cc body 22
Sig: undefined __thiscall isAsteroid(Player * this)

## decompile
```c

/* Player::isAsteroid() */

bool __thiscall Player::isAsteroid(Player *this)

{
  if (*(int *)(this + 0xd0) != 0) {
    return *(char *)(*(int *)(this + 0xd0) + 0x3c) != '\0';
  }
  return false;
}

```
## target disasm
```
  000b29cc: ldr.w r0,[r0,#0xd0]
  000b29d0: cbz r0,0x000b29de
  000b29d2: ldrb.w r0,[r0,#0x3c]
  000b29d6: cmp r0,#0x0
  000b29d8: it ne
  000b29da: mov.ne r0,#0x1
  000b29dc: bx lr
  000b29de: movs r0,#0x0
  000b29e0: bx lr
```
