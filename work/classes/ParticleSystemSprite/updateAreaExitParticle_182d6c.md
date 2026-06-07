# ParticleSystemSprite::updateAreaExitParticle
elf 0x182d6c body 12
Sig: undefined __stdcall updateAreaExitParticle(int param_1, float param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* ParticleSystemSprite::updateAreaExitParticle(int, float) */

void ParticleSystemSprite::updateAreaExitParticle(int param_1,float param_2)

{
  char in_NG;
  bool in_ZR;
  bool in_CY;
  char in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0x80e92d);
  }
  if (in_CY && !in_ZR) {
    func_0x003ce230();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  00192d6c: swige 0x3b5f0
  00192d70: swieq 0x80e92d
  00192d74: blhi 0x003ce230
```
