# AbyssEngine::operator+
elf 0x73838 body 90
Sig: undefined __thiscall operator+(AbyssEngine * this, String * param_1, float * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&, float const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,String *param_1,float *param_2)

{
  int *piVar1;
  float fVar2;
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000838ac + 0x8384e);
  local_1c = *piVar1;
  fVar2 = (float)String::String(aSStack_28,param_1,false);
  String::String(aSStack_34,fVar2);
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
  00083838: push {r4,r5,r6,r7,lr}
  0008383a: add r7,sp,#0xc
  0008383c: push.w r11
  00083840: sub sp,#0x20
  00083842: mov r4,r0
  00083844: ldr r0,[0x000838ac]
  00083846: mov r5,r2
  00083848: movs r2,#0x0
  0008384a: add r0,pc
  0008384c: ldr r6,[r0,#0x0]
  0008384e: ldr r0,[r6,#0x0]
  00083850: str r0,[sp,#0x1c]
  00083852: add r0,sp,#0x10
  00083854: blx 0x0006f5b0
  00083858: ldr r1,[r5,#0x0]
  0008385a: add r0,sp,#0x4
  0008385c: blx 0x0006f5d4
  00083860: add r0,sp,#0x10
  00083862: add r1,sp,#0x4
  00083864: blx 0x0006ef5c
  00083868: add r0,sp,#0x4
  0008386a: blx 0x0006f580
  0008386e: add r1,sp,#0x10
  00083870: mov r0,r4
  00083872: movs r2,#0x0
  00083874: blx 0x0006f5b0
  00083878: add r0,sp,#0x10
  0008387a: blx 0x0006f580
  0008387e: ldr r0,[sp,#0x1c]
  00083880: ldr r1,[r6,#0x0]
  00083882: subs r0,r1,r0
  00083884: ittt eq
  00083886: add.eq sp,#0x20
  00083888: pop.eq.w r11
  0008388c: pop.eq {r4,r5,r6,r7,pc}
  0008388e: blx 0x0006e824
```
