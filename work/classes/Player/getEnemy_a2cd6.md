# Player::getEnemy
elf 0xa2cd6 body 10
Sig: undefined __thiscall getEnemy(Player * this, int param_1)

## decompile
```c

/* Player::getEnemy(int) */

undefined4 __thiscall Player::getEnemy(Player *this,int param_1)

{
  return *(undefined4 *)(*(int *)(*(int *)(this + 0x74) + 4) + param_1 * 4);
}

```
## target disasm
```
  000b2cd6: ldr r0,[r0,#0x74]
  000b2cd8: ldr r0,[r0,#0x4]
  000b2cda: ldr.w r0,[r0,r1,lsl #0x2]
  000b2cde: bx lr
```
