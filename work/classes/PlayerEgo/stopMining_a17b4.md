# PlayerEgo::stopMining
elf 0xa17b4 body 12
Sig: undefined __stdcall stopMining(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* PlayerEgo::stopMining() */

void PlayerEgo::stopMining(void)

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
  000b17b4: swige 0x3b5f0
  000b17b8: swieq 0xf8e92d
  000b17bc: ldmdami r12!,{r2,r9,r10,lr}^
```
