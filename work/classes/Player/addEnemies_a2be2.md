# Player::addEnemies
elf 0xa2be2 body 122
Sig: undefined __thiscall addEnemies(Player * this, Array * param_1)

## decompile
```c

/* Player::addEnemies(Array<Player*>*) */

void __thiscall Player::addEnemies(Player *this,Array *param_1)

{
  Array<Player*> *this_00;
  uint uVar1;
  
  if (*(int *)(this + 0x74) == 0) {
    (*(code *)(DAT_001abcc4 + 0x1abcc8))(this,param_1);
    return;
  }
  this_00 = operator_new(0xc);
  Array<Player*>::Array(this_00);
  for (uVar1 = 0; uVar1 < **(uint **)(this + 0x74); uVar1 = uVar1 + 1) {
    ArrayAdd<Player*>(*(Player **)((*(uint **)(this + 0x74))[1] + uVar1 * 4),(Array *)this_00);
  }
  for (uVar1 = 0; uVar1 < *(uint *)param_1; uVar1 = uVar1 + 1) {
    ArrayAdd<Player*>(*(Player **)(*(int *)(param_1 + 4) + uVar1 * 4),(Array *)this_00);
  }
  setEnemies(this,(Array *)this_00);
  Array<Player*>::~Array(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```
## target disasm
```
  000b2be2: push {r4,r5,r6,r7,lr}
  000b2be4: add r7,sp,#0xc
  000b2be6: push.w r8
  000b2bea: mov r8,r0
  000b2bec: ldr r0,[r0,#0x74]
  000b2bee: mov r6,r1
  000b2bf0: cbz r0,0x000b2c4c
  000b2bf2: movs r0,#0xc
  000b2bf4: blx 0x0006eb24
  000b2bf8: mov r4,r0
  000b2bfa: blx 0x00072e74
  000b2bfe: movs r5,#0x0
  000b2c00: b 0x000b2c10
  000b2c02: ldr r0,[r0,#0x4]
  000b2c04: mov r1,r4
  000b2c06: ldr.w r0,[r0,r5,lsl #0x2]
  000b2c0a: blx 0x00072ea4
  000b2c0e: adds r5,#0x1
  000b2c10: ldr.w r0,[r8,#0x74]
  000b2c14: ldr r1,[r0,#0x0]
  000b2c16: cmp r5,r1
  000b2c18: bcc 0x000b2c02
  000b2c1a: movs r5,#0x0
  000b2c1c: b 0x000b2c2c
  000b2c1e: ldr r0,[r6,#0x4]
  000b2c20: mov r1,r4
  000b2c22: ldr.w r0,[r0,r5,lsl #0x2]
  000b2c26: blx 0x00072ea4
  000b2c2a: adds r5,#0x1
  000b2c2c: ldr r0,[r6,#0x0]
  000b2c2e: cmp r5,r0
  000b2c30: bcc 0x000b2c1e
  000b2c32: mov r0,r8
  000b2c34: mov r1,r4
  000b2c36: blx 0x00072eb0
  000b2c3a: mov r0,r4
  000b2c3c: blx 0x00072e5c
  000b2c40: pop.w r8
  000b2c44: pop.w {r4,r5,r6,r7,lr}
  000b2c48: b.w 0x001ab098
  000b2c4c: mov r0,r8
  000b2c4e: mov r1,r6
  000b2c50: pop.w r8
  000b2c54: pop.w {r4,r5,r6,r7,lr}
  000b2c58: b.w 0x001abcb8
```
