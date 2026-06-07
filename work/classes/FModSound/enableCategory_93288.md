# FModSound::enableCategory
elf 0x93288 body 108
Sig: undefined __stdcall enableCategory(int param_1, bool param_2)

## decompile
```c

/* FModSound::enableCategory(int, bool) */

void FModSound::enableCategory(int param_1,bool param_2)

{
  char *pcVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  int in_r2;
  bool bVar5;
  float in_s0;
  
  uVar3 = (uint)param_2;
  if (((uVar3 < 4) && (*(int *)(param_1 + 0x23fc) != 0)) &&
     (piVar2 = *(int **)(param_1 + uVar3 * 4 + 0x23ec), piVar2 != (int *)0x0)) {
    *(char *)(param_1 + uVar3 + 0x11) = (char)in_r2;
    if (in_r2 == 0) {
      (**(code **)(*piVar2 + 0x1c))();
    }
    else if ((uVar3 == 1) && (-1 < (int)*(Vector **)param_1)) {
      play(param_1,*(Vector **)param_1,(Vector *)0x0,in_s0);
    }
    bVar5 = false;
    iVar4 = 0x12;
    do {
      if (3 < iVar4 - 0x11U) break;
      pcVar1 = (char *)(param_1 + iVar4);
      iVar4 = iVar4 + 1;
      bVar5 = bVar5 != false || *pcVar1 != '\0';
    } while (*pcVar1 == '\0');
    *(bool *)(param_1 + 0x11) = bVar5;
  }
  return;
}

```

## target disasm
```
  000a3288: push {r2,r3,r4,r6,r7,lr}
  000a328a: add r7,sp,#0x10
  000a328c: mov r4,r0
  000a328e: cmp r1,#0x3
  000a3290: bgt 0x000a32f2
  000a3292: movw r0,#0x23fc
  000a3296: ldr r0,[r4,r0]
  000a3298: cbz r0,0x000a32f2
  000a329a: add.w r0,r4,r1, lsl #0x2
  000a329e: movw r3,#0x23ec
  000a32a2: ldr r0,[r0,r3]
  000a32a4: cbz r0,0x000a32f2
  000a32a6: adds r3,r4,r1
  000a32a8: cmp r2,#0x0
  000a32aa: strb r2,[r3,#0x11]
  000a32ac: beq 0x000a32c8
  000a32ae: cmp r1,#0x1
  000a32b0: bne 0x000a32ce
  000a32b2: ldr r1,[r4,#0x0]
  000a32b4: cmp r1,#0x0
  000a32b6: blt 0x000a32ce
  000a32b8: movs r0,#0x0
  000a32ba: movs r2,#0x0
  000a32bc: str r0,[sp,#0x0]
  000a32be: mov r0,r4
  000a32c0: movs r3,#0x0
  000a32c2: blx 0x00071548
  000a32c6: b 0x000a32ce
  000a32c8: ldr r1,[r0,#0x0]
  000a32ca: ldr r1,[r1,#0x1c]
  000a32cc: blx r1
  000a32ce: movs r0,#0x0
  000a32d0: movs r1,#0x12
  000a32d2: sub.w r2,r1,#0x11
  000a32d6: cmp r2,#0x3
  000a32d8: bhi 0x000a32ec
  000a32da: ldrb r2,[r4,r1]
  000a32dc: and r0,r0,#0x1
  000a32e0: adds r1,#0x1
  000a32e2: orrs r0,r2
  000a32e4: it ne
  000a32e6: mov.ne r0,#0x1
  000a32e8: cmp r2,#0x0
  000a32ea: beq 0x000a32d2
  000a32ec: and r0,r0,#0x1
  000a32f0: strb r0,[r4,#0x11]
  000a32f2: pop {r2,r3,r4,r6,r7,pc}
```
