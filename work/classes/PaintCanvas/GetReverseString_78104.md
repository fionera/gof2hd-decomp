# PaintCanvas::GetReverseString
elf 0x78104 body 112
Sig: undefined __stdcall GetReverseString(String param_1, bool param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetReverseString(AbyssEngine::String, bool) */

void AbyssEngine::PaintCanvas::GetReverseString
               (String *param_1,undefined4 param_2,String *param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_0008818c + 0x88118);
  iVar1 = *piVar3;
  if (param_4 == 0) {
    String::String(param_1,param_3,false);
  }
  else {
    String::String(param_1,(char *)(DAT_00088190 + 0x8812c),false);
    iVar2 = *(int *)(param_3 + 8);
    while (iVar2 = iVar2 + -1, -1 < iVar2) {
      String::SubString((uint)&stack0xffffffd0,(uint)param_3);
      String::operator+=(param_1,(String *)&stack0xffffffd0);
      String::~String((String *)&stack0xffffffd0);
    }
  }
  iVar2 = *piVar3;
  iVar1 = iVar2 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1,iVar2);
}

```

## target disasm
```
  00088104: push {r4,r5,r6,r7,lr}
  00088106: add r7,sp,#0xc
  00088108: push {r4,r5,r6,r7,r8,r9,r11}
  0008810c: mov r8,r0
  0008810e: ldr r0,[0x0008818c]
  00088110: mov r5,r2
  00088112: cmp r3,#0x0
  00088114: add r0,pc
  00088116: ldr.w r9,[r0,#0x0]
  0008811a: ldr.w r0,[r9,#0x0]
  0008811e: str r0,[sp,#0xc]
  00088120: beq 0x00088156
  00088122: ldr r1,[0x00088190]
  00088124: mov r0,r8
  00088126: movs r2,#0x0
  00088128: add r1,pc
  0008812a: blx 0x0006ee18
  0008812e: ldr r3,[r5,#0x8]
  00088130: mov r6,sp
  00088132: b 0x0008814e
  00088134: mov r0,r6
  00088136: mov r1,r5
  00088138: mov r2,r4
  0008813a: blx 0x0006f604
  0008813e: mov r0,r8
  00088140: mov r1,r6
  00088142: blx 0x0006ef5c
  00088146: mov r0,r6
  00088148: blx 0x0006ee30
  0008814c: mov r3,r4
  0008814e: subs r4,r3,#0x1
  00088150: cmp r4,#0x0
  00088152: bge 0x00088134
  00088154: b 0x00088160
  00088156: mov r0,r8
  00088158: mov r1,r5
  0008815a: movs r2,#0x0
  0008815c: blx 0x0006f028
  00088160: ldr r0,[sp,#0xc]
  00088162: ldr.w r1,[r9,#0x0]
  00088166: subs r0,r1,r0
  00088168: itt eq
  0008816a: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  0008816e: pop.eq {r4,r5,r6,r7,pc}
  00088170: blx 0x0006e824
```
