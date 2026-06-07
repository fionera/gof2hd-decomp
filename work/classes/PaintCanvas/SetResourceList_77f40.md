# PaintCanvas::SetResourceList
elf 0x77f40 body 16
Sig: undefined __thiscall SetResourceList(PaintCanvas * this, Resource * * param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetResourceList(AbyssEngine::Resource* const*, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::SetResourceList(PaintCanvas *this,Resource **param_1,uint param_2)

{
  (*(code *)(DAT_001ab174 + 0x1ab178))(param_1,param_2,this + 0x134);
  return;
}

```

## target disasm
```
  00087f40: mov r3,r2
  00087f42: add.w r2,r0,#0x134
  00087f46: mov r0,r1
  00087f48: mov r1,r3
  00087f4a: b.w 0x001ab168
  001ab168: bx pc
```
