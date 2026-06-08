# String::GetStringLength
elf 0x73634 body 16
Sig: undefined __stdcall GetStringLength(char * param_1)

## decompile
```c

/* AbyssEngine::String::GetStringLength(char const*) */

char * AbyssEngine::String::GetStringLength(char *param_1)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  
  pcVar3 = param_1;
  do {
    pcVar2 = pcVar3 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar2;
  } while (cVar1 != '\0');
  return pcVar2 + ~(uint)param_1;
}

```

## target disasm
```
  00083634: mov r1,r0
  00083636: ldrb.w r2,[r1],#0x1
  0008363a: cmp r2,#0x0
  0008363c: bne 0x00083636
  0008363e: mvns r0,r0
  00083640: add r0,r1
  00083642: bx lr
```
