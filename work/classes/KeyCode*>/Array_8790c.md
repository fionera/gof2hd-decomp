# KeyCode*>::Array
elf 0x8790c body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::KeyCode*> * this)

## decompile
```c

/* Array<AbyssEngine::KeyCode*>::Array() */

Array<AbyssEngine::KeyCode*> * __thiscall
Array<AbyssEngine::KeyCode*>::Array(Array<AbyssEngine::KeyCode*> *this)

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
  0009790c: push {r4,r6,r7,lr}
  0009790e: add r7,sp,#0x8
  00097910: mov r4,r0
  00097912: movs r0,#0x4
  00097914: blx 0x0006ec08
  00097918: movs r2,#0x0
  0009791a: movs r1,#0x1
  0009791c: str r2,[r0,#0x0]
  0009791e: strd r2,r0,[r4,#0x0]
  00097922: mov r0,r4
  00097924: str r1,[r4,#0x8]
  00097926: pop {r4,r6,r7,pc}
```
