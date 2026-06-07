# Gun::translate
elf 0x153a50 body 44
Sig: undefined __thiscall translate(Gun * this, Vector * param_1)

## decompile
```c

/* Gun::translate(AbyssEngine::AEMath::Vector const&) */

void __thiscall Gun::translate(Gun *this,Vector *param_1)

{
  uint uVar1;
  int iVar2;
  
  iVar2 = 0;
  for (uVar1 = 0; uVar1 < *(uint *)(this + 8); uVar1 = uVar1 + 1) {
    AbyssEngine::AEMath::Vector::operator+=((Vector *)(*(int *)(this + 0xc) + iVar2),param_1);
    iVar2 = iVar2 + 0xc;
  }
  return;
}

```

## target disasm
```
  00163a50: push {r4,r5,r6,r7,lr}
  00163a52: add r7,sp,#0xc
  00163a54: push.w r8
  00163a58: mov r8,r1
  00163a5a: mov r5,r0
  00163a5c: movs r6,#0x0
  00163a5e: movs r4,#0x0
  00163a60: b 0x00163a70
  00163a62: ldr r0,[r5,#0xc]
  00163a64: mov r1,r8
  00163a66: add r0,r6
  00163a68: blx 0x00073534
  00163a6c: adds r6,#0xc
  00163a6e: adds r4,#0x1
  00163a70: ldr r0,[r5,#0x8]
  00163a72: cmp r4,r0
  00163a74: bcc 0x00163a62
  00163a76: pop.w r8
  00163a7a: pop {r4,r5,r6,r7,pc}
```
