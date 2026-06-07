# PlayerEgo::isInRocketControl
elf 0x9ade4 body 12
Sig: undefined __stdcall isInRocketControl(void)

## decompile
```c

/* PlayerEgo::isInRocketControl() */

bool PlayerEgo::isInRocketControl(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x194) != 0;
}

```

## target disasm
```
  000aade4: ldr.w r0,[r0,#0x194]
  000aade8: cmp r0,#0x0
  000aadea: it ne
  000aadec: mov.ne r0,#0x1
  000aadee: bx lr
```
