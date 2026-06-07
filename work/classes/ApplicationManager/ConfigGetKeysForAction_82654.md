# ApplicationManager::ConfigGetKeysForAction
elf 0x82654 body 126
Sig: undefined __stdcall ConfigGetKeysForAction(longlong param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::ConfigGetKeysForAction(long long) */

Array * AbyssEngine::ApplicationManager::ConfigGetKeysForAction(longlong param_1)

{
  int iVar1;
  String *this;
  int in_r2;
  int in_r3;
  int iVar2;
  uint uVar3;
  Array *pAVar4;
  
  iVar1 = (int)param_1;
  iVar2 = 0;
  pAVar4 = (Array *)0x0;
  for (uVar3 = 0; uVar3 < *(uint *)(iVar1 + 0x88); uVar3 = uVar3 + 2) {
    if (*(int *)(*(int *)(iVar1 + 0x8c) + iVar2 + 4) == in_r3 &&
        *(int *)(*(int *)(iVar1 + 0x8c) + iVar2) == in_r2) {
      if (pAVar4 == (Array *)0x0) {
        pAVar4 = operator_new(0xc);
        Array<AbyssEngine::String*>::Array();
      }
      this = operator_new(0xc);
      String::String(this,(String *)
                          (*(int *)(iVar1 + 0x10) +
                           *(int *)(*(int *)(iVar1 + 0x8c) + iVar2 + 8) * 0x10 + 4),false);
      ArrayAdd<AbyssEngine::String*>(this,pAVar4);
    }
    iVar2 = iVar2 + 0x10;
  }
  return pAVar4;
}

```

## target disasm
```
  00092654: push {r4,r5,r6,r7,lr}
  00092656: add r7,sp,#0xc
  00092658: push {r7,r8,r9,r10,r11}
  0009265c: mov r8,r3
  0009265e: mov r9,r2
  00092660: mov r11,r0
  00092662: movs r4,#0x0
  00092664: mov.w r10,#0x0
  00092668: movs r6,#0x0
  0009266a: b 0x000926c2
  0009266c: ldr.w r0,[r11,#0x8c]
  00092670: ldr r1,[r0,r4]
  00092672: add r0,r4
  00092674: ldr r0,[r0,#0x4]
  00092676: eor.w r1,r1,r9
  0009267a: eor.w r0,r0,r8
  0009267e: orrs r0,r1
  00092680: bne 0x000926be
  00092682: cmp.w r10,#0x0
  00092686: bne 0x00092694
  00092688: movs r0,#0xc
  0009268a: blx 0x0006eb24
  0009268e: mov r10,r0
  00092690: blx 0x0006f628
  00092694: movs r0,#0xc
  00092696: blx 0x0006eb24
  0009269a: ldr.w r1,[r11,#0x8c]
  0009269e: mov r5,r0
  000926a0: ldr.w r0,[r11,#0x10]
  000926a4: add r1,r4
  000926a6: ldr r1,[r1,#0x8]
  000926a8: add.w r0,r0,r1, lsl #0x4
  000926ac: adds r1,r0,#0x4
  000926ae: mov r0,r5
  000926b0: movs r2,#0x0
  000926b2: blx 0x0006f028
  000926b6: mov r0,r5
  000926b8: mov r1,r10
  000926ba: blx 0x0006f634
  000926be: adds r4,#0x10
  000926c0: adds r6,#0x2
  000926c2: ldr.w r0,[r11,#0x88]
  000926c6: cmp r6,r0
  000926c8: bcc 0x0009266c
  000926ca: mov r0,r10
  000926cc: pop.w {r3,r8,r9,r10,r11}
  000926d0: pop {r4,r5,r6,r7,pc}
```
