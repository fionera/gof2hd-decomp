# Route::getExactClone
elf 0x11c074 body 66
Sig: undefined __stdcall getExactClone(void)

## decompile
```c

/* Route::getExactClone() */

undefined4 * Route::getExactClone(void)

{
  __fn *in_r0;
  undefined4 *puVar1;
  void *in_r1;
  int in_r2;
  void *in_r3;
  uint uVar2;
  
  puVar1 = (undefined4 *)clone(in_r0,in_r1,in_r2,in_r3);
  for (uVar2 = 0; uVar2 < *(uint *)puVar1[3]; uVar2 = uVar2 + 1) {
    if (*(char *)(*(int *)(*(int *)(*(int *)(in_r0 + 0xc) + 4) + uVar2 * 4) + 0x130) != '\0') {
      Waypoint::reached(*(Waypoint **)(((uint *)puVar1[3])[1] + uVar2 * 4));
    }
  }
  *puVar1 = *(undefined4 *)in_r0;
  return puVar1;
}

```

## target disasm
```
  0012c074: push {r4,r5,r6,r7,lr}
  0012c076: add r7,sp,#0xc
  0012c078: push.w r11
  0012c07c: mov r4,r0
  0012c07e: blx 0x00073f3c
  0012c082: mov r5,r0
  0012c084: movs r6,#0x0
  0012c086: b 0x0012c0a2
  0012c088: ldr r1,[r4,#0xc]
  0012c08a: ldr r1,[r1,#0x4]
  0012c08c: ldr.w r1,[r1,r6,lsl #0x2]
  0012c090: ldrb.w r1,[r1,#0x130]
  0012c094: cbz r1,0x0012c0a0
  0012c096: ldr r0,[r0,#0x4]
  0012c098: ldr.w r0,[r0,r6,lsl #0x2]
  0012c09c: blx 0x00074194
  0012c0a0: adds r6,#0x1
  0012c0a2: ldr r0,[r5,#0xc]
  0012c0a4: ldr r1,[r0,#0x0]
  0012c0a6: cmp r6,r1
  0012c0a8: bcc 0x0012c088
  0012c0aa: ldr r0,[r4,#0x0]
  0012c0ac: str r0,[r5,#0x0]
  0012c0ae: mov r0,r5
  0012c0b0: pop.w r11
  0012c0b4: pop {r4,r5,r6,r7,pc}
```
