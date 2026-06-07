# Gun::setImpact
elf 0x1523b0 body 6
Sig: undefined __thiscall setImpact(Gun * this, Sparks * param_1)

## decompile
```c

/* Gun::setImpact(Sparks*) */

void __thiscall Gun::setImpact(Gun *this,Sparks *param_1)

{
  *(Sparks **)(this + 0xb8) = param_1;
  return;
}

```

## target disasm
```
  001623b0: str.w r1,[r0,#0xb8]
  001623b4: bx lr
```
