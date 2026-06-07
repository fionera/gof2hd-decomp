# Engine::GlowEnableGlow
elf 0x869c8 body 42
Sig: undefined __thiscall GlowEnableGlow(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::GlowEnableGlow() */

void __thiscall AbyssEngine::Engine::GlowEnableGlow(Engine *this)

{
  if (this[0x41c] != (Engine)0x0) {
    return;
  }
  glClearColor(0,0,0,0);
  glClear(0x4000);
  this[0x41c] = (Engine)0x1;
  return;
}

```

## target disasm
```
  000969c8: push {r4,r6,r7,lr}
  000969ca: add r7,sp,#0x8
  000969cc: mov r4,r0
  000969ce: ldrb.w r0,[r0,#0x41c]
  000969d2: cbz r0,0x000969d6
  000969d4: pop {r4,r6,r7,pc}
  000969d6: movs r0,#0x0
  000969d8: movs r1,#0x0
  000969da: movs r2,#0x0
  000969dc: movs r3,#0x0
  000969de: blx 0x0006eecc
  000969e2: mov.w r0,#0x4000
  000969e6: blx 0x0006eed8
  000969ea: movs r0,#0x1
  000969ec: strb.w r0,[r4,#0x41c]
  000969f0: pop {r4,r6,r7,pc}
```
