# EnergyShield::EnergyShield
elf 0x8a200 body 94
Sig: undefined __thiscall EnergyShield(EnergyShield * this)

## decompile
```c

/* AbyssEngine::EnergyShield::EnergyShield() */

void __thiscall AbyssEngine::EnergyShield::EnergyShield(EnergyShield *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  EnergyShield *apEStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009a278 + 0x9a20c);
  local_14 = *piVar4;
  apEStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_0009a27c + 0x9a21e);
  puVar3 = *(undefined4 **)(DAT_0009a284 + 0x9a226);
  *(int *)this = *(int *)(DAT_0009a280 + 0x9a222) + 8;
  iVar2 = DAT_0009a288;
  *puVar3 = *puVar1;
  String::String((String *)apEStack_20,(char *)(iVar2 + 0x9a236),false);
  String::operator=((String *)(this + 0xc),(String *)apEStack_20);
  String::~String((String *)apEStack_20);
  iVar2 = *piVar4 - local_14;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  0009a200: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0009a202: add r7,sp,#0x18
  0009a204: mov r4,r0
  0009a206: ldr r0,[0x0009a278]
  0009a208: add r0,pc
  0009a20a: ldr r5,[r0,#0x0]
  0009a20c: ldr r0,[r5,#0x0]
  0009a20e: str r0,[sp,#0xc]
  0009a210: mov r0,r4
  0009a212: blx 0x00070930
  0009a216: ldr r0,[0x0009a27c]
  0009a218: ldr r1,[0x0009a280]
  0009a21a: add r0,pc
  0009a21c: ldr r2,[0x0009a284]
  0009a21e: add r1,pc
  0009a220: ldr r0,[r0,#0x0]
  0009a222: add r2,pc
  0009a224: ldr r1,[r1,#0x0]
  0009a226: ldr r2,[r2,#0x0]
  0009a228: adds r1,#0x8
  0009a22a: str r1,[r4,#0x0]
  0009a22c: ldr r0,[r0,#0x0]
  0009a22e: ldr r1,[0x0009a288]
  0009a230: str r0,[r2,#0x0]
  0009a232: add r1,pc
  0009a234: mov r0,sp
  0009a236: movs r2,#0x0
  0009a238: blx 0x0006ee18
  0009a23c: add.w r0,r4,#0xc
  0009a240: mov r1,sp
  0009a242: blx 0x0006f2b0
  0009a246: mov r0,sp
  0009a248: blx 0x0006ee30
  0009a24c: ldr r0,[sp,#0xc]
  0009a24e: ldr r1,[r5,#0x0]
  0009a250: subs r0,r1,r0
  0009a252: ittt eq
  0009a254: mov.eq r0,r4
  0009a256: add.eq sp,#0x10
  0009a258: pop.eq {r4,r5,r7,pc}
  0009a25a: blx 0x0006e824
```
