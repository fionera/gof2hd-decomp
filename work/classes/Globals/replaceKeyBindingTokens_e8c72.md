# Globals::replaceKeyBindingTokens
elf 0xe8c72 body 14
Sig: undefined __stdcall replaceKeyBindingTokens(String * param_1)

## decompile
```c

/* Globals::replaceKeyBindingTokens(AbyssEngine::String const&) */

void Globals::replaceKeyBindingTokens(String *param_1)

{
  String *in_r2;
  
  AbyssEngine::String::String((String *)param_1,in_r2,false);
  return;
}

```

## target disasm
```
  000f8c72: push {r7,lr}
  000f8c74: mov r7,sp
  000f8c76: mov r1,r2
  000f8c78: movs r2,#0x0
  000f8c7a: blx 0x0006f028
  000f8c7e: pop {r7,pc}
```
