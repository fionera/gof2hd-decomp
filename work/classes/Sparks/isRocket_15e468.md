# Sparks::isRocket
elf 0x15e468 body 12
Sig: undefined __thiscall isRocket(Sparks * this)

## decompile
```c

/* Sparks::isRocket() */

bool __thiscall Sparks::isRocket(Sparks *this)

{
  return *(int *)(this + 0x14) == 1;
}

```

## target disasm
```
  0016e468: ldr r0,[r0,#0x14]
  0016e46a: subs r0,#0x1
  0016e46c: clz r0,r0
  0016e470: lsrs r0,r0,#0x5
  0016e472: bx lr
```
