# ImageFactory::ImageFactory
elf 0x11c54c body 116
Sig: undefined __thiscall ImageFactory(ImageFactory * this)

## decompile
```c

/* ImageFactory::ImageFactory() */

ImageFactory * __thiscall ImageFactory::ImageFactory(ImageFactory *this)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar3 = DAT_0012c5c4;
  puVar1 = (undefined4 *)(DAT_0012c5c0 + 0x12c560);
  *(undefined4 *)this = 0;
  if (*(char *)*puVar1 != '\0' || **(char **)(iVar3 + 0x12c566) != '\0') {
    iVar3 = DAT_0012c5cc + 0x12c57c;
    iVar7 = *(int *)(DAT_0012c5c8 + 0x12c57a);
    for (iVar5 = 0; iVar5 != 0xd; iVar5 = iVar5 + 1) {
      iVar2 = iVar3;
      iVar4 = iVar7;
      for (iVar6 = 0; iVar6 != 4; iVar6 = iVar6 + 1) {
        for (iVar8 = 0; iVar8 != 2; iVar8 = iVar8 + 1) {
          *(undefined4 *)(iVar4 + iVar8 * 4) = *(undefined4 *)(iVar2 + iVar8 * 4);
        }
        iVar2 = iVar2 + 8;
        iVar4 = iVar4 + 8;
      }
      iVar7 = iVar7 + 0x20;
      iVar3 = iVar3 + 0x20;
    }
  }
  reload();
  return this;
}

```

## target disasm
```
  0012c54c: push {r4,r5,r6,r7,lr}
  0012c54e: add r7,sp,#0xc
  0012c550: push.w r8
  0012c554: mov r8,r0
  0012c556: ldr r0,[0x0012c5c0]
  0012c558: ldr r1,[0x0012c5c4]
  0012c55a: movs r2,#0x0
  0012c55c: add r0,pc
  0012c55e: str.w r2,[r8,#0x0]
  0012c562: add r1,pc
  0012c564: ldr r0,[r0,#0x0]
  0012c566: ldr r1,[r1,#0x0]
  0012c568: ldrb r0,[r0,#0x0]
  0012c56a: ldrb r1,[r1,#0x0]
  0012c56c: orrs r0,r1
  0012c56e: beq 0x0012c5b2
  0012c570: ldr r0,[0x0012c5c8]
  0012c572: movs r2,#0x0
  0012c574: ldr r1,[0x0012c5cc]
  0012c576: add r0,pc
  0012c578: add r1,pc
  0012c57a: ldr r4,[r0,#0x0]
  0012c57c: b 0x0012c5ae
  0012c57e: movs r3,#0x0
  0012c580: mov r12,r1
  0012c582: mov r0,r1
  0012c584: mov r1,r4
  0012c586: b 0x0012c5a0
  0012c588: movs r6,#0x0
  0012c58a: b 0x0012c596
  0012c58c: ldr.w r5,[r0,r6,lsl #0x2]
  0012c590: str.w r5,[r1,r6,lsl #0x2]
  0012c594: adds r6,#0x1
  0012c596: cmp r6,#0x2
  0012c598: bne 0x0012c58c
  0012c59a: adds r0,#0x8
  0012c59c: adds r1,#0x8
  0012c59e: adds r3,#0x1
  0012c5a0: cmp r3,#0x4
  0012c5a2: bne 0x0012c588
  0012c5a4: mov r1,r12
  0012c5a6: adds r4,#0x20
  0012c5a8: add.w r1,r12,#0x20
  0012c5ac: adds r2,#0x1
  0012c5ae: cmp r2,#0xd
  0012c5b0: bne 0x0012c57e
  0012c5b2: mov r0,r8
  0012c5b4: blx 0x00071d1c
  0012c5b8: mov r0,r8
  0012c5ba: pop.w r8
  0012c5be: pop {r4,r5,r6,r7,pc}
```
