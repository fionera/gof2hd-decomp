# PlayerFixedObject::getName
elf 0x154f02 body 16
Sig: undefined __stdcall getName(void)

## decompile
```c

/* PlayerFixedObject::getName() */

void PlayerFixedObject::getName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x1ac),false);
  return;
}

```

## target disasm
```
  00164f02: push {r7,lr}
  00164f04: mov r7,sp
  00164f06: add.w r1,r1,#0x1ac
  00164f0a: movs r2,#0x0
  00164f0c: blx 0x0006f028
  00164f10: pop {r7,pc}
```
