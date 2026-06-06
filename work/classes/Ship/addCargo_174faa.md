# Ship::addCargo
elf 0x174faa body 50
Sig: undefined __stdcall addCargo(Item * param_1)

## decompile
```c

/* Ship::addCargo(Item*) */

void Ship::addCargo(Item *param_1)

{
  Array<Item*> *this;
  Item *in_r1;
  
  this = operator_new(0xc);
  Array<Item*>::Array(this);
  ArrayAdd<Item*>(in_r1,(Array *)this);
  (*(code *)(DAT_001ac744 + 0x1ac748))(param_1,this);
  return;
}

```
## target disasm
```
  00184faa: push {r4,r5,r6,r7,lr}
  00184fac: add r7,sp,#0xc
  00184fae: push.w r11
  00184fb2: mov r4,r0
  00184fb4: movs r0,#0xc
  00184fb6: mov r6,r1
  00184fb8: blx 0x0006eb24
  00184fbc: mov r5,r0
  00184fbe: blx 0x00071860
  00184fc2: mov r0,r6
  00184fc4: mov r1,r5
  00184fc6: blx 0x000718b4
  00184fca: mov r0,r4
  00184fcc: mov r1,r5
  00184fce: pop.w r11
  00184fd2: pop.w {r4,r5,r6,r7,lr}
  00184fd6: b.w 0x001ac738
  001ac738: bx pc
```
