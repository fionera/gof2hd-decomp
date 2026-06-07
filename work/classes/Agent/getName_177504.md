# Agent::getName
elf 0x177504 body 12
Sig: undefined __stdcall getName(void)

## decompile
```c

/* Agent::getName() */

void Agent::getName(void)

{
  String *in_r0;
  String *in_r1;
  
  AbyssEngine::String::String(in_r0,in_r1,false);
  return;
}

```

## target disasm
```
  00187504: push {r7,lr}
  00187506: mov r7,sp
  00187508: movs r2,#0x0
  0018750a: blx 0x0006f028
  0018750e: pop {r7,pc}
```
