# Array<SolarSystem*>::Array
elf 0x120aac body 28
Sig: undefined __thiscall Array(Array<SolarSystem*> * this)

## decompile
```c

/* Array<SolarSystem*>::Array() */

Array<SolarSystem*> * __thiscall Array<SolarSystem*>::Array(Array<SolarSystem*> *this)

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
  00130aac: push {r4,r6,r7,lr}
  00130aae: add r7,sp,#0x8
  00130ab0: mov r4,r0
  00130ab2: movs r0,#0x4
  00130ab4: blx 0x0006ec08
  00130ab8: movs r2,#0x0
  00130aba: movs r1,#0x1
  00130abc: str r2,[r0,#0x0]
  00130abe: strd r2,r0,[r4,#0x0]
  00130ac2: mov r0,r4
  00130ac4: str r1,[r4,#0x8]
  00130ac6: pop {r4,r6,r7,pc}
```
