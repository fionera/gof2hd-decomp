# Route::reset
elf 0x11bd94 body 36
Sig: undefined __stdcall reset(void)

## decompile
```c

/* Route::reset() */

void Route::reset(void)

{
  undefined4 *in_r0;
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < *(uint *)in_r0[3]; uVar1 = uVar1 + 1) {
    Waypoint::reset(*(Waypoint **)(((uint *)in_r0[3])[1] + uVar1 * 4));
  }
  *in_r0 = 0;
  return;
}

```

## target disasm
```
  0012bd94: push {r4,r5,r7,lr}
  0012bd96: add r7,sp,#0x8
  0012bd98: mov r4,r0
  0012bd9a: movs r5,#0x0
  0012bd9c: b 0x0012bdaa
  0012bd9e: ldr r0,[r0,#0x4]
  0012bda0: ldr.w r0,[r0,r5,lsl #0x2]
  0012bda4: blx 0x00075f94
  0012bda8: adds r5,#0x1
  0012bdaa: ldr r0,[r4,#0xc]
  0012bdac: ldr r1,[r0,#0x0]
  0012bdae: cmp r5,r1
  0012bdb0: bcc 0x0012bd9e
  0012bdb2: movs r0,#0x0
  0012bdb4: str r0,[r4,#0x0]
  0012bdb6: pop {r4,r5,r7,pc}
```
