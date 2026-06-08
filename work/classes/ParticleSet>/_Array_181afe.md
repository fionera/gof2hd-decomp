# ParticleSet>::~Array
elf 0x181afe body 22
Sig: undefined __thiscall ~Array(Array<ParticleSettings::ParticleSet> * this)

## decompile
```c

/* Array<ParticleSettings::ParticleSet>::~Array() */

Array<ParticleSettings::ParticleSet> * __thiscall
Array<ParticleSettings::ParticleSet>::~Array(Array<ParticleSettings::ParticleSet> *this)

{
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  00191afe: push {r4,r6,r7,lr}
  00191b00: add r7,sp,#0x8
  00191b02: mov r4,r0
  00191b04: ldr r0,[r0,#0x4]
  00191b06: cbz r0,0x00191b0c
  00191b08: blx 0x0006ebfc
  00191b0c: movs r0,#0x0
  00191b0e: str r0,[r4,#0x4]
  00191b10: mov r0,r4
  00191b12: pop {r4,r6,r7,pc}
```
