# TouchButton::TouchButton
elf 0x1684f0 body 126
Sig: undefined __thiscall TouchButton(TouchButton * this, String * param_1, int param_2, int param_3, int param_4, uchar param_5)

## decompile
```c

/* TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, unsigned char) */

TouchButton * __thiscall
TouchButton::TouchButton
          (TouchButton *this,String *param_1,int param_2,int param_3,int param_4,uchar param_5)

{
  undefined4 uVar1;
  uint *puVar2;
  
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  puVar2 = *(uint **)(DAT_00178594 + 0x17852c);
  *(undefined4 *)(this + 8) = **(undefined4 **)(DAT_00178590 + 0x17852a);
  uVar1 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar2);
  *(undefined4 *)(this + 0xc4) = uVar1;
  init(this,(EVP_PKEY_CTX *)param_1);
  return this;
}

```

## target disasm
```
  001784f0: push {r4,r5,r6,r7,lr}
  001784f2: add r7,sp,#0xc
  001784f4: push {r8,r9,r10,r11}
  001784f8: sub sp,#0x24
  001784fa: add.w r4,r0,#0xc
  001784fe: mov r6,r0
  00178500: mov r8,r3
  00178502: mov r10,r2
  00178504: mov r0,r4
  00178506: mov r11,r1
  00178508: blx 0x0006efbc
  0017850c: add.w r9,r6,#0x18
  00178510: mov r0,r9
  00178512: str r4,[sp,#0x20]
  00178514: blx 0x0006efbc
  00178518: add.w r5,r6,#0x2c
  0017851c: mov r0,r5
  0017851e: blx 0x0006efbc
  00178522: ldr r0,[0x00178590]
  00178524: ldr r1,[0x00178594]
  00178526: add r0,pc
  00178528: add r1,pc
  0017852a: ldr r0,[r0,#0x0]
  0017852c: ldr r2,[r1,#0x0]
  0017852e: ldr r1,[r0,#0x0]
  00178530: str r1,[r6,#0x8]
  00178532: ldr r0,[r2,#0x0]
  00178534: blx 0x000764a4
  00178538: ldrd r2,r1,[r7,#0x8]
  0017853c: str.w r0,[r6,#0xc4]
  00178540: mov.w r4,#0xffffffff
  00178544: movs r0,#0x44
  00178546: movs r3,#0x0
  00178548: strd r4,r4,[sp,#0x0]
  0017854c: strd r8,r2,[sp,#0x8]
  00178550: mov.w r2,#0xffffffff
  00178554: strd r3,r1,[sp,#0x10]
  00178558: mov r1,r11
  0017855a: str r0,[sp,#0x18]
  0017855c: mov r0,r6
  0017855e: mov r3,r10
  00178560: blx 0x00077c80
  00178564: mov r0,r6
  00178566: add sp,#0x24
  00178568: pop.w {r8,r9,r10,r11}
  0017856c: pop {r4,r5,r6,r7,pc}
```
