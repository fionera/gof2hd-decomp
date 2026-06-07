# PlayerEgo::isBoostRefreshed
elf 0x9ae80 body 34
Sig: undefined __stdcall isBoostRefreshed(void)

## decompile
```c

/* PlayerEgo::isBoostRefreshed() */

undefined4 PlayerEgo::isBoostRefreshed(void)

{
  int in_r0;
  
  if (((*(char *)(in_r0 + 0x13c) == '\0') && (*(char *)(in_r0 + 0x146) != '\0')) &&
     (-1 < *(int *)(in_r0 + 0x138))) {
    return 1;
  }
  return 0;
}

```

## target disasm
```
  000aae80: ldrb.w r1,[r0,#0x13c]
  000aae84: cbz r1,0x000aae8a
  000aae86: movs r0,#0x0
  000aae88: bx lr
  000aae8a: ldrb.w r1,[r0,#0x146]
  000aae8e: cmp r1,#0x0
  000aae90: beq 0x000aae86
  000aae92: ldr.w r0,[r0,#0x138]
  000aae96: cmp.w r0,#0xffffffff
  000aae9a: itt gt
  000aae9c: mov.gt r0,#0x1
  000aae9e: bx.gt lr
  000aaea0: b 0x000aae86
```
