# PlayerFighter::setBV
elf 0xdfb8a body 44
Sig: undefined __thiscall setBV(PlayerFighter * this, BoundingVolume * param_1)

## decompile
```c

/* PlayerFighter::setBV(BoundingVolume*) */

void __thiscall PlayerFighter::setBV(PlayerFighter *this,BoundingVolume *param_1)

{
  Array<BoundingVolume*> *this_00;
  
  this_00 = operator_new(0xc);
  Array<BoundingVolume*>::Array(this_00);
  *(Array<BoundingVolume*> **)(this + 0x150) = this_00;
  (*(code *)(DAT_001ac224 + 0x1ac228))(param_1,this_00);
  return;
}

```

## target disasm
```
  000efb8a: push {r4,r5,r6,r7,lr}
  000efb8c: add r7,sp,#0xc
  000efb8e: push.w r11
  000efb92: mov r6,r0
  000efb94: movs r0,#0xc
  000efb96: mov r4,r1
  000efb98: blx 0x0006eb24
  000efb9c: mov r5,r0
  000efb9e: blx 0x000730c0
  000efba2: str.w r5,[r6,#0x150]
  000efba6: mov r0,r4
  000efba8: mov r1,r5
  000efbaa: pop.w r11
  000efbae: pop.w {r4,r5,r6,r7,lr}
  000efbb2: b.w 0x001ac218
```
