# SolarSystem::isFullyDiscovered
elf 0x15585c body 60
Sig: undefined __thiscall isFullyDiscovered(SolarSystem * this)

## decompile
```c

/* SolarSystem::isFullyDiscovered() */

undefined4 __thiscall SolarSystem::isFullyDiscovered(SolarSystem *this)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  undefined4 *puVar5;
  
  puVar3 = *(uint **)(this + 0x38);
  uVar4 = 0;
  puVar5 = *(undefined4 **)(DAT_00165898 + 0x165870);
  do {
    if (*puVar3 <= uVar4) {
      return 1;
    }
    iVar2 = Galaxy::getVisited((Galaxy *)*puVar5);
    puVar3 = *(uint **)(this + 0x38);
    iVar1 = uVar4 * 4;
    uVar4 = uVar4 + 1;
  } while (*(char *)(iVar2 + *(int *)(puVar3[1] + iVar1)) != '\0');
  return 0;
}

```

## target disasm
```
  0016585c: push {r4,r5,r6,r7,lr}
  0016585e: add r7,sp,#0xc
  00165860: push.w r11
  00165864: mov r4,r0
  00165866: ldr r1,[r0,#0x38]
  00165868: ldr r0,[0x00165898]
  0016586a: movs r5,#0x0
  0016586c: add r0,pc
  0016586e: ldr r6,[r0,#0x0]
  00165870: ldr r0,[r1,#0x0]
  00165872: cmp r5,r0
  00165874: bcs 0x00165890
  00165876: ldr r0,[r6,#0x0]
  00165878: blx 0x00073480
  0016587c: ldr r1,[r4,#0x38]
  0016587e: ldr r2,[r1,#0x4]
  00165880: ldr.w r2,[r2,r5,lsl #0x2]
  00165884: adds r5,#0x1
  00165886: ldrb r0,[r0,r2]
  00165888: cmp r0,#0x0
  0016588a: bne 0x00165870
  0016588c: movs r0,#0x0
  0016588e: b 0x00165892
  00165890: movs r0,#0x1
  00165892: pop.w r11
  00165896: pop {r4,r5,r6,r7,pc}
```
