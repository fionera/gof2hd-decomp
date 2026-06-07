# Achievements::gotAllGoldMedals
elf 0x156ecc body 6
Sig: undefined __stdcall gotAllGoldMedals(void)

## decompile
```c

/* Achievements::gotAllGoldMedals() */

undefined1 Achievements::gotAllGoldMedals(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x21);
}

```

## target disasm
```
  00166ecc: ldrb.w r0,[r0,#0x21]
  00166ed0: bx lr
```
