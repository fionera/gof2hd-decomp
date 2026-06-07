# Mission::getTargetName
elf 0x15bcb4 body 14
Sig: undefined __stdcall getTargetName(void)

## decompile
```c

/* Mission::getTargetName() */

void Mission::getTargetName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x1c),false);
  return;
}

```

## target disasm
```
  0016bcb4: push {r7,lr}
  0016bcb6: mov r7,sp
  0016bcb8: adds r1,#0x1c
  0016bcba: movs r2,#0x0
  0016bcbc: blx 0x0006f028
  0016bcc0: pop {r7,pc}
```
