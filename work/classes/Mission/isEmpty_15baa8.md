# Mission::isEmpty
elf 0x15baa8 body 12
Sig: undefined __stdcall isEmpty(void)

## decompile
```c

/* Mission::isEmpty() */

bool Mission::isEmpty(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0xc) == -1;
}

```

## target disasm
```
  0016baa8: ldr r0,[r0,#0xc]
  0016baaa: adds r0,#0x1
  0016baac: clz r0,r0
  0016bab0: lsrs r0,r0,#0x5
  0016bab2: bx lr
```
