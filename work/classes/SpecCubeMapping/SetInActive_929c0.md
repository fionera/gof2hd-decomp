# SpecCubeMapping::SetInActive
elf 0x929c0 body 30
Sig: undefined __thiscall SetInActive(SpecCubeMapping * this)

## decompile
```c

/* AbyssEngine::SpecCubeMapping::SetInActive() */

void __thiscall AbyssEngine::SpecCubeMapping::SetInActive(SpecCubeMapping *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000a29e0 + 0xa29ce);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  return;
}

```

## target disasm
```
  000a29c0: push {r4,r5,r7,lr}
  000a29c2: add r7,sp,#0x8
  000a29c4: ldr r1,[0x000a29e0]
  000a29c6: mov r4,r0
  000a29c8: ldr r0,[r0,#0x20]
  000a29ca: add r1,pc
  000a29cc: ldr r5,[r1,#0x0]
  000a29ce: blx r5
  000a29d0: ldr r0,[r4,#0x24]
  000a29d2: blx r5
  000a29d4: ldr r0,[r4,#0x28]
  000a29d6: mov r1,r5
  000a29d8: pop.w {r4,r5,r7,lr}
  000a29dc: bx r1
```
