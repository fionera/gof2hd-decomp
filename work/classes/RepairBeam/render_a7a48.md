# RepairBeam::render
elf 0xa7a48 body 44
Sig: undefined __thiscall render(RepairBeam * this)

## decompile
```c

/* RepairBeam::render() */

void __thiscall RepairBeam::render(RepairBeam *this)

{
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < **(uint **)(this + 0x14); uVar1 = uVar1 + 1) {
    if (*(int *)((*(uint **)(this + 0x14))[1] + uVar1 * 4) != -1) {
      AEGeometry::render();
    }
  }
  return;
}

```

## target disasm
```
  000b7a48: push {r4,r5,r7,lr}
  000b7a4a: add r7,sp,#0x8
  000b7a4c: mov r4,r0
  000b7a4e: movs r5,#0x0
  000b7a50: b 0x000b7a6a
  000b7a52: ldr r0,[r0,#0x4]
  000b7a54: ldr.w r0,[r0,r5,lsl #0x2]
  000b7a58: adds r0,#0x1
  000b7a5a: beq 0x000b7a68
  000b7a5c: ldr r0,[r4,#0x10]
  000b7a5e: ldr r0,[r0,#0x4]
  000b7a60: ldr.w r0,[r0,r5,lsl #0x2]
  000b7a64: blx 0x00072238
  000b7a68: adds r5,#0x1
  000b7a6a: ldr r0,[r4,#0x14]
  000b7a6c: ldr r1,[r0,#0x0]
  000b7a6e: cmp r5,r1
  000b7a70: bcc 0x000b7a52
  000b7a72: pop {r4,r5,r7,pc}
```
