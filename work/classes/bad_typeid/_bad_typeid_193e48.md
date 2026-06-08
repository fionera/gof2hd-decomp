# bad_typeid::~bad_typeid
elf 0x193e48 body 26
Sig: undefined __thiscall ~bad_typeid(bad_typeid * this)

## decompile
```c

/* std::bad_typeid::~bad_typeid() */

void __thiscall std::bad_typeid::~bad_typeid(bad_typeid *this)

{
  exception::~exception((exception *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001a3e48: push {r7,lr}
  001a3e4a: mov r7,sp
  001a3e4c: blx 0x00079288
  001a3e50: pop.w {r7,lr}
  001a3e54: b.w 0x001ab098
  001ab098: bx pc
  001ab09c: ldr r12,[0x1ab0a4]
  001ab0a0: add pc,r12,pc
```
