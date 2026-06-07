# TouchButton::TouchButton
elf 0x168f30 body 166
Sig: undefined __thiscall TouchButton(TouchButton * this, String * param_1, int param_2, int param_3, int param_4, int param_5, uchar param_6, uchar param_7, uint param_8, int param_9)

## decompile
```c

/* TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, int, unsigned char, unsigned
   char, unsigned int, int) */

TouchButton * __thiscall
TouchButton::TouchButton
          (TouchButton *this,String *param_1,int param_2,int param_3,int param_4,int param_5,
          uchar param_6,uchar param_7,uint param_8,int param_9)

{
  int iVar1;
  short sVar2;
  uint *puVar3;
  
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  iVar1 = DAT_00178ff8;
  *(int *)(this + 0xc4) = param_9;
  *(uint *)(this + 8) = param_8;
  puVar3 = *(uint **)(iVar1 + 0x178f76);
  sVar2 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar3);
  AbyssEngine::PaintCanvas::FontSetSpacing((PaintCanvas *)*puVar3,param_8,(short)param_9);
  init(this,(EVP_PKEY_CTX *)param_1);
  AbyssEngine::PaintCanvas::FontSetSpacing((PaintCanvas *)*puVar3,param_8,sVar2);
  return this;
}

```

## target disasm
```
  00178f30: push {r4,r5,r6,r7,lr}
  00178f32: add r7,sp,#0xc
  00178f34: push {r8,r9,r10,r11}
  00178f38: sub sp,#0x34
  00178f3a: add.w r4,r0,#0xc
  00178f3e: mov r6,r0
  00178f40: mov r9,r3
  00178f42: mov r8,r2
  00178f44: mov r0,r4
  00178f46: mov r5,r1
  00178f48: blx 0x0006efbc
  00178f4c: add.w r0,r6,#0x18
  00178f50: str r4,[sp,#0x28]
  00178f52: blx 0x0006efbc
  00178f56: strd r9,r5,[sp,#0x2c]
  00178f5a: mov r5,r8
  00178f5c: str r0,[sp,#0x24]
  00178f5e: add.w r0,r6,#0x2c
  00178f62: str r0,[sp,#0x20]
  00178f64: blx 0x0006efbc
  00178f68: ldrd r8,r11,[r7,#0x18]
  00178f6c: ldr r0,[0x00178ff8]
  00178f6e: str.w r11,[r6,#0xc4]
  00178f72: add r0,pc
  00178f74: str.w r8,[r6,#0x8]
  00178f78: ldr.w r10,[r0,#0x0]
  00178f7c: ldr.w r0,[r10,#0x0]
  00178f80: mov r1,r8
  00178f82: blx 0x000764a4
  00178f86: mov r9,r0
  00178f88: ldr.w r0,[r10,#0x0]
  00178f8c: sxth.w r2,r11
  00178f90: mov r1,r8
  00178f92: blx 0x00075dd8
  00178f96: ldrd r1,r0,[r7,#0x10]
  00178f9a: ldrd r3,r2,[r7,#0x8]
  00178f9e: mov.w r4,#0xffffffff
  00178fa2: strd r4,r4,[sp,#0x0]
  00178fa6: ldr r4,[sp,#0x2c]
  00178fa8: strd r4,r3,[sp,#0x8]
  00178fac: mov r3,r5
  00178fae: strd r2,r1,[sp,#0x10]
  00178fb2: mov.w r2,#0xffffffff
  00178fb6: ldr r1,[sp,#0x30]
  00178fb8: str r0,[sp,#0x18]
  00178fba: mov r0,r6
  00178fbc: blx 0x00077c80
  00178fc0: ldr.w r0,[r10,#0x0]
  00178fc4: mov r1,r8
  00178fc6: mov r2,r9
  00178fc8: blx 0x00075dd8
  00178fcc: mov r0,r6
  00178fce: add sp,#0x34
  00178fd0: pop.w {r8,r9,r10,r11}
  00178fd4: pop {r4,r5,r6,r7,pc}
```
