# TouchButton::replaceTextKeepSize
elf 0x169304 body 68
Sig: undefined __thiscall replaceTextKeepSize(TouchButton * this, String * param_1)

## decompile
```c

/* TouchButton::replaceTextKeepSize(AbyssEngine::String const&) */

void __thiscall TouchButton::replaceTextKeepSize(TouchButton *this,String *param_1)

{
  int iVar1;
  int iVar2;
  
  AbyssEngine::String::operator=((String *)(this + 0xc),param_1);
  if (*(int *)(this + 0x70) == 10) {
    iVar2 = *(int *)(this + 0x90);
    iVar1 = AbyssEngine::PaintCanvas::GetTextWidth
                      (**(uint **)(DAT_00179348 + 0x179326),*(String **)(this + 8));
    *(int *)(this + 0xa4) = iVar2 / 2 - iVar1 / 2;
  }
  return;
}

```

## target disasm
```
  00179304: push {r4,r5,r6,r7,lr}
  00179306: add r7,sp,#0xc
  00179308: push.w r11
  0017930c: mov r4,r0
  0017930e: adds r0,#0xc
  00179310: mov r5,r1
  00179312: blx 0x0006f2b0
  00179316: ldr r0,[r4,#0x70]
  00179318: cmp r0,#0xa
  0017931a: bne 0x00179342
  0017931c: ldr r0,[0x00179348]
  0017931e: mov r2,r5
  00179320: ldr r1,[r4,#0x8]
  00179322: add r0,pc
  00179324: ldr.w r6,[r4,#0x90]
  00179328: ldr r0,[r0,#0x0]
  0017932a: ldr r0,[r0,#0x0]
  0017932c: blx 0x0006faa8
  00179330: add.w r1,r6,r6, lsr #0x1f
  00179334: add.w r0,r0,r0, lsr #0x1f
  00179338: asrs r1,r1,#0x1
  0017933a: sub.w r0,r1,r0, asr #0x1
  0017933e: str.w r0,[r4,#0xa4]
  00179342: pop.w r11
  00179346: pop {r4,r5,r6,r7,pc}
```
