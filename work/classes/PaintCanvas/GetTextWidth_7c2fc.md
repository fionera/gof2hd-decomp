# PaintCanvas::GetTextWidth
elf 0x7c2fc body 66
Sig: undefined __thiscall GetTextWidth(PaintCanvas * this, uint param_1, String * param_2, uint param_3, uint param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetTextWidth(unsigned int, AbyssEngine::String const&, unsigned int,
   unsigned int) */

undefined4 __thiscall
AbyssEngine::PaintCanvas::GetTextWidth
          (PaintCanvas *this,uint param_1,String *param_2,uint param_3,uint param_4)

{
  ushort *puVar1;
  undefined4 uVar2;
  
  if (*(uint *)(this + 0x140) <= param_1) {
    return 0;
  }
  uVar2 = *(undefined4 *)(*(int *)(this + 0x144) + param_1 * 4);
  puVar1 = String::operator_cast_to_unsigned_short_((String *)param_2);
  uVar2 = (*(code *)(DAT_001ab344 + 0x1ab348))(uVar2,puVar1,param_3,param_4 - param_3);
  return uVar2;
}

```

## target disasm
```
  0008c2fc: push {r4,r5,r6,r7,lr}
  0008c2fe: add r7,sp,#0xc
  0008c300: push.w r11
  0008c304: mov r4,r3
  0008c306: ldr.w r3,[r0,#0x140]
  0008c30a: cmp r3,r1
  0008c30c: bls 0x0008c334
  0008c30e: ldr.w r0,[r0,#0x144]
  0008c312: ldr r3,[r7,#0x8]
  0008c314: ldr.w r5,[r0,r1,lsl #0x2]
  0008c318: mov r0,r2
  0008c31a: subs r6,r3,r4
  0008c31c: blx 0x0006fa54
  0008c320: mov r1,r0
  0008c322: mov r0,r5
  0008c324: mov r2,r4
  0008c326: mov r3,r6
  0008c328: pop.w r11
  0008c32c: pop.w {r4,r5,r6,r7,lr}
  0008c330: b.w 0x001ab338
  0008c334: movs r0,#0x0
  0008c336: pop.w r11
  0008c33a: pop {r4,r5,r6,r7,pc}
  001ab338: bx pc
```
