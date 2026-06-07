# Engine::LightSetRimColor
elf 0x85dd0 body 50
Sig: undefined __thiscall LightSetRimColor(Engine * this, float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::LightSetRimColor(float, float, float) */

void __thiscall
AbyssEngine::Engine::LightSetRimColor(Engine *this,float param_1,float param_2,float param_3)

{
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  
  if (**(char **)(DAT_00095e00 + 0x95dda) == '\0') {
    return;
  }
  *(undefined4 *)(this + 800) = in_r1;
  *(undefined4 *)(this + 0x324) = in_r2;
  *(undefined4 *)(this + 0x328) = in_r3;
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095dd0: push {r4,r6,r7,lr}
  00095dd2: add r7,sp,#0x8
  00095dd4: ldr r4,[0x00095e00]
  00095dd6: add r4,pc
  00095dd8: ldr r4,[r4,#0x0]
  00095dda: ldrb r4,[r4,#0x0]
  00095ddc: cbz r4,0x00095dfe
  00095dde: vmov s4,r1
  00095de2: vmov s0,r3
  00095de6: vmov s2,r2
  00095dea: vstr.32 s4,[r0,#0x320]
  00095dee: vstr.32 s2,[r0,#0x324]
  00095df2: vstr.32 s0,[r0,#0x328]
  00095df6: pop.w {r4,r6,r7,lr}
  00095dfa: b.w 0x001ab658
  00095dfe: pop {r4,r6,r7,pc}
  001ab658: bx pc
```
