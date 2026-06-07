# PlayerFixedObject::setDockingType
elf 0x154eea body 6
Sig: undefined __thiscall setDockingType(PlayerFixedObject * this, int param_1)

## decompile
```c

/* PlayerFixedObject::setDockingType(int) */

void __thiscall PlayerFixedObject::setDockingType(PlayerFixedObject *this,int param_1)

{
  *(int *)(this + 0x1a4) = param_1;
  return;
}

```

## target disasm
```
  00164eea: str.w r1,[r0,#0x1a4]
  00164eee: bx lr
```
