# AbyssEngine::operator+
elf 0x72e7c body 74
Sig: undefined __stdcall operator+(String * param_1, String * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&, AbyssEngine::String const&) */

void AbyssEngine::operator+(String *param_1,String *param_2)

{
  String *this;
  String *in_r2;
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00082ed4 + 0x82e92);
  local_1c = *piVar1;
  this = (String *)String::String(aSStack_28,param_2,false);
  String::operator+=(this,in_r2);
  String::String((String *)param_1,aSStack_28,false);
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
  00082e7c: push {r4,r5,r6,r7,lr}
  00082e7e: add r7,sp,#0xc
  00082e80: push.w r11
  00082e84: sub sp,#0x10
  00082e86: mov r4,r0
  00082e88: ldr r0,[0x00082ed4]
  00082e8a: mov r5,r2
  00082e8c: movs r2,#0x0
  00082e8e: add r0,pc
  00082e90: ldr r6,[r0,#0x0]
  00082e92: ldr r0,[r6,#0x0]
  00082e94: str r0,[sp,#0xc]
  00082e96: mov r0,sp
  00082e98: blx 0x0006f5b0
  00082e9c: mov r1,r5
  00082e9e: blx 0x0006ef5c
  00082ea2: mov r1,sp
  00082ea4: mov r0,r4
  00082ea6: movs r2,#0x0
  00082ea8: blx 0x0006f5b0
  00082eac: mov r0,sp
  00082eae: blx 0x0006f580
  00082eb2: ldr r0,[sp,#0xc]
  00082eb4: ldr r1,[r6,#0x0]
  00082eb6: subs r0,r1,r0
  00082eb8: ittt eq
  00082eba: add.eq sp,#0x10
  00082ebc: pop.eq.w r11
  00082ec0: pop.eq {r4,r5,r6,r7,pc}
  00082ec2: blx 0x0006e824
```
