# LevelScript::resetStartSequenceOver
elf 0x146030 body 10
Sig: undefined __stdcall resetStartSequenceOver(void)

## decompile
```c

/* LevelScript::resetStartSequenceOver() */

void LevelScript::resetStartSequenceOver(void)

{
  int in_r0;
  
  *(undefined4 *)(in_r0 + 0x24) = 0;
  *(undefined1 *)(in_r0 + 0x21) = 0;
  return;
}

```

## target disasm
```
  00156030: movs r1,#0x0
  00156032: str r1,[r0,#0x24]
  00156034: strb.w r1,[r0,#0x21]
  00156038: bx lr
```
