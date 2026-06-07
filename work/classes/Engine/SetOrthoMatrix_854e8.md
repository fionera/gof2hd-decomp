# Engine::SetOrthoMatrix
elf 0x854e8 body 216
Sig: undefined __thiscall SetOrthoMatrix(Engine * this, float * param_1, float * param_2, bool param_3)

## decompile
```c

/* AbyssEngine::Engine::SetOrthoMatrix(float*, float*, bool) */

void __thiscall
AbyssEngine::Engine::SetOrthoMatrix(Engine *this,float *param_1,float *param_2,bool param_3)

{
  int *piVar1;
  undefined8 local_5c;
  undefined8 uStack_54;
  undefined8 local_4c;
  undefined8 uStack_44;
  undefined8 local_3c;
  undefined8 uStack_34;
  undefined8 local_2c;
  undefined8 uStack_24;
  int local_1c;
  
  piVar1 = *(int **)(DAT_000955c0 + 0x954fa);
  local_1c = *piVar1;
  if (**(char **)(DAT_000955c4 + 0x954fc) != '\0') {
    *(float *)(this + 900) = *param_1;
    *(float *)(this + 0x388) = param_1[1];
    *(float *)(this + 0x38c) = param_1[2];
    *(float *)(this + 0x390) = param_1[3];
    *(float *)(this + 0x394) = param_1[4];
    *(float *)(this + 0x398) = param_1[5];
    *(float *)(this + 0x39c) = param_1[6];
    *(float *)(this + 0x3a0) = param_1[7];
    *(float *)(this + 0x3a4) = param_1[8];
    *(float *)(this + 0x3a8) = param_1[9];
    *(float *)(this + 0x3ac) = param_1[10];
    *(float *)(this + 0x3b0) = param_1[0xb];
    *(float *)(this + 0x3b4) = param_1[0xc];
    *(float *)(this + 0x3b8) = param_1[0xd];
    *(float *)(this + 0x3bc) = param_1[0xe];
    *(float *)(this + 0x3c0) = param_1[0xf];
    if (param_3) {
      local_5c = *(undefined8 *)param_2;
      uStack_54 = *(undefined8 *)(param_2 + 2);
      local_4c = *(undefined8 *)(param_2 + 4);
      uStack_44 = *(undefined8 *)(param_2 + 6);
      local_3c = *(undefined8 *)(param_2 + 8);
      uStack_34 = *(undefined8 *)(param_2 + 10);
      local_2c = *(undefined8 *)(param_2 + 0xc);
      uStack_24 = *(undefined8 *)(param_2 + 0xe);
      esMatrixMultiply((ESMatrix *)(this + 900),(ESMatrix *)&local_5c,(ESMatrix *)(this + 900));
    }
  }
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000954e8: push {r4,r5,r6,r7,lr}
  000954ea: add r7,sp,#0xc
  000954ec: push.w r11
  000954f0: sub sp,#0x48
  000954f2: ldr r4,[0x000955c0]
  000954f4: ldr r5,[0x000955c4]
  000954f6: add r4,pc
  000954f8: add r5,pc
  000954fa: ldr r6,[r4,#0x0]
  000954fc: ldr r5,[r5,#0x0]
  000954fe: ldr r4,[r6,#0x0]
  00095500: ldrb r5,[r5,#0x0]
  00095502: str r4,[sp,#0x44]
  00095504: cmp r5,#0x0
  00095506: beq 0x000955ac
  00095508: ldr r5,[r1,#0x0]
  0009550a: cmp r3,#0x0
  0009550c: str.w r5,[r0,#0x384]
  00095510: ldr r5,[r1,#0x4]
  00095512: str.w r5,[r0,#0x388]
  00095516: ldr r5,[r1,#0x8]
  00095518: str.w r5,[r0,#0x38c]
  0009551c: ldr r5,[r1,#0xc]
  0009551e: str.w r5,[r0,#0x390]
  00095522: ldr r5,[r1,#0x10]
  00095524: str.w r5,[r0,#0x394]
  00095528: ldr r5,[r1,#0x14]
  0009552a: str.w r5,[r0,#0x398]
  0009552e: ldr r5,[r1,#0x18]
  00095530: str.w r5,[r0,#0x39c]
  00095534: ldr r5,[r1,#0x1c]
  00095536: str.w r5,[r0,#0x3a0]
  0009553a: ldr r5,[r1,#0x20]
  0009553c: str.w r5,[r0,#0x3a4]
  00095540: ldr r5,[r1,#0x24]
  00095542: str.w r5,[r0,#0x3a8]
  00095546: ldr r5,[r1,#0x28]
  00095548: str.w r5,[r0,#0x3ac]
  0009554c: ldr r5,[r1,#0x2c]
  0009554e: str.w r5,[r0,#0x3b0]
  00095552: ldr r5,[r1,#0x30]
  00095554: str.w r5,[r0,#0x3b4]
  00095558: ldr r5,[r1,#0x34]
  0009555a: str.w r5,[r0,#0x3b8]
  0009555e: ldr r5,[r1,#0x38]
  00095560: str.w r5,[r0,#0x3bc]
  00095564: ldr r1,[r1,#0x3c]
  00095566: str.w r1,[r0,#0x3c0]
  0009556a: beq 0x000955ac
  0009556c: add r1,sp,#0x4
  0009556e: movs r3,#0x30
  00095570: mov r5,r2
  00095572: add.w r0,r0,#0x384
  00095576: vld1.32 {d16,d17},[r5],r3
  0009557a: mov r4,r1
  0009557c: vst1.32 {d16,d17},[r4],r3
  00095580: add.w r3,r2,#0x10
  00095584: adds r2,#0x20
  00095586: vld1.32 {d16,d17},[r3]
  0009558a: add.w r3,r1,#0x10
  0009558e: vst1.32 {d16,d17},[r3]
  00095592: vld1.32 {d16,d17},[r2]
  00095596: add.w r2,r1,#0x20
  0009559a: vst1.32 {d16,d17},[r2]
  0009559e: mov r2,r0
  000955a0: vld1.32 {d16,d17},[r5]
  000955a4: vst1.32 {d16,d17},[r4]
  000955a8: blx 0x00070780
  000955ac: ldr r0,[sp,#0x44]
  000955ae: ldr r1,[r6,#0x0]
  000955b0: subs r0,r1,r0
  000955b2: ittt eq
  000955b4: add.eq sp,#0x48
  000955b6: pop.eq.w r11
  000955ba: pop.eq {r4,r5,r6,r7,pc}
  000955bc: blx 0x0006e824
```
