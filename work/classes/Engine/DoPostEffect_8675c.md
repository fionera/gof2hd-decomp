# Engine::DoPostEffect
elf 0x8675c body 310
Sig: undefined __stdcall DoPostEffect(void)

## decompile
```c

/* AbyssEngine::Engine::DoPostEffect() */

void AbyssEngine::Engine::DoPostEffect(void)

{
  Engine *in_r0;
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_00096898 + 0x9676e);
  iVar1 = *piVar7;
  uVar6 = *(uint *)(in_r0 + 0x410);
  uVar4 = *(undefined4 *)(in_r0 + 0x414);
  uVar2 = *(undefined4 *)(in_r0 + 0x418);
  if (**(char **)(DAT_0009689c + 0x96770) != '\0') {
    uVar5 = uVar4;
    if ((int)(uVar6 << 0x1e) < 0) {
      uVar6 = uVar6 & 0xfffffffd;
      piVar3 = *(int **)(*(int *)(in_r0 + 0x514) + **(int **)(DAT_000968a0 + 0x967a2) * 4);
      if (uVar6 == 0) {
        (**(code **)(*piVar3 + 0x14))(piVar3,uVar4);
        uVar6 = 0;
      }
      else {
        (**(code **)(*piVar3 + 0x1c))(piVar3,uVar4,&stack0xffffffd8);
        uVar5 = uVar2;
        uVar2 = uVar4;
      }
    }
    if (((byte)in_r0[0x410] & 1) != 0) {
      uVar6 = uVar6 & 0xfffffffe;
      piVar3 = *(int **)(*(int *)(in_r0 + 0x514) + **(int **)(DAT_000968a4 + 0x967e6) * 4);
      if (uVar6 == 0) {
        (**(code **)(*piVar3 + 0x14))(piVar3,uVar5);
        uVar6 = 0;
      }
      else {
        (**(code **)(*piVar3 + 0x1c))(piVar3,uVar5,&stack0xffffffd8);
        uVar5 = uVar2;
      }
    }
    if ((int)((uint)(byte)in_r0[0x410] << 0x1d) < 0) {
      piVar3 = *(int **)(*(int *)(in_r0 + 0x514) + **(int **)(DAT_000968a8 + 0x96826) * 4);
      if ((uVar6 & 0xfffffffb) == 0) {
        (**(code **)(*piVar3 + 0x18))(piVar3,uVar5);
      }
      else {
        (**(code **)(*piVar3 + 0x20))(piVar3,uVar5,&stack0xffffffd8);
      }
    }
    if (*(char *)(DAT_000968ac + 0x9686e) == '\x01') {
      SetPostEffect(in_r0,DAT_00096894,false);
    }
  }
  if (*piVar7 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0009675c: push {r4,r5,r6,r7,lr}
  0009675e: add r7,sp,#0xc
  00096760: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  00096764: mov r4,r0
  00096766: ldr r0,[0x00096898]
  00096768: ldr r1,[0x0009689c]
  0009676a: add r0,pc
  0009676c: add r1,pc
  0009676e: ldr.w r8,[r0,#0x0]
  00096772: ldr r1,[r1,#0x0]
  00096774: ldr.w r0,[r8,#0x0]
  00096778: str r0,[sp,#0x14]
  0009677a: ldrb r1,[r1,#0x0]
  0009677c: ldr.w r6,[r4,#0x410]
  00096780: ldr.w r5,[r4,#0x414]
  00096784: cmp r1,#0x0
  00096786: ldr.w r0,[r4,#0x418]
  0009678a: str r0,[sp,#0x10]
  0009678c: beq 0x0009687c
  0009678e: lsls r0,r6,#0x1e
  00096790: bpl 0x000967cc
  00096792: ldr r0,[0x000968a0]
  00096794: bic r6,r6,#0x2
  00096798: ldr.w r1,[r4,#0x514]
  0009679c: cmp r6,#0x0
  0009679e: add r0,pc
  000967a0: ldr r0,[r0,#0x0]
  000967a2: ldr r0,[r0,#0x0]
  000967a4: ldr.w r0,[r1,r0,lsl #0x2]
  000967a8: beq 0x000967c0
  000967aa: ldr r1,[r0,#0x0]
  000967ac: add r2,sp,#0x10
  000967ae: mov r3,r4
  000967b0: ldr.w r12,[r1,#0x1c]
  000967b4: mov r1,r5
  000967b6: blx r12
  000967b8: ldr.w r9,[sp,#0x10]
  000967bc: str r5,[sp,#0x10]
  000967be: b 0x000967ce
  000967c0: ldr r1,[r0,#0x0]
  000967c2: mov r2,r4
  000967c4: ldr r3,[r1,#0x14]
  000967c6: mov r1,r5
  000967c8: blx r3
  000967ca: movs r6,#0x0
  000967cc: mov r9,r5
  000967ce: ldrb.w r0,[r4,#0x410]
  000967d2: lsls r0,r0,#0x1f
  000967d4: beq 0x0009680e
  000967d6: ldr r0,[0x000968a4]
  000967d8: bic r6,r6,#0x1
  000967dc: ldr.w r1,[r4,#0x514]
  000967e0: cmp r6,#0x0
  000967e2: add r0,pc
  000967e4: ldr r0,[r0,#0x0]
  000967e6: ldr r0,[r0,#0x0]
  000967e8: ldr.w r0,[r1,r0,lsl #0x2]
  000967ec: beq 0x00096802
  000967ee: ldr r1,[r0,#0x0]
  000967f0: add r2,sp,#0x10
  000967f2: mov r3,r4
  000967f4: ldr r5,[r1,#0x1c]
  000967f6: mov r1,r9
  000967f8: blx r5
  000967fa: ldr r5,[sp,#0x10]
  000967fc: str.w r9,[sp,#0x10]
  00096800: b 0x00096810
  00096802: ldr r1,[r0,#0x0]
  00096804: mov r2,r4
  00096806: ldr r3,[r1,#0x14]
  00096808: mov r1,r9
  0009680a: blx r3
  0009680c: movs r6,#0x0
  0009680e: mov r5,r9
  00096810: ldrb.w r0,[r4,#0x410]
  00096814: lsls r0,r0,#0x1d
  00096816: bpl 0x00096868
  00096818: ldr r0,[0x000968a8]
  0009681a: add.w r12,r4,#0x3cc
  0009681e: ldr.w r3,[r4,#0x514]
  00096822: add r0,pc
  00096824: ldr r0,[r0,#0x0]
  00096826: ldr r0,[r0,#0x0]
  00096828: ldm.w r12,{r1,r2,r12}
  0009682c: ldr.w r0,[r3,r0,lsl #0x2]
  00096830: mvn r3,#0x4
  00096834: vldr.32 s0,[r4,#0x3c8]
  00096838: tst r6,r3
  0009683a: beq 0x00096856
  0009683c: ldr r3,[r0,#0x0]
  0009683e: ldr r6,[r3,#0x20]
  00096840: add r3,sp,#0x4
  00096842: stm r3,{r1,r2,r12}
  00096846: add r2,sp,#0x10
  00096848: mov r1,r5
  0009684a: mov r3,r4
  0009684c: vstr.32 s0,[sp]
  00096850: blx r6
  00096852: str r5,[sp,#0x10]
  00096854: b 0x00096868
  00096856: vmov r3,s0
  0009685a: ldr r6,[r0,#0x0]
  0009685c: ldr r6,[r6,#0x18]
  0009685e: stm sp,{r1,r2,r12}
  00096862: mov r1,r5
  00096864: mov r2,r4
  00096866: blx r6
  00096868: ldr r0,[0x000968ac]
  0009686a: add r0,pc
  0009686c: ldrb r0,[r0,#0x0]
  0009686e: cmp r0,#0x1
  00096870: bne 0x0009687c
  00096872: ldr r1,[0x00096894]
  00096874: mov r0,r4
  00096876: movs r2,#0x0
  00096878: blx 0x000707f8
  0009687c: ldr r0,[sp,#0x14]
  0009687e: ldr.w r1,[r8,#0x0]
  00096882: subs r0,r1,r0
  00096884: ittt eq
  00096886: add.eq sp,#0x18
  00096888: pop.eq.w {r8,r9,r11}
  0009688c: pop.eq {r4,r5,r6,r7,pc}
  0009688e: blx 0x0006e824
```
