# Array<ParticleSystemSprite*>::Array
elf 0x1835e0 body 28
Sig: undefined __thiscall Array(Array<ParticleSystemSprite*> * this)

## decompile
```c

/* Array<ParticleSystemSprite*>::Array() */

Array<ParticleSystemSprite*> * __thiscall
Array<ParticleSystemSprite*>::Array(Array<ParticleSystemSprite*> *this)

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
  001935e0: push {r4,r6,r7,lr}
  001935e2: add r7,sp,#0x8
  001935e4: mov r4,r0
  001935e6: movs r0,#0x4
  001935e8: blx 0x0006ec08
  001935ec: movs r2,#0x0
  001935ee: movs r1,#0x1
  001935f0: str r2,[r0,#0x0]
  001935f2: strd r2,r0,[r4,#0x0]
  001935f6: mov r0,r4
  001935f8: str r1,[r4,#0x8]
  001935fa: pop {r4,r6,r7,pc}
```
