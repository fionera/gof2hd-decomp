# RadioMessage::reset
elf 0x1515bc body 8
Sig: undefined __thiscall reset(RadioMessage * this)

## decompile
```c

/* RadioMessage::reset() */

void __thiscall RadioMessage::reset(RadioMessage *this)

{
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined2 *)(this + 0x20) = 0;
  return;
}

```

## target disasm
```
  001615bc: movs r1,#0x0
  001615be: str r1,[r0,#0x24]
  001615c0: strh r1,[r0,#0x20]
  001615c2: bx lr
```
