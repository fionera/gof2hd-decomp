# BluePrint::isEmpty
elf 0x17736a body 10
Sig: undefined __thiscall isEmpty(BluePrint * this)

## decompile
```c

/* BluePrint::isEmpty() */

bool __thiscall BluePrint::isEmpty(BluePrint *this)

{
  return *(int *)(this + 4) == 0;
}

```

## target disasm
```
  0018736a: ldr r0,[r0,#0x4]
  0018736c: clz r0,r0
  00187370: lsrs r0,r0,#0x5
  00187372: bx lr
```
