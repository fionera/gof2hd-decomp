# RadioMessage::isTriggered
elf 0x151676 body 6
Sig: undefined __thiscall isTriggered(RadioMessage * this)

## decompile
```c

/* RadioMessage::isTriggered() */

RadioMessage __thiscall RadioMessage::isTriggered(RadioMessage *this)

{
  return this[0x20];
}

```

## target disasm
```
  00161676: ldrb.w r0,[r0,#0x20]
  0016167a: bx lr
```
