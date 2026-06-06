# Player::addEnemy
elf 0xa2c6a body 92
Sig: undefined __thiscall addEnemy(Player * this, Player * param_1)

## decompile
```c

/* Player::addEnemy(Player*) */

void __thiscall Player::addEnemy(Player *this,Player *param_1)

{
  Array<Player*> *this_00;
  
  if (*(int *)(this + 0x74) == 0) {
    (*(code *)(DAT_001abcd4 + 0x1abcd8))(this,param_1);
    return;
  }
  this_00 = operator_new(0xc);
  Array<Player*>::Array(this_00);
  if (*(int *)*(Array **)(this + 0x74) != 0) {
    ArrayAdd<Player*>(*(Array **)(this + 0x74),(Array *)this_00);
  }
  ArrayAdd<Player*>(param_1,(Array *)this_00);
  setEnemies(this,(Array *)this_00);
  Array<Player*>::~Array(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```
## target disasm
```
  000b2c6a: push {r4,r5,r6,r7,lr}
  000b2c6c: add r7,sp,#0xc
  000b2c6e: push.w r11
  000b2c72: mov r5,r0
  000b2c74: ldr r0,[r0,#0x74]
  000b2c76: mov r6,r1
  000b2c78: cbz r0,0x000b2cb4
  000b2c7a: movs r0,#0xc
  000b2c7c: blx 0x0006eb24
  000b2c80: mov r4,r0
  000b2c82: blx 0x00072e74
  000b2c86: ldr r0,[r5,#0x74]
  000b2c88: ldr r1,[r0,#0x0]
  000b2c8a: cbz r1,0x000b2c92
  000b2c8c: mov r1,r4
  000b2c8e: blx 0x00072e80
  000b2c92: mov r0,r6
  000b2c94: mov r1,r4
  000b2c96: blx 0x00072ea4
  000b2c9a: mov r0,r5
  000b2c9c: mov r1,r4
  000b2c9e: blx 0x00072eb0
  000b2ca2: mov r0,r4
  000b2ca4: blx 0x00072e5c
  000b2ca8: pop.w r11
  000b2cac: pop.w {r4,r5,r6,r7,lr}
  000b2cb0: b.w 0x001ab098
  000b2cb4: mov r0,r5
  000b2cb6: mov r1,r6
  000b2cb8: pop.w r11
  000b2cbc: pop.w {r4,r5,r6,r7,lr}
  000b2cc0: b.w 0x001abcc8
  001abcc8: bx pc
```
