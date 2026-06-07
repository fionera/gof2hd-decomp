# Wanted::getName
elf 0x122924 body 12
Sig: undefined __stdcall getName(void)

## decompile
```c

/* Wanted::getName() */

void Wanted::getName(void)

{
  String *in_r0;
  String *in_r1;
  
  AbyssEngine::String::String(in_r0,in_r1,false);
  return;
}

```

## target disasm
```
  00132924: push {r7,lr}
  00132926: mov r7,sp
  00132928: movs r2,#0x0
  0013292a: blx 0x0006f028
  0013292e: pop {r7,pc}
```
