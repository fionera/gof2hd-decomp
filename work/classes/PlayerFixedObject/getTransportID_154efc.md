# PlayerFixedObject::getTransportID
elf 0x154efc body 6
Sig: undefined __thiscall getTransportID(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::getTransportID() */

undefined4 __thiscall PlayerFixedObject::getTransportID(PlayerFixedObject *this)

{
  return *(undefined4 *)(this + 0x1a8);
}

```

## target disasm
```
  00164efc: ldr.w r0,[r0,#0x1a8]
  00164f00: bx lr
```
