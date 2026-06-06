# Player::setEnemy
elf 0xa2b76 body 58
Sig: undefined __thiscall setEnemy(Player * this, Player * param_1)

## decompile
```c

/* Player::setEnemy(Player*) */

void __thiscall Player::setEnemy(Player *this,Player *param_1)

{
  Array<Player*> *this_00;
  
  this_00 = operator_new(0xc);
  Array<Player*>::Array(this_00);
  ArrayAdd<Player*>(param_1,(Array *)this_00);
  setEnemies(this,(Array *)this_00);
  Array<Player*>::~Array(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```
## target disasm
```
  000b2b76: push {r4,r5,r6,r7,lr}
  000b2b78: add r7,sp,#0xc
  000b2b7a: push.w r11
  000b2b7e: mov r5,r0
  000b2b80: movs r0,#0xc
  000b2b82: mov r6,r1
  000b2b84: blx 0x0006eb24
  000b2b88: mov r4,r0
  000b2b8a: blx 0x00072e74
  000b2b8e: mov r0,r6
  000b2b90: mov r1,r4
  000b2b92: blx 0x00072ea4
  000b2b96: mov r0,r5
  000b2b98: mov r1,r4
  000b2b9a: blx 0x00072eb0
  000b2b9e: mov r0,r4
  000b2ba0: blx 0x00072e5c
  000b2ba4: pop.w r11
  000b2ba8: pop.w {r4,r5,r6,r7,lr}
  000b2bac: b.w 0x001ab098
```
