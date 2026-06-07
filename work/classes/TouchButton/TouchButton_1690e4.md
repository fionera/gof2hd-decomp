# TouchButton::TouchButton
elf 0x1690e4 body 122
Sig: undefined __thiscall TouchButton(TouchButton * this, int param_1, int param_2, String * param_3, int param_4, int param_5, uchar param_6)

## decompile
```c

/* TouchButton::TouchButton(int, int, AbyssEngine::String const&, int, int, unsigned char) */

TouchButton * __thiscall
TouchButton::TouchButton
          (TouchButton *this,int param_1,int param_2,String *param_3,int param_4,int param_5,
          uchar param_6)

{
  undefined4 uVar1;
  uint *puVar2;
  
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  puVar2 = *(uint **)(DAT_00179184 + 0x17911c);
  *(undefined4 *)(this + 8) = **(undefined4 **)(DAT_00179180 + 0x17911a);
  uVar1 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar2);
  *(undefined4 *)(this + 0xc4) = uVar1;
  init(this,(EVP_PKEY_CTX *)param_3);
  return this;
}

```

## target disasm
```
  001790e4: push {r4,r5,r6,r7,lr}
  001790e6: add r7,sp,#0xc
  001790e8: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  001790ec: add.w r8,r0,#0xc
  001790f0: mov r6,r0
  001790f2: mov r10,r3
  001790f4: mov r4,r2
  001790f6: mov r0,r8
  001790f8: mov r5,r1
  001790fa: blx 0x0006efbc
  001790fe: add.w r9,r6,#0x18
  00179102: mov r0,r9
  00179104: blx 0x0006efbc
  00179108: add.w r11,r6,#0x2c
  0017910c: mov r0,r11
  0017910e: blx 0x0006efbc
  00179112: ldr r0,[0x00179180]
  00179114: ldr r1,[0x00179184]
  00179116: add r0,pc
  00179118: add r1,pc
  0017911a: ldr r0,[r0,#0x0]
  0017911c: ldr r2,[r1,#0x0]
  0017911e: ldr r1,[r0,#0x0]
  00179120: str r1,[r6,#0x8]
  00179122: ldr r0,[r2,#0x0]
  00179124: blx 0x000764a4
  00179128: ldrd r3,r2,[r7,#0x8]
  0017912c: ldr.w r12,[r7,#0x10]
  00179130: str.w r0,[r6,#0xc4]
  00179134: movs r0,#0x44
  00179136: movs r1,#0x0
  00179138: strd r5,r4,[sp,#0x0]
  0017913c: strd r3,r2,[sp,#0x8]
  00179140: mov.w r2,#0xffffffff
  00179144: strd r1,r12,[sp,#0x10]
  00179148: mov r1,r10
  0017914a: str r0,[sp,#0x18]
  0017914c: mov r0,r6
  0017914e: movs r3,#0x4
  00179150: blx 0x00077c80
  00179154: mov r0,r6
  00179156: add sp,#0x1c
  00179158: pop.w {r8,r9,r10,r11}
  0017915c: pop {r4,r5,r6,r7,pc}
```
