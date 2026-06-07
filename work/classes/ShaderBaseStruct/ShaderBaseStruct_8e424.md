# ShaderBaseStruct::ShaderBaseStruct
elf 0x8e424 body 120
Sig: undefined __thiscall ShaderBaseStruct(ShaderBaseStruct * this)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::ShaderBaseStruct() */

void __thiscall AbyssEngine::ShaderBaseStruct::ShaderBaseStruct(ShaderBaseStruct *this)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  String aSStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_0009e4b4 + 0x9e43c);
  local_1c = *piVar3;
  *(int *)this = *(int *)(DAT_0009e4b8 + 0x9e43e) + 8;
  String::String((String *)(this + 0xc));
  iVar2 = DAT_0009e4bc;
  *(undefined4 *)(this + 4) = 0xffffffff;
  *(undefined2 *)(this + 8) = 0x100;
  iVar1 = DAT_0009e4c0;
  **(int **)(iVar2 + 0x9e45e) = **(int **)(iVar2 + 0x9e45e) + 1;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  String::String(aSStack_28,(char *)(iVar1 + 0x9e46c),false);
  String::operator=((String *)(this + 0xc),aSStack_28);
  String::~String(aSStack_28);
  iVar2 = *piVar3 - local_1c;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  0009e424: push {r4,r5,r6,r7,lr}
  0009e426: add r7,sp,#0xc
  0009e428: push.w r11
  0009e42c: sub sp,#0x10
  0009e42e: mov r4,r0
  0009e430: ldr r0,[0x0009e4b4]
  0009e432: ldr r1,[0x0009e4b8]
  0009e434: add.w r5,r4,#0xc
  0009e438: add r0,pc
  0009e43a: add r1,pc
  0009e43c: ldr r6,[r0,#0x0]
  0009e43e: ldr r0,[r1,#0x0]
  0009e440: ldr r1,[r6,#0x0]
  0009e442: adds r0,#0x8
  0009e444: str r1,[sp,#0xc]
  0009e446: str r0,[r4,#0x0]
  0009e448: mov r0,r5
  0009e44a: blx 0x0006efbc
  0009e44e: ldr r0,[0x0009e4bc]
  0009e450: mov.w r1,#0xffffffff
  0009e454: str r1,[r4,#0x4]
  0009e456: mov.w r1,#0x100
  0009e45a: add r0,pc
  0009e45c: strh r1,[r4,#0x8]
  0009e45e: ldr r0,[r0,#0x0]
  0009e460: ldr r2,[r0,#0x0]
  0009e462: ldr r1,[0x0009e4c0]
  0009e464: adds r2,#0x1
  0009e466: str r2,[r0,#0x0]
  0009e468: add r1,pc
  0009e46a: movs r0,#0x0
  0009e46c: strd r0,r0,[r4,#0x18]
  0009e470: mov r0,sp
  0009e472: movs r2,#0x0
  0009e474: blx 0x0006ee18
  0009e478: mov r1,sp
  0009e47a: mov r0,r5
  0009e47c: blx 0x0006f2b0
  0009e480: mov r0,sp
  0009e482: blx 0x0006ee30
  0009e486: ldr r0,[sp,#0xc]
  0009e488: ldr r1,[r6,#0x0]
  0009e48a: subs r0,r1,r0
  0009e48c: itttt eq
  0009e48e: mov.eq r0,r4
  0009e490: add.eq sp,#0x10
  0009e492: pop.eq.w r11
  0009e496: pop.eq {r4,r5,r6,r7,pc}
  0009e498: blx 0x0006e824
```
