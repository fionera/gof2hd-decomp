# Agent::setImageParts
elf 0x177550 body 6
Sig: undefined __thiscall setImageParts(Agent * this, int * param_1)

## decompile
```c

/* Agent::setImageParts(int*) */

void __thiscall Agent::setImageParts(Agent *this,int *param_1)

{
  *(int **)(this + 0x88) = param_1;
  return;
}

```

## target disasm
```
  00187550: str.w r1,[r0,#0x88]
  00187554: bx lr
```
