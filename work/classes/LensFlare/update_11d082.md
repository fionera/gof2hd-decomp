# LensFlare::update
elf 0x11d082 body 6
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* LensFlare::update(int) */

void LensFlare::update(int param_1)

{
  *(undefined4 *)param_1 = 0;
  return;
}

```

## target disasm
```
  0012d082: movs r1,#0x0
  0012d084: str r1,[r0,#0x0]
  0012d086: bx lr
```
