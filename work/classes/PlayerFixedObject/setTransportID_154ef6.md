# PlayerFixedObject::setTransportID
elf 0x154ef6 body 6
Sig: undefined __thiscall setTransportID(PlayerFixedObject * this, int param_1)

## decompile
```c

/* PlayerFixedObject::setTransportID(int) */

void __thiscall PlayerFixedObject::setTransportID(PlayerFixedObject *this,int param_1)

{
  *(int *)(this + 0x1a8) = param_1;
  return;
}

```

## target disasm
```
  00164ef6: str.w r1,[r0,#0x1a8]
  00164efa: bx lr
```
