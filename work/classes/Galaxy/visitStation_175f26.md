# Galaxy::visitStation
elf 0x175f26 body 8
Sig: undefined __thiscall visitStation(Galaxy * this, int param_1)

## decompile
```c

/* Galaxy::visitStation(int) */

void __thiscall Galaxy::visitStation(Galaxy *this,int param_1)

{
  this->stations[param_1] = '\x01';
  return;
}

```

## target disasm
```
  00185f26: ldr r0,[r0,#0x0]
  00185f28: movs r2,#0x1
  00185f2a: strb r2,[r0,r1]
  00185f2c: bx lr
```
