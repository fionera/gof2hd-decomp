# ChoiceWindow::set
elf 0x146d5c body 52
Sig: undefined __thiscall set(ChoiceWindow * this, String * param_1, bool param_2)

## decompile
```c

/* ChoiceWindow::set(AbyssEngine::String const&, bool) */

void __thiscall ChoiceWindow::set(ChoiceWindow *this,String *param_1,bool param_2)

{
  undefined4 uVar1;
  
  uVar1 = GameText::getText(**(int **)(DAT_00156d90 + 0x156d72));
  (*(code *)(DAT_001ac454 + 0x1ac458))(this,uVar1,param_1,param_2);
  return;
}

```

## target disasm
```
  00156d5c: push {r4,r5,r6,r7,lr}
  00156d5e: add r7,sp,#0xc
  00156d60: push.w r11
  00156d64: mov r6,r0
  00156d66: ldr r0,[0x00156d90]
  00156d68: mov r5,r1
  00156d6a: mov.w r1,#0x186
  00156d6e: add r0,pc
  00156d70: mov r4,r2
  00156d72: ldr r0,[r0,#0x0]
  00156d74: ldr r0,[r0,#0x0]
  00156d76: blx 0x00072f70
  00156d7a: mov r1,r0
  00156d7c: mov r0,r6
  00156d7e: mov r2,r5
  00156d80: mov r3,r4
  00156d82: pop.w r11
  00156d86: pop.w {r4,r5,r6,r7,lr}
  00156d8a: b.w 0x001ac448
  001ac448: bx pc
```
