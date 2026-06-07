# CheatHandler::SetCheatFunc
elf 0x74656 body 6
Sig: undefined __thiscall SetCheatFunc(CheatHandler * this, _func_void_int_void_ptr * param_1, void * param_2)

## decompile
```c

/* AbyssEngine::CheatHandler::SetCheatFunc(void (*)(int, void*), void*) */

void __thiscall
AbyssEngine::CheatHandler::SetCheatFunc
          (CheatHandler *this,_func_void_int_void_ptr *param_1,void *param_2)

{
  *(_func_void_int_void_ptr **)this = param_1;
  *(void **)(this + 4) = param_2;
  return;
}

```

## target disasm
```
  00084656: strd r1,r2,[r0,#0x0]
  0008465a: bx lr
```
