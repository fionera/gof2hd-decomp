# SpacePoint::giveFree
elf 0x1603f0 body 6
Sig: undefined __stdcall giveFree(void)

## decompile
```c

/* SpacePoint::giveFree() */

void SpacePoint::giveFree(void)

{
  int in_r0;
  
  *(undefined1 *)(in_r0 + 0x1c) = 1;
  return;
}

```

## target disasm
```
  001703f0: movs r1,#0x1
  001703f2: strb r1,[r0,#0x1c]
  001703f4: bx lr
```
