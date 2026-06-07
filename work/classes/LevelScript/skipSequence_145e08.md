# LevelScript::skipSequence
elf 0x145e08 body 56
Sig: undefined __thiscall skipSequence(LevelScript * this)

## decompile
```c

/* LevelScript::skipSequence() */

void __thiscall LevelScript::skipSequence(LevelScript *this)

{
  int iVar1;
  undefined4 *puVar2;
  
  if ((0 < this->field_24) && (iVar1 = Status::getCurrentCampaignMission(), 0 < iVar1)) {
    this->field_24 = 0x1b59;
    puVar2 = (undefined4 *)Level::getPlayer();
    (*(code *)(DAT_001ac444 + 0x1ac448))(*puVar2,1);
    return;
  }
  return;
}

```

## target disasm
```
  00155e08: push {r4,r6,r7,lr}
  00155e0a: add r7,sp,#0x8
  00155e0c: mov r4,r0
  00155e0e: ldr r0,[r0,#0x24]
  00155e10: cmp r0,#0x1
  00155e12: blt 0x00155e3c
  00155e14: ldr r0,[0x00155e40]
  00155e16: add r0,pc
  00155e18: ldr r0,[r0,#0x0]
  00155e1a: ldr r0,[r0,#0x0]
  00155e1c: blx 0x00071770
  00155e20: cmp r0,#0x1
  00155e22: blt 0x00155e3c
  00155e24: ldr r0,[r4,#0x18]
  00155e26: movw r1,#0x1b59
  00155e2a: str r1,[r4,#0x24]
  00155e2c: blx 0x00072034
  00155e30: ldr r0,[r0,#0x0]
  00155e32: movs r1,#0x1
  00155e34: pop.w {r4,r6,r7,lr}
  00155e38: b.w 0x001ac438
  00155e3c: pop {r4,r6,r7,pc}
  001ac438: bx pc
```
