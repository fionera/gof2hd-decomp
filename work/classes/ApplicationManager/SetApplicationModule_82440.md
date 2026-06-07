# ApplicationManager::SetApplicationModule
elf 0x82440 body 14
Sig: undefined __thiscall SetApplicationModule(ApplicationManager * this, IApplicationModule * param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SetApplicationModule(AbyssEngine::IApplicationModule*) */

void __thiscall
AbyssEngine::ApplicationManager::SetApplicationModule
          (ApplicationManager *this,IApplicationModule *param_1)

{
  *(IApplicationModule **)(this + 0x60) = param_1;
  *(uint *)(this + 0x3c) = (uint)(*(int *)(this + 0x18) != 0);
  return;
}

```

## target disasm
```
  00092440: ldr r2,[r0,#0x18]
  00092442: str r1,[r0,#0x60]
  00092444: cmp r2,#0x0
  00092446: it ne
  00092448: mov.ne r2,#0x1
  0009244a: str r2,[r0,#0x3c]
  0009244c: bx lr
```
