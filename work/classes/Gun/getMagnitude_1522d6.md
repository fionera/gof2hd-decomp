# Gun::getMagnitude
elf 0x1522d6 body 6
Sig: undefined __stdcall getMagnitude(void)

## decompile
```c

/* Gun::getMagnitude() */

undefined4 Gun::getMagnitude(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x100);
}

```

## target disasm
```
  001622d6: ldr.w r0,[r0,#0x100]
  001622da: bx lr
```
