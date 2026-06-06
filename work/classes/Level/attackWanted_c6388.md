# Level::attackWanted
elf 0xc6388 body 104
Sig: undefined __thiscall attackWanted(Level * this, int param_1)

## decompile
```c

/* Level::attackWanted(int) */

void __thiscall Level::attackWanted(Level *this,int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  
  if (this[0x29c] == (Level)0x0) {
    this[0x29c] = (Level)0x1;
    createRadioMessage((int)this,0x10);
    puVar2 = *(undefined4 **)(DAT_000d63f0 + 0xd63b0);
    for (iVar3 = 1;
        iVar1 = Wanted::getNumWingmen(*(Wanted **)(*(int *)(*(int *)*puVar2 + 4) + param_1 * 4)),
        iVar3 + -1 < iVar1; iVar3 = iVar3 + 1) {
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar3 * 4) + 4),0));
      Player::turnEnemy();
    }
  }
  return;
}

```
## target disasm
```
  000d6388: push {r4,r5,r6,r7,lr}
  000d638a: add r7,sp,#0xc
  000d638c: push.w r8
  000d6390: mov r5,r0
  000d6392: ldrb.w r0,[r0,#0x29c]
  000d6396: mov r8,r1
  000d6398: cbnz r0,0x000d63ea
  000d639a: movs r6,#0x1
  000d639c: mov r0,r5
  000d639e: movs r1,#0x10
  000d63a0: mov r2,r8
  000d63a2: strb.w r6,[r5,#0x29c]
  000d63a6: blx 0x00072754
  000d63aa: ldr r0,[0x000d63f0]
  000d63ac: add r0,pc
  000d63ae: ldr r4,[r0,#0x0]
  000d63b0: b 0x000d63d6
  000d63b2: ldr.w r0,[r5,#0xf8]
  000d63b6: movs r1,#0x1
  000d63b8: ldr r0,[r0,#0x4]
  000d63ba: ldr.w r0,[r0,r6,lsl #0x2]
  000d63be: ldr r0,[r0,#0x4]
  000d63c0: blx 0x00073e94
  000d63c4: ldr.w r0,[r5,#0xf8]
  000d63c8: ldr r0,[r0,#0x4]
  000d63ca: ldr.w r0,[r0,r6,lsl #0x2]
  000d63ce: ldr r0,[r0,#0x4]
  000d63d0: blx 0x000744e8
  000d63d4: adds r6,#0x1
  000d63d6: ldr r0,[r4,#0x0]
  000d63d8: ldr r0,[r0,#0x0]
  000d63da: ldr r0,[r0,#0x4]
  000d63dc: ldr.w r0,[r0,r8,lsl #0x2]
  000d63e0: blx 0x0007402c
  000d63e4: subs r1,r6,#0x1
  000d63e6: cmp r1,r0
  000d63e8: blt 0x000d63b2
  000d63ea: pop.w r8
  000d63ee: pop {r4,r5,r6,r7,pc}
```
