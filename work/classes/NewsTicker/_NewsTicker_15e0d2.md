# NewsTicker::~NewsTicker
elf 0x15e0d2 body 16
Sig: undefined __thiscall ~NewsTicker(NewsTicker * this)

## decompile
```c

/* NewsTicker::~NewsTicker() */

NewsTicker * __thiscall NewsTicker::~NewsTicker(NewsTicker *this)

{
  AbyssEngine::String::~String((String *)(this + 0x14));
  return this;
}

```

## target disasm
```
  0016e0d2: push {r4,r6,r7,lr}
  0016e0d4: add r7,sp,#0x8
  0016e0d6: mov r4,r0
  0016e0d8: adds r0,#0x14
  0016e0da: blx 0x0006ee30
  0016e0de: mov r0,r4
  0016e0e0: pop {r4,r6,r7,pc}
```
