# RadioMessage::finish

ELF vaddr 0x151682, body 8 bytes
Signature: undefined __thiscall finish(RadioMessage * this)

## Ghidra decompile
```c

/* RadioMessage::finish() */

void __thiscall RadioMessage::finish(RadioMessage *this)

{
  this[0x21] = (RadioMessage)0x1;
  return;
}

```

## Target disassembly (must match)
```
  00161682: movs r1,#0x1
  00161684: strb.w r1,[r0,#0x21]
  00161688: bx lr
```

Verify: tools/try.sh work/src/finish.cpp finish 151682 32
