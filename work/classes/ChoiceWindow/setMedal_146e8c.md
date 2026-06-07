# ChoiceWindow::setMedal
elf 0x146e8c body 350
Sig: undefined __thiscall setMedal(ChoiceWindow * this, int param_1, int param_2)

## decompile
```c

/* ChoiceWindow::setMedal(int, int) */

void __thiscall ChoiceWindow::setMedal(ChoiceWindow *this,int param_1,int param_2)

{
  String *pSVar1;
  int iVar2;
  PaintCanvas *this_00;
  undefined4 uVar3;
  code *pcVar4;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  String aSStack_84 [12];
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  undefined1 auStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar6 = *(int **)(DAT_0015703c + 0x156ea2);
  piVar7 = *(int **)(DAT_00157040 + 0x156ea6);
  local_24 = *piVar6;
  pSVar1 = (String *)GameText::getText(*piVar7);
  AbyssEngine::String::operator=((String *)(this + 0x3c),pSVar1);
  AbyssEngine::String::String(aSStack_48,(char *)(DAT_00157044 + 0x156ed2),false);
  uVar3 = **(undefined4 **)(DAT_00157048 + 0x156eda);
  pSVar1 = (String *)GameText::getText(*piVar7);
  AbyssEngine::String::String(aSStack_60,pSVar1,false);
  iVar2 = Achievements::getValue
                    ((Achievements *)**(undefined4 **)(DAT_0015704c + 0x156ef8),param_1,param_2);
  AbyssEngine::String::String(aSStack_78,iVar2);
  AbyssEngine::String::String(aSStack_6c,aSStack_78,false);
  Status::replaceHash(auStack_54,uVar3,aSStack_60,aSStack_6c);
  AbyssEngine::operator+(aSStack_3c,aSStack_48);
  AbyssEngine::String::String(aSStack_84,(char *)(DAT_00157050 + 0x156f30),false);
  AbyssEngine::operator+(aSStack_30,aSStack_3c);
  pcVar4 = *(code **)(DAT_00157054 + 0x156f46);
  (*pcVar4)(aSStack_84);
  (*pcVar4)(aSStack_3c);
  (*pcVar4)(auStack_54);
  (*pcVar4)(aSStack_6c);
  (*pcVar4)(aSStack_78);
  (*pcVar4)(aSStack_60);
  (*pcVar4)(aSStack_48);
  pSVar1 = (String *)GameText::getText(*piVar7);
  set(this,pSVar1,aSStack_30,false);
  ScrollTouchWindow::setTextCentered(SUB41(*(undefined4 *)(this + 0x1c),0));
  puVar5 = *(undefined4 **)(DAT_00157058 + 0x156f8c);
  this_00 = (PaintCanvas *)*puVar5;
  if (param_2 < 0x24) {
    AbyssEngine::PaintCanvas::Image2DCreate
              (this_00,*(ushort *)(DAT_00157060 + 0x156fa8 + param_2 * 4),(uint *)(this + 0x34));
  }
  else {
    AbyssEngine::PaintCanvas::Image2DCreate
              (this_00,*(ushort *)(DAT_0015705c + 0x156f96 + param_2 * 4),(uint *)(this + 0x34));
  }
  AbyssEngine::PaintCanvas::Image2DCreate
            ((PaintCanvas *)*puVar5,*(ushort *)(DAT_00157064 + 0x156fba + param_1 * 4),
             (uint *)(this + 0x38));
  *(int *)(this + 0x2c) = param_2;
  *(int *)(this + 0x30) = param_1;
  this[0x59] = (ChoiceWindow)0x1;
  AbyssEngine::String::~String(aSStack_30);
  if (*piVar6 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00156e8c: push {r4,r5,r6,r7,lr}
  00156e8e: add r7,sp,#0xc
  00156e90: push {r8,r9,r10}
  00156e94: sub sp,#0x68
  00156e96: mov r4,r0
  00156e98: ldr r0,[0x0015703c]
  00156e9a: mov r5,r1
  00156e9c: ldr r1,[0x00157040]
  00156e9e: add r0,pc
  00156ea0: mov r8,r2
  00156ea2: add r1,pc
  00156ea4: ldr.w r9,[r0,#0x0]
  00156ea8: ldr.w r10,[r1,#0x0]
  00156eac: ldr.w r1,[r9,#0x0]
  00156eb0: ldr.w r0,[r10,#0x0]
  00156eb4: str r1,[sp,#0x64]
  00156eb6: addw r1,r5,#0x5e3
  00156eba: blx 0x00072f70
  00156ebe: mov r1,r0
  00156ec0: add.w r0,r4,#0x3c
  00156ec4: blx 0x0006f2b0
  00156ec8: ldr r1,[0x00157044]
  00156eca: add r0,sp,#0x40
  00156ecc: movs r2,#0x0
  00156ece: add r1,pc
  00156ed0: blx 0x0006ee18
  00156ed4: ldr r0,[0x00157048]
  00156ed6: add r0,pc
  00156ed8: ldr r0,[r0,#0x0]
  00156eda: ldr r6,[r0,#0x0]
  00156edc: ldr.w r0,[r10,#0x0]
  00156ee0: add.w r1,r5,#0x610
  00156ee4: blx 0x00072f70
  00156ee8: mov r1,r0
  00156eea: add r0,sp,#0x28
  00156eec: movs r2,#0x0
  00156eee: blx 0x0006f028
  00156ef2: ldr r0,[0x0015704c]
  00156ef4: add r0,pc
  00156ef6: ldr r0,[r0,#0x0]
  00156ef8: ldr r0,[r0,#0x0]
  00156efa: mov r1,r5
  00156efc: mov r2,r8
  00156efe: blx 0x00072f04
  00156f02: mov r1,r0
  00156f04: add r0,sp,#0x10
  00156f06: blx 0x0006f658
  00156f0a: add r0,sp,#0x1c
  00156f0c: add r1,sp,#0x10
  00156f0e: movs r2,#0x0
  00156f10: blx 0x0006f028
  00156f14: add r0,sp,#0x34
  00156f16: add r2,sp,#0x28
  00156f18: add r3,sp,#0x1c
  00156f1a: mov r1,r6
  00156f1c: blx 0x00076d8c
  00156f20: add r0,sp,#0x4c
  00156f22: add r1,sp,#0x40
  00156f24: add r2,sp,#0x34
  00156f26: blx 0x0006ef98
  00156f2a: ldr r1,[0x00157050]
  00156f2c: add r1,pc
  00156f2e: add r0,sp,#0x4
  00156f30: movs r2,#0x0
  00156f32: blx 0x0006ee18
  00156f36: add r0,sp,#0x58
  00156f38: add r1,sp,#0x4c
  00156f3a: add r2,sp,#0x4
  00156f3c: blx 0x0006ef98
  00156f40: ldr r0,[0x00157054]
  00156f42: add r0,pc
  00156f44: ldr r6,[r0,#0x0]
  00156f46: add r0,sp,#0x4
  00156f48: blx r6
  00156f4a: add r0,sp,#0x4c
  00156f4c: blx r6
  00156f4e: add r0,sp,#0x34
  00156f50: blx r6
  00156f52: add r0,sp,#0x1c
  00156f54: blx r6
  00156f56: add r0,sp,#0x10
  00156f58: blx r6
  00156f5a: add r0,sp,#0x28
  00156f5c: blx r6
  00156f5e: add r0,sp,#0x40
  00156f60: blx r6
  00156f62: ldr.w r0,[r10,#0x0]
  00156f66: movw r1,#0x161
  00156f6a: blx 0x00072f70
  00156f6e: mov r1,r0
  00156f70: add r2,sp,#0x58
  00156f72: mov r0,r4
  00156f74: movs r3,#0x0
  00156f76: blx 0x00074ea8
  00156f7a: ldr r0,[r4,#0x1c]
  00156f7c: movs r1,#0x1
  00156f7e: blx 0x00076528
  00156f82: ldr r0,[0x00157058]
  00156f84: cmp.w r8,#0x24
  00156f88: add r0,pc
  00156f8a: ldr r6,[r0,#0x0]
  00156f8c: ldr r0,[r6,#0x0]
  00156f8e: blt 0x00156fa2
  00156f90: ldr r1,[0x0015705c]
  00156f92: add r1,pc
  00156f94: ldrh.w r1,[r1,r8,lsl #0x2]
  00156f98: add.w r2,r4,#0x34
  00156f9c: blx 0x00071cf8
  00156fa0: b 0x00156fb2
  00156fa2: ldr r1,[0x00157060]
  00156fa4: add r1,pc
  00156fa6: ldrh.w r1,[r1,r8,lsl #0x2]
  00156faa: add.w r2,r4,#0x34
  00156fae: blx 0x00071cf8
  00156fb2: ldr r1,[0x00157064]
  00156fb4: ldr r0,[r6,#0x0]
  00156fb6: add r1,pc
  00156fb8: ldrh.w r1,[r1,r5,lsl #0x2]
  00156fbc: add.w r2,r4,#0x38
  00156fc0: blx 0x00071cf8
  00156fc4: movs r0,#0x1
  00156fc6: strd r8,r5,[r4,#0x2c]
  00156fca: strb.w r0,[r4,#0x59]
  00156fce: add r0,sp,#0x58
  00156fd0: blx 0x0006ee30
  00156fd4: ldr r0,[sp,#0x64]
  00156fd6: ldr.w r1,[r9,#0x0]
  00156fda: subs r0,r1,r0
  00156fdc: ittt eq
  00156fde: add.eq sp,#0x68
  00156fe0: pop.eq.w {r8,r9,r10}
  00156fe4: pop.eq {r4,r5,r6,r7,pc}
  00156fe6: blx 0x0006e824
```
