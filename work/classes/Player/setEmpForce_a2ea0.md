# Player::setEmpForce
elf 0xa2ea0 body 6
Sig: undefined __thiscall setEmpForce(Player * this, float param_1)

## decompile
```c

/* Player::setEmpForce(float) */

void __thiscall Player::setEmpForce(Player *this,float param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(this + 0xd8) = in_r1;
  return;
}

```
## target disasm
```
  000b2ea0: str.w r1,[r0,#0xd8]
  000b2ea4: bx lr
```
