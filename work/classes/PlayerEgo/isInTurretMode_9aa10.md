# PlayerEgo::isInTurretMode
elf 0x9aa10 body 6
Sig: undefined __stdcall isInTurretMode(void)

## decompile
```c

/* PlayerEgo::isInTurretMode() */

undefined1 PlayerEgo::isInTurretMode(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x1a0);
}

```

## target disasm
```
  000aaa10: ldrb.w r0,[r0,#0x1a0]
  000aaa14: bx lr
```
