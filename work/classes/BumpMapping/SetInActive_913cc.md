# BumpMapping::SetInActive
elf 0x913cc body 38
Sig: undefined __thiscall SetInActive(BumpMapping * this)

## decompile
```c

/* AbyssEngine::BumpMapping::SetInActive() */

void __thiscall AbyssEngine::BumpMapping::SetInActive(BumpMapping *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000a13f4 + 0xa13da);
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
  000a13cc: push {r4,r5,r7,lr}
  000a13ce: add r7,sp,#0x8
  000a13d0: ldr r1,[0x000a13f4]
  000a13d2: mov r4,r0
  000a13d4: ldr r0,[r0,#0x20]
  000a13d6: add r1,pc
  000a13d8: ldr r5,[r1,#0x0]
  000a13da: blx r5
  000a13dc: ldr r0,[r4,#0x24]
  000a13de: blx r5
  000a13e0: ldr r0,[r4,#0x28]
  000a13e2: blx r5
  000a13e4: ldr r0,[r4,#0x2c]
  000a13e6: blx r5
  000a13e8: ldr r0,[r4,#0x30]
  000a13ea: mov r1,r5
  000a13ec: pop.w {r4,r5,r7,lr}
  000a13f0: bx r1
```
