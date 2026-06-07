# SpacePoint::take
elf 0x1603ea body 6
Sig: undefined __stdcall take(void)

## decompile
```c

/* SpacePoint::take() */

void SpacePoint::take(void)

{
  int in_r0;
  
  *(undefined1 *)(in_r0 + 0x1c) = 0;
  return;
}

```

## target disasm
```
  001703ea: movs r1,#0x0
  001703ec: strb r1,[r0,#0x1c]
  001703ee: bx lr
```
