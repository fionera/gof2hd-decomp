# Mission::~Mission
elf 0x15ba98 body 16
Sig: undefined __thiscall ~Mission(Mission * this)

## decompile
```c

/* Mission::~Mission() */

void __thiscall Mission::~Mission(Mission *this)

{
  ~Mission(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0016ba98: push {r7,lr}
  0016ba9a: mov r7,sp
  0016ba9c: blx 0x000777ac
  0016baa0: pop.w {r7,lr}
  0016baa4: b.w 0x001ab098
```
