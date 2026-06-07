# ApplicationManager::GetCurrentTimeMillis
elf 0x83e98 body 6
Sig: undefined __stdcall GetCurrentTimeMillis(void)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetCurrentTimeMillis() */

undefined8 AbyssEngine::ApplicationManager::GetCurrentTimeMillis(void)

{
  int in_r0;
  
  return *(undefined8 *)(in_r0 + 0x68);
}

```

## target disasm
```
  00093e98: ldrd r0,r1,[r0,#0x68]
  00093e9c: bx lr
```
