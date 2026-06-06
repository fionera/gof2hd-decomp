# Player::getEmpForce
elf 0xa2eac body 6
Sig: undefined __thiscall getEmpForce(Player * this)

## decompile
```c

/* Player::getEmpForce() */

undefined4 __thiscall Player::getEmpForce(Player *this)

{
  return *(undefined4 *)(this + 0xd8);
}

```
## target disasm
```
  000b2eac: ldr.w r0,[r0,#0xd8]
  000b2eb0: bx lr
```
