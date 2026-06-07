# ParticleSystemManager::emitManual
elf 0x183dc4 body 56
Sig: undefined __stdcall emitManual(int param_1, Vector * param_2, int param_3, Vector * param_4, float param_5)

## decompile
```c

/* ParticleSystemManager::emitManual(int, AbyssEngine::AEMath::Vector const&, int,
   AbyssEngine::AEMath::Vector const&, float) */

Vector * ParticleSystemManager::emitManual
                   (int param_1,Vector *param_2,int param_3,Vector *param_4,float param_5)

{
  int iVar1;
  undefined4 local_20;
  
  local_20 = (Vector *)param_1;
  if (param_2 != (Vector *)0xffffffff) {
    if ((int)param_2 << 0x11 < 0) {
      iVar1 = *(int *)(param_1 + 0x40);
      param_2 = (Vector *)((uint)param_2 & DAT_00193dfc);
    }
    else {
      iVar1 = *(int *)(param_1 + 0x1c);
    }
    IParticleSystem::emitManual
              (*(undefined4 *)(iVar1 + (int)param_2 * 4),*(undefined4 *)param_3,
               *(undefined4 *)(param_3 + 4),*(undefined4 *)(param_3 + 8));
    local_20 = param_4;
  }
  return local_20;
}

```

## target disasm
```
  00193dc4: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00193dc6: add r7,sp,#0x18
  00193dc8: adds r4,r1,#0x1
  00193dca: beq 0x00193dfa
  00193dcc: ldr r5,[r7,#0x8]
  00193dce: lsls r4,r1,#0x11
  00193dd0: vldr.32 s0,[r7,#0xc]
  00193dd4: bmi 0x00193dda
  00193dd6: ldr r0,[r0,#0x1c]
  00193dd8: b 0x00193de0
  00193dda: ldr r4,[0x00193dfc]
  00193ddc: ldr r0,[r0,#0x40]
  00193dde: ands r1,r4
  00193de0: add.w r0,r0,r1, lsl #0x2
  00193de4: ldm.w r2,{r1,r12,lr}
  00193de8: ldr r0,[r0,#0x0]
  00193dea: vstr.32 s0,[sp,#0x8]
  00193dee: strd r3,r5,[sp,#0x0]
  00193df2: mov r2,r12
  00193df4: mov r3,lr
  00193df6: blx 0x00078b44
  00193dfa: pop {r0,r1,r2,r3,r4,r5,r7,pc}
```
