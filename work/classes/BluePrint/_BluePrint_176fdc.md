# BluePrint::~BluePrint
elf 0x176fdc body 42
Sig: undefined __thiscall ~BluePrint(BluePrint * this)

## decompile
```c

/* BluePrint::~BluePrint() */

BluePrint * __thiscall BluePrint::~BluePrint(BluePrint *this)

{
  void *pvVar1;
  
  if (*(Array **)this != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)this);
    if (*(Array<int> **)this != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)this);
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)this = 0;
  AbyssEngine::String::~String((String *)(this + 0x14));
  return this;
}

```

## target disasm
```
  00186fdc: push {r4,r6,r7,lr}
  00186fde: add r7,sp,#0x8
  00186fe0: mov r4,r0
  00186fe2: ldr r0,[r0,#0x0]
  00186fe4: cbz r0,0x00186ff6
  00186fe6: blx 0x0007369c
  00186fea: ldr r0,[r4,#0x0]
  00186fec: cbz r0,0x00186ff6
  00186fee: blx 0x00070204
  00186ff2: blx 0x0006eb48
  00186ff6: movs r1,#0x0
  00186ff8: mov r0,r4
  00186ffa: str.w r1,[r0],#0x14
  00186ffe: blx 0x0006ee30
  00187002: mov r0,r4
  00187004: pop {r4,r6,r7,pc}
```
