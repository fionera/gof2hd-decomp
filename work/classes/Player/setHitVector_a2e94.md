# Player::setHitVector
elf 0xa2e94 body 6
Sig: undefined __thiscall setHitVector(Player * this, float param_1, float param_2, float param_3)

## decompile
```c

/* Player::setHitVector(float, float, float) */

Player * __thiscall Player::setHitVector(Player *this,float param_1,float param_2,float param_3)

{
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  
  *(undefined4 *)(this + 0xc4) = in_r1;
  *(undefined4 *)(this + 200) = in_r2;
  *(undefined4 *)(this + 0xcc) = in_r3;
  return this + 0xd0;
}

```
## target disasm
```
  000b2e94: adds r0,#0xc4
  000b2e96: stmia r0!,{r1,r2,r3}
  000b2e98: bx lr
```
