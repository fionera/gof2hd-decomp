# AbyssEngine::operator+
elf 0x738b0 body 74
Sig: undefined __thiscall operator+(AbyssEngine * this, float * param_1, String * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(float const&, AbyssEngine::String const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,float *param_1,String *param_2)

{
  String *this_00;
  int *piVar1;
  float in_s0;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00083908 + 0x838c4);
  local_1c = *piVar1;
  this_00 = (String *)String::String(aSStack_28,in_s0);
  String::operator+=(this_00,param_2);
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
  000838b0: push {r4,r5,r6,r7,lr}
  000838b2: add r7,sp,#0xc
  000838b4: push.w r11
  000838b8: sub sp,#0x10
  000838ba: mov r4,r0
  000838bc: ldr r0,[0x00083908]
  000838be: mov r5,r2
  000838c0: add r0,pc
  000838c2: ldr r6,[r0,#0x0]
  000838c4: ldr r0,[r6,#0x0]
  000838c6: str r0,[sp,#0xc]
  000838c8: mov r0,sp
  000838ca: ldr r1,[r1,#0x0]
  000838cc: blx 0x0006f5d4
  000838d0: mov r1,r5
  000838d2: blx 0x0006ef5c
  000838d6: mov r1,sp
  000838d8: mov r0,r4
  000838da: movs r2,#0x0
  000838dc: blx 0x0006f5b0
  000838e0: mov r0,sp
  000838e2: blx 0x0006f580
  000838e6: ldr r0,[sp,#0xc]
  000838e8: ldr r1,[r6,#0x0]
  000838ea: subs r0,r1,r0
  000838ec: ittt eq
  000838ee: add.eq sp,#0x10
  000838f0: pop.eq.w r11
  000838f4: pop.eq {r4,r5,r6,r7,pc}
  000838f6: blx 0x0006e824
```
