# Route::reachWaypoint
elf 0x11bef6 body 104
Sig: undefined __stdcall reachWaypoint(int param_1)

## decompile
```c

/* Route::reachWaypoint(int) */

void Route::reachWaypoint(int param_1)

{
  uint *puVar1;
  int in_r1;
  uint uVar2;
  uint uVar3;
  
  puVar1 = *(uint **)(param_1 + 0xc);
  uVar2 = *puVar1;
  if (*(int *)param_1 < (int)(uVar2 - 1)) {
    *(int *)param_1 = in_r1 + 1;
  }
  else if (*(char *)(param_1 + 4) != '\0') {
    *(undefined4 *)param_1 = 0;
    for (uVar3 = 0; uVar3 < uVar2; uVar3 = uVar3 + 1) {
      Waypoint::reset((Waypoint *)((undefined4 *)puVar1[1])[uVar3]);
      puVar1 = *(uint **)(param_1 + 0xc);
      uVar2 = *puVar1;
    }
    Waypoint::setActive(SUB41(*(undefined4 *)puVar1[1],0));
    puVar1 = *(uint **)(param_1 + 0xc);
  }
  Waypoint::setActive(SUB41(*(undefined4 *)(puVar1[1] + in_r1 * 4),0));
  (*(code *)(DAT_001ac344 + 0x1ac348))
            (*(undefined4 *)(*(int *)(*(int *)(param_1 + 0xc) + 4) + in_r1 * 4));
  return;
}

```

## target disasm
```
  0012bef6: push {r4,r5,r6,r7,lr}
  0012bef8: add r7,sp,#0xc
  0012befa: push.w r11
  0012befe: mov r5,r0
  0012bf00: ldr r2,[r0,#0x0]
  0012bf02: ldr r0,[r0,#0xc]
  0012bf04: mov r4,r1
  0012bf06: ldr r1,[r0,#0x0]
  0012bf08: subs r3,r1,#0x1
  0012bf0a: cmp r2,r3
  0012bf0c: bge 0x0012bf14
  0012bf0e: adds r1,r4,#0x1
  0012bf10: str r1,[r5,#0x0]
  0012bf12: b 0x0012bf3c
  0012bf14: ldrb r2,[r5,#0x4]
  0012bf16: cbz r2,0x0012bf3c
  0012bf18: movs r6,#0x0
  0012bf1a: str r6,[r5,#0x0]
  0012bf1c: b 0x0012bf2c
  0012bf1e: ldr.w r0,[r0,r6,lsl #0x2]
  0012bf22: blx 0x00075f94
  0012bf26: ldr r0,[r5,#0xc]
  0012bf28: adds r6,#0x1
  0012bf2a: ldr r1,[r0,#0x0]
  0012bf2c: ldr r0,[r0,#0x4]
  0012bf2e: cmp r6,r1
  0012bf30: bcc 0x0012bf1e
  0012bf32: ldr r0,[r0,#0x0]
  0012bf34: movs r1,#0x1
  0012bf36: blx 0x00075fac
  0012bf3a: ldr r0,[r5,#0xc]
  0012bf3c: ldr r0,[r0,#0x4]
  0012bf3e: movs r1,#0x0
  0012bf40: ldr.w r0,[r0,r4,lsl #0x2]
  0012bf44: blx 0x00075fac
  0012bf48: ldr r0,[r5,#0xc]
  0012bf4a: ldr r0,[r0,#0x4]
  0012bf4c: ldr.w r0,[r0,r4,lsl #0x2]
  0012bf50: pop.w r11
  0012bf54: pop.w {r4,r5,r6,r7,lr}
  0012bf58: b.w 0x001ac338
  001ac338: bx pc
```
