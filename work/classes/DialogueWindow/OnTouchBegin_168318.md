# DialogueWindow::OnTouchBegin
elf 0x168318 body 82
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* DialogueWindow::OnTouchBegin(int, int) */

undefined4 DialogueWindow::OnTouchBegin(int param_1,int param_2)

{
  code *pcVar1;
  
  if (*(char *)(param_1 + 0x54) == '\0') {
    ScrollTouchWindow::OnTouchBegin(*(int *)(param_1 + 0x40),param_2);
    pcVar1 = *(code **)(DAT_0017836c + 0x17834c);
    (*pcVar1)(*(undefined4 *)param_1,param_2);
    (*pcVar1)(*(undefined4 *)(param_1 + 4),param_2);
    (*pcVar1)(*(undefined4 *)(param_1 + 8),param_2);
  }
  else {
    ChoiceWindow::OnTouchBegin(*(int *)(param_1 + 0x50),param_2);
  }
  return 0;
}

```

## target disasm
```
  00178318: push {r4,r5,r6,r7,lr}
  0017831a: add r7,sp,#0xc
  0017831c: push.w r8
  00178320: mov r6,r0
  00178322: ldrb.w r0,[r0,#0x54]
  00178326: mov r4,r2
  00178328: mov r5,r1
  0017832a: cbz r0,0x00178338
  0017832c: ldr r0,[r6,#0x50]
  0017832e: mov r1,r5
  00178330: mov r2,r4
  00178332: blx 0x0007471c
  00178336: b 0x00178362
  00178338: ldr r0,[r6,#0x40]
  0017833a: mov r1,r5
  0017833c: mov r2,r4
  0017833e: blx 0x00075ba4
  00178342: ldr r1,[0x0017836c]
  00178344: mov r2,r4
  00178346: ldr r0,[r6,#0x0]
  00178348: add r1,pc
  0017834a: ldr.w r8,[r1,#0x0]
  0017834e: mov r1,r5
  00178350: blx r8
  00178352: ldr r0,[r6,#0x4]
  00178354: mov r1,r5
  00178356: mov r2,r4
  00178358: blx r8
  0017835a: ldr r0,[r6,#0x8]
  0017835c: mov r1,r5
  0017835e: mov r2,r4
  00178360: blx r8
  00178362: movs r0,#0x0
  00178364: pop.w r8
  00178368: pop {r4,r5,r6,r7,pc}
```
