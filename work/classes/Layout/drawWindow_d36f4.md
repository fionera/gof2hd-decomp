# Layout::drawWindow
elf 0xd36f4 body 108
Sig: undefined __thiscall drawWindow(Layout * this, String param_1, bool param_2)

## decompile
```c

/* Layout::drawWindow(AbyssEngine::String, bool) */

void __thiscall Layout::drawWindow(Layout *this,String *param_2,undefined4 param_3)

{
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000e3770 + 0xe370c);
  local_1c = *piVar1;
  AbyssEngine::String::String(aSStack_28,param_2,false);
  drawWindow(this,aSStack_28,0,0,**(undefined4 **)(DAT_000e3778 + 0xe3724),
             **(int **)(DAT_000e377c + 0xe3728) - *(int *)(**(int **)(DAT_000e3774 + 0xe3720) + 8),
             param_3);
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
  000e36f4: push {r4,r5,r6,r7,lr}
  000e36f6: add r7,sp,#0xc
  000e36f8: push.w r8
  000e36fc: sub sp,#0x20
  000e36fe: mov r5,r0
  000e3700: ldr r0,[0x000e3770]
  000e3702: add.w r8,sp,#0x10
  000e3706: mov r4,r2
  000e3708: add r0,pc
  000e370a: movs r2,#0x0
  000e370c: ldr r6,[r0,#0x0]
  000e370e: ldr r0,[r6,#0x0]
  000e3710: str r0,[sp,#0x1c]
  000e3712: mov r0,r8
  000e3714: blx 0x0006f028
  000e3718: ldr r0,[0x000e3774]
  000e371a: ldr r1,[0x000e3778]
  000e371c: add r0,pc
  000e371e: ldr r2,[0x000e377c]
  000e3720: add r1,pc
  000e3722: ldr r0,[r0,#0x0]
  000e3724: add r2,pc
  000e3726: ldr r1,[r1,#0x0]
  000e3728: ldr r2,[r2,#0x0]
  000e372a: ldr r0,[r0,#0x0]
  000e372c: ldr r1,[r1,#0x0]
  000e372e: ldr r2,[r2,#0x0]
  000e3730: ldr r0,[r0,#0x8]
  000e3732: subs r0,r2,r0
  000e3734: movs r2,#0x0
  000e3736: movs r3,#0x0
  000e3738: strd r1,r0,[sp,#0x0]
  000e373c: mov r0,r5
  000e373e: mov r1,r8
  000e3740: str r4,[sp,#0x8]
  000e3742: blx 0x00074e0c
  000e3746: add r0,sp,#0x10
  000e3748: blx 0x0006ee30
  000e374c: ldr r0,[sp,#0x1c]
  000e374e: ldr r1,[r6,#0x0]
  000e3750: subs r0,r1,r0
  000e3752: ittt eq
  000e3754: add.eq sp,#0x20
  000e3756: pop.eq.w r8
  000e375a: pop.eq {r4,r5,r6,r7,pc}
  000e375c: blx 0x0006e824
```
