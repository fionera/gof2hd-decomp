# TargetFollowCamera::setCamOffset
elf 0x15a9b2 body 26
Sig: undefined __stdcall setCamOffset(Vector * param_1)

## decompile
```c

/* TargetFollowCamera::setCamOffset(AbyssEngine::AEMath::Vector const&) */

void TargetFollowCamera::setCamOffset(Vector *param_1)

{
  undefined4 uVar1;
  Vector *in_r1;
  
  AbyssEngine::AEMath::Vector::operator=(param_1 + 0x38,in_r1);
  uVar1 = AbyssEngine::AEMath::VectorLength(in_r1);
  *(undefined4 *)(param_1 + 0xb0) = uVar1;
  return;
}

```

## target disasm
```
  0016a9b2: push {r4,r5,r7,lr}
  0016a9b4: add r7,sp,#0x8
  0016a9b6: mov r5,r0
  0016a9b8: adds r0,#0x38
  0016a9ba: mov r4,r1
  0016a9bc: blx 0x0006eb3c
  0016a9c0: mov r0,r4
  0016a9c2: blx 0x0006ec44
  0016a9c6: str.w r0,[r5,#0xb0]
  0016a9ca: pop {r4,r5,r7,pc}
```
