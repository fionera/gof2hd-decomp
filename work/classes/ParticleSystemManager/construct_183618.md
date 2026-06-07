# ParticleSystemManager::construct
elf 0x183618 body 30
Sig: undefined __thiscall construct(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::construct() */

void __thiscall ParticleSystemManager::construct(ParticleSystemManager *this)

{
  this[0x14] = (ParticleSystemManager)0x0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x2c) = 0xffffffff;
  *(undefined4 *)(this + 0x30) = 0xffffffff;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x54) = 0xffffffff;
  *(undefined4 *)(this + 0x58) = 0xffffffff;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined2 *)this = 0x101;
  return;
}

```

## target disasm
```
  00193618: movs r1,#0x0
  0019361a: mov.w r2,#0xffffffff
  0019361e: strb r1,[r0,#0x14]
  00193620: str r1,[r0,#0x10]
  00193622: strd r2,r2,[r0,#0x2c]
  00193626: str r1,[r0,#0x34]
  00193628: strd r2,r2,[r0,#0x54]
  0019362c: str r1,[r0,#0x5c]
  0019362e: movw r1,#0x101
  00193632: strh r1,[r0,#0x0]
  00193634: bx lr
```
