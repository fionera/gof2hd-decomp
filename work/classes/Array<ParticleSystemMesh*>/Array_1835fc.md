# Array<ParticleSystemMesh*>::Array
elf 0x1835fc body 28
Sig: undefined __thiscall Array(Array<ParticleSystemMesh*> * this)

## decompile
```c

/* Array<ParticleSystemMesh*>::Array() */

Array<ParticleSystemMesh*> * __thiscall
Array<ParticleSystemMesh*>::Array(Array<ParticleSystemMesh*> *this)

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
  001935fc: push {r4,r6,r7,lr}
  001935fe: add r7,sp,#0x8
  00193600: mov r4,r0
  00193602: movs r0,#0x4
  00193604: blx 0x0006ec08
  00193608: movs r2,#0x0
  0019360a: movs r1,#0x1
  0019360c: str r2,[r0,#0x0]
  0019360e: strd r2,r0,[r4,#0x0]
  00193612: mov r0,r4
  00193614: str r1,[r4,#0x8]
  00193616: pop {r4,r6,r7,pc}
```
