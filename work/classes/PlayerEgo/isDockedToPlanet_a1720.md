# PlayerEgo::isDockedToPlanet
elf 0xa1720 body 18
Sig: undefined __stdcall isDockedToPlanet(void)

## decompile
```c

/* PlayerEgo::isDockedToPlanet() */

bool PlayerEgo::isDockedToPlanet(void)

{
  int in_r0;
  
  return 3000 < *(int *)(in_r0 + 0x1f0);
}

```

## target disasm
```
  000b1720: ldr.w r1,[r0,#0x1f0]
  000b1724: movs r0,#0x0
  000b1726: movw r2,#0xbb8
  000b172a: cmp r1,r2
  000b172c: it gt
  000b172e: mov.gt r0,#0x1
  000b1730: bx lr
```
