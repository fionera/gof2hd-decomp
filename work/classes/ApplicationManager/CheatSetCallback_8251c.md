# ApplicationManager::CheatSetCallback
elf 0x8251c body 14
Sig: undefined __stdcall CheatSetCallback(_func_void_int_void_ptr * param_1, void * param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::CheatSetCallback(void (*)(int, void*), void*) */

void AbyssEngine::ApplicationManager::CheatSetCallback
               (_func_void_int_void_ptr *param_1,void *param_2)

{
  if (*(int *)(param_1 + 0x30) == 0) {
    return;
  }
  (*(code *)(DAT_001ab5d4 + 0x1ab5d8))();
  return;
}

```

## target disasm
```
  0009251c: ldr r0,[r0,#0x30]
  0009251e: cmp r0,#0x0
  00092520: it ne
  00092522: b.ne.w 0x001ab5c8
  00092526: bx lr
  001ab5c8: bx pc
```
