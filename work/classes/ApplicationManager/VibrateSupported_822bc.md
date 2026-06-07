# ApplicationManager::VibrateSupported
elf 0x822bc body 10
Sig: undefined __stdcall VibrateSupported(void)

## decompile
```c

/* AbyssEngine::ApplicationManager::VibrateSupported() */

void AbyssEngine::ApplicationManager::VibrateSupported(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab324 + 0x1ab328))(*(undefined4 *)(in_r0 + 0xa8));
  return;
}

```

## target disasm
```
  000922bc: ldr.w r0,[r0,#0xa8]
  000922c0: b.w 0x001ab318
  001ab318: bx pc
```
