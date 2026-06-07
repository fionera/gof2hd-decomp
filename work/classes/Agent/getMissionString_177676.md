# Agent::getMissionString
elf 0x177676 body 14
Sig: undefined __stdcall getMissionString(void)

## decompile
```c

/* Agent::getMissionString() */

void Agent::getMissionString(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x6c),false);
  return;
}

```

## target disasm
```
  00187676: push {r7,lr}
  00187678: mov r7,sp
  0018767a: adds r1,#0x6c
  0018767c: movs r2,#0x0
  0018767e: blx 0x0006f028
  00187682: pop {r7,pc}
```
