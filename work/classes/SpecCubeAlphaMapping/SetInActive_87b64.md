# SpecCubeAlphaMapping::SetInActive
elf 0x87b64 body 30
Sig: undefined __thiscall SetInActive(SpecCubeAlphaMapping * this)

## decompile
```c

/* AbyssEngine::SpecCubeAlphaMapping::SetInActive() */

void __thiscall AbyssEngine::SpecCubeAlphaMapping::SetInActive(SpecCubeAlphaMapping *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_00097b84 + 0x97b72);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  return;
}

```

## target disasm
```
  00097b64: push {r4,r5,r7,lr}
  00097b66: add r7,sp,#0x8
  00097b68: ldr r1,[0x00097b84]
  00097b6a: mov r4,r0
  00097b6c: ldr r0,[r0,#0x20]
  00097b6e: add r1,pc
  00097b70: ldr r5,[r1,#0x0]
  00097b72: blx r5
  00097b74: ldr r0,[r4,#0x24]
  00097b76: blx r5
  00097b78: ldr r0,[r4,#0x28]
  00097b7a: mov r1,r5
  00097b7c: pop.w {r4,r5,r7,lr}
  00097b80: bx r1
```
