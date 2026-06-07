# LevelScript::startSequenceOver
elf 0x14603a body 6
Sig: undefined __stdcall startSequenceOver(void)

## decompile
```c

/* LevelScript::startSequenceOver() */

undefined1 LevelScript::startSequenceOver(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x21);
}

```

## target disasm
```
  0015603a: ldrb.w r0,[r0,#0x21]
  0015603e: bx lr
```
