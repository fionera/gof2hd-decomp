# Mission::getClientName
elf 0x15bb44 body 14
Sig: undefined __stdcall getClientName(void)

## decompile
```c

/* Mission::getClientName() */

void Mission::getClientName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x10),false);
  return;
}

```

## target disasm
```
  0016bb44: push {r7,lr}
  0016bb46: mov r7,sp
  0016bb48: adds r1,#0x10
  0016bb4a: movs r2,#0x0
  0016bb4c: blx 0x0006f028
  0016bb50: pop {r7,pc}
```
