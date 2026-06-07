# AEGeometry::rotate
elf 0xa4914 body 14
Sig: undefined __stdcall rotate(Vector * param_1)

## decompile
```c

/* AEGeometry::rotate(AbyssEngine::AEMath::Vector const&) */

void AEGeometry::rotate(Vector *param_1)

{
  undefined4 *in_r1;
  
  (*(code *)(DAT_001abb34 + 0x1abb38))(param_1,*in_r1,in_r1[1],in_r1[2]);
  return;
}

```

## target disasm
```
  000b4914: ldrd r12,r2,[r1,#0x0]
  000b4918: ldr r3,[r1,#0x8]
  000b491a: mov r1,r12
  000b491c: b.w 0x001abb28
  001abb28: bx pc
```
