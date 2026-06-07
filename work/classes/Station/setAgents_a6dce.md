# Station::setAgents
elf 0xa6dce body 36
Sig: undefined __thiscall setAgents(Station * this, Array * param_1)

## decompile
```c

/* Station::setAgents(Array<Agent*>*) */

void __thiscall Station::setAgents(Station *this,Array *param_1)

{
  Array *pAVar1;
  void *pvVar2;
  
  pAVar1 = *(Array **)(this + 0x30);
  if (pAVar1 != param_1) {
    if (pAVar1 != (Array *)0x0) {
      ArrayReleaseClasses<Agent*>(pAVar1);
      if (*(Array<Agent*> **)(this + 0x30) != (Array<Agent*> *)0x0) {
        pvVar2 = (void *)Array<Agent*>::~Array(*(Array<Agent*> **)(this + 0x30));
        operator_delete(pvVar2);
      }
    }
    *(Array **)(this + 0x30) = param_1;
  }
  return;
}

```

## target disasm
```
  000b6dce: push {r4,r5,r7,lr}
  000b6dd0: add r7,sp,#0x8
  000b6dd2: mov r5,r0
  000b6dd4: ldr r0,[r0,#0x30]
  000b6dd6: mov r4,r1
  000b6dd8: cmp r0,r1
  000b6dda: beq 0x000b6df0
  000b6ddc: cbz r0,0x000b6dee
  000b6dde: blx 0x000734d4
  000b6de2: ldr r0,[r5,#0x30]
  000b6de4: cbz r0,0x000b6dee
  000b6de6: blx 0x0007345c
  000b6dea: blx 0x0006eb48
  000b6dee: str r4,[r5,#0x30]
  000b6df0: pop {r4,r5,r7,pc}
```
