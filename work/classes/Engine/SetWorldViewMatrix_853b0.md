# Engine::SetWorldViewMatrix
elf 0x853b0 body 192
Sig: undefined __stdcall SetWorldViewMatrix(Matrix * param_1)

## decompile
```c

/* AbyssEngine::Engine::SetWorldViewMatrix(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::Engine::SetWorldViewMatrix(Matrix *param_1)

{
  Matrix *in_r1;
  int *piVar1;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  int local_24;
  
  piVar1 = *(int **)(DAT_00095474 + 0x953c6);
  local_24 = *piVar1;
  if (**(char **)(DAT_00095470 + 0x953c4) != '\0') {
    local_68 = *(undefined4 *)in_r1;
    local_58 = *(undefined4 *)(in_r1 + 4);
    uStack_48 = *(undefined4 *)(in_r1 + 8);
    uStack_38 = *(undefined4 *)(in_r1 + 0xc);
    uStack_64 = *(undefined4 *)(in_r1 + 0x10);
    uStack_54 = *(undefined4 *)(in_r1 + 0x14);
    local_44 = *(undefined4 *)(in_r1 + 0x18);
    local_34 = *(undefined4 *)(in_r1 + 0x1c);
    local_60 = *(undefined4 *)(in_r1 + 0x20);
    uStack_5c = 0;
    local_50 = *(undefined4 *)(in_r1 + 0x24);
    uStack_4c = 0;
    local_40 = *(undefined4 *)(in_r1 + 0x28);
    uStack_3c = 0;
    local_30 = *(undefined4 *)(in_r1 + 0x2c);
    uStack_2c = 0x3f800000;
    __aeabi_memcpy(param_1 + 0x184,&local_68,0x40);
    esMatrixMultiply((ESMatrix *)(param_1 + 0x104),(ESMatrix *)&local_68,(ESMatrix *)(param_1 + 900)
                    );
    if (*piVar1 == local_24) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  AEMath::MatrixGetGL(in_r1,(float *)(param_1 + 0x428));
  if (*piVar1 - local_24 == 0) {
    (*(code *)(DAT_001ab654 + 0x1ab658))();
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(*piVar1 - local_24);
}

```

## target disasm
```
  000953b0: push {r4,r5,r6,r7,lr}
  000953b2: add r7,sp,#0xc
  000953b4: push {r8,r9,r11}
  000953b8: sub sp,#0x48
  000953ba: mov r4,r0
  000953bc: ldr r0,[0x00095470]
  000953be: ldr r2,[0x00095474]
  000953c0: add r0,pc
  000953c2: add r2,pc
  000953c4: ldr r0,[r0,#0x0]
  000953c6: ldr r6,[r2,#0x0]
  000953c8: ldrb r0,[r0,#0x0]
  000953ca: ldr r2,[r6,#0x0]
  000953cc: cmp r0,#0x0
  000953ce: str r2,[sp,#0x44]
  000953d0: beq 0x00095444
  000953d2: add.w r9,r1,#0x14
  000953d6: ldm.w r1,{r0,r2,lr}
  000953da: ldrd r12,r5,[r1,#0xc]
  000953de: ldm.w r9,{r3,r8,r9}
  000953e2: strd r0,r5,[sp,#0x0]
  000953e6: movs r5,#0x0
  000953e8: ldr r0,[r1,#0x20]
  000953ea: strd r0,r5,[sp,#0x8]
  000953ee: strd r2,r3,[sp,#0x10]
  000953f2: add r2,sp,#0x18
  000953f4: ldr r0,[r1,#0x24]
  000953f6: stm.w r2!,{r0,r5,lr}
  000953fa: add r2,sp,#0x28
  000953fc: str.w r8,[sp,#0x24]
  00095400: ldr r0,[r1,#0x28]
  00095402: stm r2,{r0,r5,r12}
  00095406: mov r5,sp
  00095408: movs r2,#0x40
  0009540a: str.w r9,[sp,#0x34]
  0009540e: ldr r0,[r1,#0x2c]
  00095410: mov.w r1,#0x3f800000
  00095414: strd r0,r1,[sp,#0x38]
  00095418: add.w r0,r4,#0x184
  0009541c: mov r1,r5
  0009541e: blx 0x0006f1e4
  00095422: add.w r0,r4,#0x104
  00095426: add.w r2,r4,#0x384
  0009542a: mov r1,r5
  0009542c: blx 0x00070780
  00095430: ldr r0,[sp,#0x44]
  00095432: ldr r1,[r6,#0x0]
  00095434: subs r0,r1,r0
  00095436: ittt eq
  00095438: add.eq sp,#0x48
  0009543a: pop.eq.w {r8,r9,r11}
  0009543e: pop.eq {r4,r5,r6,r7,pc}
  00095440: blx 0x0006e824
  00095444: add.w r4,r4,#0x428
  00095448: mov r0,r1
  0009544a: mov r1,r4
  0009544c: blx 0x00070750
  00095450: ldr r0,[sp,#0x44]
  00095452: ldr r1,[r6,#0x0]
  00095454: subs r0,r1,r0
  00095456: itttt eq
  00095458: mov.eq r0,r4
  0009545a: add.eq sp,#0x48
  0009545c: pop.eq.w {r8,r9,r11}
  00095460: pop.eq.w {r4,r5,r6,r7,lr}
  00095464: it eq
  00095466: b.eq.w 0x001ab648
  0009546a: blx 0x0006e824
  001ab648: bx pc
```
