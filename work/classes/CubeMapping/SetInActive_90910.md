# CubeMapping::SetInActive
elf 0x90910 body 30
Sig: undefined __thiscall SetInActive(CubeMapping * this)

## decompile
```c

/* AbyssEngine::CubeMapping::SetInActive() */

void __thiscall AbyssEngine::CubeMapping::SetInActive(CubeMapping *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000a0930 + 0xa091e);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  return;
}

```

## target disasm
```
  000a0910: push {r4,r5,r7,lr}
  000a0912: add r7,sp,#0x8
  000a0914: ldr r1,[0x000a0930]
  000a0916: mov r4,r0
  000a0918: ldr r0,[r0,#0x20]
  000a091a: add r1,pc
  000a091c: ldr r5,[r1,#0x0]
  000a091e: blx r5
  000a0920: ldr r0,[r4,#0x24]
  000a0922: blx r5
  000a0924: ldr r0,[r4,#0x28]
  000a0926: mov r1,r5
  000a0928: pop.w {r4,r5,r7,lr}
  000a092c: bx r1
```
