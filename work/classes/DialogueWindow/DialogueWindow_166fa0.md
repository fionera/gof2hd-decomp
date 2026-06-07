# DialogueWindow::DialogueWindow
elf 0x166fa0 body 300
Sig: undefined __thiscall DialogueWindow(DialogueWindow * this, String * param_1, String * param_2, int * param_3)

## decompile
```c

/* DialogueWindow::DialogueWindow(AbyssEngine::String*, AbyssEngine::String*, int*) */

void __thiscall
DialogueWindow::DialogueWindow(DialogueWindow *this,String *param_1,String *param_2,int *param_3)

{
  uint32_t *this_00;
  int *piVar1;
  void *pvVar2;
  TouchButton *this_01;
  String *pSVar3;
  EVP_PKEY_CTX *ctx;
  ScrollTouchWindow *pSVar4;
  int iVar5;
  int *piVar6;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar6 = *(int **)(DAT_00177104 + 0x176fb6);
  local_28 = *piVar6;
  AbyssEngine::String::String((String *)&this->m_pText);
  this_00 = &(this->m_pAgentName).size;
  AbyssEngine::String::String((String *)this_00);
  init(this,ctx);
  pSVar4 = (ScrollTouchWindow *)this->field_40;
  AbyssEngine::String::String(aSStack_34,(char *)(DAT_00177108 + 0x176fe2),false);
  AbyssEngine::String::String(aSStack_40,param_1,false);
  ScrollTouchWindow::setText(pSVar4,aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_34);
  TouchButton::setVisible((TouchButton *)this->field_8,false);
  TouchButton::setVisible((TouchButton *)this->field_0,false);
  piVar1 = (int *)ImageFactory::loadChar
                            ((ImageFactory *)**(undefined4 **)(DAT_0017710c + 0x17701e),param_3);
  this->field_C = piVar1;
  if ((TouchButton *)this->field_4 != (TouchButton *)0x0) {
    pvVar2 = (void *)TouchButton::~TouchButton((TouchButton *)this->field_4);
    operator_delete(pvVar2);
  }
  this->field_4 = (int *)0x0;
  this_01 = operator_new(200);
  pSVar3 = (String *)GameText::getText(**(int **)(DAT_00177110 + 0x17704e));
  iVar5 = *(int *)(**(int **)(DAT_00177114 + 0x177064) + 0x4c);
  TouchButton::TouchButton
            (this_01,pSVar3,0,this->m_nFramePosX + this->m_nFrameScaleX / 2,
             (this->m_nFrameScaleY + this->m_nFramePosY) - iVar5,
             this->m_nFrameScaleX + iVar5 * -4 +
             *(int *)(**(int **)(DAT_00177114 + 0x177064) + 0x50) * -2,'$','\x04');
  this->field_4 = (int *)this_01;
  AbyssEngine::String::operator=((String *)this_00,param_2);
  this->field_64 = -1;
  this->field_48 = (int *)0x0;
  this[1].field_0 = (int *)0x0;
  iVar5 = *piVar6 - local_28;
  if (iVar5 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar5);
  }
  return;
}

```

## target disasm
```
  00176fa0: push {r4,r5,r6,r7,lr}
  00176fa2: add r7,sp,#0xc
  00176fa4: push {r8,r9,r10,r11}
  00176fa8: sub sp,#0x3c
  00176faa: mov r6,r0
  00176fac: ldr r0,[0x00177104]
  00176fae: mov r11,r3
  00176fb0: mov r9,r2
  00176fb2: add r0,pc
  00176fb4: mov r5,r1
  00176fb6: ldr.w r8,[r0,#0x0]
  00176fba: ldr.w r0,[r8,#0x0]
  00176fbe: str r0,[sp,#0x38]
  00176fc0: add.w r0,r6,#0x28
  00176fc4: str r0,[sp,#0x14]
  00176fc6: blx 0x0006efbc
  00176fca: add.w r0,r6,#0x34
  00176fce: str r0,[sp,#0x1c]
  00176fd0: blx 0x0006efbc
  00176fd4: mov r0,r6
  00176fd6: blx 0x00077be4
  00176fda: ldr r4,[r6,#0x40]
  00176fdc: ldr r1,[0x00177108]
  00176fde: add r1,pc
  00176fe0: add r0,sp,#0x2c
  00176fe2: movs r2,#0x0
  00176fe4: blx 0x0006ee18
  00176fe8: add r0,sp,#0x20
  00176fea: mov r1,r5
  00176fec: movs r2,#0x0
  00176fee: blx 0x0006f028
  00176ff2: add r1,sp,#0x2c
  00176ff4: add r2,sp,#0x20
  00176ff6: mov r0,r4
  00176ff8: blx 0x00075b2c
  00176ffc: add r0,sp,#0x20
  00176ffe: blx 0x0006ee30
  00177002: add r0,sp,#0x2c
  00177004: blx 0x0006ee30
  00177008: ldr r0,[r6,#0x8]
  0017700a: movs r1,#0x0
  0017700c: blx 0x00074e48
  00177010: ldr r0,[r6,#0x0]
  00177012: movs r1,#0x0
  00177014: blx 0x00074e48
  00177018: ldr r0,[0x0017710c]
  0017701a: add r0,pc
  0017701c: ldr r0,[r0,#0x0]
  0017701e: ldr r0,[r0,#0x0]
  00177020: mov r1,r11
  00177022: blx 0x00075b14
  00177026: mov r1,r0
  00177028: ldr r0,[r6,#0x4]
  0017702a: str r1,[r6,#0xc]
  0017702c: cbz r0,0x00177036
  0017702e: blx 0x00074d7c
  00177032: blx 0x0006eb48
  00177036: movs r0,#0x0
  00177038: str r0,[r6,#0x4]
  0017703a: movs r0,#0xc8
  0017703c: blx 0x0006eb24
  00177040: str.w r8,[sp,#0x18]
  00177044: mov r11,r0
  00177046: ldr r0,[0x00177110]
  00177048: mov r8,r9
  0017704a: add r0,pc
  0017704c: ldr r0,[r0,#0x0]
  0017704e: ldr r0,[r0,#0x0]
  00177050: mov.w r1,#0x20c
  00177054: blx 0x00072f70
  00177058: mov r12,r0
  0017705a: ldr r0,[0x00177114]
  0017705c: add.w r5,r6,#0x14
  00177060: add r0,pc
  00177062: ldr r0,[r0,#0x0]
  00177064: ldr r0,[r0,#0x0]
  00177066: ldmia r5,{r2,r3,r5}
  00177068: ldr r1,[r6,#0x20]
  0017706a: ldrd r4,r0,[r0,#0x4c]
  0017706e: sub.w r10,r5,r4, lsl #0x2
  00177072: add r1,r3
  00177074: sub.w r0,r10,r0, lsl #0x1
  00177078: subs r1,r1,r4
  0017707a: mov.w lr,#0x4
  0017707e: mov.w r9,#0x24
  00177082: strd r1,r0,[sp,#0x0]
  00177086: add.w r0,r5,r5, lsr #0x1f
  0017708a: mov r1,r12
  0017708c: strd r9,lr,[sp,#0x8]
  00177090: add.w r3,r2,r0, asr #0x1
  00177094: mov r0,r11
  00177096: movs r2,#0x0
  00177098: blx 0x00074f50
  0017709c: str.w r11,[r6,#0x4]
  001770a0: ldr r0,[sp,#0x1c]
  001770a2: mov r1,r8
  001770a4: blx 0x0006f2b0
  001770a8: ldr r1,[sp,#0x18]
  001770aa: mov.w r0,#0xffffffff
  001770ae: str r0,[r6,#0x64]
  001770b0: movs r0,#0x0
  001770b2: str r0,[r6,#0x48]
  001770b4: str r0,[r6,#0x6c]
  001770b6: ldr r0,[sp,#0x38]
  001770b8: ldr r1,[r1,#0x0]
  001770ba: subs r0,r1,r0
  001770bc: itttt eq
  001770be: mov.eq r0,r6
  001770c0: add.eq sp,#0x3c
  001770c2: pop.eq.w {r8,r9,r10,r11}
  001770c6: pop.eq {r4,r5,r6,r7,pc}
  001770c8: blx 0x0006e824
```
