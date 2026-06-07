# PaintCanvas::ChangeCubeTexture
elf 0x7c288 body 96
Sig: undefined __stdcall ChangeCubeTexture(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::ChangeCubeTexture(unsigned int) */

void AbyssEngine::PaintCanvas::ChangeCubeTexture(uint param_1)

{
  uint in_r1;
  
  if ((**(char **)(DAT_0008c2e8 + 0x8c296) != '\0') && (in_r1 < *(uint *)(param_1 + 0x10))) {
    if (*(char *)(*(int *)(*(int *)(param_1 + 0x14) + in_r1 * 4) + 0x14) == '\0') {
      String::GetAEChar();
      (*(code *)(DAT_001ab114 + 0x1ab118))();
      return;
    }
    **(uint **)(DAT_0008c2ec + 0x8c2b0) = in_r1;
    glActiveTexture(0x84c7);
    glBindTexture(0x8513,**(undefined4 **)(*(int *)(param_1 + 0x14) + in_r1 * 4));
    (*(code *)(DAT_001ab314 + 0x1ab318))(0x84c0);
    return;
  }
  return;
}

```

## target disasm
```
  0008c288: push {r4,r5,r7,lr}
  0008c28a: add r7,sp,#0x8
  0008c28c: mov r5,r0
  0008c28e: ldr r0,[0x0008c2e8]
  0008c290: mov r4,r1
  0008c292: add r0,pc
  0008c294: ldr r0,[r0,#0x0]
  0008c296: ldrb r0,[r0,#0x0]
  0008c298: cbz r0,0x0008c2d6
  0008c29a: ldr r0,[r5,#0x10]
  0008c29c: cmp r0,r4
  0008c29e: bls 0x0008c2d6
  0008c2a0: ldr r0,[r5,#0x14]
  0008c2a2: ldr.w r0,[r0,r4,lsl #0x2]
  0008c2a6: ldrb r1,[r0,#0x14]
  0008c2a8: cbz r1,0x0008c2d8
  0008c2aa: ldr r0,[0x0008c2ec]
  0008c2ac: add r0,pc
  0008c2ae: ldr r0,[r0,#0x0]
  0008c2b0: str r4,[r0,#0x0]
  0008c2b2: movw r0,#0x84c7
  0008c2b6: blx 0x0006fb8c
  0008c2ba: ldr r0,[r5,#0x14]
  0008c2bc: ldr.w r0,[r0,r4,lsl #0x2]
  0008c2c0: ldr r1,[r0,#0x0]
  0008c2c2: movw r0,#0x8513
  0008c2c6: blx 0x0006f244
  0008c2ca: movw r0,#0x84c0
  0008c2ce: pop.w {r4,r5,r7,lr}
  0008c2d2: b.w 0x001ab308
  0008c2d6: pop {r4,r5,r7,pc}
  0008c2d8: adds r0,#0x4
  0008c2da: blx 0x0006ef68
  0008c2de: pop.w {r4,r5,r7,lr}
  0008c2e2: b.w 0x001ab108
  001ab308: bx pc
```
