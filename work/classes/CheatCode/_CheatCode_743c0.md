# CheatCode::~CheatCode
elf 0x743c0 body 26
Sig: undefined __thiscall ~CheatCode(CheatCode * this)

## decompile
```c

/* AbyssEngine::CheatCode::~CheatCode() */

CheatCode * __thiscall AbyssEngine::CheatCode::~CheatCode(CheatCode *this)

{
  void *pvVar1;
  
  if (*(Array<unsigned_short> **)this != (Array<unsigned_short> *)0x0) {
    pvVar1 = (void *)Array<unsigned_short>::~Array(*(Array<unsigned_short> **)this);
    operator_delete(pvVar1);
  }
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  000843c0: push {r4,r6,r7,lr}
  000843c2: add r7,sp,#0x8
  000843c4: mov r4,r0
  000843c6: ldr r0,[r0,#0x0]
  000843c8: cbz r0,0x000843d2
  000843ca: blx 0x0006f6b8
  000843ce: blx 0x0006eb48
  000843d2: movs r0,#0x0
  000843d4: str r0,[r4,#0x0]
  000843d6: mov r0,r4
  000843d8: pop {r4,r6,r7,pc}
```
