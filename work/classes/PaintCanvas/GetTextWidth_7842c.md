# PaintCanvas::GetTextWidth
elf 0x7842c body 48
Sig: undefined __stdcall GetTextWidth(uint param_1, String * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetTextWidth(unsigned int, AbyssEngine::String const&) */

undefined4 AbyssEngine::PaintCanvas::GetTextWidth(uint param_1,String *param_2)

{
  ushort *puVar1;
  String *in_r2;
  undefined4 uVar2;
  
  if (*(String **)(param_1 + 0x140) <= param_2) {
    return 0;
  }
  uVar2 = *(undefined4 *)(*(int *)(param_1 + 0x144) + (int)param_2 * 4);
  puVar1 = String::operator_cast_to_unsigned_short_(in_r2);
  uVar2 = (*(code *)(DAT_001ab1b4 + 0x1ab1b8))(uVar2,puVar1,*(undefined4 *)(in_r2 + 8));
  return uVar2;
}

```

## target disasm
```
  0008842c: push {r4,r5,r7,lr}
  0008842e: add r7,sp,#0x8
  00088430: mov r4,r2
  00088432: ldr.w r2,[r0,#0x140]
  00088436: cmp r2,r1
  00088438: bls 0x00088456
  0008843a: ldr.w r0,[r0,#0x144]
  0008843e: ldr.w r5,[r0,r1,lsl #0x2]
  00088442: mov r0,r4
  00088444: blx 0x0006fa54
  00088448: mov r1,r0
  0008844a: ldr r2,[r4,#0x8]
  0008844c: mov r0,r5
  0008844e: pop.w {r4,r5,r7,lr}
  00088452: b.w 0x001ab1a8
  00088456: movs r0,#0x0
  00088458: pop {r4,r5,r7,pc}
  001ab1a8: bx pc
```
