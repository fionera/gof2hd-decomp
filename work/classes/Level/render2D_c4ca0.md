# Level::render2D
elf 0xc4ca0 body 10
Sig: undefined __stdcall render2D(void)

## decompile
```c

/* Level::render2D() */

void Level::render2D(void)

{
  int in_r0;
  
  (*(code *)(DAT_001abfe4 + 0x1abfe8))(*(undefined4 *)(in_r0 + 0xec));
  return;
}

```
## target disasm
```
  000d4ca0: ldr.w r0,[r0,#0xec]
  000d4ca4: b.w 0x001abfd8
  001abfd8: bx pc
```
