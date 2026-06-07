# PlayerFixedObject::hideShip
elf 0x154f1a body 8
Sig: undefined __thiscall hideShip(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::hideShip() */

void __thiscall PlayerFixedObject::hideShip(PlayerFixedObject *this)

{
  this[0x1b8] = (PlayerFixedObject)0x1;
  return;
}

```

## target disasm
```
  00164f1a: movs r1,#0x1
  00164f1c: strb.w r1,[r0,#0x1b8]
  00164f20: bx lr
```
