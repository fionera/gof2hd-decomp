# String::PrintOut
elf 0x73644 body 16
Sig: undefined __stdcall PrintOut(void)

## decompile
```c

/* AbyssEngine::String::PrintOut() */

void AbyssEngine::String::PrintOut(void)

{
  GetAEChar();
  (*(code *)(DAT_001ab114 + 0x1ab118))();
  return;
}

```

## target disasm
```
  00083644: push {r7,lr}
  00083646: mov r7,sp
  00083648: blx 0x0006ef68
  0008364c: pop.w {r7,lr}
  00083650: b.w 0x001ab108
```
