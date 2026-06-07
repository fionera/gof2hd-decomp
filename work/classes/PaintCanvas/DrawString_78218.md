# PaintCanvas::DrawString
elf 0x78218 body 90
Sig: undefined __stdcall DrawString(uint param_1, String * param_2, int param_3, int param_4, bool param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawString(unsigned int, AbyssEngine::String const&, int, int, bool) */

undefined8
AbyssEngine::PaintCanvas::DrawString
          (uint param_1,String *param_2,int param_3,int param_4,bool param_5)

{
  ushort *puVar1;
  int unaff_r4;
  ImageFont *pIVar2;
  uint unaff_r5;
  undefined3 in_stack_00000001;
  bool in_stack_00000004;
  
  if (param_2 < *(String **)(param_1 + 0x140)) {
    SetTexture(param_1,*(uint *)(*(int *)(*(int *)(param_1 + 0x144) + (int)param_2 * 4) + 8));
    pIVar2 = *(ImageFont **)(*(int *)(param_1 + 0x144) + (int)param_2 * 4);
    puVar1 = String::operator_cast_to_unsigned_short_((String *)param_3);
    ImageFontDrawString(pIVar2,puVar1,*(uint *)(param_3 + 8),param_4,_param_5,(PaintCanvas *)param_1
                        ,*(Engine **)(param_1 + 0x34),in_stack_00000004);
    unaff_r4 = _param_5;
    unaff_r5 = param_1;
  }
  return CONCAT44(unaff_r5,unaff_r4);
}

```

## target disasm
```
  00088218: push {r4,r5,r6,r7,lr}
  0008821a: add r7,sp,#0xc
  0008821c: push {r4,r5,r6,r7,r8,r9,r10}
  00088220: mov r5,r0
  00088222: ldr.w r0,[r0,#0x140]
  00088226: mov r8,r3
  00088228: mov r6,r2
  0008822a: mov r4,r1
  0008822c: cmp r0,r1
  0008822e: bls 0x0008826c
  00088230: ldrd r10,r9,[r7,#0x8]
  00088234: mov.w r2,#0xffffffff
  00088238: ldr.w r0,[r5,#0x144]
  0008823c: ldr.w r0,[r0,r4,lsl #0x2]
  00088240: ldr r1,[r0,#0x8]
  00088242: mov r0,r5
  00088244: blx 0x0006fa6c
  00088248: ldr.w r0,[r5,#0x144]
  0008824c: ldr.w r4,[r0,r4,lsl #0x2]
  00088250: mov r0,r6
  00088252: blx 0x0006fa54
  00088256: ldr r2,[r6,#0x8]
  00088258: mov r1,r0
  0008825a: ldr r0,[r5,#0x34]
  0008825c: mov r3,r8
  0008825e: strd r10,r5,[sp,#0x0]
  00088262: strd r0,r9,[sp,#0x8]
  00088266: mov r0,r4
  00088268: blx 0x0006f520
  0008826c: pop.w {r0,r1,r2,r3,r8,r9,r10}
  00088270: pop {r4,r5,r6,r7,pc}
```
