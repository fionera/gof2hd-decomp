# Transform::Update
elf 0x74cc8 body 12
Sig: undefined __stdcall Update(longlong param_1, bool param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AbyssEngine::Transform::Update(long long, bool) */

void AbyssEngine::Transform::Update(longlong param_1,bool param_2)

{
  char in_NG;
  bool in_ZR;
  char in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0xe92d);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  00084cc8: swige 0x3b5f0
  00084ccc: swieq 0xe92d
  00084cd0: movwls r11,#0x208d
```
