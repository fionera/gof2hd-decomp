# Globals::getLine
elf 0xe4230 body 306
Sig: undefined __thiscall getLine(Globals * this, uint param_1, String param_2, int param_3, String * param_4)

## decompile
```c

/* Globals::getLine(unsigned int, AbyssEngine::String, int, AbyssEngine::String*) */

void __thiscall
Globals::getLine(undefined4 param_1_00,uint param_1,String *param_3,int param_4,String *param_5)

{
  short sVar1;
  int *piVar2;
  uint uVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  uint uVar8;
  String aSStack_34 [12];
  int local_28;
  
  piVar2 = *(int **)(DAT_000f4378 + 0xf4246);
  local_28 = *piVar2;
  uVar3 = GameText::getLanguage();
  iVar6 = 5;
  if ((uVar3 | 1) == 0xb) {
    iVar6 = 0xf;
  }
  if (uVar3 == 0xf) {
    iVar6 = 0xf;
  }
  uVar8 = 0;
  puVar7 = *(undefined4 **)(DAT_000f437c + 0xf4272);
  uVar3 = 0;
  while( true ) {
    if (*(uint *)(param_3 + 8) <= uVar3) {
      if ((int)*(uint *)(param_3 + 8) < 2) {
        AbyssEngine::String::String(aSStack_34,(char *)(DAT_000f4380 + 0xf4322),false);
        AbyssEngine::String::operator=(param_5,aSStack_34);
      }
      else {
        AbyssEngine::String::SubString((uint)aSStack_34,(uint)param_3);
        AbyssEngine::String::operator=(param_5,aSStack_34);
      }
      goto LAB_000f4346;
    }
    psVar4 = (short *)AbyssEngine::String::operator[](param_3,uVar3);
    sVar1 = *psVar4;
    iVar5 = AbyssEngine::PaintCanvas::GetTextWidth
                      ((PaintCanvas *)*puVar7,param_1,param_3,uVar3,uVar3 + 1);
    iVar6 = iVar6 + iVar5;
    if (sVar1 == 0x20) {
      uVar8 = uVar3;
    }
    if (param_4 <= iVar6) break;
    psVar4 = (short *)AbyssEngine::String::operator[](param_3,uVar3);
    if ((*psVar4 == 10) ||
       (psVar4 = (short *)AbyssEngine::String::operator[](param_3,uVar3), uVar3 = uVar3 + 1,
       *psVar4 == 0xd)) {
      AbyssEngine::String::SubString((uint)aSStack_34,(uint)param_3);
      AbyssEngine::String::operator=(param_5,aSStack_34);
LAB_000f4346:
      AbyssEngine::String::~String(aSStack_34);
      if (*piVar2 != local_28) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return;
    }
  }
  if ((int)uVar8 < 1) {
    AbyssEngine::String::SubString((uint)aSStack_34,(uint)param_3);
    AbyssEngine::String::operator=(param_5,aSStack_34);
  }
  else {
    AbyssEngine::String::SubString((uint)aSStack_34,(uint)param_3);
    AbyssEngine::String::operator=(param_5,aSStack_34);
  }
  goto LAB_000f4346;
}

```

## target disasm
```
  000f4230: push {r4,r5,r6,r7,lr}
  000f4232: add r7,sp,#0xc
  000f4234: push {r8,r9,r10,r11}
  000f4238: sub sp,#0x24
  000f423a: str r3,[sp,#0x10]
  000f423c: mov r5,r2
  000f423e: str r1,[sp,#0xc]
  000f4240: ldr r0,[0x000f4378]
  000f4242: add r0,pc
  000f4244: ldr r0,[r0,#0x0]
  000f4246: str r0,[sp,#0x8]
  000f4248: ldr r0,[r0,#0x0]
  000f424a: str r0,[sp,#0x20]
  000f424c: blx 0x0006f544
  000f4250: orr r1,r0,#0x1
  000f4254: mov.w r8,#0x5
  000f4258: cmp r1,#0xb
  000f425a: it eq
  000f425c: mov.eq.w r8,#0xf
  000f4260: cmp r0,#0xf
  000f4262: it eq
  000f4264: mov.eq r8,r0
  000f4266: ldr r0,[0x000f437c]
  000f4268: mov.w r11,#0x0
  000f426c: movs r6,#0x0
  000f426e: add r0,pc
  000f4270: ldr.w r9,[r0,#0x0]
  000f4274: ldr r3,[r5,#0x8]
  000f4276: cmp r6,r3
  000f4278: bcs 0x000f42de
  000f427a: mov r0,r5
  000f427c: mov r1,r6
  000f427e: blx 0x0006f3b8
  000f4282: ldrh.w r10,[r0,#0x0]
  000f4286: mov r2,r5
  000f4288: ldr.w r0,[r9,#0x0]
  000f428c: mov r3,r6
  000f428e: ldr r1,[sp,#0xc]
  000f4290: adds r4,r6,#0x1
  000f4292: str r4,[sp,#0x0]
  000f4294: blx 0x0006fdf0
  000f4298: cmp.w r10,#0x20
  000f429c: add r8,r0
  000f429e: it eq
  000f42a0: mov.eq r11,r6
  000f42a2: ldr r0,[sp,#0x10]
  000f42a4: cmp r8,r0
  000f42a6: bge 0x000f42f8
  000f42a8: mov r0,r5
  000f42aa: mov r1,r6
  000f42ac: blx 0x0006f3b8
  000f42b0: ldrh r0,[r0,#0x0]
  000f42b2: cmp r0,#0xa
  000f42b4: beq 0x000f42c6
  000f42b6: mov r0,r5
  000f42b8: mov r1,r6
  000f42ba: blx 0x0006f3b8
  000f42be: ldrh r0,[r0,#0x0]
  000f42c0: mov r6,r4
  000f42c2: cmp r0,#0xd
  000f42c4: bne 0x000f4274
  000f42c6: add r6,sp,#0x14
  000f42c8: mov r1,r5
  000f42ca: movs r2,#0x0
  000f42cc: mov r3,r4
  000f42ce: mov r0,r6
  000f42d0: blx 0x0006f604
  000f42d4: ldr r0,[r7,#0x8]
  000f42d6: mov r1,r6
  000f42d8: blx 0x0006f2b0
  000f42dc: b 0x000f4346
  000f42de: cmp r3,#0x2
  000f42e0: blt 0x000f4318
  000f42e2: add r4,sp,#0x14
  000f42e4: mov r1,r5
  000f42e6: movs r2,#0x0
  000f42e8: mov r0,r4
  000f42ea: blx 0x0006f604
  000f42ee: ldr r0,[r7,#0x8]
  000f42f0: mov r1,r4
  000f42f2: blx 0x0006f2b0
  000f42f6: b 0x000f4346
  000f42f8: cmp.w r11,#0x1
  000f42fc: blt 0x000f4330
  000f42fe: add r4,sp,#0x14
  000f4300: add.w r3,r11,#0x1
  000f4304: mov r1,r5
  000f4306: movs r2,#0x0
  000f4308: mov r0,r4
  000f430a: blx 0x0006f604
  000f430e: ldr r0,[r7,#0x8]
  000f4310: mov r1,r4
  000f4312: blx 0x0006f2b0
  000f4316: b 0x000f4346
  000f4318: ldr r1,[0x000f4380]
  000f431a: add r4,sp,#0x14
  000f431c: movs r2,#0x0
  000f431e: add r1,pc
  000f4320: mov r0,r4
  000f4322: blx 0x0006ee18
  000f4326: ldr r0,[r7,#0x8]
  000f4328: mov r1,r4
  000f432a: blx 0x0006f2b0
  000f432e: b 0x000f4346
  000f4330: add r6,sp,#0x14
  000f4332: mov r1,r5
  000f4334: movs r2,#0x0
  000f4336: mov r3,r4
  000f4338: mov r0,r6
  000f433a: blx 0x0006f604
  000f433e: ldr r0,[r7,#0x8]
  000f4340: mov r1,r6
  000f4342: blx 0x0006f2b0
  000f4346: add r0,sp,#0x14
  000f4348: blx 0x0006ee30
  000f434c: ldr r1,[sp,#0x8]
  000f434e: ldr r0,[sp,#0x20]
  000f4350: ldr r1,[r1,#0x0]
  000f4352: subs r0,r1,r0
  000f4354: ittt eq
  000f4356: add.eq sp,#0x24
  000f4358: pop.eq.w {r8,r9,r10,r11}
  000f435c: pop.eq {r4,r5,r6,r7,pc}
  000f435e: blx 0x0006e824
```
