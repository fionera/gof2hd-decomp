# Level::enableParticleEffects
elf 0xbd68c body 42
Sig: undefined __stdcall enableParticleEffects(bool param_1, bool param_2)

## decompile
```c

/* Level::enableParticleEffects(bool, bool) */

void Level::enableParticleEffects(bool param_1,bool param_2)

{
  uint uVar1;
  undefined1 in_r2;
  
  uVar1 = (uint)param_1;
  ParticleSystemManager::enableSystemEmit
            (*(int *)(uVar1 + 0x7c),SUB41(*(undefined4 *)(uVar1 + 0x284),0));
  ParticleSystemManager::enableSystemRender
            (*(int *)(uVar1 + 0x7c),SUB41(*(undefined4 *)(uVar1 + 0x284),0));
  **(undefined1 **)(uVar1 + 0x84) = in_r2;
  **(undefined1 **)(uVar1 + 0x78) = in_r2;
  return;
}

```
## target disasm
```
  000cd68c: push {r4,r5,r7,lr}
  000cd68e: add r7,sp,#0x8
  000cd690: mov r5,r0
  000cd692: ldr.w r1,[r0,#0x284]
  000cd696: ldr r0,[r0,#0x7c]
  000cd698: mov r4,r2
  000cd69a: blx 0x000723c4
  000cd69e: ldr.w r1,[r5,#0x284]
  000cd6a2: mov r2,r4
  000cd6a4: ldr r0,[r5,#0x7c]
  000cd6a6: blx 0x00072a24
  000cd6aa: ldr.w r0,[r5,#0x84]
  000cd6ae: strb r4,[r0,#0x0]
  000cd6b0: ldr r0,[r5,#0x78]
  000cd6b2: strb r4,[r0,#0x0]
  000cd6b4: pop {r4,r5,r7,pc}
```
