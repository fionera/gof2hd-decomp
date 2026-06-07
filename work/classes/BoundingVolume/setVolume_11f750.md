# BoundingVolume::setVolume
elf 0x11f750 body 42
Sig: undefined __thiscall setVolume(BoundingVolume * this, BoundingVolume * param_1)

## decompile
```c

/* BoundingVolume::setVolume(BoundingVolume*) */

void __thiscall BoundingVolume::setVolume(BoundingVolume *this,BoundingVolume *param_1)

{
  Array<BoundingVolume*> *this_00;
  
  this_00 = operator_new(0xc);
  Array<BoundingVolume*>::Array(this_00);
  *(Array<BoundingVolume*> **)(this + 4) = this_00;
  (*(code *)(DAT_001ac224 + 0x1ac228))(param_1,this_00);
  return;
}

```

## target disasm
```
  0012f750: push {r4,r5,r6,r7,lr}
  0012f752: add r7,sp,#0xc
  0012f754: push.w r11
  0012f758: mov r6,r0
  0012f75a: movs r0,#0xc
  0012f75c: mov r4,r1
  0012f75e: blx 0x0006eb24
  0012f762: mov r5,r0
  0012f764: blx 0x000730c0
  0012f768: str r5,[r6,#0x4]
  0012f76a: mov r0,r4
  0012f76c: mov r1,r5
  0012f76e: pop.w r11
  0012f772: pop.w {r4,r5,r6,r7,lr}
  0012f776: b.w 0x001ac218
```
