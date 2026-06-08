# Vector>::Array
elf 0x1346f0 body 32
Sig: undefined __thiscall Array(Array<AbyssEngine::AEMath::Vector> * this)

## decompile
```c

/* Array<AbyssEngine::AEMath::Vector>::Array() */

Array<AbyssEngine::AEMath::Vector> * __thiscall
Array<AbyssEngine::AEMath::Vector>::Array(Array<AbyssEngine::AEMath::Vector> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(0xc);
  *puVar1 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  001446f0: push {r4,r6,r7,lr}
  001446f2: add r7,sp,#0x8
  001446f4: mov r4,r0
  001446f6: movs r0,#0xc
  001446f8: blx 0x0006ec08
  001446fc: movs r2,#0x0
  001446fe: movs r1,#0x1
  00144700: strd r2,r2,[r0,#0x0]
  00144704: str r2,[r0,#0x8]
  00144706: strd r2,r0,[r4,#0x0]
  0014470a: mov r0,r4
  0014470c: str r1,[r4,#0x8]
  0014470e: pop {r4,r6,r7,pc}
```
