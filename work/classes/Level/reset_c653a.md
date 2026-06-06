# Level::reset
elf 0xc653a body 256
Sig: undefined __thiscall reset(Level * this)

## decompile
```c

/* Level::reset() */

void __thiscall Level::reset(Level *this)

{
  uint *puVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  
  if (*(int *)(this + 0x108) != 0) {
    Route::reset();
  }
  if (*(int *)(this + 0x110) != 0) {
    Route::reset();
  }
  if (*(int *)(this + 0x10c) != 0) {
    Route::reset();
  }
  puVar1 = *(uint **)(this + 0xf8);
  if (puVar1 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      KIPlayer::reset(*(KIPlayer **)(puVar1[1] + uVar4 * 4));
      puVar1 = *(uint **)(this + 0xf8);
    }
  }
  createPlayer();
  assignGuns(this);
  connectPlayers();
  puVar1 = *(uint **)(this + 0x114);
  if (puVar1 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      RadioMessage::reset(*(RadioMessage **)(puVar1[1] + uVar4 * 4));
      puVar1 = *(uint **)(this + 0x114);
    }
  }
  PlayerEgo::setRoute(*(Route **)(this + 0xf0));
  puVar1 = *(uint **)(this + 0xf8);
  if (puVar1 != (uint *)0x0) {
    iVar5 = 0;
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      iVar2 = *(int *)(puVar1[1] + uVar4 * 4);
      if (((*(char *)(iVar2 + 0x41) == '\0') && (*(char *)(iVar2 + 0x71) == '\0')) &&
         (*(char *)(iVar2 + 0x3f) == '\0')) {
        iVar2 = KIPlayer::isWingMan();
        puVar1 = *(uint **)(this + 0xf8);
        if (((iVar2 == 0) &&
            (iVar2 = *(int *)(puVar1[1] + uVar4 * 4), *(char *)(iVar2 + 0x44) == '\0')) &&
           (*(char *)(iVar2 + 0x3c) == '\0')) {
          iVar5 = iVar5 + (*(byte *)(iVar2 + 0x3d) ^ 1);
        }
      }
    }
    if (puVar1 != (uint *)0x0) {
      iVar5 = iVar5 - *(int *)(this + 0x120);
      goto LAB_000d661e;
    }
  }
  iVar5 = 0;
LAB_000d661e:
  *(int *)(this + 0x118) = iVar5;
  if (*(undefined4 **)(this + 0xfc) == (undefined4 *)0x0) {
    uVar3 = 0;
  }
  else {
    uVar3 = **(undefined4 **)(this + 0xfc);
  }
  *(undefined4 *)(this + 0x128) = uVar3;
  *(undefined4 *)(this + 300) = 0;
  return;
}

```
## target disasm
```
  000d653a: push {r4,r5,r6,r7,lr}
  000d653c: add r7,sp,#0xc
  000d653e: push.w r11
  000d6542: mov r4,r0
  000d6544: ldr.w r0,[r0,#0x108]
  000d6548: cbz r0,0x000d654e
  000d654a: blx 0x000732e8
  000d654e: ldr.w r0,[r4,#0x110]
  000d6552: cbz r0,0x000d6558
  000d6554: blx 0x000732e8
  000d6558: ldr.w r0,[r4,#0x10c]
  000d655c: cbz r0,0x000d6562
  000d655e: blx 0x000732e8
  000d6562: ldr.w r0,[r4,#0xf8]
  000d6566: cbz r0,0x000d6582
  000d6568: movs r5,#0x0
  000d656a: b 0x000d657c
  000d656c: ldr r0,[r0,#0x4]
  000d656e: ldr.w r0,[r0,r5,lsl #0x2]
  000d6572: blx 0x00074518
  000d6576: ldr.w r0,[r4,#0xf8]
  000d657a: adds r5,#0x1
  000d657c: ldr r1,[r0,#0x0]
  000d657e: cmp r5,r1
  000d6580: bcc 0x000d656c
  000d6582: mov r0,r4
  000d6584: blx 0x00073c00
  000d6588: mov r0,r4
  000d658a: blx 0x00073ca8
  000d658e: mov r0,r4
  000d6590: blx 0x00073cb4
  000d6594: ldr.w r0,[r4,#0x114]
  000d6598: cbz r0,0x000d65b4
  000d659a: movs r5,#0x0
  000d659c: b 0x000d65ae
  000d659e: ldr r0,[r0,#0x4]
  000d65a0: ldr.w r0,[r0,r5,lsl #0x2]
  000d65a4: blx 0x00074524
  000d65a8: ldr.w r0,[r4,#0x114]
  000d65ac: adds r5,#0x1
  000d65ae: ldr r1,[r0,#0x0]
  000d65b0: cmp r5,r1
  000d65b2: bcc 0x000d659e
  000d65b4: ldr.w r0,[r4,#0xf0]
  000d65b8: ldr.w r1,[r4,#0x108]
  000d65bc: blx 0x00073cc0
  000d65c0: ldr.w r1,[r4,#0xf8]
  000d65c4: cbz r1,0x000d661c
  000d65c6: movs r5,#0x0
  000d65c8: movs r6,#0x0
  000d65ca: b 0x000d660c
  000d65cc: ldr r0,[r1,#0x4]
  000d65ce: ldr.w r0,[r0,r6,lsl #0x2]
  000d65d2: ldrb.w r2,[r0,#0x41]
  000d65d6: cbnz r2,0x000d660a
  000d65d8: ldrb.w r2,[r0,#0x71]
  000d65dc: cbnz r2,0x000d660a
  000d65de: ldrb.w r2,[r0,#0x3f]
  000d65e2: cbnz r2,0x000d660a
  000d65e4: blx 0x00072ed4
  000d65e8: ldr.w r1,[r4,#0xf8]
  000d65ec: cbnz r0,0x000d660a
  000d65ee: ldr r0,[r1,#0x4]
  000d65f0: ldr.w r0,[r0,r6,lsl #0x2]
  000d65f4: ldrb.w r2,[r0,#0x44]
  000d65f8: cbnz r2,0x000d660a
  000d65fa: ldrb.w r2,[r0,#0x3c]
  000d65fe: cbnz r2,0x000d660a
  000d6600: ldrb.w r0,[r0,#0x3d]
  000d6604: eor r0,r0,#0x1
  000d6608: add r5,r0
  000d660a: adds r6,#0x1
  000d660c: ldr r0,[r1,#0x0]
  000d660e: cmp r6,r0
  000d6610: bcc 0x000d65cc
  000d6612: cbz r1,0x000d661c
  000d6614: ldr.w r0,[r4,#0x120]
  000d6618: subs r0,r5,r0
  000d661a: b 0x000d661e
  000d661c: movs r0,#0x0
  000d661e: str.w r0,[r4,#0x118]
  000d6622: movs r0,#0x0
  000d6624: ldr.w r1,[r4,#0xfc]
  000d6628: cmp r1,#0x0
  000d662a: ite ne
  000d662c: ldr.ne r1,[r1,#0x0]
  000d662e: mov.eq r1,#0x0
  000d6630: strd r1,r0,[r4,#0x128]
  000d6634: pop.w r11
  000d6638: pop {r4,r5,r6,r7,pc}
```
