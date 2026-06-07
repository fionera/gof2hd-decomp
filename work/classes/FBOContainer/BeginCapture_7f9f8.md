# FBOContainer::BeginCapture
elf 0x7f9f8 body 34
Sig: undefined __thiscall BeginCapture(FBOContainer * this)

## decompile
```c

/* AbyssEngine::FBOContainer::BeginCapture() */

void __thiscall AbyssEngine::FBOContainer::BeginCapture(FBOContainer *this)

{
  glBindFramebuffer(0x8d40,*(undefined4 *)this);
  (*(code *)(DAT_001ab084 + 0x1ab088))(0,0,*(undefined4 *)(this + 0xc),*(undefined4 *)(this + 0x10))
  ;
  return;
}

```

## target disasm
```
  0008f9f8: push {r4,r6,r7,lr}
  0008f9fa: add r7,sp,#0x8
  0008f9fc: ldr r1,[r0,#0x0]
  0008f9fe: mov r4,r0
  0008fa00: movw r0,#0x8d40
  0008fa04: blx 0x0006ee6c
  0008fa08: ldrd r2,r3,[r4,#0xc]
  0008fa0c: movs r0,#0x0
  0008fa0e: movs r1,#0x0
  0008fa10: pop.w {r4,r6,r7,lr}
  0008fa14: b.w 0x001ab078
  001ab078: bx pc
```
