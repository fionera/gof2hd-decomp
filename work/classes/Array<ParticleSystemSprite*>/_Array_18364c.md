# Array<ParticleSystemSprite*>::~Array
elf 0x18364c body 22
Sig: undefined __thiscall ~Array(Array<ParticleSystemSprite*> * this)

## decompile
```c

/* Array<ParticleSystemSprite*>::~Array() */

Array<ParticleSystemSprite*> * __thiscall
Array<ParticleSystemSprite*>::~Array(Array<ParticleSystemSprite*> *this)

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
  0019364c: push {r4,r6,r7,lr}
  0019364e: add r7,sp,#0x8
  00193650: mov r4,r0
  00193652: ldr r0,[r0,#0x4]
  00193654: cbz r0,0x0019365a
  00193656: blx 0x0006ebfc
  0019365a: movs r0,#0x0
  0019365c: str r0,[r4,#0x4]
  0019365e: mov r0,r4
  00193660: pop {r4,r6,r7,pc}
```
