# AbyssEngine::operator+
elf 0x737dc body 74
Sig: undefined __thiscall operator+(AbyssEngine * this, int * param_1, String * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(int const&, AbyssEngine::String const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,int *param_1,String *param_2)

{
  String *this_00;
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00083834 + 0x837f0);
  local_1c = *piVar1;
  this_00 = (String *)String::String(aSStack_28,*param_1);
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
  000837dc: push {r4,r5,r6,r7,lr}
  000837de: add r7,sp,#0xc
  000837e0: push.w r11
  000837e4: sub sp,#0x10
  000837e6: mov r4,r0
  000837e8: ldr r0,[0x00083834]
  000837ea: mov r5,r2
  000837ec: add r0,pc
  000837ee: ldr r6,[r0,#0x0]
  000837f0: ldr r0,[r6,#0x0]
  000837f2: str r0,[sp,#0xc]
  000837f4: mov r0,sp
  000837f6: ldr r1,[r1,#0x0]
  000837f8: blx 0x0006f658
  000837fc: mov r1,r5
  000837fe: blx 0x0006ef5c
  00083802: mov r1,sp
  00083804: mov r0,r4
  00083806: movs r2,#0x0
  00083808: blx 0x0006f5b0
  0008380c: mov r0,sp
  0008380e: blx 0x0006f580
  00083812: ldr r0,[sp,#0xc]
  00083814: ldr r1,[r6,#0x0]
  00083816: subs r0,r1,r0
  00083818: ittt eq
  0008381a: add.eq sp,#0x10
  0008381c: pop.eq.w r11
  00083820: pop.eq {r4,r5,r6,r7,pc}
  00083822: blx 0x0006e824
```
