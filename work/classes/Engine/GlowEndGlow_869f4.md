# Engine::GlowEndGlow
elf 0x869f4 body 48
Sig: undefined __thiscall GlowEndGlow(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::GlowEndGlow() */

void __thiscall AbyssEngine::Engine::GlowEndGlow(Engine *this)

{
  if (**(char **)(DAT_00096a24 + 0x969fa) == '\0') {
    return;
  }
  this[0x41c] = (Engine)0x0;
  glColorMask(1,1,1,1);
  (*(code *)(DAT_001ab6f4 + 0x1ab6f8))(0x201);
  return;
}

```

## target disasm
```
  000969f4: ldr r1,[0x00096a24]
  000969f6: add r1,pc
  000969f8: ldr r1,[r1,#0x0]
  000969fa: ldrb r1,[r1,#0x0]
  000969fc: cbz r1,0x00096a20
  000969fe: push {r7,lr}
  00096a00: mov r7,sp
  00096a02: movs r1,#0x0
  00096a04: movs r2,#0x1
  00096a06: strb.w r1,[r0,#0x41c]
  00096a0a: movs r0,#0x1
  00096a0c: movs r1,#0x1
  00096a0e: movs r3,#0x1
  00096a10: blx 0x00070804
  00096a14: movw r0,#0x201
  00096a18: pop.w {r7,lr}
  00096a1c: b.w 0x001ab6e8
  00096a20: bx lr
  001ab6e8: bx pc
```
