# ApplicationManager::GetSystemTimeMillis
elf 0x83e9e body 6
Sig: undefined __stdcall GetSystemTimeMillis(void)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetSystemTimeMillis() */

undefined8 AbyssEngine::ApplicationManager::GetSystemTimeMillis(void)

{
  int in_r0;
  
  return *(undefined8 *)(in_r0 + 0x68);
}

```

## target disasm
```
  00093e9e: ldrd r0,r1,[r0,#0x68]
  00093ea2: bx lr
```
