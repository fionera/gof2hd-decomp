# String*>*>::Array
elf 0x125ad6 body 28
Sig: undefined __thiscall Array(Array<Array<AbyssEngine::String*>*> * this)

## decompile
```c

/* Array<Array<AbyssEngine::String*>*>::Array() */

Array<Array<AbyssEngine::String*>*> * __thiscall
Array<Array<AbyssEngine::String*>*>::Array(Array<Array<AbyssEngine::String*>*> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  00135ad6: push {r4,r6,r7,lr}
  00135ad8: add r7,sp,#0x8
  00135ada: mov r4,r0
  00135adc: movs r0,#0x4
  00135ade: blx 0x0006ec08
  00135ae2: movs r2,#0x0
  00135ae4: movs r1,#0x1
  00135ae6: str r2,[r0,#0x0]
  00135ae8: strd r2,r0,[r4,#0x0]
  00135aec: mov r0,r4
  00135aee: str r1,[r4,#0x8]
  00135af0: pop {r4,r6,r7,pc}
```
