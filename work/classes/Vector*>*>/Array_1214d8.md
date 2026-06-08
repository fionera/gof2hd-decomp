# Vector*>*>::Array
elf 0x1214d8 body 28
Sig: undefined __thiscall Array(Array<Array<AbyssEngine::AEMath::Vector*>*> * this)

## decompile
```c

/* Array<Array<AbyssEngine::AEMath::Vector*>*>::Array() */

Array<Array<AbyssEngine::AEMath::Vector*>*> * __thiscall
Array<Array<AbyssEngine::AEMath::Vector*>*>::Array
          (Array<Array<AbyssEngine::AEMath::Vector*>*> *this)

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
  001314d8: push {r4,r6,r7,lr}
  001314da: add r7,sp,#0x8
  001314dc: mov r4,r0
  001314de: movs r0,#0x4
  001314e0: blx 0x0006ec08
  001314e4: movs r2,#0x0
  001314e6: movs r1,#0x1
  001314e8: str r2,[r0,#0x0]
  001314ea: strd r2,r0,[r4,#0x0]
  001314ee: mov r0,r4
  001314f0: str r1,[r4,#0x8]
  001314f2: pop {r4,r6,r7,pc}
```
