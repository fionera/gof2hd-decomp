# PlayerTurret::setPosition
elf 0x1573fc body 28
Sig: undefined __thiscall setPosition(PlayerTurret * this, Vector * param_1)

## decompile
```c

/* PlayerTurret::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerTurret::setPosition(PlayerTurret *this,Vector *param_1)

{
  AEGeometry::setPosition(*(AEGeometry **)(this + 8),param_1);
  *(undefined4 *)(this + 0x58) = *(undefined4 *)param_1;
  *(undefined4 *)(this + 0x5c) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 0x60) = *(undefined4 *)(param_1 + 8);
  return;
}

```

## target disasm
```
  001673fc: push {r4,r5,r7,lr}
  001673fe: add r7,sp,#0x8
  00167400: mov r5,r0
  00167402: ldr r0,[r0,#0x8]
  00167404: mov r4,r1
  00167406: blx 0x00072148
  0016740a: ldr r0,[r4,#0x0]
  0016740c: str r0,[r5,#0x58]
  0016740e: ldr r0,[r4,#0x4]
  00167410: str r0,[r5,#0x5c]
  00167412: ldr r0,[r4,#0x8]
  00167414: str r0,[r5,#0x60]
  00167416: pop {r4,r5,r7,pc}
```
