# Layout::formatCredits
elf 0xd34d8 body 74
Sig: undefined __stdcall formatCredits(int param_1)

## decompile
```c

/* Layout::formatCredits(int) */

void Layout::formatCredits(int param_1)

{
  int *piVar1;
  String aSStack_2c [12];
  String aSStack_20 [12];
  int local_14;
  
  piVar1 = *(int **)(DAT_000e353c + 0xe34e6);
  local_14 = *piVar1;
  formatNumber((int)aSStack_20);
  AbyssEngine::String::String(aSStack_2c,(char *)(DAT_000e3540 + 0xe34f6),false);
  AbyssEngine::operator+((String *)param_1,aSStack_20);
  AbyssEngine::String::~String(aSStack_2c);
  AbyssEngine::String::~String(aSStack_20);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e34d8: push {r4,r5,r7,lr}
  000e34da: add r7,sp,#0x8
  000e34dc: sub sp,#0x20
  000e34de: mov r4,r0
  000e34e0: ldr r0,[0x000e353c]
  000e34e2: add r0,pc
  000e34e4: ldr r5,[r0,#0x0]
  000e34e6: ldr r0,[r5,#0x0]
  000e34e8: str r0,[sp,#0x1c]
  000e34ea: add r0,sp,#0x10
  000e34ec: blx 0x00074df4
  000e34f0: ldr r1,[0x000e3540]
  000e34f2: add r1,pc
  000e34f4: add r0,sp,#0x4
  000e34f6: movs r2,#0x0
  000e34f8: blx 0x0006ee18
  000e34fc: add r1,sp,#0x10
  000e34fe: add r2,sp,#0x4
  000e3500: mov r0,r4
  000e3502: blx 0x0006ef98
  000e3506: add r0,sp,#0x4
  000e3508: blx 0x0006ee30
  000e350c: add r0,sp,#0x10
  000e350e: blx 0x0006ee30
  000e3512: ldr r0,[sp,#0x1c]
  000e3514: ldr r1,[r5,#0x0]
  000e3516: subs r0,r1,r0
  000e3518: itt eq
  000e351a: add.eq sp,#0x20
  000e351c: pop.eq {r4,r5,r7,pc}
  000e351e: blx 0x0006e824
```
