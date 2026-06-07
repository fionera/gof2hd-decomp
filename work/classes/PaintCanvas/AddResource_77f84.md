# PaintCanvas::AddResource
elf 0x77f84 body 14
Sig: undefined __stdcall AddResource(Resource * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::AddResource(AbyssEngine::Resource*) */

void AbyssEngine::PaintCanvas::AddResource(Resource *param_1)

{
  (*(code *)(DAT_001ab184 + 0x1ab188))();
  return;
}

```

## target disasm
```
  00087f84: mov r2,r1
  00087f86: add.w r1,r0,#0x134
  00087f8a: mov r0,r2
  00087f8c: b.w 0x001ab178
  001ab178: bx pc
```
