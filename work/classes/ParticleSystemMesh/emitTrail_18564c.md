# ParticleSystemMesh::emitTrail
elf 0x18564c body 12
Sig: undefined __stdcall emitTrail(int param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* ParticleSystemMesh::emitTrail(int) */

void ParticleSystemMesh::emitTrail(int param_1)

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
    func_0x002d0b10();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  0019564c: swige 0x3b5f0
  00195650: swieq 0x80e92d
  00195654: blhi 0x002d0b10
```
