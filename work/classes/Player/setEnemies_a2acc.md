# Player::setEnemies
elf 0xa2acc body 118
Sig: undefined __thiscall setEnemies(Player * this, Array * param_1)

## decompile
```c

/* Player::setEnemies(Array<Player*>*) */

void __thiscall Player::setEnemies(Player *this,Array *param_1)

{
  void *pvVar1;
  Array<Player*> *this_00;
  Array *pAVar2;
  uint *puVar3;
  uint *puVar4;
  uint uVar5;
  uint uVar6;
  
  if (*(Array<Player*> **)(this + 0x74) != (Array<Player*> *)0x0) {
    pvVar1 = (void *)Array<Player*>::~Array(*(Array<Player*> **)(this + 0x74));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x74) = 0;
  if (param_1 != (Array *)0x0) {
    this_00 = operator_new(0xc);
    Array<Player*>::Array(this_00);
    *(Array<Player*> **)(this + 0x74) = this_00;
    ArrayAdd<Player*>(param_1,(Array *)this_00);
  }
  puVar3 = *(uint **)this;
  if (puVar3 != (uint *)0x0) {
    for (uVar5 = 0; uVar5 < *puVar3; uVar5 = uVar5 + 1) {
      puVar4 = *(uint **)(puVar3[1] + uVar5 * 4);
      if (puVar4 != (uint *)0x0) {
        for (uVar6 = 0; uVar6 < *puVar4; uVar6 = uVar6 + 1) {
          pAVar2 = *(Array **)(puVar4[1] + uVar6 * 4);
          if (pAVar2 != (Array *)0x0) {
            Gun::setEnemies(pAVar2);
            puVar3 = *(uint **)this;
            puVar4 = *(uint **)(puVar3[1] + uVar5 * 4);
          }
        }
      }
    }
  }
  return;
}

```
## target disasm
```
  000b2acc: push {r4,r5,r6,r7,lr}
  000b2ace: add r7,sp,#0xc
  000b2ad0: push.w r11
  000b2ad4: mov r4,r0
  000b2ad6: ldr r0,[r0,#0x74]
  000b2ad8: mov r5,r1
  000b2ada: cbz r0,0x000b2ae4
  000b2adc: blx 0x00072e5c
  000b2ae0: blx 0x0006eb48
  000b2ae4: movs r0,#0x0
  000b2ae6: cmp r5,#0x0
  000b2ae8: str r0,[r4,#0x74]
  000b2aea: beq 0x000b2b02
  000b2aec: movs r0,#0xc
  000b2aee: blx 0x0006eb24
  000b2af2: mov r6,r0
  000b2af4: blx 0x00072e74
  000b2af8: mov r0,r5
  000b2afa: mov r1,r6
  000b2afc: str r6,[r4,#0x74]
  000b2afe: blx 0x00072e80
  000b2b02: ldr r1,[r4,#0x0]
  000b2b04: cbz r1,0x000b2b3c
  000b2b06: movs r5,#0x0
  000b2b08: b 0x000b2b36
  000b2b0a: ldr r0,[r1,#0x4]
  000b2b0c: ldr.w r2,[r0,r5,lsl #0x2]
  000b2b10: cbz r2,0x000b2b34
  000b2b12: movs r6,#0x0
  000b2b14: b 0x000b2b2e
  000b2b16: ldr r0,[r2,#0x4]
  000b2b18: ldr.w r0,[r0,r6,lsl #0x2]
  000b2b1c: cbz r0,0x000b2b2c
  000b2b1e: ldr r1,[r4,#0x74]
  000b2b20: blx 0x00072e8c
  000b2b24: ldr r1,[r4,#0x0]
  000b2b26: ldr r0,[r1,#0x4]
  000b2b28: ldr.w r2,[r0,r5,lsl #0x2]
  000b2b2c: adds r6,#0x1
  000b2b2e: ldr r0,[r2,#0x0]
  000b2b30: cmp r6,r0
  000b2b32: bcc 0x000b2b16
  000b2b34: adds r5,#0x1
  000b2b36: ldr r0,[r1,#0x0]
  000b2b38: cmp r5,r0
  000b2b3a: bcc 0x000b2b0a
  000b2b3c: pop.w r11
  000b2b40: pop {r4,r5,r6,r7,pc}
```
