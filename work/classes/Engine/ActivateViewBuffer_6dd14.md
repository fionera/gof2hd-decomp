# Engine::ActivateViewBuffer
elf 0x6dd14 body 34
Sig: undefined __thiscall ActivateViewBuffer(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ActivateViewBuffer() */

void __thiscall AbyssEngine::Engine::ActivateViewBuffer(Engine *this)

{
  glBindFramebuffer(0x8d40,*(undefined4 *)(this + 0x40c));
  (*(code *)(DAT_001ab084 + 0x1ab088))
            (0,0,*(undefined4 *)(this + 0x370),*(undefined4 *)(this + 0x374));
  return;
}

```

## target disasm
```
  0007dd14: push {r4,r6,r7,lr}
  0007dd16: add r7,sp,#0x8
  0007dd18: ldr.w r1,[r0,#0x40c]
  0007dd1c: mov r4,r0
  0007dd1e: movw r0,#0x8d40
  0007dd22: blx 0x0006ee6c
  0007dd26: ldrd r2,r3,[r4,#0x370]
  0007dd2a: movs r0,#0x0
  0007dd2c: movs r1,#0x0
  0007dd2e: pop.w {r4,r6,r7,lr}
  0007dd32: b.w 0x001ab078
```
