# Mission::getDescription
elf 0x15bb2c body 16
Sig: undefined __thiscall getDescription(Mission * this)

## decompile
```c

/* Mission::getDescription() */

void __thiscall Mission::getDescription(Mission *this)

{
  AbyssEngine::String::String((String *)this,(char *)(DAT_0016bb3c + 0x16bb38),false);
  return;
}

```

## target disasm
```
  0016bb2c: push {r7,lr}
  0016bb2e: mov r7,sp
  0016bb30: ldr r1,[0x0016bb3c]
  0016bb32: movs r2,#0x0
  0016bb34: add r1,pc
  0016bb36: blx 0x0006ee18
  0016bb3a: pop {r7,pc}
```
