# LevelScript::canSkipCutsceneNow
elf 0x145e44 body 76
Sig: undefined __stdcall canSkipCutsceneNow(void)

## decompile
```c

/* LevelScript::canSkipCutsceneNow() */

undefined4 LevelScript::canSkipCutsceneNow(void)

{
  int in_r0;
  int iVar1;
  
  iVar1 = Status::getCurrentCampaignMission();
  if (iVar1 == 0x9a) {
    if (8 < *(int *)(in_r0 + 0x1c) - 1U) {
      return 0;
    }
  }
  else {
    iVar1 = Status::getCurrentCampaignMission();
    if (iVar1 == 0x9d) {
      if (2 < *(int *)(in_r0 + 0x1c) - 2U) {
        return 0;
      }
    }
    else {
      iVar1 = Status::getCurrentCampaignMission();
      if ((iVar1 == 0x9e) && (1 < *(int *)(in_r0 + 0x1c))) {
        return 0;
      }
    }
  }
  return 1;
}

```

## target disasm
```
  00155e44: push {r4,r5,r7,lr}
  00155e46: add r7,sp,#0x8
  00155e48: mov r4,r0
  00155e4a: ldr r0,[0x00155e90]
  00155e4c: add r0,pc
  00155e4e: ldr r5,[r0,#0x0]
  00155e50: ldr r0,[r5,#0x0]
  00155e52: blx 0x00071770
  00155e56: cmp r0,#0x9a
  00155e58: bne 0x00155e64
  00155e5a: ldr r0,[r4,#0x1c]
  00155e5c: subs r0,#0x1
  00155e5e: cmp r0,#0x9
  00155e60: bcs 0x00155e88
  00155e62: b 0x00155e8c
  00155e64: ldr r0,[r5,#0x0]
  00155e66: blx 0x00071770
  00155e6a: cmp r0,#0x9d
  00155e6c: bne 0x00155e78
  00155e6e: ldr r0,[r4,#0x1c]
  00155e70: subs r0,#0x2
  00155e72: cmp r0,#0x3
  00155e74: bcs 0x00155e88
  00155e76: b 0x00155e8c
  00155e78: ldr r0,[r5,#0x0]
  00155e7a: blx 0x00071770
  00155e7e: cmp r0,#0x9e
  00155e80: bne 0x00155e8c
  00155e82: ldr r0,[r4,#0x1c]
  00155e84: cmp r0,#0x1
  00155e86: ble 0x00155e8c
  00155e88: movs r0,#0x0
  00155e8a: pop {r4,r5,r7,pc}
  00155e8c: movs r0,#0x1
  00155e8e: pop {r4,r5,r7,pc}
```
