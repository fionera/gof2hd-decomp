# ParticleSet>::Array
elf 0x181ad8 body 28
Sig: undefined __thiscall Array(Array<ParticleSettings::ParticleSet> * this)

## decompile
```c

/* Array<ParticleSettings::ParticleSet>::Array() */

Array<ParticleSettings::ParticleSet> * __thiscall
Array<ParticleSettings::ParticleSet>::Array(Array<ParticleSettings::ParticleSet> *this)

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
  00191ad8: push {r4,r6,r7,lr}
  00191ada: add r7,sp,#0x8
  00191adc: mov r4,r0
  00191ade: movs r0,#0x4
  00191ae0: blx 0x0006ec08
  00191ae4: movs r2,#0x0
  00191ae6: movs r1,#0x1
  00191ae8: str r2,[r0,#0x0]
  00191aea: strd r2,r0,[r4,#0x0]
  00191aee: mov r0,r4
  00191af0: str r1,[r4,#0x8]
  00191af2: pop {r4,r6,r7,pc}
```
