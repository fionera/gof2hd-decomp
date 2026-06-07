# Layout::drawHeader
elf 0xd40a0 body 72
Sig: undefined __stdcall drawHeader(void)

## decompile
```c

/* Layout::drawHeader() */

void Layout::drawHeader(void)

{
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000e40f8 + 0xe40b6);
  local_1c = *piVar1;
  AbyssEngine::String::String(aSStack_28,(char *)(DAT_000e40fc + 0xe40c2),false);
  drawHeader();
  AbyssEngine::String::~String(aSStack_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e40a0: push {r4,r5,r6,r7,lr}
  000e40a2: add r7,sp,#0xc
  000e40a4: push.w r11
  000e40a8: sub sp,#0x10
  000e40aa: mov r4,r0
  000e40ac: ldr r0,[0x000e40f8]
  000e40ae: mov r5,sp
  000e40b0: movs r2,#0x0
  000e40b2: add r0,pc
  000e40b4: ldr r6,[r0,#0x0]
  000e40b6: ldr r0,[r6,#0x0]
  000e40b8: ldr r1,[0x000e40fc]
  000e40ba: str r0,[sp,#0xc]
  000e40bc: mov r0,r5
  000e40be: add r1,pc
  000e40c0: blx 0x0006ee18
  000e40c4: mov r0,r4
  000e40c6: mov r1,r5
  000e40c8: movs r2,#0x0
  000e40ca: blx 0x00074e6c
  000e40ce: mov r0,sp
  000e40d0: blx 0x0006ee30
  000e40d4: ldr r0,[sp,#0xc]
  000e40d6: ldr r1,[r6,#0x0]
  000e40d8: subs r0,r1,r0
  000e40da: ittt eq
  000e40dc: add.eq sp,#0x10
  000e40de: pop.eq.w r11
  000e40e2: pop.eq {r4,r5,r6,r7,pc}
  000e40e4: blx 0x0006e824
```
