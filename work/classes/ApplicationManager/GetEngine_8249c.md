# ApplicationManager::GetEngine
elf 0x8249c body 6
Sig: undefined __stdcall GetEngine(void)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetEngine() */

undefined4 AbyssEngine::ApplicationManager::GetEngine(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xa8);
}

```

## target disasm
```
  0009249c: ldr.w r0,[r0,#0xa8]
  000924a0: bx lr
```
