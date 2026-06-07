# PlayerEgo::setThrust
elf 0x9b86c body 6
Sig: undefined __stdcall setThrust(float param_1)

## decompile
```c

/* PlayerEgo::setThrust(float) */

void PlayerEgo::setThrust(float param_1)

{
  int in_r0;
  undefined4 in_r1;
  
  *(undefined4 *)(in_r0 + 0xbc) = in_r1;
  return;
}

```

## target disasm
```
  000ab86c: str.w r1,[r0,#0xbc]
  000ab870: bx lr
```
