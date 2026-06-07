# PaintCanvas::CheckString
elf 0x78194 body 46
Sig: undefined __thiscall CheckString(PaintCanvas * this, uint param_1, String * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::CheckString(unsigned int, AbyssEngine::String const&) */

void __thiscall
AbyssEngine::PaintCanvas::CheckString(PaintCanvas *this,uint param_1,String *param_2)

{
  ushort *puVar1;
  undefined4 uVar2;
  
  if (param_1 < *(uint *)(this + 0x140)) {
    uVar2 = *(undefined4 *)(*(int *)(this + 0x144) + param_1 * 4);
    puVar1 = String::operator_cast_to_unsigned_short_((String *)param_2);
    (*(code *)(DAT_001ab194 + 0x1ab198))(uVar2,puVar1,*(undefined4 *)(param_2 + 8));
    return;
  }
  return;
}

```

## target disasm
```
  00088194: push {r4,r5,r7,lr}
  00088196: add r7,sp,#0x8
  00088198: mov r4,r2
  0008819a: ldr.w r2,[r0,#0x140]
  0008819e: cmp r2,r1
  000881a0: it ls
  000881a2: pop.ls {r4,r5,r7,pc}
  000881a4: ldr.w r0,[r0,#0x144]
  000881a8: ldr.w r5,[r0,r1,lsl #0x2]
  000881ac: mov r0,r4
  000881ae: blx 0x0006fa54
  000881b2: mov r1,r0
  000881b4: ldr r2,[r4,#0x8]
  000881b6: mov r0,r5
  000881b8: pop.w {r4,r5,r7,lr}
  000881bc: b.w 0x001ab188
  001ab188: bx pc
```
