# Globals::getRandomEnemyFighter
elf 0xe49dc body 160
Sig: undefined __thiscall getRandomEnemyFighter(Globals * this, int param_1)

## decompile
```c

/* Globals::getRandomEnemyFighter(int) */

uint __thiscall Globals::getRandomEnemyFighter(Globals *this,int param_1)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  iVar2 = param_1;
  if (1 < param_1 - 9U) {
    iVar2 = 8;
  }
  if (param_1 < 4) {
    iVar2 = param_1;
  }
  if (iVar2 == 1) {
    iVar2 = Status::dlc1Won();
    if (iVar2 == 0) {
      uVar1 = 9;
    }
    else {
      iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f4a88 + 0xf4a20));
      uVar1 = 0x27;
      if (iVar2 < 0x55) {
        uVar1 = 0x29;
      }
      if (iVar2 < 0x3c) {
        uVar1 = 9;
      }
    }
  }
  else if (iVar2 == 9) {
    uVar1 = 8;
  }
  else if (iVar2 == 10) {
    uVar1 = 0x2c;
  }
  else {
    iVar4 = DAT_000f4a80 + 0xf4a4a;
    piVar3 = *(int **)(DAT_000f4a7c + 0xf4a48);
    do {
      do {
        uVar1 = AbyssEngine::AERandom::nextInt(*piVar3);
      } while ((uVar1 & 0xfffffffb) - 9 < 2);
    } while (((uVar1 < 0x10) && ((1 << (uVar1 & 0xff) & 0x8101U) != 0)) ||
            (*(int *)(iVar4 + uVar1 * 4) != iVar2));
  }
  return uVar1;
}

```

## target disasm
```
  000f49dc: push {r4,r5,r6,r7,lr}
  000f49de: add r7,sp,#0xc
  000f49e0: push {r8,r9,r11}
  000f49e4: sub.w r0,r1,#0x9
  000f49e8: mov r4,r1
  000f49ea: cmp r0,#0x1
  000f49ec: it hi
  000f49ee: mov.hi r4,#0x8
  000f49f0: cmp r1,#0x3
  000f49f2: it le
  000f49f4: mov.le r4,r1
  000f49f6: cmp r4,#0x1
  000f49f8: beq 0x000f4a0a
  000f49fa: cmp r4,#0x9
  000f49fc: beq 0x000f4a06
  000f49fe: cmp r4,#0xa
  000f4a00: bne 0x000f4a38
  000f4a02: movs r0,#0x2c
  000f4a04: b 0x000f4a76
  000f4a06: movs r0,#0x8
  000f4a08: b 0x000f4a76
  000f4a0a: ldr r0,[0x000f4a84]
  000f4a0c: add r0,pc
  000f4a0e: ldr r0,[r0,#0x0]
  000f4a10: ldr r0,[r0,#0x0]
  000f4a12: blx 0x000719a4
  000f4a16: cbz r0,0x000f4a74
  000f4a18: ldr r0,[0x000f4a88]
  000f4a1a: movs r1,#0x64
  000f4a1c: add r0,pc
  000f4a1e: ldr r0,[r0,#0x0]
  000f4a20: ldr r0,[r0,#0x0]
  000f4a22: blx 0x00071848
  000f4a26: mov r1,r0
  000f4a28: movs r0,#0x27
  000f4a2a: cmp r1,#0x55
  000f4a2c: it lt
  000f4a2e: mov.lt r0,#0x29
  000f4a30: cmp r1,#0x3c
  000f4a32: it lt
  000f4a34: mov.lt r0,#0x9
  000f4a36: b 0x000f4a76
  000f4a38: ldr r0,[0x000f4a7c]
  000f4a3a: mov.w r8,#0x1
  000f4a3e: ldr r6,[0x000f4a80]
  000f4a40: movw r9,#0x8101
  000f4a44: add r0,pc
  000f4a46: add r6,pc
  000f4a48: ldr r5,[r0,#0x0]
  000f4a4a: ldr r0,[r5,#0x0]
  000f4a4c: movs r1,#0x25
  000f4a4e: blx 0x00071848
  000f4a52: bic r1,r0,#0x4
  000f4a56: subs r1,#0x9
  000f4a58: cmp r1,#0x2
  000f4a5a: bcc 0x000f4a4a
  000f4a5c: cmp r0,#0xf
  000f4a5e: bhi 0x000f4a6a
  000f4a60: lsl.w r1,r8,r0
  000f4a64: tst.w r1,r9
  000f4a68: bne 0x000f4a4a
  000f4a6a: ldr.w r1,[r6,r0,lsl #0x2]
  000f4a6e: cmp r1,r4
  000f4a70: bne 0x000f4a4a
  000f4a72: b 0x000f4a76
  000f4a74: movs r0,#0x9
  000f4a76: pop.w {r8,r9,r11}
  000f4a7a: pop {r4,r5,r6,r7,pc}
```
