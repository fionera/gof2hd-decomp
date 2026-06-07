# Engine::GlowBeginGlow
elf 0x86988 body 60
Sig: undefined __thiscall GlowBeginGlow(Engine * this, int param_1)

## decompile
```c

/* AbyssEngine::Engine::GlowBeginGlow(int) */

void __thiscall AbyssEngine::Engine::GlowBeginGlow(Engine *this,int param_1)

{
  if ((this[0x41c] == (Engine)0x0) && (**(char **)(DAT_000969c4 + 0x9699c) != '\0')) {
    glColorMask(0,0,0,1);
    GlowEnableGlow(this);
    if (this[0x41c] != (Engine)0x0) {
      (*(code *)(DAT_001ab6f4 + 0x1ab6f8))(param_1);
      return;
    }
  }
  return;
}

```

## target disasm
```
  00096988: push {r4,r5,r7,lr}
  0009698a: add r7,sp,#0x8
  0009698c: mov r5,r0
  0009698e: ldrb.w r0,[r0,#0x41c]
  00096992: mov r4,r1
  00096994: cbnz r0,0x000969c2
  00096996: ldr r0,[0x000969c4]
  00096998: add r0,pc
  0009699a: ldr r0,[r0,#0x0]
  0009699c: ldrb r0,[r0,#0x0]
  0009699e: cbz r0,0x000969c2
  000969a0: movs r0,#0x0
  000969a2: movs r1,#0x0
  000969a4: movs r2,#0x0
  000969a6: movs r3,#0x1
  000969a8: blx 0x00070804
  000969ac: mov r0,r5
  000969ae: blx 0x00070810
  000969b2: ldrb.w r0,[r5,#0x41c]
  000969b6: cbz r0,0x000969c2
  000969b8: mov r0,r4
  000969ba: pop.w {r4,r5,r7,lr}
  000969be: b.w 0x001ab6e8
  000969c2: pop {r4,r5,r7,pc}
```
