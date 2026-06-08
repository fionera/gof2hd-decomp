# Transform*>::Array
elf 0x7489c body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::Transform*> * this)

## decompile
```c

/* Array<AbyssEngine::Transform*>::Array() */

Array<AbyssEngine::Transform*> * __thiscall
Array<AbyssEngine::Transform*>::Array(Array<AbyssEngine::Transform*> *this)

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
  0008489c: push {r4,r6,r7,lr}
  0008489e: add r7,sp,#0x8
  000848a0: mov r4,r0
  000848a2: movs r0,#0x4
  000848a4: blx 0x0006ec08
  000848a8: movs r2,#0x0
  000848aa: movs r1,#0x1
  000848ac: str r2,[r0,#0x0]
  000848ae: strd r2,r0,[r4,#0x0]
  000848b2: mov r0,r4
  000848b4: str r1,[r4,#0x8]
  000848b6: pop {r4,r6,r7,pc}
```
