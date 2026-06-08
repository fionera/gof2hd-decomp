# IApplicationModule*>::Array
elf 0x81fc0 body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::IApplicationModule*> * this)

## decompile
```c

/* Array<AbyssEngine::IApplicationModule*>::Array() */

Array<AbyssEngine::IApplicationModule*> * __thiscall
Array<AbyssEngine::IApplicationModule*>::Array(Array<AbyssEngine::IApplicationModule*> *this)

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
  00091fc0: push {r4,r6,r7,lr}
  00091fc2: add r7,sp,#0x8
  00091fc4: mov r4,r0
  00091fc6: movs r0,#0x4
  00091fc8: blx 0x0006ec08
  00091fcc: movs r2,#0x0
  00091fce: movs r1,#0x1
  00091fd0: str r2,[r0,#0x0]
  00091fd2: strd r2,r0,[r4,#0x0]
  00091fd6: mov r0,r4
  00091fd8: str r1,[r4,#0x8]
  00091fda: pop {r4,r6,r7,pc}
```
