# Level::uncoverWanted
elf 0xc6320 body 100
Sig: undefined __stdcall uncoverWanted(int param_1)

## decompile
```c

/* Level::uncoverWanted(int) */

void Level::uncoverWanted(int param_1)

{
  int iVar1;
  int in_r1;
  undefined4 *puVar2;
  int iVar3;
  
  if (*(char *)(param_1 + 0x29c) == '\0') {
    createRadioMessage(param_1,0x12);
    puVar2 = *(undefined4 **)(DAT_000d6384 + 0xd6344);
    for (iVar3 = 1;
        iVar1 = Wanted::getNumWingmen(*(Wanted **)(*(int *)(*(int *)*puVar2 + 4) + in_r1 * 4)),
        iVar3 + -1 < iVar1; iVar3 = iVar3 + 1) {
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(param_1 + 0xf8) + 4) + iVar3 * 4) + 4),0));
      Player::turnEnemy();
    }
  }
  return;
}

```
## target disasm
```
  000d6320: push {r4,r5,r6,r7,lr}
  000d6322: add r7,sp,#0xc
  000d6324: push.w r8
  000d6328: mov r5,r0
  000d632a: ldrb.w r0,[r0,#0x29c]
  000d632e: mov r8,r1
  000d6330: cbnz r0,0x000d637e
  000d6332: mov r0,r5
  000d6334: movs r1,#0x12
  000d6336: mov r2,r8
  000d6338: blx 0x00072754
  000d633c: ldr r0,[0x000d6384]
  000d633e: movs r6,#0x1
  000d6340: add r0,pc
  000d6342: ldr r4,[r0,#0x0]
  000d6344: b 0x000d636a
  000d6346: ldr.w r0,[r5,#0xf8]
  000d634a: movs r1,#0x1
  000d634c: ldr r0,[r0,#0x4]
  000d634e: ldr.w r0,[r0,r6,lsl #0x2]
  000d6352: ldr r0,[r0,#0x4]
  000d6354: blx 0x00073e94
  000d6358: ldr.w r0,[r5,#0xf8]
  000d635c: ldr r0,[r0,#0x4]
  000d635e: ldr.w r0,[r0,r6,lsl #0x2]
  000d6362: ldr r0,[r0,#0x4]
  000d6364: blx 0x000744e8
  000d6368: adds r6,#0x1
  000d636a: ldr r0,[r4,#0x0]
  000d636c: ldr r0,[r0,#0x0]
  000d636e: ldr r0,[r0,#0x4]
  000d6370: ldr.w r0,[r0,r8,lsl #0x2]
  000d6374: blx 0x0007402c
  000d6378: subs r1,r6,#0x1
  000d637a: cmp r1,r0
  000d637c: blt 0x000d6346
  000d637e: pop.w r8
  000d6382: pop {r4,r5,r6,r7,pc}
```
