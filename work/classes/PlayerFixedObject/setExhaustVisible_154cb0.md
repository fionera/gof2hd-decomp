# PlayerFixedObject::setExhaustVisible
elf 0x154cb0 body 42
Sig: undefined __thiscall setExhaustVisible(PlayerFixedObject * this, bool param_1)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* PlayerFixedObject::setExhaustVisible(bool) */

void __thiscall PlayerFixedObject::setExhaustVisible(PlayerFixedObject *this,bool param_1)

{
  undefined4 uVar1;
  
  if ((*(int *)(this + 8) != 0) && (*(int *)(*(int *)(this + 8) + 0x14) != -1)) {
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(_FUN_00164cd8 + 0x164cc6));
    (*(code *)(DAT_001ac1d4 + 0x1ac1d8))(uVar1,param_1);
    return;
  }
  return;
}

```

## target disasm
```
  00164cb0: push {r4,r6,r7,lr}
  00164cb2: add r7,sp,#0x8
  00164cb4: ldr r0,[r0,#0x8]
  00164cb6: mov r4,r1
  00164cb8: cbz r0,0x00164cd6
  00164cba: ldr r1,[r0,#0x14]
  00164cbc: adds r0,r1,#0x1
  00164cbe: beq 0x00164cd6
  00164cc0: ldr r0,[0x00164cd8]
  00164cc2: add r0,pc
  00164cc4: ldr r0,[r0,#0x0]
  00164cc6: ldr r0,[r0,#0x0]
  00164cc8: blx 0x00072088
  00164ccc: mov r1,r4
  00164cce: pop.w {r4,r6,r7,lr}
  00164cd2: b.w 0x001ac1c8
  00164cd6: pop {r4,r6,r7,pc}
  001ac1c8: bx pc
```
