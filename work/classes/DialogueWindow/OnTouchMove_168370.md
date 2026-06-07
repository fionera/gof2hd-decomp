# DialogueWindow::OnTouchMove
elf 0x168370 body 82
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* DialogueWindow::OnTouchMove(int, int) */

undefined4 DialogueWindow::OnTouchMove(int param_1,int param_2)

{
  code *pcVar1;
  
  if (*(char *)(param_1 + 0x54) == '\0') {
    ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 0x40),param_2);
    pcVar1 = *(code **)(DAT_001783c4 + 0x1783a4);
    (*pcVar1)(*(undefined4 *)param_1,param_2);
    (*pcVar1)(*(undefined4 *)(param_1 + 4),param_2);
    (*pcVar1)(*(undefined4 *)(param_1 + 8),param_2);
  }
  else {
    ChoiceWindow::OnTouchMove(*(int *)(param_1 + 0x50),param_2);
  }
  return 0;
}

```

## target disasm
```
  00178370: push {r4,r5,r6,r7,lr}
  00178372: add r7,sp,#0xc
  00178374: push.w r8
  00178378: mov r6,r0
  0017837a: ldrb.w r0,[r0,#0x54]
  0017837e: mov r4,r2
  00178380: mov r5,r1
  00178382: cbz r0,0x00178390
  00178384: ldr r0,[r6,#0x50]
  00178386: mov r1,r5
  00178388: mov r2,r4
  0017838a: blx 0x0007474c
  0017838e: b 0x001783ba
  00178390: ldr r0,[r6,#0x40]
  00178392: mov r1,r5
  00178394: mov r2,r4
  00178396: blx 0x00075bbc
  0017839a: ldr r1,[0x001783c4]
  0017839c: mov r2,r4
  0017839e: ldr r0,[r6,#0x0]
  001783a0: add r1,pc
  001783a2: ldr.w r8,[r1,#0x0]
  001783a6: mov r1,r5
  001783a8: blx r8
  001783aa: ldr r0,[r6,#0x4]
  001783ac: mov r1,r5
  001783ae: mov r2,r4
  001783b0: blx r8
  001783b2: ldr r0,[r6,#0x8]
  001783b4: mov r1,r5
  001783b6: mov r2,r4
  001783b8: blx r8
  001783ba: movs r0,#0x0
  001783bc: pop.w r8
  001783c0: pop {r4,r5,r6,r7,pc}
```
