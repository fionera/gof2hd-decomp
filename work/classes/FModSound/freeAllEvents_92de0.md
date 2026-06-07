# FModSound::freeAllEvents
elf 0x92de0 body 12
Sig: undefined __stdcall freeAllEvents(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* FModSound::freeAllEvents() */

void FModSound::freeAllEvents(void)

{
  char in_NG;
  bool in_ZR;
  char in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0xf8e92d);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  000a2de0: swige 0x3b5f0
  000a2de4: swieq 0xf8e92d
  000a2de8: ldmdami r12!,{r2,r9,r10,lr}
```
