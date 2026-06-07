# Mission::getTargetSystemName
elf 0x15bb9e body 14
Sig: undefined __stdcall getTargetSystemName(void)

## decompile
```c

/* Mission::getTargetSystemName() */

void Mission::getTargetSystemName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x4c),false);
  return;
}

```

## target disasm
```
  0016bb9e: push {r7,lr}
  0016bba0: mov r7,sp
  0016bba2: adds r1,#0x4c
  0016bba4: movs r2,#0x0
  0016bba6: blx 0x0006f028
  0016bbaa: pop {r7,pc}
```
