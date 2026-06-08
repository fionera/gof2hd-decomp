# AbyssEngine::operator+
elf 0x73660 body 92
Sig: undefined __thiscall operator+(AbyssEngine * this, String * param_1, longlong * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&, long long const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,String *param_1,longlong *param_2)

{
  int *piVar1;
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000836d8 + 0x83676);
  local_1c = *piVar1;
  String::String(aSStack_28,param_1,false);
  String::String(aSStack_34,*param_2);
  String::operator+=(aSStack_28,aSStack_34);
  String::~String(aSStack_34);
  String::String((String *)this,aSStack_28,false);
  String::~String(aSStack_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00083660: push {r4,r5,r6,r7,lr}
  00083662: add r7,sp,#0xc
  00083664: push.w r11
  00083668: sub sp,#0x20
  0008366a: mov r4,r0
  0008366c: ldr r0,[0x000836d8]
  0008366e: mov r5,r2
  00083670: movs r2,#0x0
  00083672: add r0,pc
  00083674: ldr r6,[r0,#0x0]
  00083676: ldr r0,[r6,#0x0]
  00083678: str r0,[sp,#0x1c]
  0008367a: add r0,sp,#0x10
  0008367c: blx 0x0006f5b0
  00083680: ldrd r2,r3,[r5,#0x0]
  00083684: add r0,sp,#0x4
  00083686: blx 0x0006f5bc
  0008368a: add r0,sp,#0x10
  0008368c: add r1,sp,#0x4
  0008368e: blx 0x0006ef5c
  00083692: add r0,sp,#0x4
  00083694: blx 0x0006f580
  00083698: add r1,sp,#0x10
  0008369a: mov r0,r4
  0008369c: movs r2,#0x0
  0008369e: blx 0x0006f5b0
  000836a2: add r0,sp,#0x10
  000836a4: blx 0x0006f580
  000836a8: ldr r0,[sp,#0x1c]
  000836aa: ldr r1,[r6,#0x0]
  000836ac: subs r0,r1,r0
  000836ae: ittt eq
  000836b0: add.eq sp,#0x20
  000836b2: pop.eq.w r11
  000836b6: pop.eq {r4,r5,r6,r7,pc}
  000836b8: blx 0x0006e824
```
