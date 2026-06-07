# Layout::drawWindow
elf 0xd38d4 body 84
Sig: undefined __thiscall drawWindow(Layout * this, String param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* Layout::drawWindow(AbyssEngine::String, int, int, int, int) */

void __thiscall
Layout::drawWindow(Layout *this,String *param_2,undefined4 param_3,undefined4 param_4,
                  undefined4 param_5,undefined4 param_6)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  
  piVar2 = *(int **)(DAT_000e3938 + 0xe38ea);
  iVar1 = *piVar2;
  uVar3 = param_4;
  AbyssEngine::String::String((String *)&stack0xffffffd0,param_2,false);
  drawWindow(this,(String *)&stack0xffffffd0,param_3,param_4,param_5,param_6,1,uVar3);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  if (*piVar2 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e38d4: push {r4,r5,r6,r7,lr}
  000e38d6: add r7,sp,#0xc
  000e38d8: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000e38dc: mov r6,r0
  000e38de: ldr r0,[0x000e3938]
  000e38e0: add.w r8,sp,#0x10
  000e38e4: mov r5,r2
  000e38e6: add r0,pc
  000e38e8: movs r2,#0x0
  000e38ea: mov r9,r3
  000e38ec: ldr r4,[r0,#0x0]
  000e38ee: ldr r0,[r4,#0x0]
  000e38f0: str r0,[sp,#0x1c]
  000e38f2: mov r0,r8
  000e38f4: blx 0x0006f028
  000e38f8: ldrd r0,r1,[r7,#0x8]
  000e38fc: movs r2,#0x1
  000e38fe: mov r3,r9
  000e3900: stm sp,{r0,r1,r2}
  000e3904: mov r0,r6
  000e3906: mov r1,r8
  000e3908: mov r2,r5
  000e390a: blx 0x00074e0c
  000e390e: add r0,sp,#0x10
  000e3910: blx 0x0006ee30
  000e3914: ldr r0,[sp,#0x1c]
  000e3916: ldr r1,[r4,#0x0]
  000e3918: subs r0,r1,r0
  000e391a: ittt eq
  000e391c: add.eq sp,#0x20
  000e391e: pop.eq.w {r8,r9,r11}
  000e3922: pop.eq {r4,r5,r6,r7,pc}
  000e3924: blx 0x0006e824
```
