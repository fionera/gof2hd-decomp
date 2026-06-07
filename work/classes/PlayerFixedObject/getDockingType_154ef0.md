# PlayerFixedObject::getDockingType
elf 0x154ef0 body 6
Sig: undefined __stdcall getDockingType(void)

## decompile
```c

/* PlayerFixedObject::getDockingType() */

undefined4 PlayerFixedObject::getDockingType(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x1a4);
}

```

## target disasm
```
  00164ef0: ldr.w r0,[r0,#0x1a4]
  00164ef4: bx lr
```
