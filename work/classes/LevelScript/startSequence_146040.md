# LevelScript::startSequence
elf 0x146040 body 6
Sig: undefined __thiscall startSequence(LevelScript * this)

## decompile
```c

/* LevelScript::startSequence() */

undefined1 __thiscall LevelScript::startSequence(LevelScript *this)

{
  return (char)this->field_20;
}

```

## target disasm
```
  00156040: ldrb.w r0,[r0,#0x20]
  00156044: bx lr
```
