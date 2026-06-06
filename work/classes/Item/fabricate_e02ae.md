# Item::fabricate
elf 0xe02ae body 126
Sig: undefined __stdcall fabricate(Item * param_1, Array * param_2, int param_3)

## decompile
```c

/* Item::fabricate(Item*, Array<Item*>*, int) */

void Item::fabricate(Item *param_1,Array *param_2,int param_3)

{
  uint *puVar1;
  Array<Item*> *this;
  Item *pIVar2;
  int *piVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  
  puVar1 = *(uint **)(param_1 + 0x28);
  iVar6 = *(int *)(param_1 + 0x2c);
  uVar4 = 0;
  uVar7 = *puVar1;
  do {
    if (uVar7 <= uVar4) {
      this = operator_new(0xc);
      Array<Item*>::Array(this);
      pIVar2 = (Item *)makeItem((int)param_1);
      ArrayAdd<Item*>(pIVar2,(Array *)this);
      (*(code *)(DAT_001ac254 + 0x1ac258))(param_2,this);
      return;
    }
    uVar5 = 0;
    do {
      if (*(uint *)param_2 <= uVar5) goto LAB_000f02fa;
      piVar3 = *(int **)(*(int *)(param_2 + 4) + uVar5 * 4);
      uVar5 = uVar5 + 1;
    } while (*piVar3 != *(int *)(puVar1[1] + uVar4 * 4));
    piVar3[0xd] = piVar3[0xd] - *(int *)(*(int *)(iVar6 + 4) + uVar4 * 4) * param_3;
LAB_000f02fa:
    uVar4 = uVar4 + 1;
  } while( true );
}

```
## target disasm
```
  000f02ae: push {r4,r5,r6,r7,lr}
  000f02b0: add r7,sp,#0xc
  000f02b2: push {r8,r9,r10}
  000f02b6: mov r8,r0
  000f02b8: ldr r0,[r0,#0x28]
  000f02ba: ldr.w r12,[r8,#0x2c]
  000f02be: mov r9,r2
  000f02c0: mov r10,r1
  000f02c2: movs r3,#0x0
  000f02c4: ldr.w lr,[r0,#0x0]
  000f02c8: b 0x000f02fc
  000f02ca: ldr.w r6,[r10,#0x0]
  000f02ce: movs r5,#0x0
  000f02d0: cmp r5,r6
  000f02d2: bcs 0x000f02fa
  000f02d4: ldr.w r1,[r10,#0x4]
  000f02d8: ldr r2,[r0,#0x4]
  000f02da: ldr.w r1,[r1,r5,lsl #0x2]
  000f02de: adds r5,#0x1
  000f02e0: ldr.w r2,[r2,r3,lsl #0x2]
  000f02e4: ldr r4,[r1,#0x0]
  000f02e6: cmp r4,r2
  000f02e8: bne 0x000f02d0
  000f02ea: ldr.w r2,[r12,#0x4]
  000f02ee: ldr r4,[r1,#0x34]
  000f02f0: ldr.w r2,[r2,r3,lsl #0x2]
  000f02f4: mls r2,r2,r9,r4
  000f02f8: str r2,[r1,#0x34]
  000f02fa: adds r3,#0x1
  000f02fc: cmp r3,lr
  000f02fe: bcc 0x000f02ca
  000f0300: movs r0,#0xc
  000f0302: blx 0x0006eb24
  000f0306: mov r6,r0
  000f0308: blx 0x00071860
  000f030c: mov r0,r8
  000f030e: mov r1,r9
  000f0310: blx 0x000718c0
  000f0314: mov r1,r6
  000f0316: blx 0x000718b4
  000f031a: mov r0,r10
  000f031c: mov r1,r6
  000f031e: pop.w {r8,r9,r10}
  000f0322: pop.w {r4,r5,r6,r7,lr}
  000f0326: b.w 0x001ac248
  001ac248: bx pc
```
