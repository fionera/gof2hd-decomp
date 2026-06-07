# SolarSystem::getName
elf 0x1556f0 body 14
Sig: undefined __stdcall getName(void)

## decompile
```c

/* SolarSystem::getName() */

void SolarSystem::getName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0xc),false);
  return;
}

```

## target disasm
```
  001656f0: push {r7,lr}
  001656f2: mov r7,sp
  001656f4: adds r1,#0xc
  001656f6: movs r2,#0x0
  001656f8: blx 0x0006f028
  001656fc: pop {r7,pc}
```
