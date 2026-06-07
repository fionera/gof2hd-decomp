# Radar::getPlanetDockIndex
elf 0x130974 body 32
Sig: undefined __stdcall getPlanetDockIndex(void)

## decompile
```c

/* Radar::getPlanetDockIndex() */

undefined4 Radar::getPlanetDockIndex(void)

{
  int in_r0;
  SolarSystem *this;
  int iVar1;
  
  this = (SolarSystem *)Status::getSystem();
  iVar1 = SolarSystem::getStations(this);
  return *(undefined4 *)(*(int *)(iVar1 + 4) + *(int *)(in_r0 + 0x40) * 4);
}

```

## target disasm
```
  00140974: push {r4,r6,r7,lr}
  00140976: add r7,sp,#0x8
  00140978: mov r4,r0
  0014097a: ldr r0,[0x00140994]
  0014097c: add r0,pc
  0014097e: ldr r0,[r0,#0x0]
  00140980: ldr r0,[r0,#0x0]
  00140982: blx 0x00071a10
  00140986: blx 0x00071a88
  0014098a: ldr r1,[r4,#0x40]
  0014098c: ldr r0,[r0,#0x4]
  0014098e: ldr.w r0,[r0,r1,lsl #0x2]
  00140992: pop {r4,r6,r7,pc}
```
