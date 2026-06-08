# bad_typeid::~bad_typeid
elf 0x193e44 body 14
Sig: undefined __thiscall ~bad_typeid(bad_typeid * this)

## decompile
```c

/* std::bad_typeid::~bad_typeid() */

void __thiscall std::bad_typeid::~bad_typeid(bad_typeid *this)

{
  (*(code *)(&UNK_001acbc8 + DAT_001acbc4))(this);
  return;
}

```

## target disasm
```
  001a3e44: b.w 0x001acbb8
  001acbb8: bx pc
  001acbbc: ldr r12,[0x1acbc4]
  001acbc0: add pc,r12,pc
```
