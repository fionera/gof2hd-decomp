# Status::replaceHash
elf 0xac8d4 body 218
Sig: undefined __stdcall replaceHash(String param_1, String param_2, String param_3)

## decompile
```c

/* Status::replaceHash(AbyssEngine::String, AbyssEngine::String, AbyssEngine::String) */

void Status::replaceHash(String *param_1,undefined4 param_2,String *param_3,String *param_4)

{
  int iVar1;
  code *pcVar2;
  int *piVar3;
  undefined1 auStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [8];
  int local_28;
  int local_24;
  
  piVar3 = *(int **)(DAT_000bc9ec + 0xbc8ec);
  local_24 = *piVar3;
  iVar1 = AbyssEngine::String::IndexOf(param_3);
  if (iVar1 < 0) {
    AbyssEngine::String::String(param_1,param_3,false);
  }
  else {
    AbyssEngine::String::SubString((uint)aSStack_30,(uint)param_3);
    if (local_28 == 0) {
      AbyssEngine::String::String(aSStack_3c,param_4,false);
      AbyssEngine::String::SubString((uint)aSStack_48,(uint)param_3);
      AbyssEngine::operator+(param_1,aSStack_3c);
      AbyssEngine::String::~String(aSStack_48);
      AbyssEngine::String::~String(aSStack_3c);
    }
    else {
      AbyssEngine::String::String(aSStack_48,param_4,false);
      AbyssEngine::operator+(aSStack_3c,aSStack_30);
      AbyssEngine::String::SubString((uint)auStack_54,(uint)param_3);
      AbyssEngine::operator+(param_1,aSStack_3c);
      pcVar2 = *(code **)(DAT_000bc9f0 + 0xbc948);
      (*pcVar2)(auStack_54);
      (*pcVar2)(aSStack_3c);
      (*pcVar2)(aSStack_48);
    }
    AbyssEngine::String::~String(aSStack_30);
  }
  if (*piVar3 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000bc8d4: push {r4,r5,r6,r7,lr}
  000bc8d6: add r7,sp,#0xc
  000bc8d8: push {r8,r9,r10}
  000bc8dc: sub sp,#0x38
  000bc8de: mov r9,r0
  000bc8e0: ldr r0,[0x000bc9ec]
  000bc8e2: ldr.w r8,[r7,#0x8]
  000bc8e6: mov r4,r3
  000bc8e8: add r0,pc
  000bc8ea: mov r5,r2
  000bc8ec: ldr.w r10,[r0,#0x0]
  000bc8f0: mov r1,r8
  000bc8f2: ldr.w r0,[r10,#0x0]
  000bc8f6: str r0,[sp,#0x34]
  000bc8f8: mov r0,r2
  000bc8fa: blx 0x0006f3a0
  000bc8fe: mov r6,r0
  000bc900: cmp r0,#0x0
  000bc902: blt 0x000bc956
  000bc904: add r0,sp,#0x28
  000bc906: mov r1,r5
  000bc908: movs r2,#0x0
  000bc90a: mov r3,r6
  000bc90c: blx 0x0006f604
  000bc910: ldr r0,[sp,#0x30]
  000bc912: cbz r0,0x000bc962
  000bc914: add r0,sp,#0x10
  000bc916: mov r1,r4
  000bc918: movs r2,#0x0
  000bc91a: blx 0x0006f028
  000bc91e: add r0,sp,#0x1c
  000bc920: add r1,sp,#0x28
  000bc922: add r2,sp,#0x10
  000bc924: blx 0x0006ef98
  000bc928: ldr.w r0,[r8,#0x8]
  000bc92c: ldr r3,[r5,#0x8]
  000bc92e: adds r2,r0,r6
  000bc930: add r0,sp,#0x4
  000bc932: mov r1,r5
  000bc934: blx 0x0006f604
  000bc938: add r1,sp,#0x1c
  000bc93a: add r2,sp,#0x4
  000bc93c: mov r0,r9
  000bc93e: blx 0x0006ef98
  000bc942: ldr r0,[0x000bc9f0]
  000bc944: add r0,pc
  000bc946: ldr r4,[r0,#0x0]
  000bc948: add r0,sp,#0x4
  000bc94a: blx r4
  000bc94c: add r0,sp,#0x1c
  000bc94e: blx r4
  000bc950: add r0,sp,#0x10
  000bc952: blx r4
  000bc954: b 0x000bc992
  000bc956: mov r0,r9
  000bc958: mov r1,r5
  000bc95a: movs r2,#0x0
  000bc95c: blx 0x0006f028
  000bc960: b 0x000bc998
  000bc962: add r0,sp,#0x1c
  000bc964: mov r1,r4
  000bc966: movs r2,#0x0
  000bc968: blx 0x0006f028
  000bc96c: ldr.w r0,[r8,#0x8]
  000bc970: ldr r3,[r5,#0x8]
  000bc972: adds r2,r0,r6
  000bc974: add r0,sp,#0x10
  000bc976: mov r1,r5
  000bc978: blx 0x0006f604
  000bc97c: add r1,sp,#0x1c
  000bc97e: add r2,sp,#0x10
  000bc980: mov r0,r9
  000bc982: blx 0x0006ef98
  000bc986: add r0,sp,#0x10
  000bc988: blx 0x0006ee30
  000bc98c: add r0,sp,#0x1c
  000bc98e: blx 0x0006ee30
  000bc992: add r0,sp,#0x28
  000bc994: blx 0x0006ee30
  000bc998: ldr r0,[sp,#0x34]
  000bc99a: ldr.w r1,[r10,#0x0]
  000bc99e: subs r0,r1,r0
  000bc9a0: ittt eq
  000bc9a2: add.eq sp,#0x38
  000bc9a4: pop.eq.w {r8,r9,r10}
  000bc9a8: pop.eq {r4,r5,r6,r7,pc}
  000bc9aa: blx 0x0006e824
```
