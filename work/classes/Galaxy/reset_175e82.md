# Galaxy::reset
elf 0x175e82 body 18
Sig: undefined __thiscall reset(Galaxy * this)

## decompile
```c

/* Galaxy::reset() */

void __thiscall Galaxy::reset(Galaxy *this)

{
  uint8_t *puVar1;
  int iVar2;
  
  puVar1 = this->stations;
  for (iVar2 = 0; iVar2 != 0x87; iVar2 = iVar2 + 1) {
    puVar1[iVar2] = '\0';
  }
  return;
}

```

## target disasm
```
  00185e82: ldr r0,[r0,#0x0]
  00185e84: movs r1,#0x0
  00185e86: movs r2,#0x0
  00185e88: cmp r2,#0x87
  00185e8a: it eq
  00185e8c: bx.eq lr
  00185e8e: strb r1,[r0,r2]
  00185e90: adds r2,#0x1
  00185e92: b 0x00185e88
```
