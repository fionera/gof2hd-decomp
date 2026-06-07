# TouchButton::TouchButton
elf 0x168e88 body 124
Sig: undefined __thiscall TouchButton(TouchButton * this, String * param_1, int param_2, int param_3, int param_4, int param_5, uchar param_6, uchar param_7)

## decompile
```c

/* TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, int, unsigned char, unsigned
   char) */

TouchButton * __thiscall
TouchButton::TouchButton
          (TouchButton *this,String *param_1,int param_2,int param_3,int param_4,int param_5,
          uchar param_6,uchar param_7)

{
  undefined4 uVar1;
  uint *puVar2;
  
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  puVar2 = *(uint **)(DAT_00178f2c + 0x178ec0);
  *(undefined4 *)(this + 8) = **(undefined4 **)(DAT_00178f28 + 0x178ebe);
  uVar1 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar2);
  *(undefined4 *)(this + 0xc4) = uVar1;
  init(this,(EVP_PKEY_CTX *)param_1);
  return this;
}

```

## target disasm
```
  00178e88: push {r4,r5,r6,r7,lr}
  00178e8a: add r7,sp,#0xc
  00178e8c: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00178e90: add.w r8,r0,#0xc
  00178e94: mov r6,r0
  00178e96: mov r4,r3
  00178e98: mov r10,r2
  00178e9a: mov r0,r8
  00178e9c: mov r11,r1
  00178e9e: blx 0x0006efbc
  00178ea2: add.w r9,r6,#0x18
  00178ea6: mov r0,r9
  00178ea8: blx 0x0006efbc
  00178eac: add.w r5,r6,#0x2c
  00178eb0: mov r0,r5
  00178eb2: blx 0x0006efbc
  00178eb6: ldr r0,[0x00178f28]
  00178eb8: ldr r1,[0x00178f2c]
  00178eba: add r0,pc
  00178ebc: add r1,pc
  00178ebe: ldr r0,[r0,#0x0]
  00178ec0: ldr r2,[r1,#0x0]
  00178ec2: ldr r1,[r0,#0x0]
  00178ec4: str r1,[r6,#0x8]
  00178ec6: ldr r0,[r2,#0x0]
  00178ec8: blx 0x000764a4
  00178ecc: ldrd r2,r12,[r7,#0x10]
  00178ed0: ldrd r1,r3,[r7,#0x8]
  00178ed4: str.w r0,[r6,#0xc4]
  00178ed8: mov.w r0,#0xffffffff
  00178edc: strd r0,r0,[sp,#0x0]
  00178ee0: mov r0,r6
  00178ee2: strd r4,r1,[sp,#0x8]
  00178ee6: mov r1,r11
  00178ee8: strd r3,r2,[sp,#0x10]
  00178eec: mov.w r2,#0xffffffff
  00178ef0: mov r3,r10
  00178ef2: str.w r12,[sp,#0x18]
  00178ef6: blx 0x00077c80
  00178efa: mov r0,r6
  00178efc: add sp,#0x1c
  00178efe: pop.w {r8,r9,r10,r11}
  00178f02: pop {r4,r5,r6,r7,pc}
```
