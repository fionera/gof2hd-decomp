# IParticleSystem::setMatrix
elf 0x182b40 body 10
Sig: undefined __thiscall setMatrix(IParticleSystem * this, Matrix * param_1)

## decompile
```c

/* IParticleSystem::setMatrix(AbyssEngine::AEMath::Matrix const*) */

void __thiscall IParticleSystem::setMatrix(IParticleSystem *this,Matrix *param_1)

{
  *(Matrix **)(this + 0x18) = param_1;
  *(undefined2 *)(this + 4) = 0x100;
  return;
}

```

## target disasm
```
  00192b40: str r1,[r0,#0x18]
  00192b42: mov.w r1,#0x100
  00192b46: strh r1,[r0,#0x4]
  00192b48: bx lr
```
