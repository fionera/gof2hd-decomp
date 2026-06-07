# ParticleSystemSprite::release
elf 0x1833b8 body 38
Sig: undefined __thiscall release(ParticleSystemSprite * this)

## decompile
```c

/* ParticleSystemSprite::release() */

void __thiscall ParticleSystemSprite::release(ParticleSystemSprite *this)

{
  if (*(void **)(this + 100) != (void *)0x0) {
    operator_delete__(*(void **)(this + 100));
  }
  *(undefined4 *)(this + 100) = 0;
  if (*(void **)(this + 0x68) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x68));
  }
  *(undefined4 *)(this + 0x68) = 0;
  operator_delete__(*(void **)(this + 0x6c));
  *(undefined4 *)(this + 0x6c) = 0;
  return;
}

```

## target disasm
```
  001933b8: push {r4,r5,r7,lr}
  001933ba: add r7,sp,#0x8
  001933bc: mov r4,r0
  001933be: ldr r0,[r0,#0x64]
  001933c0: cbz r0,0x001933c6
  001933c2: blx 0x0006ebfc
  001933c6: ldr r0,[r4,#0x68]
  001933c8: movs r5,#0x0
  001933ca: str r5,[r4,#0x64]
  001933cc: cbz r0,0x001933d2
  001933ce: blx 0x0006ebfc
  001933d2: ldr r0,[r4,#0x6c]
  001933d4: str r5,[r4,#0x68]
  001933d6: blx 0x0006ebfc
  001933da: str r5,[r4,#0x6c]
  001933dc: pop {r4,r5,r7,pc}
```
