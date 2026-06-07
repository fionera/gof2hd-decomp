# Trail::update
elf 0x15c1fc body 44
Sig: undefined __stdcall update(Vector * param_1, Vector * param_2)

## decompile
```c

/* Trail::update(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) */

undefined8 Trail::update(Vector *param_1,Vector *param_2)

{
  undefined8 uVar1;
  float *in_r2;
  float in_s1;
  float in_s3;
  float in_s5;
  
  uVar1 = *(undefined8 *)in_r2;
  update(*in_r2,in_s1,in_r2[1],in_s3,in_r2[2],in_s5);
  return uVar1;
}

```

## target disasm
```
  0016c1fc: push {r3,r4,r5,r6,r7,lr}
  0016c1fe: add r7,sp,#0x10
  0016c200: ldrd lr,r12,[r1,#0x0]
  0016c204: ldr r3,[r1,#0x8]
  0016c206: vldr.32 s0,[r2]
  0016c20a: vldr.32 s2,[r2,#0x4]
  0016c20e: mov r1,lr
  0016c210: vldr.32 s4,[r2,#0x8]
  0016c214: mov r2,r12
  0016c216: vstr.32 s0,[sp]
  0016c21a: vstr.32 s2,[sp,#0x4]
  0016c21e: vstr.32 s4,[sp,#0x8]
  0016c222: blx 0x00077800
  0016c226: pop {r0,r1,r2,r3,r7,pc}
```
