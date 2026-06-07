# MGame::startChargingJumpDrive
elf 0x179fb0 body 360
Sig: undefined __stdcall startChargingJumpDrive(void)

## decompile
```c

/* MGame::startChargingJumpDrive() */

void MGame::startChargingJumpDrive(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  ChoiceWindow *pCVar3;
  String *pSVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  
  if (*(char *)(in_r0 + 0xdd) != '\0') {
    piVar5 = *(int **)(DAT_0018a128 + 0x189fca);
    iVar1 = Status::getShip();
    iVar7 = 1;
    iVar1 = Ship::hasCargo(iVar1,0x7a);
    if (iVar1 == 0) {
      pCVar3 = *(ChoiceWindow **)(in_r0 + 0x94);
      if (pCVar3 == (ChoiceWindow *)0x0) {
        pCVar3 = operator_new(0x5c);
        ChoiceWindow::ChoiceWindow(pCVar3);
        *(ChoiceWindow **)(in_r0 + 0x94) = pCVar3;
      }
      GameText::getText(**(int **)(DAT_0018a12c + 0x18a03a));
      ChoiceWindow::set(pCVar3);
      *(undefined1 *)(in_r0 + 0x5d) = 1;
      *(undefined1 *)(in_r0 + 0xce) = 1;
      pauseSounds();
      **(undefined4 **)(DAT_0018a130 + 0x18a060) = 0;
    }
    else {
      iVar1 = Status::hardCoreMode();
      piVar6 = *(int **)(DAT_0018a134 + 0x189fee);
      if (iVar1 != 0) {
        iVar7 = 2;
      }
      if (*piVar6 == *(int *)(*piVar5 + 0x78)) {
        iVar1 = iVar7 << 1;
      }
      else {
        iVar2 = Status::inAlienOrbit();
        iVar1 = **(int **)(DAT_0018a138 + 0x18a010);
        if (iVar2 != 0) {
          iVar1 = iVar7;
        }
      }
      iVar7 = Status::getShip();
      Ship::getCargo(iVar7);
      iVar7 = Item::getAmount();
      if (iVar7 < iVar1) {
        if (*(int *)(in_r0 + 0x94) == 0) {
          pCVar3 = operator_new(0x5c);
          ChoiceWindow::ChoiceWindow(pCVar3);
          *(ChoiceWindow **)(in_r0 + 0x94) = pCVar3;
        }
        Status::hardCoreMode();
        pSVar4 = *(String **)(in_r0 + 0x94);
        GameText::getText(**(int **)(DAT_0018a13c + 0x18a0a4));
        ChoiceWindow::set(pSVar4);
        *(undefined1 *)(in_r0 + 0x5d) = 1;
        *(undefined1 *)(in_r0 + 0xce) = 1;
        pauseSounds();
        *piVar6 = 0;
      }
      else {
        PlayerEgo::startJumpDrive();
        if (*piVar6 != *(int *)(*piVar5 + 0x78)) {
          Status::inAlienOrbit();
        }
        Hud::hudEvent(*(int *)(in_r0 + 0x74),(PlayerEgo *)0x1e,*(int *)(in_r0 + 0x58));
        iVar1 = Status::getShip();
        Ship::removeCargo(iVar1,0x7a);
      }
    }
  }
  return;
}

```

## target disasm
```
  00189fb0: push {r4,r5,r6,r7,lr}
  00189fb2: add r7,sp,#0xc
  00189fb4: push {r8,r9,r11}
  00189fb8: mov r4,r0
  00189fba: ldrb.w r0,[r0,#0xdd]
  00189fbe: cmp r0,#0x0
  00189fc0: beq.w 0x0018a112
  00189fc4: ldr r0,[0x0018a128]
  00189fc6: add r0,pc
  00189fc8: ldr r6,[r0,#0x0]
  00189fca: ldr r0,[r6,#0x0]
  00189fcc: blx 0x00071a58
  00189fd0: movs r1,#0x7a
  00189fd2: movs r2,#0x1
  00189fd4: mov.w r9,#0x1
  00189fd8: blx 0x00071a70
  00189fdc: cbz r0,0x0018a01a
  00189fde: ldr r0,[r6,#0x0]
  00189fe0: blx 0x0007192c
  00189fe4: mov r1,r0
  00189fe6: ldr r0,[0x0018a134]
  00189fe8: cmp r1,#0x0
  00189fea: add r0,pc
  00189fec: ldr.w r8,[r0,#0x0]
  00189ff0: ldr r0,[r6,#0x0]
  00189ff2: ldr.w r2,[r8,#0x0]
  00189ff6: ldr r3,[r0,#0x78]
  00189ff8: it ne
  00189ffa: mov.ne.w r9,#0x2
  00189ffe: cmp r2,r3
  0018a000: beq 0x0018a064
  0018a002: blx 0x000723d0
  0018a006: mov r1,r0
  0018a008: ldr r0,[0x0018a138]
  0018a00a: cmp r1,#0x0
  0018a00c: add r0,pc
  0018a00e: ldr r0,[r0,#0x0]
  0018a010: ldr r5,[r0,#0x0]
  0018a012: ldr r0,[r6,#0x0]
  0018a014: it ne
  0018a016: mov.ne r5,r9
  0018a018: b 0x0018a068
  0018a01a: ldr.w r5,[r4,#0x94]
  0018a01e: cbnz r5,0x0018a030
  0018a020: movs r0,#0x5c
  0018a022: blx 0x0006eb24
  0018a026: mov r5,r0
  0018a028: blx 0x00074584
  0018a02c: str.w r5,[r4,#0x94]
  0018a030: ldr r0,[0x0018a12c]
  0018a032: movw r1,#0x243
  0018a036: add r0,pc
  0018a038: ldr r0,[r0,#0x0]
  0018a03a: ldr r0,[r0,#0x0]
  0018a03c: blx 0x00072f70
  0018a040: mov r1,r0
  0018a042: mov r0,r5
  0018a044: blx 0x00074794
  0018a048: movs r0,#0x1
  0018a04a: strb.w r0,[r4,#0x5d]
  0018a04e: strb.w r0,[r4,#0xce]
  0018a052: mov r0,r4
  0018a054: blx 0x0007822c
  0018a058: ldr r0,[0x0018a130]
  0018a05a: movs r1,#0x0
  0018a05c: add r0,pc
  0018a05e: ldr r0,[r0,#0x0]
  0018a060: str r1,[r0,#0x0]
  0018a062: b 0x0018a112
  0018a064: lsl.w r5,r9,#0x1
  0018a068: blx 0x00071a58
  0018a06c: movs r1,#0x7a
  0018a06e: blx 0x00072424
  0018a072: blx 0x0007183c
  0018a076: cmp r5,r0
  0018a078: ble 0x0018a0d4
  0018a07a: ldr.w r0,[r4,#0x94]
  0018a07e: cbnz r0,0x0018a090
  0018a080: movs r0,#0x5c
  0018a082: blx 0x0006eb24
  0018a086: mov r5,r0
  0018a088: blx 0x00074584
  0018a08c: str.w r5,[r4,#0x94]
  0018a090: ldr r0,[r6,#0x0]
  0018a092: blx 0x0007192c
  0018a096: mov r1,r0
  0018a098: ldr r0,[0x0018a13c]
  0018a09a: ldr.w r5,[r4,#0x94]
  0018a09e: cmp r1,#0x0
  0018a0a0: add r0,pc
  0018a0a2: ldr r0,[r0,#0x0]
  0018a0a4: ldr r0,[r0,#0x0]
  0018a0a6: ite eq
  0018a0a8: mov.eq.w r1,#0x244
  0018a0ac: movw.ne r1,#0x243
  0018a0b0: blx 0x00072f70
  0018a0b4: mov r1,r0
  0018a0b6: mov r0,r5
  0018a0b8: blx 0x00074794
  0018a0bc: movs r0,#0x1
  0018a0be: strb.w r0,[r4,#0x5d]
  0018a0c2: strb.w r0,[r4,#0xce]
  0018a0c6: mov r0,r4
  0018a0c8: blx 0x0007822c
  0018a0cc: movs r0,#0x0
  0018a0ce: str.w r0,[r8,#0x0]
  0018a0d2: b 0x0018a112
  0018a0d4: ldr r0,[r4,#0x58]
  0018a0d6: blx 0x000782b0
  0018a0da: ldr r0,[r6,#0x0]
  0018a0dc: ldr.w r1,[r8,#0x0]
  0018a0e0: ldr r2,[r0,#0x78]
  0018a0e2: cmp r1,r2
  0018a0e4: beq 0x0018a0f8
  0018a0e6: blx 0x000723d0
  0018a0ea: ldr r1,[0x0018a140]
  0018a0ec: cmp r0,#0x0
  0018a0ee: add r1,pc
  0018a0f0: ldr r1,[r1,#0x0]
  0018a0f2: ldr r1,[r1,#0x0]
  0018a0f4: it eq
  0018a0f6: mov.eq r9,r1
  0018a0f8: ldr r2,[r4,#0x58]
  0018a0fa: movs r1,#0x1e
  0018a0fc: ldr r0,[r4,#0x74]
  0018a0fe: mov r3,r9
  0018a100: blx 0x00072418
  0018a104: ldr r0,[r6,#0x0]
  0018a106: blx 0x00071a58
  0018a10a: movs r1,#0x7a
  0018a10c: mov r2,r9
  0018a10e: blx 0x00072454
  0018a112: pop.w {r8,r9,r11}
  0018a116: pop {r4,r5,r6,r7,pc}
```
