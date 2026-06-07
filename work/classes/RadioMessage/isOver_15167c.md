# RadioMessage::isOver
elf 0x15167c body 6
Sig: undefined __stdcall isOver(void)

## decompile
```c

/* RadioMessage::isOver() */

undefined1 RadioMessage::isOver(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x21);
}

```

## target disasm
```
  0016167c: ldrb.w r0,[r0,#0x21]
  00161680: bx lr
```
