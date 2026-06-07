# PlayerEgo::isDockingToDockingPoint
elf 0xa069c body 22
Sig: undefined __stdcall isDockingToDockingPoint(void)

## decompile
```c

/* PlayerEgo::isDockingToDockingPoint() */

bool PlayerEgo::isDockingToDockingPoint(void)

{
  int in_r0;
  
  if (*(char *)(in_r0 + 0x356) != '\0') {
    return *(int *)(in_r0 + 0x1c4) != 1;
  }
  return false;
}

```

## target disasm
```
  000b069c: ldrb.w r1,[r0,#0x356]
  000b06a0: cbz r1,0x000b06ae
  000b06a2: ldr.w r0,[r0,#0x1c4]
  000b06a6: subs r0,#0x1
  000b06a8: it ne
  000b06aa: mov.ne r0,#0x1
  000b06ac: bx lr
  000b06ae: movs r0,#0x0
  000b06b0: bx lr
```
