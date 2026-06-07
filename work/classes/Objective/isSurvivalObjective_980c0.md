# Objective::isSurvivalObjective
elf 0x980c0 body 12
Sig: undefined __thiscall isSurvivalObjective(Objective * this)

## decompile
```c

/* Objective::isSurvivalObjective() */

bool __thiscall Objective::isSurvivalObjective(Objective *this)

{
  return *(int *)this == 3;
}

```

## target disasm
```
  000a80c0: ldr r0,[r0,#0x0]
  000a80c2: subs r0,#0x3
  000a80c4: clz r0,r0
  000a80c8: lsrs r0,r0,#0x5
  000a80ca: bx lr
```
