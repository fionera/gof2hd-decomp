# ApplicationManager::CheatAddCode
elf 0x82528 body 14
Sig: undefined __stdcall CheatAddCode(String * param_1, int param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::CheatAddCode(AbyssEngine::String const&, int) */

void AbyssEngine::ApplicationManager::CheatAddCode(String *param_1,int param_2)

{
  if (*(int *)(param_1 + 0x30) == 0) {
    return;
  }
  (*(code *)(DAT_001ab5e4 + 0x1ab5e8))();
  return;
}

```

## target disasm
```
  00092528: ldr r0,[r0,#0x30]
  0009252a: cmp r0,#0x0
  0009252c: it ne
  0009252e: b.ne.w 0x001ab5d8
  00092532: bx lr
  001ab5d8: bx pc
```
