# Agent::getSystemName
elf 0x17771e body 14
Sig: undefined __stdcall getSystemName(void)

## decompile
```c

/* Agent::getSystemName() */

void Agent::getSystemName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x18),false);
  return;
}

```

## target disasm
```
  0018771e: push {r7,lr}
  00187720: mov r7,sp
  00187722: adds r1,#0x18
  00187724: movs r2,#0x0
  00187726: blx 0x0006f028
  0018772a: pop {r7,pc}
```
