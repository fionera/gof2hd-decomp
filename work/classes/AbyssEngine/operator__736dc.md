# AbyssEngine::operator+
elf 0x736dc body 76
Sig: undefined __thiscall operator+(AbyssEngine * this, longlong * param_1, String * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(long long const&, AbyssEngine::String const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,longlong *param_1,String *param_2)

{
  String *this_00;
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00083738 + 0x836f0);
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
  000836dc: push {r4,r5,r6,r7,lr}
  000836de: add r7,sp,#0xc
  000836e0: push.w r11
  000836e4: sub sp,#0x10
  000836e6: mov r4,r0
  000836e8: ldr r0,[0x00083738]
  000836ea: mov r5,r2
  000836ec: add r0,pc
  000836ee: ldr r6,[r0,#0x0]
  000836f0: ldr r0,[r6,#0x0]
  000836f2: str r0,[sp,#0xc]
  000836f4: mov r0,sp
  000836f6: ldrd r2,r3,[r1,#0x0]
  000836fa: blx 0x0006f5bc
  000836fe: mov r1,r5
  00083700: blx 0x0006ef5c
  00083704: mov r1,sp
  00083706: mov r0,r4
  00083708: movs r2,#0x0
  0008370a: blx 0x0006f5b0
  0008370e: mov r0,sp
  00083710: blx 0x0006f580
  00083714: ldr r0,[sp,#0xc]
  00083716: ldr r1,[r6,#0x0]
  00083718: subs r0,r1,r0
  0008371a: ittt eq
  0008371c: add.eq sp,#0x10
  0008371e: pop.eq.w r11
  00083722: pop.eq {r4,r5,r6,r7,pc}
  00083724: blx 0x0006e824
```
