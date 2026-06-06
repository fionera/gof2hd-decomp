# AbyssEngine::ApplicationManager::Quit

ELF vaddr 0x82476, body 8 bytes
Signature: undefined __thiscall Quit(ApplicationManager * this)

## Ghidra decompile
```c

/* AbyssEngine::ApplicationManager::Quit() */

void __thiscall AbyssEngine::ApplicationManager::Quit(ApplicationManager *this)

{
  if (*(code **)(this + 0x1c) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0009247a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(this + 0x1c))();
    return;
  }
  return;
}

```

## Target disassembly (must match)
```
  00092476: ldr r0,[r0,#0x1c]
  00092478: cbz r0,0x0009247c
  0009247a: bx r0
  0009247c: bx lr
```

Verify: tools/try.sh work/src/Quit.cpp Quit 82476 32
