# Globals::loadFont
elf 0xe47c4 body 438
Sig: undefined __stdcall loadFont(int param_1)

## decompile
```c

/* Globals::loadFont(int) */

void Globals::loadFont(int param_1)

{
  PaintCanvas *this;
  int iVar1;
  undefined1 uVar2;
  int in_r1;
  uint uVar3;
  short sVar4;
  uint *puVar5;
  uint *puVar6;
  undefined4 *puVar7;
  int *piVar8;
  
  switch(in_r1) {
  case 9:
    puVar7 = *(undefined4 **)(DAT_000f4980 + 0xf47e4);
    puVar5 = *(uint **)(DAT_000f4984 + 0xf47e6);
    AbyssEngine::PaintCanvas::FontCreate((ushort)*puVar7,(uint *)0x2d74,SUB41(puVar5,0));
    if (**(char **)(DAT_000f4988 + 0xf47fc) == '\0') {
      sVar4 = -8;
      if (**(char **)(DAT_000f498c + 0xf48e6) == '\0') {
        sVar4 = -4;
      }
    }
    else {
      sVar4 = -6;
    }
    AbyssEngine::PaintCanvas::FontSetSpacing((PaintCanvas *)*puVar7,*puVar5,sVar4);
    uVar2 = 0;
    goto LAB_000f48f6;
  case 10:
    puVar7 = *(undefined4 **)(DAT_000f4990 + 0xf4814);
    puVar6 = *(uint **)(DAT_000f4994 + 0xf4816);
    puVar5 = (uint *)0x2d78;
    break;
  case 0xb:
    puVar7 = *(undefined4 **)(DAT_000f499c + 0xf4826);
    puVar6 = *(uint **)(DAT_000f49a0 + 0xf4828);
    puVar5 = (uint *)0x2d76;
    break;
  default:
    puVar7 = *(undefined4 **)(DAT_000f49b0 + 0xf4838);
    if (in_r1 == 0xf) {
      puVar5 = *(uint **)(DAT_000f49b4 + 0xf4844);
      AbyssEngine::PaintCanvas::FontCreate((ushort)*puVar7,(uint *)0x2d7e,SUB41(puVar5,0));
      uVar3 = *puVar5;
      this = (PaintCanvas *)*puVar7;
      if (**(char **)(DAT_000f49b8 + 0xf4856) == '\0') {
        sVar4 = -10;
        if (**(char **)(DAT_000f49bc + 0xf4948) == '\0') {
          sVar4 = -5;
        }
      }
      else {
        sVar4 = -7;
      }
    }
    else {
      puVar5 = *(uint **)(DAT_000f49c0 + 0xf48ba);
      AbyssEngine::PaintCanvas::FontCreate((ushort)*puVar7,(uint *)0x457,SUB41(puVar5,0));
      if (**(char **)(DAT_000f49c4 + 0xf48ca) == '\0') {
        if (**(char **)(DAT_000f49c8 + 0xf495e) == '\0') {
          uVar3 = *puVar5;
          this = (PaintCanvas *)*puVar7;
          if (**(char **)(DAT_000f49cc + 0xf4970) == '\0') {
            sVar4 = -2;
            goto LAB_000f48aa;
          }
        }
        else {
          uVar3 = *puVar5;
          this = (PaintCanvas *)*puVar7;
        }
        sVar4 = -4;
      }
      else {
        uVar3 = *puVar5;
        this = (PaintCanvas *)*puVar7;
        sVar4 = -5;
      }
    }
    goto LAB_000f48aa;
  case 0xe:
    puVar7 = *(undefined4 **)(DAT_000f49a4 + 0xf486e);
    puVar6 = *(uint **)(DAT_000f49a8 + 0xf4870);
    puVar5 = (uint *)0x2d7c;
  }
  AbyssEngine::PaintCanvas::FontCreate((ushort)*puVar7,puVar5,SUB41(puVar6,0));
  uVar3 = *puVar6;
  this = (PaintCanvas *)*puVar7;
  if (**(char **)(DAT_000f4998 + 0xf4888) == '\0') {
    sVar4 = -8;
    if (**(char **)(DAT_000f49ac + 0xf48a0) == '\0') {
      sVar4 = -4;
    }
  }
  else {
    sVar4 = -6;
  }
LAB_000f48aa:
  AbyssEngine::PaintCanvas::FontSetSpacing(this,uVar3,sVar4);
  uVar2 = 1;
LAB_000f48f6:
  piVar8 = *(int **)(DAT_000f49d0 + 0xf4900);
  puVar5 = *(uint **)(DAT_000f49d4 + 0xf4902);
  iVar1 = *piVar8;
  *(undefined1 *)(iVar1 + 0x1c) = uVar2;
  AbyssEngine::PaintCanvas::FontCreate((ushort)iVar1,(uint *)0x51e,SUB41(puVar5,0));
  AbyssEngine::PaintCanvas::FontSetSpacing((PaintCanvas *)*piVar8,*puVar5,0);
  puVar7 = *(undefined4 **)(DAT_000f49d8 + 0xf4928);
  AbyssEngine::PaintCanvas::FontCreate((ushort)*piVar8,(uint *)0x2d7a,SUB41(puVar7,0));
  (*(code *)(DAT_001ac2d4 + 0x1ac2d8))(*piVar8,*puVar7,0);
  return;
}

```

## target disasm
```
  000f47c4: push {r4,r5,r7,lr}
  000f47c6: add r7,sp,#0x8
  000f47c8: sub.w r0,r1,#0x9
  000f47cc: cmp r0,#0x5
  000f47ce: bhi 0x000f4830
  000f47d0: tbb [pc,r0]
  000f47da: ldr r0,[0x000f4980]
  000f47dc: movs r3,#0x1
  000f47de: ldr r1,[0x000f4984]
  000f47e0: add r0,pc
  000f47e2: add r1,pc
  000f47e4: ldr r5,[r0,#0x0]
  000f47e6: ldr r4,[r1,#0x0]
  000f47e8: movw r1,#0x2d74
  000f47ec: ldr r0,[r5,#0x0]
  000f47ee: mov r2,r4
  000f47f0: blx 0x00075dcc
  000f47f4: ldr r0,[0x000f4988]
  000f47f6: ldr r1,[r4,#0x0]
  000f47f8: add r0,pc
  000f47fa: ldr r2,[r0,#0x0]
  000f47fc: ldr r0,[r5,#0x0]
  000f47fe: ldrb r2,[r2,#0x0]
  000f4800: cmp r2,#0x0
  000f4802: beq 0x000f48dc
  000f4804: mvn r2,#0x7
  000f4808: adds r2,#0x2
  000f480a: b 0x000f48ee
  000f480c: ldr r0,[0x000f4990]
  000f480e: ldr r1,[0x000f4994]
  000f4810: add r0,pc
  000f4812: add r1,pc
  000f4814: ldr r5,[r0,#0x0]
  000f4816: ldr r4,[r1,#0x0]
  000f4818: movw r1,#0x2d78
  000f481c: b 0x000f4876
  000f481e: ldr r0,[0x000f499c]
  000f4820: ldr r1,[0x000f49a0]
  000f4822: add r0,pc
  000f4824: add r1,pc
  000f4826: ldr r5,[r0,#0x0]
  000f4828: ldr r4,[r1,#0x0]
  000f482a: movw r1,#0x2d76
  000f482e: b 0x000f4876
  000f4830: ldr r0,[0x000f49b0]
  000f4832: cmp r1,#0xf
  000f4834: add r0,pc
  000f4836: ldr r5,[r0,#0x0]
  000f4838: ldr r0,[r5,#0x0]
  000f483a: bne 0x000f48b2
  000f483c: ldr r1,[0x000f49b4]
  000f483e: movs r3,#0x1
  000f4840: add r1,pc
  000f4842: ldr r4,[r1,#0x0]
  000f4844: movw r1,#0x2d7e
  000f4848: mov r2,r4
  000f484a: blx 0x00075dcc
  000f484e: ldr r0,[0x000f49b8]
  000f4850: ldr r1,[r4,#0x0]
  000f4852: add r0,pc
  000f4854: ldr r2,[r0,#0x0]
  000f4856: ldr r0,[r5,#0x0]
  000f4858: ldrb r2,[r2,#0x0]
  000f485a: cmp r2,#0x0
  000f485c: beq 0x000f493e
  000f485e: mvn r2,#0x7
  000f4862: adds r2,#0x1
  000f4864: b 0x000f48a8
  000f4866: ldr r0,[0x000f49a4]
  000f4868: ldr r1,[0x000f49a8]
  000f486a: add r0,pc
  000f486c: add r1,pc
  000f486e: ldr r5,[r0,#0x0]
  000f4870: ldr r4,[r1,#0x0]
  000f4872: movw r1,#0x2d7c
  000f4876: ldr r0,[r5,#0x0]
  000f4878: mov r2,r4
  000f487a: movs r3,#0x1
  000f487c: blx 0x00075dcc
  000f4880: ldr r0,[0x000f4998]
  000f4882: ldr r1,[r4,#0x0]
  000f4884: add r0,pc
  000f4886: ldr r2,[r0,#0x0]
  000f4888: ldr r0,[r5,#0x0]
  000f488a: ldrb r2,[r2,#0x0]
  000f488c: cbz r2,0x000f4896
  000f488e: mvn r2,#0x7
  000f4892: adds r2,#0x2
  000f4894: b 0x000f48a8
  000f4896: ldr r3,[0x000f49ac]
  000f4898: movw r2,#0xfff8
  000f489c: add r3,pc
  000f489e: ldr r3,[r3,#0x0]
  000f48a0: ldrb r3,[r3,#0x0]
  000f48a2: cmp r3,#0x0
  000f48a4: it eq
  000f48a6: add.eq r2,#0x4
  000f48a8: sxth r2,r2
  000f48aa: blx 0x00075dd8
  000f48ae: movs r1,#0x1
  000f48b0: b 0x000f48f6
  000f48b2: ldr r1,[0x000f49c0]
  000f48b4: movs r3,#0x1
  000f48b6: add r1,pc
  000f48b8: ldr r4,[r1,#0x0]
  000f48ba: movw r1,#0x457
  000f48be: mov r2,r4
  000f48c0: blx 0x00075dcc
  000f48c4: ldr r0,[0x000f49c4]
  000f48c6: add r0,pc
  000f48c8: ldr r0,[r0,#0x0]
  000f48ca: ldrb r0,[r0,#0x0]
  000f48cc: cmp r0,#0x0
  000f48ce: beq 0x000f4954
  000f48d0: mvn r2,#0x7
  000f48d4: ldr r1,[r4,#0x0]
  000f48d6: ldr r0,[r5,#0x0]
  000f48d8: adds r2,#0x3
  000f48da: b 0x000f48aa
  000f48dc: ldr r3,[0x000f498c]
  000f48de: movw r2,#0xfff8
  000f48e2: add r3,pc
  000f48e4: ldr r3,[r3,#0x0]
  000f48e6: ldrb r3,[r3,#0x0]
  000f48e8: cmp r3,#0x0
  000f48ea: it eq
  000f48ec: add.eq r2,#0x4
  000f48ee: sxth r2,r2
  000f48f0: blx 0x00075dd8
  000f48f4: movs r1,#0x0
  000f48f6: ldr r0,[0x000f49d0]
  000f48f8: movs r3,#0x1
  000f48fa: ldr r2,[0x000f49d4]
  000f48fc: add r0,pc
  000f48fe: add r2,pc
  000f4900: ldr r5,[r0,#0x0]
  000f4902: ldr r4,[r2,#0x0]
  000f4904: ldr r0,[r5,#0x0]
  000f4906: mov r2,r4
  000f4908: strb r1,[r0,#0x1c]
  000f490a: movw r1,#0x51e
  000f490e: blx 0x00075dcc
  000f4912: ldr r1,[r4,#0x0]
  000f4914: movs r2,#0x0
  000f4916: ldr r0,[r5,#0x0]
  000f4918: blx 0x00075dd8
  000f491c: ldr r0,[0x000f49d8]
  000f491e: movw r1,#0x2d7a
  000f4922: movs r3,#0x1
  000f4924: add r0,pc
  000f4926: ldr r4,[r0,#0x0]
  000f4928: ldr r0,[r5,#0x0]
  000f492a: mov r2,r4
  000f492c: blx 0x00075dcc
  000f4930: ldr r1,[r4,#0x0]
  000f4932: movs r2,#0x0
  000f4934: ldr r0,[r5,#0x0]
  000f4936: pop.w {r4,r5,r7,lr}
  000f493a: b.w 0x001ac2c8
  000f493e: ldr r2,[0x000f49bc]
  000f4940: movw r3,#0xfff8
  000f4944: add r2,pc
  000f4946: ldr r2,[r2,#0x0]
  000f4948: ldrb r5,[r2,#0x0]
  000f494a: subs r2,r3,#0x2
  000f494c: cmp r5,#0x0
  000f494e: it eq
  000f4950: add.eq r2,r3,#0x3
  000f4952: b 0x000f48a8
  000f4954: ldr r0,[0x000f49c8]
  000f4956: movw r2,#0xfff8
  000f495a: add r0,pc
  000f495c: ldr r0,[r0,#0x0]
  000f495e: ldrb r0,[r0,#0x0]
  000f4960: cbz r0,0x000f4968
  000f4962: ldr r1,[r4,#0x0]
  000f4964: ldr r0,[r5,#0x0]
  000f4966: b 0x000f4976
  000f4968: ldr r0,[0x000f49cc]
  000f496a: ldr r1,[r4,#0x0]
  000f496c: add r0,pc
  000f496e: ldr r3,[r0,#0x0]
  000f4970: ldr r0,[r5,#0x0]
  000f4972: ldrb r3,[r3,#0x0]
  000f4974: cbz r3,0x000f497a
  000f4976: adds r2,#0x4
  000f4978: b 0x000f48a8
  000f497a: adds r2,#0x6
  000f497c: b 0x000f48a8
  001ac2c8: bx pc
```
