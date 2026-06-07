# AEGeometry::render
elf 0xa4a50 body 18
Sig: undefined __stdcall render(void)

## decompile
```c

/* AEGeometry::render() */

void AEGeometry::render(void)

{
  int in_r0;
  
  if (*(char *)(in_r0 + 0x48) == '\0') {
    return;
  }
  (*(code *)(DAT_001abda4 + 0x1abda8))(*(undefined4 *)(in_r0 + 0x2c),*(undefined4 *)(in_r0 + 0xc),0)
  ;
  return;
}

```

## target disasm
```
  000b4a50: ldrb.w r1,[r0,#0x48]
  000b4a54: cbz r1,0x000b4a60
  000b4a56: ldr r1,[r0,#0xc]
  000b4a58: movs r2,#0x0
  000b4a5a: ldr r0,[r0,#0x2c]
  000b4a5c: b.w 0x001abd98
  000b4a60: bx lr
```
