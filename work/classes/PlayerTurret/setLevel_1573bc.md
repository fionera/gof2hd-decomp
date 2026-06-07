# PlayerTurret::setLevel
elf 0x1573bc body 52
Sig: undefined __stdcall setLevel(Level * param_1)

## decompile
```c

/* PlayerTurret::setLevel(Level*) */

void PlayerTurret::setLevel(Level *param_1)

{
  undefined4 uVar1;
  Level *in_r1;
  undefined4 uVar2;
  
  KIPlayer::setLevel((KIPlayer *)param_1,in_r1);
  uVar2 = *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x74);
  uVar1 = AEGeometry::getReferenceMatrix();
  uVar1 = ParticleSystemManager::addSystem(uVar2,uVar1,9,0);
  *(undefined4 *)(param_1 + 0x138) = uVar1;
  (*(code *)(DAT_001aba84 + 0x1aba88))(*(undefined4 *)(*(int *)(param_1 + 0x54) + 0x74),uVar1,0);
  return;
}

```

## target disasm
```
  001673bc: push {r4,r5,r7,lr}
  001673be: add r7,sp,#0x8
  001673c0: mov r4,r0
  001673c2: blx 0x00073138
  001673c6: ldr r0,[r4,#0x8]
  001673c8: ldr r1,[r4,#0x54]
  001673ca: ldr r5,[r1,#0x74]
  001673cc: blx 0x00072d00
  001673d0: mov r1,r0
  001673d2: mov r0,r5
  001673d4: movs r2,#0x9
  001673d6: movs r3,#0x0
  001673d8: blx 0x000724d8
  001673dc: mov r1,r0
  001673de: ldr r0,[r4,#0x54]
  001673e0: str.w r1,[r4,#0x138]
  001673e4: movs r2,#0x0
  001673e6: ldr r0,[r0,#0x74]
  001673e8: pop.w {r4,r5,r7,lr}
  001673ec: b.w 0x001aba78
```
