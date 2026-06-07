# ShaderBaseStruct::ES2LoadProgram
elf 0x8e584 body 12
Sig: undefined __stdcall ES2LoadProgram(char * param_1, char * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AbyssEngine::ShaderBaseStruct::ES2LoadProgram(char const*, char const*) */

void AbyssEngine::ShaderBaseStruct::ES2LoadProgram(char *param_1,char *param_2)

{
  char in_NG;
  bool in_ZR;
  char in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    func_0xffcd8a44();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  0009e584: swige 0x3b5f0
  0009e588: bleq 0xffcd8a44
  0009e58c: ldmdami r8!,{r7,r9,r10,lr}
```
