# Globals::getLineArray
elf 0xe4384 body 426
Sig: undefined __stdcall getLineArray(uint param_1, String * param_2, int param_3, Array * param_4)

## decompile
```c

/* Globals::getLineArray(unsigned int, AbyssEngine::String const&, int,
   Array<AbyssEngine::String*>*) */

void Globals::getLineArray(uint param_1,String *param_2,int param_3,Array *param_4)

{
  String *pSVar1;
  Globals *pGVar2;
  short *psVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  uint uVar7;
  int iVar8;
  Array *in_stack_00000000;
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [8];
  int local_2c;
  int local_28;
  
  piVar6 = *(int **)(DAT_000f4584 + 0xf439a);
  local_28 = *piVar6;
  pSVar1 = operator_new(0xc);
  AbyssEngine::String::String(pSVar1);
  AbyssEngine::String::String(aSStack_34,(String *)param_3,false);
  AbyssEngine::String::String(aSStack_40,(char *)(DAT_000f4588 + 0xf43ba),false);
  AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  uVar7 = 0;
  for (iVar8 = 0; iVar8 < local_2c; iVar8 = iVar8 + iVar5) {
    AbyssEngine::String::SubString((uint)aSStack_40,(uint)aSStack_34);
    pGVar2 = (Globals *)AbyssEngine::String::String(aSStack_4c,aSStack_40,false);
    getLine(pGVar2,param_2,aSStack_4c,param_4,pSVar1);
    AbyssEngine::String::~String(aSStack_4c);
    iVar5 = *(int *)(pSVar1 + 8);
    AbyssEngine::String::~String(aSStack_40);
    uVar7 = uVar7 + 1;
  }
  (**(code **)(*(int *)pSVar1 + 4))(pSVar1);
  ArraySetLength<AbyssEngine::String*>(uVar7,in_stack_00000000);
  for (uVar4 = 0; uVar4 < uVar7; uVar4 = uVar4 + 1) {
    pSVar1 = operator_new(0xc);
    AbyssEngine::String::String(pSVar1);
    *(String **)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4) = pSVar1;
  }
  uVar4 = 0;
  while( true ) {
    if (uVar7 <= uVar4) break;
    AbyssEngine::String::SubString((uint)aSStack_40,(uint)aSStack_34);
    AbyssEngine::String::String(aSStack_58,aSStack_40,false);
    pGVar2 = *(Globals **)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4);
    getLine(pGVar2,param_2,aSStack_58,param_4,pGVar2);
    AbyssEngine::String::~String(aSStack_58);
    iVar8 = 0;
    pSVar1 = *(String **)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4);
    iVar5 = *(int *)(pSVar1 + 8);
    while (psVar3 = (short *)AbyssEngine::String::operator[](pSVar1,iVar8), *psVar3 == 0x20) {
      iVar8 = iVar8 + 1;
      pSVar1 = *(String **)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4);
    }
    iVar5 = iVar5 + 1;
    do {
      psVar3 = (short *)AbyssEngine::String::operator[]
                                  (*(String **)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4),
                                   iVar5 + -2);
      iVar5 = iVar5 + -1;
    } while (*psVar3 == 0x20);
    AbyssEngine::String::SubString
              ((uint)aSStack_64,*(uint *)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4));
    AbyssEngine::String::operator=
              (*(String **)(*(int *)(in_stack_00000000 + 4) + uVar4 * 4),aSStack_64);
    AbyssEngine::String::~String(aSStack_64);
    AbyssEngine::String::~String(aSStack_40);
    uVar4 = uVar4 + 1;
  }
  AbyssEngine::String::~String(aSStack_34);
  if (*piVar6 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f4384: push {r4,r5,r6,r7,lr}
  000f4386: add r7,sp,#0xc
  000f4388: push {r8,r9,r10,r11}
  000f438c: sub sp,#0x54
  000f438e: str r3,[sp,#0x10]
  000f4390: mov r5,r2
  000f4392: str r1,[sp,#0xc]
  000f4394: ldr r0,[0x000f4584]
  000f4396: add r0,pc
  000f4398: ldr r6,[r0,#0x0]
  000f439a: ldr r0,[r6,#0x0]
  000f439c: str r0,[sp,#0x50]
  000f439e: movs r0,#0xc
  000f43a0: blx 0x0006eb24
  000f43a4: mov r4,r0
  000f43a6: blx 0x0006efbc
  000f43aa: add r0,sp,#0x44
  000f43ac: mov r1,r5
  000f43ae: movs r2,#0x0
  000f43b0: blx 0x0006f028
  000f43b4: ldr r1,[0x000f4588]
  000f43b6: add r1,pc
  000f43b8: add r0,sp,#0x38
  000f43ba: movs r2,#0x0
  000f43bc: blx 0x0006ee18
  000f43c0: add r0,sp,#0x44
  000f43c2: add r1,sp,#0x38
  000f43c4: str r6,[sp,#0x8]
  000f43c6: blx 0x0006ef5c
  000f43ca: add r5,sp,#0x38
  000f43cc: mov r0,r5
  000f43ce: blx 0x0006ee30
  000f43d2: add.w r9,sp,#0x44
  000f43d6: add.w r8,sp,#0x2c
  000f43da: movs r6,#0x0
  000f43dc: mov.w r10,#0x0
  000f43e0: b 0x000f4416
  000f43e2: mov r0,r5
  000f43e4: mov r1,r9
  000f43e6: mov r2,r10
  000f43e8: blx 0x0006f604
  000f43ec: mov r0,r8
  000f43ee: mov r1,r5
  000f43f0: movs r2,#0x0
  000f43f2: blx 0x0006f028
  000f43f6: ldr r1,[sp,#0xc]
  000f43f8: mov r2,r8
  000f43fa: ldr r3,[sp,#0x10]
  000f43fc: str r4,[sp,#0x0]
  000f43fe: blx 0x00075dc0
  000f4402: mov r0,r8
  000f4404: blx 0x0006ee30
  000f4408: mov r0,r5
  000f440a: ldr.w r11,[r4,#0x8]
  000f440e: blx 0x0006ee30
  000f4412: add r10,r11
  000f4414: adds r6,#0x1
  000f4416: ldr r3,[sp,#0x4c]
  000f4418: cmp r10,r3
  000f441a: blt 0x000f43e2
  000f441c: ldr r0,[r4,#0x0]
  000f441e: ldr r1,[r0,#0x4]
  000f4420: mov r0,r4
  000f4422: blx r1
  000f4424: ldr.w r8,[r7,#0x8]
  000f4428: mov r0,r6
  000f442a: mov r1,r8
  000f442c: blx 0x0006fe08
  000f4430: movs r5,#0x0
  000f4432: b 0x000f444a
  000f4434: movs r0,#0xc
  000f4436: blx 0x0006eb24
  000f443a: mov r4,r0
  000f443c: blx 0x0006efbc
  000f4440: ldr.w r0,[r8,#0x4]
  000f4444: str.w r4,[r0,r5,lsl #0x2]
  000f4448: adds r5,#0x1
  000f444a: cmp r5,r6
  000f444c: bcc 0x000f4434
  000f444e: add.w r8,sp,#0x38
  000f4452: add r1,sp,#0x44
  000f4454: add r5,sp,#0x20
  000f4456: add.w r11,sp,#0x14
  000f445a: mov.w r9,#0x0
  000f445e: mov.w r10,#0x0
  000f4462: b 0x000f450e
  000f4464: ldr r3,[sp,#0x4c]
  000f4466: mov r0,r8
  000f4468: mov r2,r9
  000f446a: blx 0x0006f604
  000f446e: mov r0,r5
  000f4470: mov r1,r8
  000f4472: movs r2,#0x0
  000f4474: blx 0x0006f028
  000f4478: ldr r4,[r7,#0x8]
  000f447a: mov r8,r11
  000f447c: ldr r0,[r4,#0x4]
  000f447e: ldr.w r0,[r0,r10,lsl #0x2]
  000f4482: ldr r1,[sp,#0xc]
  000f4484: mov r2,r5
  000f4486: ldr r3,[sp,#0x10]
  000f4488: str r0,[sp,#0x0]
  000f448a: blx 0x00075dc0
  000f448e: mov r0,r5
  000f4490: blx 0x0006ee30
  000f4494: ldr r0,[r4,#0x4]
  000f4496: movs r4,#0x0
  000f4498: ldr.w r0,[r0,r10,lsl #0x2]
  000f449c: ldr r5,[r0,#0x8]
  000f449e: add r9,r5
  000f44a0: b 0x000f44ac
  000f44a2: ldr r0,[r7,#0x8]
  000f44a4: adds r4,#0x1
  000f44a6: ldr r0,[r0,#0x4]
  000f44a8: ldr.w r0,[r0,r10,lsl #0x2]
  000f44ac: mov r1,r4
  000f44ae: blx 0x0006f3b8
  000f44b2: ldrh r0,[r0,#0x0]
  000f44b4: cmp r0,#0x20
  000f44b6: beq 0x000f44a2
  000f44b8: adds r5,#0x1
  000f44ba: ldr r0,[r7,#0x8]
  000f44bc: ldr r0,[r0,#0x4]
  000f44be: ldr.w r0,[r0,r10,lsl #0x2]
  000f44c2: subs r1,r5,#0x2
  000f44c4: blx 0x0006f3b8
  000f44c8: ldrh r0,[r0,#0x0]
  000f44ca: subs r5,#0x1
  000f44cc: cmp r0,#0x20
  000f44ce: beq 0x000f44ba
  000f44d0: ldr.w r11,[r7,#0x8]
  000f44d4: ldr.w r0,[r11,#0x4]
  000f44d8: ldr.w r1,[r0,r10,lsl #0x2]
  000f44dc: mov r0,r8
  000f44de: mov r2,r4
  000f44e0: mov r3,r5
  000f44e2: blx 0x0006f604
  000f44e6: ldr r0,[r7,#0x8]
  000f44e8: ldr r0,[r0,#0x4]
  000f44ea: ldr.w r0,[r0,r10,lsl #0x2]
  000f44ee: mov r1,r8
  000f44f0: blx 0x0006f2b0
  000f44f4: mov r0,r8
  000f44f6: mov r11,r8
  000f44f8: blx 0x0006ee30
  000f44fc: add.w r8,sp,#0x38
  000f4500: mov r0,r8
  000f4502: blx 0x0006ee30
  000f4506: add.w r10,r10,#0x1
  000f450a: add r1,sp,#0x44
  000f450c: add r5,sp,#0x20
  000f450e: cmp r10,r6
  000f4510: bcc 0x000f4464
  000f4512: add r0,sp,#0x44
  000f4514: blx 0x0006ee30
  000f4518: ldr r0,[sp,#0x50]
  000f451a: ldr r1,[sp,#0x8]
  000f451c: ldr r1,[r1,#0x0]
  000f451e: subs r0,r1,r0
  000f4520: ittt eq
  000f4522: add.eq sp,#0x54
  000f4524: pop.eq.w {r8,r9,r10,r11}
  000f4528: pop.eq {r4,r5,r6,r7,pc}
  000f452a: blx 0x0006e824
```
