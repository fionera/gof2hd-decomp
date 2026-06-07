# Engine::SetAccelValue
elf 0x86582 body 34
Sig: undefined __stdcall SetAccelValue(double param_1, double param_2, double param_3)

## decompile
```c

/* AbyssEngine::Engine::SetAccelValue(double, double, double) */

undefined4 AbyssEngine::Engine::SetAccelValue(double param_1,double param_2,double param_3)

{
  int in_r0;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined8 in_stack_00000000;
  undefined8 in_stack_00000008;
  
  *(undefined4 *)(in_r0 + 0x4b0) = in_r2;
  *(undefined4 *)(in_r0 + 0x4b4) = in_r3;
  *(undefined8 *)(in_r0 + 0x4b8) = in_stack_00000000;
  *(undefined8 *)(in_r0 + 0x4c0) = in_stack_00000008;
  return SUB84(param_1,0);
}

```

## target disasm
```
  00096582: str.w r2,[r0,#0x4b0]
  00096586: add.w r1,r0,#0x4b8
  0009658a: str.w r3,[r0,#0x4b4]
  0009658e: add.w r0,r0,#0x4c0
  00096592: vldr.64 d17,[sp]
  00096596: vldr.64 d16,[sp,#0x8]
  0009659a: vstr.64 d17,[r1]
  0009659e: vstr.64 d16,[r0]
  000965a2: bx lr
```
