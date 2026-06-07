# PlayerCreature::hook
elf 0x11cd44 body 12
Sig: undefined __stdcall hook(int param_1)

## decompile
```c

/* PlayerCreature::hook(int) */

void PlayerCreature::hook(int param_1)

{
  *(undefined1 *)(param_1 + 0x129) = 1;
  (*(code *)(DAT_001ac3a4 + 0x1ac3a8))();
  return;
}

```

## target disasm
```
  0012cd44: movs r2,#0x1
  0012cd46: strb.w r2,[r0,#0x129]
  0012cd4a: b.w 0x001ac398
  001ac398: bx pc
```
