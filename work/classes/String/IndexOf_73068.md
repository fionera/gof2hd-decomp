# String::IndexOf
elf 0x73068 body 10
Sig: undefined __stdcall IndexOf(String * param_1)

## decompile
```c

/* AbyssEngine::String::IndexOf(AbyssEngine::String const&) */

void AbyssEngine::String::IndexOf(String *param_1)

{
  (*(code *)(DAT_001ab124 + 0x1ab128))(param_1,0);
  return;
}

```

## target disasm
```
  00083068: mov r2,r1
  0008306a: movs r1,#0x0
  0008306c: b.w 0x001ab118
  001ab118: bx pc
```
