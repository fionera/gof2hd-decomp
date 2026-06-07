# ApplicationManager::CheatUpdate
elf 0x8253a body 18
Sig: undefined __stdcall CheatUpdate(ushort param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::CheatUpdate(unsigned short) */

void AbyssEngine::ApplicationManager::CheatUpdate(ushort param_1)

{
  if ((*(char *)(param_1 + 0x34) != '\0') && (*(int *)(param_1 + 0x30) != 0)) {
    (*(code *)(DAT_001ab5f4 + 0x1ab5f8))();
    return;
  }
  return;
}

```

## target disasm
```
  0009253a: ldrb.w r2,[r0,#0x34]
  0009253e: cbz r2,0x00092548
  00092540: ldr r0,[r0,#0x30]
  00092542: cbz r0,0x00092548
  00092544: b.w 0x001ab5e8
  00092548: bx lr
  001ab5e8: bx pc
```
