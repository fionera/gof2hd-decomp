# PlayerFixedObject::setMoving
elf 0x153ee4 body 6
Sig: undefined __thiscall setMoving(PlayerFixedObject * this, bool param_1)

## decompile
```c

/* PlayerFixedObject::setMoving(bool) */

void __thiscall PlayerFixedObject::setMoving(PlayerFixedObject *this,bool param_1)

{
  this[0x134] = (PlayerFixedObject)param_1;
  return;
}

```

## target disasm
```
  00163ee4: strb.w r1,[r0,#0x134]
  00163ee8: bx lr
```
