# String::StrLen
elf 0x729e0 body 16
Sig: undefined __thiscall StrLen(String * this, char * param_1)

## decompile
```c

/* AbyssEngine::String::StrLen(char const*) */

char * __thiscall AbyssEngine::String::StrLen(String *this,char *param_1)

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
  000829e0: mov r0,r1
  000829e2: ldrb.w r2,[r0],#0x1
  000829e6: cmp r2,#0x0
  000829e8: bne 0x000829e2
  000829ea: mvns r1,r1
  000829ec: add r0,r1
  000829ee: bx lr
```
