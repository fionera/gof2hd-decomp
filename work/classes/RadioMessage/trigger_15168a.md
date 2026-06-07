# RadioMessage::trigger
elf 0x15168a body 8
Sig: undefined __thiscall trigger(RadioMessage * this)

## decompile
```c

/* RadioMessage::trigger() */

void __thiscall RadioMessage::trigger(RadioMessage *this)

{
  this[0x20] = (RadioMessage)0x1;
  return;
}

```

## target disasm
```
  0016168a: movs r1,#0x1
  0016168c: strb.w r1,[r0,#0x20]
  00161690: bx lr
```
