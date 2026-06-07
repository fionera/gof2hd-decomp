# CubeNormalMapping::SetInActive
elf 0x90cc4 body 38
Sig: undefined __thiscall SetInActive(CubeNormalMapping * this)

## decompile
```c

/* AbyssEngine::CubeNormalMapping::SetInActive() */

void __thiscall AbyssEngine::CubeNormalMapping::SetInActive(CubeNormalMapping *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000a0cec + 0xa0cd2);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  (*pcVar1)(*(undefined4 *)(this + 0x2c));
  (*pcVar1)(*(undefined4 *)(this + 0x30));
  return;
}

```

## target disasm
```
  000a0cc4: push {r4,r5,r7,lr}
  000a0cc6: add r7,sp,#0x8
  000a0cc8: ldr r1,[0x000a0cec]
  000a0cca: mov r4,r0
  000a0ccc: ldr r0,[r0,#0x20]
  000a0cce: add r1,pc
  000a0cd0: ldr r5,[r1,#0x0]
  000a0cd2: blx r5
  000a0cd4: ldr r0,[r4,#0x24]
  000a0cd6: blx r5
  000a0cd8: ldr r0,[r4,#0x28]
  000a0cda: blx r5
  000a0cdc: ldr r0,[r4,#0x2c]
  000a0cde: blx r5
  000a0ce0: ldr r0,[r4,#0x30]
  000a0ce2: mov r1,r5
  000a0ce4: pop.w {r4,r5,r7,lr}
  000a0ce8: bx r1
```
