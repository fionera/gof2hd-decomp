# AbyssEngine::operator==
elf 0x7390c body 76
Sig: undefined __stdcall operator==(String * param_1, String * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&, AbyssEngine::String const&) */

void AbyssEngine::operator==(String *param_1,String *param_2)

{
  String *this;
  int iVar1;
  int *piVar2;
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_00083958 + 0x83924);
  local_1c = *piVar2;
  this = (String *)String::String(aSStack_28,param_1,false);
  String::Compare(this,param_2);
  String::~String(aSStack_28);
  iVar1 = *piVar2 - local_1c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  0008390c: push {r4,r5,r6,r7,lr}
  0008390e: add r7,sp,#0xc
  00083910: push.w r11
  00083914: sub sp,#0x10
  00083916: mov r4,r1
  00083918: mov r1,r0
  0008391a: ldr r0,[0x00083958]
  0008391c: mov r5,sp
  0008391e: movs r2,#0x0
  00083920: add r0,pc
  00083922: ldr r6,[r0,#0x0]
  00083924: ldr r0,[r6,#0x0]
  00083926: str r0,[sp,#0xc]
  00083928: mov r0,r5
  0008392a: blx 0x0006f5b0
  0008392e: mov r1,r4
  00083930: blx 0x0006f664
  00083934: mov r4,r0
  00083936: mov r0,r5
  00083938: blx 0x0006f580
  0008393c: ldr r0,[sp,#0xc]
  0008393e: ldr r1,[r6,#0x0]
  00083940: subs r0,r1,r0
  00083942: itttt eq
  00083944: clz.eq r0,r4
  00083948: lsr.eq r0,r0,#0x5
  0008394a: add.eq sp,#0x10
  0008394c: pop.eq.w r11
  00083950: it eq
  00083952: pop.eq {r4,r5,r6,r7,pc}
  00083954: blx 0x0006e824
```
