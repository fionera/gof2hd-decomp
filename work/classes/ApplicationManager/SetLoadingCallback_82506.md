# ApplicationManager::SetLoadingCallback
elf 0x82506 body 6
Sig: undefined __thiscall SetLoadingCallback(ApplicationManager * this, _func_void_PaintCanvas_ptr_int_void_ptr * param_1, void * param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::SetLoadingCallback(void (*)(AbyssEngine::PaintCanvas*, int,
   void*), void*) */

void __thiscall
AbyssEngine::ApplicationManager::SetLoadingCallback
          (ApplicationManager *this,_func_void_PaintCanvas_ptr_int_void_ptr *param_1,void *param_2)

{
  *(_func_void_PaintCanvas_ptr_int_void_ptr **)(this + 0x20) = param_1;
  *(void **)(this + 0x24) = param_2;
  return;
}

```

## target disasm
```
  00092506: strd r1,r2,[r0,#0x20]
  0009250a: bx lr
```
