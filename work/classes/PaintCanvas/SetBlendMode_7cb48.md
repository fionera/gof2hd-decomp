# PaintCanvas::SetBlendMode
elf 0x7cb48 body 730
Sig: undefined __thiscall SetBlendMode(PaintCanvas * this, BlendMode param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetBlendMode(AbyssEngine::BlendMode) */

void __thiscall AbyssEngine::PaintCanvas::SetBlendMode(PaintCanvas *this,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  code *UNRECOVERED_JUMPTABLE;
  char *pcVar3;
  
  Engine::LightEnable(SUB41(*(undefined4 *)(this + 0x34),0));
  pcVar3 = *(char **)(DAT_0008ce38 + 0x8cb62);
  if (*pcVar3 == '\0') {
    glTexEnvi(0x2300,0x2200,0x2100);
  }
  else {
    Engine::GlEnable(*(uint *)(this + 0x34),SUB41(DAT_0008ce34,0));
  }
  switch(param_2) {
  case 1:
  case 5:
    goto switchD_0008cb86_caseD_1;
  case 2:
    goto switchD_0008cb86_caseD_2;
  case 3:
    glDisable(0xb44);
    goto LAB_0008cc08;
  case 4:
    glEnable(0xb44);
    glEnable(0xbe2);
    uVar1 = 0;
    uVar2 = 0x301;
    goto LAB_0008cc40;
  case 6:
    Engine::LightEnable(SUB41(*(undefined4 *)(this + 0x34),0));
    Engine::LightSetLight(*(Engine **)(this + 0x34),0x4000);
  case 0:
    glEnable(0xb44);
    glDisable(0xbe2);
LAB_0008cc76:
    uVar1 = 1;
LAB_0008cc78:
    (*(code *)(DAT_001ab3a4 + 0x1ab3a8))(uVar1);
    return;
  case 7:
    Engine::LightEnable(SUB41(*(undefined4 *)(this + 0x34),0));
    Engine::LightSetLight(*(Engine **)(this + 0x34),0x4000);
    goto switchD_0008cb86_caseD_2;
  case 8:
    Engine::LightEnable(SUB41(*(undefined4 *)(this + 0x34),0));
    Engine::LightSetLight(*(Engine **)(this + 0x34),0x4000);
    goto switchD_0008cb86_caseD_1;
  case 9:
    Engine::LightEnable(SUB41(*(undefined4 *)(this + 0x34),0));
    Engine::LightSetLight(*(Engine **)(this + 0x34),0x4000);
    goto LAB_0008cc5a;
  case 10:
    glEnable(0xb44);
    glDisable(0xbe2);
    glDepthMask(1);
    Engine::GlEnable(*(uint *)(this + 0x34),false);
    if (*pcVar3 == '\0') {
      (*(code *)(DAT_001ab3b4 + 0x1ab3b8))(0x204,0x3f000000);
      return;
    }
    break;
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
    break;
  case 0x15:
    glEnable(0xb44);
    glEnable(0xbe2);
    glBlendFunc(0x302,0x303);
    glDepthMask(0);
    if (*pcVar3 != '\0') {
      (*(code *)(DAT_001ab3c4 + 0x1ab3c8))(*(undefined4 *)(this + 0x34),DAT_0008ce34,1);
      return;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_0008ce3c + 0x8cd30);
    (*UNRECOVERED_JUMPTABLE)(0x2300,0x2200,0x8570);
    (*UNRECOVERED_JUMPTABLE)(0x2300,0x8571,0x104);
    (*UNRECOVERED_JUMPTABLE)(0x2300,0x8572,0x1e01);
    (*UNRECOVERED_JUMPTABLE)(0x2300,0x8580,0x1702);
    (*UNRECOVERED_JUMPTABLE)(0x2300,0x8590,0x300);
    (*UNRECOVERED_JUMPTABLE)(0x2300,0x8581,0x8577);
    uVar1 = 0x8591;
    uVar2 = 0x300;
LAB_0008ce28:
                    /* WARNING: Could not recover jumptable at 0x0008ce32. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(0x2300,uVar1,uVar2);
    return;
  case 0x16:
    glEnable(0xb44);
    glEnable(0xbe2);
    glBlendFunc(0x302,0x303);
    glDepthMask(0);
    if (*pcVar3 == '\0') {
      UNRECOVERED_JUMPTABLE = *(code **)(DAT_0008ce40 + 0x8cd98);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x2200,0x8570);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8571,0x8575);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8572,0x1e01);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8580,0x1702);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8590,0x300);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8588,0x1702);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8598,0x302);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8581,0x8577);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8591,0x300);
      (*UNRECOVERED_JUMPTABLE)(0x2300,0x8582,0x8577);
      uVar1 = 0x8592;
      uVar2 = 0x302;
      goto LAB_0008ce28;
    }
    break;
  default:
    if (param_2 != 0x25) {
      return;
    }
LAB_0008cc5a:
    glEnable(0xb44);
    glEnable(0xbe2);
    glBlendFunc(0x302,0x303);
    goto LAB_0008cc76;
  }
  return;
switchD_0008cb86_caseD_2:
  glEnable(0xb44);
LAB_0008cc08:
  glEnable(0xbe2);
  uVar1 = 1;
  uVar2 = 1;
  goto LAB_0008cc40;
switchD_0008cb86_caseD_1:
  glEnable(0xb44);
  glEnable(0xbe2);
  uVar1 = 0x302;
  uVar2 = 0x303;
LAB_0008cc40:
  glBlendFunc(uVar1,uVar2);
  uVar1 = 0;
  goto LAB_0008cc78;
}

```

## target disasm
```
  0008cb48: push {r4,r5,r6,r7,lr}
  0008cb4a: add r7,sp,#0xc
  0008cb4c: push.w r11
  0008cb50: mov r4,r0
  0008cb52: ldr r0,[r0,#0x34]
  0008cb54: mov r5,r1
  0008cb56: movs r1,#0x0
  0008cb58: blx 0x0006fe50
  0008cb5c: ldr r0,[0x0008ce38]
  0008cb5e: add r0,pc
  0008cb60: ldr r6,[r0,#0x0]
  0008cb62: ldrb r0,[r6,#0x0]
  0008cb64: cbz r0,0x0008cb72
  0008cb66: ldr r1,[0x0008ce34]
  0008cb68: movs r2,#0x0
  0008cb6a: ldr r0,[r4,#0x34]
  0008cb6c: blx 0x0006eddc
  0008cb70: b 0x0008cb82
  0008cb72: mov.w r0,#0x2300
  0008cb76: mov.w r1,#0x2200
  0008cb7a: mov.w r2,#0x2100
  0008cb7e: blx 0x0006f268
  0008cb82: cmp r5,#0x16
  0008cb84: bhi 0x0008cbac
  0008cb86: tbb [pc,r5]
  0008cba2: movw r0,#0xb44
  0008cba6: blx 0x0006edc4
  0008cbaa: b 0x0008cc08
  0008cbac: cmp r5,#0x25
  0008cbae: beq 0x0008cc5a
  0008cbb0: b 0x0008cd1c
  0008cbb2: movw r0,#0xb44
  0008cbb6: blx 0x0006edb8
  0008cbba: movw r0,#0xbe2
  0008cbbe: blx 0x0006edb8
  0008cbc2: movs r0,#0x0
  0008cbc4: movw r1,#0x301
  0008cbc8: b 0x0008cc40
  0008cbca: ldr r0,[r4,#0x34]
  0008cbcc: movs r1,#0x1
  0008cbce: blx 0x0006fe50
  0008cbd2: ldr r0,[r4,#0x34]
  0008cbd4: mov.w r1,#0x4000
  0008cbd8: blx 0x0006fe5c
  0008cbdc: movw r0,#0xb44
  0008cbe0: blx 0x0006edb8
  0008cbe4: movw r0,#0xbe2
  0008cbe8: blx 0x0006edc4
  0008cbec: b 0x0008cc76
  0008cbee: ldr r0,[r4,#0x34]
  0008cbf0: movs r1,#0x1
  0008cbf2: blx 0x0006fe50
  0008cbf6: ldr r0,[r4,#0x34]
  0008cbf8: mov.w r1,#0x4000
  0008cbfc: blx 0x0006fe5c
  0008cc00: movw r0,#0xb44
  0008cc04: blx 0x0006edb8
  0008cc08: movw r0,#0xbe2
  0008cc0c: blx 0x0006edb8
  0008cc10: movs r0,#0x1
  0008cc12: movs r1,#0x1
  0008cc14: b 0x0008cc40
  0008cc16: ldr r0,[r4,#0x34]
  0008cc18: movs r1,#0x1
  0008cc1a: blx 0x0006fe50
  0008cc1e: ldr r0,[r4,#0x34]
  0008cc20: mov.w r1,#0x4000
  0008cc24: blx 0x0006fe5c
  0008cc28: movw r0,#0xb44
  0008cc2c: blx 0x0006edb8
  0008cc30: movw r0,#0xbe2
  0008cc34: blx 0x0006edb8
  0008cc38: movw r0,#0x302
  0008cc3c: movw r1,#0x303
  0008cc40: blx 0x0006fe68
  0008cc44: movs r0,#0x0
  0008cc46: b 0x0008cc78
  0008cc48: ldr r0,[r4,#0x34]
  0008cc4a: movs r1,#0x1
  0008cc4c: blx 0x0006fe50
  0008cc50: ldr r0,[r4,#0x34]
  0008cc52: mov.w r1,#0x4000
  0008cc56: blx 0x0006fe5c
  0008cc5a: movw r0,#0xb44
  0008cc5e: blx 0x0006edb8
  0008cc62: movw r0,#0xbe2
  0008cc66: blx 0x0006edb8
  0008cc6a: movw r0,#0x302
  0008cc6e: movw r1,#0x303
  0008cc72: blx 0x0006fe68
  0008cc76: movs r0,#0x1
  0008cc78: pop.w r11
  0008cc7c: pop.w {r4,r5,r6,r7,lr}
  0008cc80: b.w 0x001ab398
  0008cc84: movw r0,#0xb44
  0008cc88: blx 0x0006edb8
  0008cc8c: movw r0,#0xbe2
  0008cc90: blx 0x0006edc4
  0008cc94: movs r0,#0x1
  0008cc96: blx 0x0006eec0
  0008cc9a: ldr r0,[r4,#0x34]
  0008cc9c: mov.w r1,#0x1000000
  0008cca0: movs r2,#0x1
  0008cca2: blx 0x0006eddc
  0008cca6: ldrb r0,[r6,#0x0]
  0008cca8: cbnz r0,0x0008cd1c
  0008ccaa: mov.w r0,#0x204
  0008ccae: mov.w r1,#0x3f000000
  0008ccb2: pop.w r11
  0008ccb6: pop.w {r4,r5,r6,r7,lr}
  0008ccba: b.w 0x001ab3a8
  0008ccbe: movw r0,#0xb44
  0008ccc2: blx 0x0006edb8
  0008ccc6: movw r0,#0xbe2
  0008ccca: blx 0x0006edb8
  0008ccce: movw r0,#0x302
  0008ccd2: movw r1,#0x303
  0008ccd6: blx 0x0006fe68
  0008ccda: movs r0,#0x0
  0008ccdc: blx 0x0006eec0
  0008cce0: ldrb r0,[r6,#0x0]
  0008cce2: cbz r0,0x0008cd22
  0008cce4: ldr r1,[0x0008ce34]
  0008cce6: movs r2,#0x1
  0008cce8: ldr r0,[r4,#0x34]
  0008ccea: pop.w r11
  0008ccee: pop.w {r4,r5,r6,r7,lr}
  0008ccf2: b.w 0x001ab3b8
  0008ccf6: movw r0,#0xb44
  0008ccfa: blx 0x0006edb8
  0008ccfe: movw r0,#0xbe2
  0008cd02: blx 0x0006edb8
  0008cd06: movw r0,#0x302
  0008cd0a: movw r1,#0x303
  0008cd0e: blx 0x0006fe68
  0008cd12: movs r0,#0x0
  0008cd14: blx 0x0006eec0
  0008cd18: ldrb r0,[r6,#0x0]
  0008cd1a: cbz r0,0x0008cd8a
  0008cd1c: pop.w r11
  0008cd20: pop {r4,r5,r6,r7,pc}
  0008cd22: ldr r0,[0x0008ce3c]
  0008cd24: mov.w r1,#0x2200
  0008cd28: movw r2,#0x8570
  0008cd2c: add r0,pc
  0008cd2e: ldr r4,[r0,#0x0]
  0008cd30: mov.w r0,#0x2300
  0008cd34: blx r4
  0008cd36: mov.w r0,#0x2300
  0008cd3a: movw r1,#0x8571
  0008cd3e: mov.w r2,#0x104
  0008cd42: blx r4
  0008cd44: mov.w r0,#0x2300
  0008cd48: movw r1,#0x8572
  0008cd4c: movw r2,#0x1e01
  0008cd50: blx r4
  0008cd52: mov.w r0,#0x2300
  0008cd56: movw r1,#0x8580
  0008cd5a: movw r2,#0x1702
  0008cd5e: blx r4
  0008cd60: mov.w r0,#0x2300
  0008cd64: movw r1,#0x8590
  0008cd68: mov.w r2,#0x300
  0008cd6c: blx r4
  0008cd6e: mov.w r0,#0x2300
  0008cd72: movw r1,#0x8581
  0008cd76: movw r2,#0x8577
  0008cd7a: blx r4
  0008cd7c: mov.w r0,#0x2300
  0008cd80: movw r1,#0x8591
  0008cd84: mov.w r2,#0x300
  0008cd88: b 0x0008ce28
  0008cd8a: ldr r0,[0x0008ce40]
  0008cd8c: mov.w r1,#0x2200
  0008cd90: movw r2,#0x8570
  0008cd94: add r0,pc
  0008cd96: ldr r4,[r0,#0x0]
  0008cd98: mov.w r0,#0x2300
  0008cd9c: blx r4
  0008cd9e: mov.w r0,#0x2300
  0008cda2: movw r1,#0x8571
  0008cda6: movw r2,#0x8575
  0008cdaa: blx r4
  0008cdac: mov.w r0,#0x2300
  0008cdb0: movw r1,#0x8572
  0008cdb4: movw r2,#0x1e01
  0008cdb8: blx r4
  0008cdba: mov.w r0,#0x2300
  0008cdbe: movw r1,#0x8580
  0008cdc2: movw r2,#0x1702
  0008cdc6: blx r4
  0008cdc8: mov.w r0,#0x2300
  0008cdcc: movw r1,#0x8590
  0008cdd0: mov.w r2,#0x300
  0008cdd4: blx r4
  0008cdd6: mov.w r0,#0x2300
  0008cdda: movw r1,#0x8588
  0008cdde: movw r2,#0x1702
  0008cde2: blx r4
  0008cde4: mov.w r0,#0x2300
  0008cde8: movw r1,#0x8598
  0008cdec: movw r2,#0x302
  0008cdf0: blx r4
  0008cdf2: mov.w r0,#0x2300
  0008cdf6: movw r1,#0x8581
  0008cdfa: movw r2,#0x8577
  0008cdfe: blx r4
  0008ce00: mov.w r0,#0x2300
  0008ce04: movw r1,#0x8591
  0008ce08: mov.w r2,#0x300
  0008ce0c: blx r4
  0008ce0e: mov.w r0,#0x2300
  0008ce12: movw r1,#0x8582
  0008ce16: movw r2,#0x8577
  0008ce1a: blx r4
  0008ce1c: mov.w r0,#0x2300
  0008ce20: movw r1,#0x8592
  0008ce24: movw r2,#0x302
  0008ce28: mov r3,r4
  0008ce2a: pop.w r11
  0008ce2e: pop.w {r4,r5,r6,r7,lr}
  0008ce32: bx r3
  001ab398: bx pc
  001ab3a8: bx pc
  001ab3b8: bx pc
```
