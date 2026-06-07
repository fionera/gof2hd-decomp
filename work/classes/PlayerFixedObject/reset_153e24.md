# PlayerFixedObject::reset
elf 0x153e24 body 182
Sig: undefined __thiscall reset(PlayerFixedObject * this)

## decompile
```c

/* PlayerFixedObject::reset() */

void __thiscall PlayerFixedObject::reset(PlayerFixedObject *this)

{
  code *pcVar1;
  int *piVar2;
  undefined8 local_28;
  undefined4 local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_00163edc + 0x163e36);
  local_1c = *piVar2;
  KIPlayer::reset((KIPlayer *)this);
  (**(code **)(*(int *)this + 0x48))
            (this,*(undefined4 *)(this + 0x58),*(undefined4 *)(this + 0x5c),
             *(undefined4 *)(this + 0x60));
  *(undefined4 *)(this + 0x168) = 0;
  local_28 = *(undefined8 *)(this + 0x58);
  local_20 = *(undefined4 *)(this + 0x60);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x138),(Vector *)&local_28);
  local_20 = *(undefined4 *)(this + 0x140);
  local_28 = *(undefined8 *)(this + 0x138);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x2c),(Vector *)&local_28);
  *(undefined4 *)(this + 0x130) = 0;
  if (*(int *)(this + 0x88) != 5) {
    *(undefined4 *)(this + 0x88) = 0;
  }
  local_28 = 0;
  local_20 = 0;
  pcVar1 = *(code **)(DAT_00163ee0 + 0x163ea2);
  (*pcVar1)(this + 0x90,&local_28);
  local_28 = 0;
  local_20 = 0;
  (*pcVar1)(this + 0x144,&local_28);
  local_28 = 0;
  local_20 = 0;
  (*pcVar1)(this + 0x150,&local_28);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00163e24: push {r4,r5,r6,r7,lr}
  00163e26: add r7,sp,#0xc
  00163e28: push.w r8
  00163e2c: sub sp,#0x10
  00163e2e: mov r4,r0
  00163e30: ldr r0,[0x00163edc]
  00163e32: add r0,pc
  00163e34: ldr.w r8,[r0,#0x0]
  00163e38: ldr.w r0,[r8,#0x0]
  00163e3c: str r0,[sp,#0xc]
  00163e3e: mov r0,r4
  00163e40: blx 0x00074518
  00163e44: ldr r0,[r4,#0x0]
  00163e46: add.w r3,r4,#0x58
  00163e4a: ldmia r3,{r1,r2,r3}
  00163e4c: ldr r6,[r0,#0x48]
  00163e4e: mov r0,r4
  00163e50: blx r6
  00163e52: movs r6,#0x0
  00163e54: add.w r2,r4,#0x58
  00163e58: str.w r6,[r4,#0x168]
  00163e5c: ldmia r2,{r0,r1,r2}
  00163e5e: stm sp,{r0,r1,r2}
  00163e62: add.w r0,r4,#0x138
  00163e66: mov r1,sp
  00163e68: blx 0x0006eb3c
  00163e6c: ldr.w r0,[r4,#0x140]
  00163e70: mov r1,sp
  00163e72: vldr.64 d16,[r4,#0x138]
  00163e76: str r0,[sp,#0x8]
  00163e78: add.w r0,r4,#0x2c
  00163e7c: vstr.64 d16,[sp]
  00163e80: blx 0x0006eb3c
  00163e84: ldr.w r0,[r4,#0x88]
  00163e88: str.w r6,[r4,#0x130]
  00163e8c: cmp r0,#0x5
  00163e8e: it ne
  00163e90: str.w.ne r6,[r4,#0x88]
  00163e94: ldr r1,[0x00163ee0]
  00163e96: add.w r0,r4,#0x90
  00163e9a: strd r6,r6,[sp,#0x0]
  00163e9e: add r1,pc
  00163ea0: str r6,[sp,#0x8]
  00163ea2: ldr r5,[r1,#0x0]
  00163ea4: mov r1,sp
  00163ea6: blx r5
  00163ea8: add.w r0,r4,#0x144
  00163eac: mov r1,sp
  00163eae: strd r6,r6,[sp,#0x0]
  00163eb2: str r6,[sp,#0x8]
  00163eb4: blx r5
  00163eb6: add.w r0,r4,#0x150
  00163eba: mov r1,sp
  00163ebc: strd r6,r6,[sp,#0x0]
  00163ec0: str r6,[sp,#0x8]
  00163ec2: blx r5
  00163ec4: ldr r0,[sp,#0xc]
  00163ec6: ldr.w r1,[r8,#0x0]
  00163eca: subs r0,r1,r0
  00163ecc: ittt eq
  00163ece: add.eq sp,#0x10
  00163ed0: pop.eq.w r8
  00163ed4: pop.eq {r4,r5,r6,r7,pc}
  00163ed6: blx 0x0006e824
```
