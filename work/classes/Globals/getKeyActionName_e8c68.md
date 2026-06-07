# Globals::getKeyActionName
elf 0xe8c68 body 10
Sig: undefined __stdcall getKeyActionName(int param_1)

## decompile
```c

/* Globals::getKeyActionName(int) */

void Globals::getKeyActionName(int param_1)

{
  AbyssEngine::String::String((String *)param_1);
  return;
}

```

## target disasm
```
  000f8c68: push {r7,lr}
  000f8c6a: mov r7,sp
  000f8c6c: blx 0x0006efbc
  000f8c70: pop {r7,pc}
```
