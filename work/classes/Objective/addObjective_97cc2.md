# Objective::addObjective
elf 0x97cc2 body 46
Sig: undefined __thiscall addObjective(Objective * this, Objective * param_1)

## decompile
```c

/* Objective::addObjective(Objective*) */

Objective * __thiscall Objective::addObjective(Objective *this,Objective *param_1)

{
  Array<Objective*> *this_00;
  
  this_00 = *(Array<Objective*> **)(this + 0x10);
  if (this_00 == (Array<Objective*> *)0x0) {
    this_00 = operator_new(0xc);
    Array<Objective*>::Array(this_00);
    *(Array<Objective*> **)(this + 0x10) = this_00;
  }
  ArrayAdd<Objective*>(param_1,(Array *)this_00);
  return this;
}

```

## target disasm
```
  000a7cc2: push {r4,r5,r6,r7,lr}
  000a7cc4: add r7,sp,#0xc
  000a7cc6: push.w r11
  000a7cca: ldr r6,[r0,#0x10]
  000a7ccc: mov r5,r1
  000a7cce: mov r4,r0
  000a7cd0: cbnz r6,0x000a7ce0
  000a7cd2: movs r0,#0xc
  000a7cd4: blx 0x0006eb24
  000a7cd8: mov r6,r0
  000a7cda: blx 0x00071e9c
  000a7cde: str r6,[r4,#0x10]
  000a7ce0: mov r0,r5
  000a7ce2: mov r1,r6
  000a7ce4: blx 0x00071ea8
  000a7ce8: mov r0,r4
  000a7cea: pop.w r11
  000a7cee: pop {r4,r5,r6,r7,pc}
```
