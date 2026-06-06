# AbyssEngine::Quaternion::Quaternion
elf 0x8100a body 14
Sig: undefined __thiscall Quaternion(Quaternion * this, float param_1, float param_2, float param_3, float param_4)

## decompile
```c

/* AbyssEngine::Quaternion::Quaternion(float, float, float, float) */

void __thiscall
AbyssEngine::Quaternion::Quaternion
          (Quaternion *this,float param_1,float param_2,float param_3,float param_4)

{
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined4 in_stack_00000000;
  
  *(undefined4 *)this = in_r1;
  *(undefined4 *)(this + 4) = in_r2;
  *(undefined4 *)(this + 8) = in_r3;
  *(undefined4 *)(this + 0xc) = in_stack_00000000;
  return;
}

```
## target disasm
```
  0009100a: vldr.32 s0,[sp]
  0009100e: stm r0,{r1,r2,r3}
  00091012: vstr.32 s0,[r0,#0xc]
  00091016: bx lr
```
