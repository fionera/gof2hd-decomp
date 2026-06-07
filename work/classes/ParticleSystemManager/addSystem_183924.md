# ParticleSystemManager::addSystem
elf 0x183924 body 16
Sig: undefined __stdcall addSystem(Matrix * param_1, ParticleSet param_2, bool param_3)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* ParticleSystemManager::addSystem(AbyssEngine::AEMath::Matrix const*,
   ParticleSettings::ParticleSet, bool) */

void ParticleSystemManager::addSystem(int param_1)

{
  uint unaff_r7;
  uint *unaff_r8;
  uint unaff_r9;
  uint unaff_r10;
  uint unaff_lr;
  bool in_NG;
  bool in_ZR;
  char in_OV;
  
  if (in_NG == (bool)in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    unaff_lr = (uint)*(byte *)(param_1 + ((uint)&stack0x00000000 >> 0x12));
  }
  if (in_NG) {
    *unaff_r8 = unaff_lr;
    unaff_r8[-1] = unaff_r10;
    unaff_r8[-2] = unaff_r9;
    unaff_r8[-3] = unaff_r7;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  00193924: swige 0x3b5f0
  00193928: ldrbeq lr,[r0,sp,lsr #0x12]!
  0019392c: stmdami r8!,{r7,r9,r10,lr}
  00193930: ldrmi r4,[r9],sp,ror #0xc
```
