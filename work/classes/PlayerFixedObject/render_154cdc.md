# PlayerFixedObject::render
elf 0x154cdc body 120
Sig: undefined __thiscall render(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::render() */

void __thiscall PlayerFixedObject::render(PlayerFixedObject *this)

{
  int iVar1;
  
  if ((*(int *)(this + 0x78) != 0) && (this[0x1b8] == (PlayerFixedObject)0x0)) {
    AEGeometry::render();
  }
  iVar1 = *(int *)(this + 0x88);
  if (iVar1 == 5) {
LAB_00164d38:
    if (this[0x1b8] != (PlayerFixedObject)0x0) {
      return;
    }
    (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(this + 8));
    return;
  }
  if (iVar1 == 4) {
    if (this[0x1b8] == (PlayerFixedObject)0x0) {
      AEGeometry::render();
    }
    iVar1 = *(int *)(this + 0x18c);
    if (iVar1 == 0) {
      return;
    }
  }
  else {
    if (iVar1 != 3) {
      iVar1 = Player::isActive();
      if (iVar1 == 0) {
        return;
      }
      goto LAB_00164d38;
    }
    if (this[0x1b8] == (PlayerFixedObject)0x0) {
      AEGeometry::render();
    }
    iVar1 = *(int *)(this + 0x18c);
  }
  (*(code *)(DAT_001ac2b4 + 0x1ac2b8))(iVar1);
  return;
}

```

## target disasm
```
  00164cdc: push {r4,r6,r7,lr}
  00164cde: add r7,sp,#0x8
  00164ce0: mov r4,r0
  00164ce2: ldr r0,[r0,#0x78]
  00164ce4: cbz r0,0x00164cf0
  00164ce6: ldrb.w r1,[r4,#0x1b8]
  00164cea: cbnz r1,0x00164cf0
  00164cec: blx 0x00072238
  00164cf0: ldr.w r0,[r4,#0x88]
  00164cf4: cmp r0,#0x5
  00164cf6: beq 0x00164d38
  00164cf8: cmp r0,#0x4
  00164cfa: beq 0x00164d14
  00164cfc: cmp r0,#0x3
  00164cfe: bne 0x00164d30
  00164d00: ldrb.w r0,[r4,#0x1b8]
  00164d04: cbnz r0,0x00164d0e
  00164d06: ldr.w r0,[r4,#0x124]
  00164d0a: blx 0x00072238
  00164d0e: ldr.w r0,[r4,#0x18c]
  00164d12: b 0x00164d28
  00164d14: ldrb.w r0,[r4,#0x1b8]
  00164d18: cbnz r0,0x00164d22
  00164d1a: ldr.w r0,[r4,#0x124]
  00164d1e: blx 0x00072238
  00164d22: ldr.w r0,[r4,#0x18c]
  00164d26: cbz r0,0x00164d3e
  00164d28: pop.w {r4,r6,r7,lr}
  00164d2c: b.w 0x001ac2a8
  00164d30: ldr r0,[r4,#0x4]
  00164d32: blx 0x00071f14
  00164d36: cbz r0,0x00164d3e
  00164d38: ldrb.w r0,[r4,#0x1b8]
  00164d3c: cbz r0,0x00164d40
  00164d3e: pop {r4,r6,r7,pc}
  00164d40: ldr r0,[r4,#0x8]
  00164d42: pop.w {r4,r6,r7,lr}
  00164d46: b.w 0x001abdc8
  001ac2a8: bx pc
  001ac2ac: ldr r12,[0x1ac2b4]
  001ac2b0: add pc,r12,pc
```
