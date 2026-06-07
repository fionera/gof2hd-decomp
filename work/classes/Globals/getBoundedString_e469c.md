# Globals::getBoundedString
elf 0xe469c body 186
Sig: undefined __stdcall getBoundedString(String * param_1, int param_2)

## decompile
```c

/* Globals::getBoundedString(AbyssEngine::String const&, int) */

void Globals::getBoundedString(String *param_1,int param_2)

{
  int iVar1;
  String *this;
  Globals *pGVar2;
  String *in_r2;
  int in_r3;
  undefined4 *puVar3;
  undefined4 uVar4;
  int *piVar5;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar5 = *(int **)(DAT_000f478c + 0xf46b2);
  local_28 = *piVar5;
  AbyssEngine::String::String((String *)param_1,in_r2,false);
  puVar3 = *(undefined4 **)(DAT_000f4790 + 0xf46cc);
  iVar1 = AbyssEngine::PaintCanvas::GetTextWidth
                    (**(uint **)(DAT_000f4794 + 0xf46ce),(String *)*puVar3);
  if (in_r3 < iVar1) {
    this = operator_new(0xc);
    AbyssEngine::String::String(this);
    uVar4 = *puVar3;
    pGVar2 = (Globals *)AbyssEngine::String::String(aSStack_34,in_r2,false);
    getLine(pGVar2,uVar4,aSStack_34,in_r3 + -3,this);
    AbyssEngine::String::~String(aSStack_34);
    AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f4798 + 0xf4712),false);
    AbyssEngine::operator+(aSStack_40,this);
    AbyssEngine::String::operator=((String *)param_1,aSStack_40);
    AbyssEngine::String::~String(aSStack_40);
    AbyssEngine::String::~String(aSStack_4c);
    (**(code **)(*(int *)this + 4))(this);
  }
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f469c: push {r4,r5,r6,r7,lr}
  000f469e: add r7,sp,#0xc
  000f46a0: push {r8,r9,r10,r11}
  000f46a4: sub sp,#0x2c
  000f46a6: mov r8,r0
  000f46a8: ldr r0,[0x000f478c]
  000f46aa: mov r4,r2
  000f46ac: mov r1,r2
  000f46ae: add r0,pc
  000f46b0: movs r2,#0x0
  000f46b2: mov r6,r3
  000f46b4: ldr.w r10,[r0,#0x0]
  000f46b8: ldr.w r0,[r10,#0x0]
  000f46bc: str r0,[sp,#0x28]
  000f46be: mov r0,r8
  000f46c0: blx 0x0006f028
  000f46c4: ldr r0,[0x000f4790]
  000f46c6: ldr r1,[0x000f4794]
  000f46c8: add r0,pc
  000f46ca: add r1,pc
  000f46cc: ldr r5,[r0,#0x0]
  000f46ce: ldr r0,[r1,#0x0]
  000f46d0: ldr r1,[r5,#0x0]
  000f46d2: ldr r0,[r0,#0x0]
  000f46d4: mov r2,r4
  000f46d6: blx 0x0006faa8
  000f46da: cmp r0,r6
  000f46dc: ble 0x000f4740
  000f46de: movs r0,#0xc
  000f46e0: blx 0x0006eb24
  000f46e4: mov r11,r0
  000f46e6: blx 0x0006efbc
  000f46ea: ldr.w r9,[r5,#0x0]
  000f46ee: add r0,sp,#0x1c
  000f46f0: mov r1,r4
  000f46f2: movs r2,#0x0
  000f46f4: blx 0x0006f028
  000f46f8: subs r3,r6,#0x3
  000f46fa: add r2,sp,#0x1c
  000f46fc: mov r1,r9
  000f46fe: str.w r11,[sp,#0x0]
  000f4702: blx 0x00075dc0
  000f4706: add r0,sp,#0x1c
  000f4708: blx 0x0006ee30
  000f470c: ldr r1,[0x000f4798]
  000f470e: add r1,pc
  000f4710: add r0,sp,#0x4
  000f4712: movs r2,#0x0
  000f4714: blx 0x0006ee18
  000f4718: add r0,sp,#0x10
  000f471a: add r2,sp,#0x4
  000f471c: mov r1,r11
  000f471e: blx 0x0006ef98
  000f4722: add r1,sp,#0x10
  000f4724: mov r0,r8
  000f4726: blx 0x0006f2b0
  000f472a: add r0,sp,#0x10
  000f472c: blx 0x0006ee30
  000f4730: add r0,sp,#0x4
  000f4732: blx 0x0006ee30
  000f4736: ldr.w r0,[r11,#0x0]
  000f473a: ldr r1,[r0,#0x4]
  000f473c: mov r0,r11
  000f473e: blx r1
  000f4740: ldr r0,[sp,#0x28]
  000f4742: ldr.w r1,[r10,#0x0]
  000f4746: subs r0,r1,r0
  000f4748: ittt eq
  000f474a: add.eq sp,#0x2c
  000f474c: pop.eq.w {r8,r9,r10,r11}
  000f4750: pop.eq {r4,r5,r6,r7,pc}
  000f4752: blx 0x0006e824
```
