# Agent::getStationName
elf 0x17770a body 14
Sig: undefined __stdcall getStationName(void)

## decompile
```c

/* Agent::getStationName() */

void Agent::getStationName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x78),false);
  return;
}

```

## target disasm
```
  0018770a: push {r7,lr}
  0018770c: mov r7,sp
  0018770e: adds r1,#0x78
  00187710: movs r2,#0x0
  00187712: blx 0x0006f028
  00187716: pop {r7,pc}
```
