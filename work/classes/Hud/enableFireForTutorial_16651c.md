# Hud::enableFireForTutorial
elf 0x16651c body 6
Sig: undefined __stdcall enableFireForTutorial(bool param_1)

## decompile
```c

/* Hud::enableFireForTutorial(bool) */

void Hud::enableFireForTutorial(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x4a5) = in_r1;
  return;
}

```

## target disasm
```
  0017651c: strb.w r1,[r0,#0x4a5]
  00176520: bx lr
```
