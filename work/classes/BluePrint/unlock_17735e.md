# BluePrint::unlock
elf 0x17735e body 6
Sig: undefined __thiscall unlock(BluePrint * this)

## decompile
```c

/* BluePrint::unlock() */

void __thiscall BluePrint::unlock(BluePrint *this)

{
  this[8] = (BluePrint)0x1;
  return;
}

```

## target disasm
```
  0018735e: movs r1,#0x1
  00187360: strb r1,[r0,#0x8]
  00187362: bx lr
```
