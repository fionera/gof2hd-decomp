# AbyssEngine::IApplicationModule::SetApplicationManager

ELF vaddr 0x74154, body 8 bytes
Signature: undefined __thiscall SetApplicationManager(IApplicationModule * this, ApplicationManager * param_1)

## Ghidra decompile
```c

/* AbyssEngine::IApplicationModule::SetApplicationManager(AbyssEngine::ApplicationManager*) */

void __thiscall
AbyssEngine::IApplicationModule::SetApplicationManager
          (IApplicationModule *this,ApplicationManager *param_1)

{
  *(ApplicationManager **)(this + 8) = param_1;
  *(undefined4 *)(this + 4) = *(undefined4 *)param_1;
  return;
}

```

## Target disassembly (must match)
```
  00084154: str r1,[r0,#0x8]
  00084156: ldr r1,[r1,#0x0]
  00084158: str r1,[r0,#0x4]
  0008415a: bx lr
```

Verify with: tools/try.sh <yourfile.cpp> SetApplicationManager 74154 32
