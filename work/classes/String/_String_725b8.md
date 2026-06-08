# String::~String
elf 0x725b8 body 16
Sig: undefined __thiscall ~String(String * this)

## decompile
```c

/* AbyssEngine::String::~String() */

void __thiscall AbyssEngine::String::~String(String *this)

{
  ~String(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000825b8: push {r7,lr}
  000825ba: mov r7,sp
  000825bc: blx 0x0006f580
  000825c0: pop.w {r7,lr}
  000825c4: b.w 0x001ab098
```
