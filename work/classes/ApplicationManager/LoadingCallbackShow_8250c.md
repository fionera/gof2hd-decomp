# ApplicationManager::LoadingCallbackShow
elf 0x8250c body 10
Sig: undefined __stdcall LoadingCallbackShow(int param_1, void * param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::LoadingCallbackShow(int, void*) */

void AbyssEngine::ApplicationManager::LoadingCallbackShow(int param_1,void *param_2)

{
  if (*(code **)(param_1 + 0x20) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00092512. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 0x20))(*(undefined4 *)param_1);
    return;
  }
  return;
}

```

## target disasm
```
  0009250c: ldr r3,[r0,#0x20]
  0009250e: cbz r3,0x00092514
  00092510: ldr r0,[r0,#0x0]
  00092512: bx r3
  00092514: bx lr
```
