# AEGeometry::translate
elf 0xa4920 body 12
Sig: undefined __stdcall translate(Vector * param_1)

## decompile
```c

/* AEGeometry::translate(AbyssEngine::AEMath::Vector const&) */

void AEGeometry::translate(Vector *param_1)

{
  undefined4 *in_r1;
  
  (*(code *)(DAT_001abc84 + 0x1abc88))(param_1,*in_r1,in_r1[1],in_r1[2]);
  return;
}

```

## target disasm
```
  000b4920: ldrd r12,r2,[r1,#0x0]
  000b4924: ldr r3,[r1,#0x8]
  000b4926: mov r1,r12
  000b4928: b.w 0x001abc78
```
