# ScrollTouchBox::setText
elf 0x13570c body 74
Sig: undefined __thiscall setText(ScrollTouchBox * this, String param_1)

## decompile
```c

/* ScrollTouchBox::setText(AbyssEngine::String) */

void __thiscall ScrollTouchBox::setText(ScrollTouchBox *this,String *param_2)

{
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00145764 + 0x145722);
  local_1c = *piVar1;
  AbyssEngine::String::String(aSStack_28,param_2,false);
  setText(this,aSStack_28,**(undefined4 **)(DAT_00145768 + 0x145732));
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
  0014570c: push {r4,r5,r6,r7,lr}
  0014570e: add r7,sp,#0xc
  00145710: push.w r11
  00145714: sub sp,#0x10
  00145716: mov r4,r0
  00145718: ldr r0,[0x00145764]
  0014571a: mov r5,sp
  0014571c: movs r2,#0x0
  0014571e: add r0,pc
  00145720: ldr r6,[r0,#0x0]
  00145722: ldr r0,[r6,#0x0]
  00145724: str r0,[sp,#0xc]
  00145726: mov r0,r5
  00145728: blx 0x0006f028
  0014572c: ldr r0,[0x00145768]
  0014572e: add r0,pc
  00145730: ldr r0,[r0,#0x0]
  00145732: ldr r2,[r0,#0x0]
  00145734: mov r0,r4
  00145736: mov r1,r5
  00145738: blx 0x00076a50
  0014573c: mov r0,sp
  0014573e: blx 0x0006ee30
  00145742: ldr r0,[sp,#0xc]
  00145744: ldr r1,[r6,#0x0]
  00145746: subs r0,r1,r0
  00145748: ittt eq
  0014574a: add.eq sp,#0x10
  0014574c: pop.eq.w r11
  00145750: pop.eq {r4,r5,r6,r7,pc}
  00145752: blx 0x0006e824
```
