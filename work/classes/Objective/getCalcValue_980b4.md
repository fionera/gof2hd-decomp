# Objective::getCalcValue
elf 0x980b4 body 12
Sig: undefined __thiscall getCalcValue(Objective * this)

## decompile
```c

/* Objective::getCalcValue() */

bool __thiscall Objective::getCalcValue(Objective *this)

{
  return *(int *)this == 3;
}

```

## target disasm
```
  000a80b4: ldr r0,[r0,#0x0]
  000a80b6: subs r0,#0x3
  000a80b8: clz r0,r0
  000a80bc: lsrs r0,r0,#0x5
  000a80be: bx lr
```
