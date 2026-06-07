# PlayerFixedObject::setBV
elf 0x15401c body 46
Sig: undefined __thiscall setBV(PlayerFixedObject * this, BoundingVolume * param_1)

## decompile
```c

/* PlayerFixedObject::setBV(BoundingVolume*) */

void __thiscall PlayerFixedObject::setBV(PlayerFixedObject *this,BoundingVolume *param_1)

{
  Array<BoundingVolume*> *this_00;
  
  this_00 = operator_new(0xc);
  Array<BoundingVolume*>::Array(this_00);
  *(Array<BoundingVolume*> **)(this + 0x128) = this_00;
  (*(code *)(DAT_001ac224 + 0x1ac228))(param_1,this_00);
  return;
}

```

## target disasm
```
  0016401c: push {r4,r5,r6,r7,lr}
  0016401e: add r7,sp,#0xc
  00164020: push.w r11
  00164024: mov r6,r0
  00164026: movs r0,#0xc
  00164028: mov r4,r1
  0016402a: blx 0x0006eb24
  0016402e: mov r5,r0
  00164030: blx 0x000730c0
  00164034: str.w r5,[r6,#0x128]
  00164038: mov r0,r4
  0016403a: mov r1,r5
  0016403c: pop.w r11
  00164040: pop.w {r4,r5,r6,r7,lr}
  00164044: b.w 0x001ac218
  001ac218: bx pc
```
