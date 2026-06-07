# GameText::GameText
elf 0x8165c body 104
Sig: undefined __thiscall GameText(GameText * this)

## decompile
```c

/* GameText::GameText() */

void __thiscall GameText::GameText(GameText *this)

{
  int iVar1;
  char *pcVar2;
  int *piVar3;
  String aSStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_000916e8 + 0x9166e);
  local_1c = *piVar3;
  Array<int>::Array();
  AbyssEngine::String::String((String *)(this + 0x10));
  pcVar2 = (char *)(DAT_000916f0 + 0x91690);
  **(undefined2 **)(DAT_000916ec + 0x9168e) = 0xffff;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  AbyssEngine::String::String(aSStack_28,pcVar2,false);
  AbyssEngine::String::operator=((String *)(this + 0x10),aSStack_28);
  AbyssEngine::String::~String(aSStack_28);
  iVar1 = *piVar3 - local_1c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  0009165c: push {r4,r5,r6,r7,lr}
  0009165e: add r7,sp,#0xc
  00091660: push.w r11
  00091664: sub sp,#0x10
  00091666: mov r4,r0
  00091668: ldr r0,[0x000916e8]
  0009166a: add r0,pc
  0009166c: ldr r6,[r0,#0x0]
  0009166e: ldr r0,[r6,#0x0]
  00091670: str r0,[sp,#0xc]
  00091672: mov r0,r4
  00091674: blx 0x000701f8
  00091678: add.w r5,r4,#0x10
  0009167c: mov r0,r5
  0009167e: blx 0x0006efbc
  00091682: ldr r0,[0x000916ec]
  00091684: movw r2,#0xffff
  00091688: ldr r1,[0x000916f0]
  0009168a: add r0,pc
  0009168c: add r1,pc
  0009168e: ldr r0,[r0,#0x0]
  00091690: strh r2,[r0,#0x0]
  00091692: movs r0,#0x0
  00091694: str r0,[r4,#0xc]
  00091696: str r0,[r4,#0x1c]
  00091698: mov r0,sp
  0009169a: movs r2,#0x0
  0009169c: blx 0x0006ee18
  000916a0: mov r1,sp
  000916a2: mov r0,r5
  000916a4: blx 0x0006f2b0
  000916a8: mov r0,sp
  000916aa: blx 0x0006ee30
  000916ae: ldr r0,[sp,#0xc]
  000916b0: ldr r1,[r6,#0x0]
  000916b2: subs r0,r1,r0
  000916b4: itttt eq
  000916b6: mov.eq r0,r4
  000916b8: add.eq sp,#0x10
  000916ba: pop.eq.w r11
  000916be: pop.eq {r4,r5,r6,r7,pc}
  000916c0: blx 0x0006e824
```
