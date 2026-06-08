# bad_cast::~bad_cast
elf 0x193e18 body 16
Sig: undefined __thiscall ~bad_cast(bad_cast * this)

## decompile
```c

/* std::bad_cast::~bad_cast() */

void __thiscall std::bad_cast::~bad_cast(bad_cast *this)

{
  exception::~exception((exception *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001a3e18: push {r7,lr}
  001a3e1a: mov r7,sp
  001a3e1c: blx 0x00079288
  001a3e20: pop.w {r7,lr}
  001a3e24: b.w 0x001ab098
```
