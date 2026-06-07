# Engine::SetGravValue
elf 0x86560 body 34
Sig: undefined __stdcall SetGravValue(double param_1, double param_2, double param_3)

## decompile
```c

/* AbyssEngine::Engine::SetGravValue(double, double, double) */

undefined4 AbyssEngine::Engine::SetGravValue(double param_1,double param_2,double param_3)

{
  int in_r0;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined8 in_stack_00000000;
  undefined8 in_stack_00000008;
  
  *(undefined4 *)(in_r0 + 0x4e0) = in_r2;
  *(undefined4 *)(in_r0 + 0x4e4) = in_r3;
  *(undefined8 *)(in_r0 + 0x4e8) = in_stack_00000000;
  *(undefined8 *)(in_r0 + 0x4f0) = in_stack_00000008;
  return SUB84(param_1,0);
}

```

## target disasm
```
  00096560: str.w r2,[r0,#0x4e0]
  00096564: add.w r1,r0,#0x4e8
  00096568: str.w r3,[r0,#0x4e4]
  0009656c: add.w r0,r0,#0x4f0
  00096570: vldr.64 d17,[sp]
  00096574: vldr.64 d16,[sp,#0x8]
  00096578: vstr.64 d17,[r1]
  0009657c: vstr.64 d16,[r0]
  00096580: bx lr
```
