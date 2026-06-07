# PlayerEgo::isDockingToAsteroid
elf 0xa161e body 22
Sig: undefined __stdcall isDockingToAsteroid(void)

## decompile
```c

/* PlayerEgo::isDockingToAsteroid() */

bool PlayerEgo::isDockingToAsteroid(void)

{
  int in_r0;
  
  if (*(char *)(in_r0 + 0x1c0) != '\0') {
    return *(int *)(in_r0 + 0x1c4) != 1;
  }
  return false;
}

```

## target disasm
```
  000b161e: ldrb.w r1,[r0,#0x1c0]
  000b1622: cbz r1,0x000b1630
  000b1624: ldr.w r0,[r0,#0x1c4]
  000b1628: subs r0,#0x1
  000b162a: it ne
  000b162c: mov.ne r0,#0x1
  000b162e: bx lr
  000b1630: movs r0,#0x0
  000b1632: bx lr
```
