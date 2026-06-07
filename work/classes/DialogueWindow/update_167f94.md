# DialogueWindow::update
elf 0x167f94 body 108
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* DialogueWindow::update(int) */

void DialogueWindow::update(int param_1)

{
  int iVar1;
  int in_r1;
  int *piVar2;
  
  if (*(int *)(param_1 + 0x40) != 0) {
    ScrollTouchWindow::update(*(int *)(param_1 + 0x40));
  }
  if (*(char *)(param_1 + 0x54) != '\0') {
    ChoiceWindow::update(*(int *)(param_1 + 0x50));
  }
  if ((*(char *)(*(int *)(DAT_00178000 + 0x177fbe) + 0xe) != '\0') &&
     (*(int *)(param_1 + 100) != -1)) {
    piVar2 = *(int **)(DAT_00178004 + 0x177fce);
    FModSound::getPlayingProgress((FModSound *)*piVar2,*(int *)(param_1 + 100));
    iVar1 = FModSound::isPlaying(*piVar2);
    if ((iVar1 == 0) && (iVar1 = isLastPage((DialogueWindow *)param_1), iVar1 == 0)) {
      iVar1 = *(int *)(param_1 + 0x68);
      if (*(int *)(param_1 + 0x6c) <= iVar1) {
        nextPage((DialogueWindow *)param_1);
        iVar1 = *(int *)(param_1 + 0x68);
      }
      *(int *)(param_1 + 0x68) = iVar1 + in_r1;
    }
  }
  return;
}

```

## target disasm
```
  00177f94: push {r4,r5,r6,r7,lr}
  00177f96: add r7,sp,#0xc
  00177f98: push.w r11
  00177f9c: mov r4,r0
  00177f9e: ldr r0,[r0,#0x40]
  00177fa0: mov r5,r1
  00177fa2: cbz r0,0x00177faa
  00177fa4: mov r1,r5
  00177fa6: blx 0x00075b98
  00177faa: ldrb.w r0,[r4,#0x54]
  00177fae: cbz r0,0x00177fb8
  00177fb0: ldr r0,[r4,#0x50]
  00177fb2: mov r1,r5
  00177fb4: blx 0x00074eb4
  00177fb8: ldr r0,[0x00178000]
  00177fba: add r0,pc
  00177fbc: ldr r0,[r0,#0x0]
  00177fbe: ldrb r0,[r0,#0xe]
  00177fc0: cbz r0,0x00177ffa
  00177fc2: ldr r1,[r4,#0x64]
  00177fc4: adds r0,r1,#0x1
  00177fc6: beq 0x00177ffa
  00177fc8: ldr r0,[0x00178004]
  00177fca: add r0,pc
  00177fcc: ldr r6,[r0,#0x0]
  00177fce: ldr r0,[r6,#0x0]
  00177fd0: blx 0x00077c5c
  00177fd4: ldr r0,[r6,#0x0]
  00177fd6: ldr r1,[r4,#0x64]
  00177fd8: blx 0x0007153c
  00177fdc: cbnz r0,0x00177ffa
  00177fde: mov r0,r4
  00177fe0: blx 0x00077c44
  00177fe4: cbnz r0,0x00177ffa
  00177fe6: ldrd r0,r1,[r4,#0x68]
  00177fea: cmp r0,r1
  00177fec: blt 0x00177ff6
  00177fee: mov r0,r4
  00177ff0: blx 0x00077c68
  00177ff4: ldr r0,[r4,#0x68]
  00177ff6: add r0,r5
  00177ff8: str r0,[r4,#0x68]
  00177ffa: pop.w r11
  00177ffe: pop {r4,r5,r6,r7,pc}
```
