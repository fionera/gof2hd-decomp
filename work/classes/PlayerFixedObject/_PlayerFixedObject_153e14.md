# PlayerFixedObject::~PlayerFixedObject
elf 0x153e14 body 16
Sig: undefined __thiscall ~PlayerFixedObject(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::~PlayerFixedObject() */

void __thiscall PlayerFixedObject::~PlayerFixedObject(PlayerFixedObject *this)

{
  ~PlayerFixedObject(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00163e14: push {r7,lr}
  00163e16: mov r7,sp
  00163e18: blx 0x000773f8
  00163e1c: pop.w {r7,lr}
  00163e20: b.w 0x001ab098
```
