# PaintCanvas::GetReverseString
elf 0x780a4 body 78
Sig: undefined __stdcall GetReverseString(String param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetReverseString(AbyssEngine::String) */

void AbyssEngine::PaintCanvas::GetReverseString(undefined4 param_1,int param_2,String *param_3)

{
  undefined4 extraout_r1;
  int *piVar1;
  String local_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00088100 + 0x880ba);
  local_1c = *piVar1;
  String::String(local_28,param_3,false);
  GetReverseString(param_1,extraout_r1,local_28,*(char *)(param_2 + 0x1c) == '\0');
  String::~String(local_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000880a4: push {r4,r5,r6,r7,lr}
  000880a6: add r7,sp,#0xc
  000880a8: push.w r8
  000880ac: sub sp,#0x10
  000880ae: mov r5,r0
  000880b0: ldr r0,[0x00088100]
  000880b2: mov r8,sp
  000880b4: mov r4,r1
  000880b6: add r0,pc
  000880b8: mov r1,r2
  000880ba: movs r2,#0x0
  000880bc: ldr r6,[r0,#0x0]
  000880be: ldr r0,[r6,#0x0]
  000880c0: str r0,[sp,#0xc]
  000880c2: mov r0,r8
  000880c4: blx 0x0006f028
  000880c8: ldrb r0,[r4,#0x1c]
  000880ca: clz r0,r0
  000880ce: mov r2,r8
  000880d0: lsrs r3,r0,#0x5
  000880d2: mov r0,r5
  000880d4: blx 0x0006fa48
  000880d8: mov r0,sp
  000880da: blx 0x0006ee30
  000880de: ldr r0,[sp,#0xc]
  000880e0: ldr r1,[r6,#0x0]
  000880e2: subs r0,r1,r0
  000880e4: ittt eq
  000880e6: add.eq sp,#0x10
  000880e8: pop.eq.w r8
  000880ec: pop.eq {r4,r5,r6,r7,pc}
  000880ee: blx 0x0006e824
```
