# GameText::getLanguage
elf 0x81790 body 12
Sig: undefined __stdcall getLanguage(void)

## decompile
```c

/* GameText::getLanguage() */

int GameText::getLanguage(void)

{
  return (int)**(short **)(DAT_0009179c + 0x91796);
}

```

## target disasm
```
  00091790: ldr r0,[0x0009179c]
  00091792: add r0,pc
  00091794: ldr r0,[r0,#0x0]
  00091796: ldrsh.w r0,[r0,#0x0]
  0009179a: bx lr
```
