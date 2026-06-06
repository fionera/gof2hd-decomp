# Level::killWanted
elf 0xc64f0 body 22
Sig: undefined __stdcall killWanted(int param_1)

## decompile
```c

/* Level::killWanted(int) */

void Level::killWanted(int param_1)

{
  if (*(char *)(param_1 + 0x29d) == '\0') {
    *(undefined1 *)(param_1 + 0x29d) = 1;
    (*(code *)(DAT_001ac024 + 0x1ac028))(param_1,0x11,0);
    return;
  }
  return;
}

```
## target disasm
```
  000d64f0: ldrb.w r1,[r0,#0x29d]
  000d64f4: cbz r1,0x000d64f8
  000d64f6: bx lr
  000d64f8: movs r1,#0x1
  000d64fa: movs r2,#0x0
  000d64fc: strb.w r1,[r0,#0x29d]
  000d6500: movs r1,#0x11
  000d6502: b.w 0x001ac018
```
