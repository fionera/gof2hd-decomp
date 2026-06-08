# AELoadedTexture*>::Array
elf 0x7787c body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::AELoadedTexture*> * this)

## decompile
```c

/* Array<AbyssEngine::AELoadedTexture*>::Array() */

Array<AbyssEngine::AELoadedTexture*> * __thiscall
Array<AbyssEngine::AELoadedTexture*>::Array(Array<AbyssEngine::AELoadedTexture*> *this)

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
  0008787c: push {r4,r6,r7,lr}
  0008787e: add r7,sp,#0x8
  00087880: mov r4,r0
  00087882: movs r0,#0x4
  00087884: blx 0x0006ec08
  00087888: movs r2,#0x0
  0008788a: movs r1,#0x1
  0008788c: str r2,[r0,#0x0]
  0008788e: strd r2,r0,[r4,#0x0]
  00087892: mov r0,r4
  00087894: str r1,[r4,#0x8]
  00087896: pop {r4,r6,r7,pc}
```
