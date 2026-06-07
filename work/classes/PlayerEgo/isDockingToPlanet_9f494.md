# PlayerEgo::isDockingToPlanet
elf 0x9f494 body 6
Sig: undefined __stdcall isDockingToPlanet(void)

## decompile
```c

/* PlayerEgo::isDockingToPlanet() */

undefined1 PlayerEgo::isDockingToPlanet(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x1ee);
}

```

## target disasm
```
  000af494: ldrb.w r0,[r0,#0x1ee]
  000af498: bx lr
```
