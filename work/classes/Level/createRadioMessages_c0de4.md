# Level::createRadioMessages
elf 0xc0de4 body 11188
Sig: undefined __thiscall createRadioMessages(Level * this, int param_1)

## decompile
```c

/* Level::createRadioMessages(int) */

undefined4 __thiscall Level::createRadioMessages(Level *this,int param_1)

{
  Array<RadioMessage*> *pAVar1;
  RadioMessage *pRVar2;
  Level *pLVar3;
  
  pLVar3 = this + 0x114;
  *(undefined4 *)(this + 0x114) = 0;
  switch(param_1) {
  case 0:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(0x17,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x684,0x11,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x685,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x686,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x687,10,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x688,0xb,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x689,9,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x68a,9,6,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x68b,9,6,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x68c,0,6,7);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x693,0,9,0,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x694,0,6,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x28) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x695,0,6,10);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x2c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x696,0xf,6,0xb);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x30) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x697,0,6,0xc);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x34) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x698,0,6,0xd);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x38) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x699,0,6,0xe);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x3c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x69a,0,0x1b,0xc);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x40) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x69b,0xf,6,0x10);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x44) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x69c,0,6,0x11);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x48) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x69d,0xf,6,0x12);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x4c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x69e,0,6,0x13);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x50) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x69f,0,6,0x14);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x54) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x6a0,0xf,6,0x15);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x58) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 1:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x6a1,2,5,10000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x6a2,2,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x6a3,2,6,1);
    goto LAB_000d31a4;
  default:
    goto switchD_000d0dfe_caseD_2;
  case 7:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x6dc,2,0x10,0);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x6dd,0,6,0);
    goto LAB_000d3560;
  case 0xe:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(5,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x71c,0x12,5,10000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x71d,0x12,0x14,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x71e,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x71f,0x12,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x720,0x11,6,3);
    goto LAB_000d346e;
  case 0x10:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(5,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x72e,0x13,5,10000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x72f,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x730,0,9,0,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x731,1,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x732,0,6,3);
    goto LAB_000d346e;
  case 0x15:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(5,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x759,10,0x10,0);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x75a,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x75b,10,0x19,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x75c,0xe,8,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x75d,0xe,0x15,0);
    goto LAB_000d346e;
  case 0x18:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(5,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x77d,0x13,5,12000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x77e,6,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x77f,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x780,6,0x16,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x781,6,6,3);
    goto LAB_000d346e;
  case 0x19:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x785,0,5,20000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x786,6,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x787,0,6,1);
    goto LAB_000d31a4;
  case 0x1c:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x798,0,5,20000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x799,0x13,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x79a,0,6,1);
    goto LAB_000d31a4;
  case 0x1d:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(6,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x79c,0,0x17,0);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x79d,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x79e,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x79f,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7a0,0x13,5,DAT_000d2cd0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7a1,0,6,4);
    goto LAB_000d2f6e;
  case 0x26:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7ed,0x15,5,15000);
    break;
  case 0x28:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(7,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7fa,0,5,10000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7fb,8,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7fc,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7fd,7,5,40000);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7fe,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x7ff,7,0xc,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x800,0,0x18,0);
    goto LAB_000d1fce;
  case 0x29:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(8,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x804,0,5,DAT_000d19e0);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x805,7,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x806,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x807,7,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x808,7,0x1a,DAT_000d19e4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x809,7,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x80f,0,1,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x810,0,6,6);
    goto LAB_000d25b4;
  case 0x31:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x848,0,5,8000);
    break;
  case 0x32:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x849,0x3f,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x84a,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x84b,0x3f,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x84c,0,6,2);
    goto LAB_000d3508;
  case 0x33:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x84d,0x3f,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x84e,0,6,0);
    goto LAB_000d3560;
  case 0x34:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x84f,0x3f,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x850,0,6,0);
    goto LAB_000d3560;
  case 0x35:
  case 0x36:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x851,0,5,8000);
    break;
  case 0x37:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x85a,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x85b,0,6,0);
    goto LAB_000d3560;
  case 0x38:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(6,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x86a,0x1b,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x86b,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x86c,0x1c,0x10,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x86d,0,0x14,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x86e,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x86f,0x1b,0x1c,0);
    goto LAB_000d2f6e;
  case 0x3e:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8a1,0,5,8000);
    break;
  case 0x3f:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8af,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b0,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b1,0x1d,6,1);
    goto LAB_000d31a4;
  case 0x40:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(7,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b4,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b5,0x14,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b6,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b7,0x1e,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b8,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8b9,0x1e,0x14,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8ba,0,6,5);
    goto LAB_000d1fce;
  case 0x41:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8cb,0,5,12000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8cc,0x14,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8cd,0,6,1);
    goto LAB_000d31a4;
  case 0x43:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(0xc,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8ef,0,0x10,0);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f0,0x1f,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f1,0x1e,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f2,0,0x14,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f3,0x1f,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f4,0,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f5,0,0x14,8);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f6,0x1f,6,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f7,0,6,7);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f8,0x1f,6,8);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8f9,0,6,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x28) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x8fa,0x1f,6,10);
    goto LAB_000d36f8;
  case 0x45:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x90e,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x90f,0,6,0);
    goto LAB_000d3560;
  case 0x46:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x910,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x911,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x912,0x22,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x913,0,6,2);
    goto LAB_000d3508;
  case 0x49:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(8,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x92b,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x92c,0,0x10,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x92d,0xb,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x92e,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x92f,0,0x1b,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x930,0x21,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x931,0,0x1b,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x932,0,6,6);
    goto LAB_000d25b4;
  case 0x4e:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x969,0,5,2000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x96a,0,0x1b,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x96b,0,0x1b,0xb);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x96c,0xb,6,2);
    goto LAB_000d3508;
  case 0x4f:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x96d,0,5,6000);
    break;
  case 0x50:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(0xc,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x96e,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x96f,6,5,25000);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x970,0x1a,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x971,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x972,6,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x973,0x1a,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x974,6,6,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x975,0x1a,6,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x976,0,6,7);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x977,0x1a,9,1,0x12);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x978,0x1a,0x1b,0xc);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x28) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x979,0x1a,6,10);
    goto LAB_000d36f8;
  case 0x51:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x980,0x1a,5,16000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x981,0x1f,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x982,0x1a,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x983,0x1a,6,2);
    goto LAB_000d3508;
  case 0x57:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(6,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9b0,6,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9b1,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9b2,6,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9b3,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9b4,6,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9b5,0,6,4);
    goto LAB_000d2f6e;
  case 0x59:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9be,0x11,5,2000);
    break;
  case 0x5b:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(7,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9c8,0,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9c9,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9ca,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9cb,0x3c,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9cc,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9cd,0,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9ce,0x3c,0x1b,4);
LAB_000d1fce:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x5c:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(10,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d1,0x3b,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d2,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d3,0x3b,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d4,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d5,0,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d6,0,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d7,0,0x1b,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d8,0,6,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9d9,0,9,0,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9da,0x3b,0x1b,10);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x5e:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(6,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9ed,0,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9ee,0x3b,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9ef,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9f0,0,0x1b,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9f1,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9f2,0,6,4);
    goto LAB_000d2f6e;
  case 0x5f:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0x9fb,0x11,0x1b,1);
    break;
  case 0x61:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa16,0,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa17,0x16,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa18,0,6,1);
    goto LAB_000d31a4;
  case 99:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa28,0x11,0x1b,1);
    break;
  case 100:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa35,0,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa36,0,6,0);
    goto LAB_000d3560;
  case 0x66:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(9,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa41,0x12,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa42,0x12,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa43,0x12,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa44,0x12,0x1b,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa45,0,0x1e,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa46,0,0x1e,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa47,0,0x1e,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa48,0x12,0x1b,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa49,0,0x1b,7);
    goto LAB_000d30fc;
  case 0x69:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(9,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa61,0x14,5,0);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa62,0x3d,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa63,0,0x1b,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa64,0x14,0x1b,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa65,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa66,0,0x1b,7);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa67,0x14,6,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa68,0,0x1b,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa69,0,0x1b,10);
LAB_000d30fc:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x6a:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(6,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa6a,0,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa6b,0x14,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa6c,0x14,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa6d,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa6e,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa6f,0,0x1b,6);
    goto LAB_000d2f6e;
  case 0x6d:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa7f,0x11,0x1b,1);
    break;
  case 0x72:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa94,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa95,10,0x1b,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xa96,0,6,1);
    goto LAB_000d31a4;
  case 0x77:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xab9,0x11,0x1b,1);
    break;
  case 0x78:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xac5,0,5,0x5dc);
    break;
  case 0x7b:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xae1,0x16,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xae2,0x26,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xae3,0x16,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xae4,0,6,2);
    goto LAB_000d3508;
  case 0x7d:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(8,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xaf4,0,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xafe,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xaff,0,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb00,10,0x1b,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb01,0,0x1b,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb02,0,0x1b,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb04,0,0x1b,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb05,0,0x1b,7);
LAB_000d25b4:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x7e:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb0c,0x11,0x1b,1);
    break;
  case 0x83:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb2b,0,0x1b,2);
    break;
  case 0x85:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb33,0x11,0x1b,1);
    break;
  case 0x87:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb43,0x31,0x1b,1);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb44,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb45,0,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb46,0,0x1b,3);
    goto LAB_000d3508;
  case 0x89:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(5,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb4f,0x32,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb50,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb51,0x32,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb52,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb53,0x32,6,3);
    goto LAB_000d346e;
  case 0x8b:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(6,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb6c,0,5,8000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb6d,0,0x1b,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb6e,0x32,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb6f,0x32,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb70,0,0x1b,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb71,0,0x1b,3);
LAB_000d2f6e:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x8e:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(5,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb88,2,5,0x5dc);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb89,2,0x1b,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb8a,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb8b,2,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb8c,2,0x1b,3);
LAB_000d346e:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x90:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(4,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb98,0x27,5,7000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb99,0x27,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb9a,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb9b,0x27,6,2);
LAB_000d3508:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x91:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(3,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb9c,0,5,7000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb9d,0x27,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xb9e,0x27,0x1b,5);
LAB_000d31a4:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x93:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(2,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbac,0,5,7000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbad,0,6,0);
LAB_000d3560:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x9a:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(0xc,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbdf,0,5,9000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe0,0x1a,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe1,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe2,0x1a,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe3,0,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe4,0x1a,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe5,0,6,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe6,0x1a,0x1b,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe7,0,0x1b,10);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe8,0x37,6,8);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbe9,0,6,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x28) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xbea,0,0x1b,0xb);
LAB_000d36f8:
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x2c) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x9d:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(0x11,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc05,0,5,7000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc06,0x26,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc07,0,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc08,0x27,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc09,1,0x1b,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc0a,0,6,4);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc0b,6,0xc,0x15);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc0c,0,6,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc0d,6,0x1b,6);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc0e,0x1a,6,8);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc0f,6,6,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x28) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc10,1,6,10);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x2c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc11,0x27,0x1b,8);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x30) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc12,0,0x1b,9);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x34) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc13,0x1a,6,0xd);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x38) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc14,0,6,0xe);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x3c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc15,0x1a,6,0xf);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x40) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0x9e:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(0xb,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc16,0,5,10000);
    **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc17,0,6,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 4) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc18,0x27,6,1);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 8) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc19,0,6,2);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0xc) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc1a,0x27,6,3);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x10) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc1b,0x27,0x1f,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x14) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc1c,0,6,5);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x18) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc1d,0x27,0xc,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x1c) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc1e,0,6,7);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x20) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc1f,0x27,0x13,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x24) = pRVar2;
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc20,0,1,0);
    *(RadioMessage **)(*(int *)(*(int *)pLVar3 + 4) + 0x28) = pRVar2;
    goto switchD_000d0dfe_caseD_2;
  case 0xa0:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc31,0x11,0x1b,1);
    break;
  case 0xa1:
    pAVar1 = operator_new(0xc);
    Array<RadioMessage*>::Array(pAVar1);
    *(Array<RadioMessage*> **)pLVar3 = pAVar1;
    ArraySetLength<RadioMessage*>(1,(Array *)pAVar1);
    pRVar2 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar2,0xc35,0x11,0x1b,1);
  }
  **(undefined4 **)(*(int *)pLVar3 + 4) = pRVar2;
switchD_000d0dfe_caseD_2:
  return *(undefined4 *)pLVar3;
}

```
## target disasm
```
  000d0de4: push {r4,r5,r6,r7,lr}
  000d0de6: add r7,sp,#0xc
  000d0de8: push.w r11
  000d0dec: sub sp,#0x8
  000d0dee: add.w r4,r0,#0x114
  000d0df2: movs r2,#0x0
  000d0df4: cmp r1,#0xa1
  000d0df6: str.w r2,[r0,#0x114]
  000d0dfa: bhi.w 0x000d3ade
  000d0dfe: tbh [pc,r1]
  000d0f46: movs r0,#0xc
  000d0f48: blx 0x0006eb24
  000d0f4c: mov r5,r0
  000d0f4e: blx 0x00074380
  000d0f52: movs r0,#0x1
  000d0f54: mov r1,r5
  000d0f56: str r5,[r4,#0x0]
  000d0f58: blx 0x0007438c
  000d0f5c: movs r0,#0x28
  000d0f5e: blx 0x0006eb24
  000d0f62: mov r5,r0
  000d0f64: mov.w r0,#0x1f40
  000d0f68: movw r1,#0x851
  000d0f6c: str r0,[sp,#0x0]
  000d0f6e: mov r0,r5
  000d0f70: movs r2,#0x0
  000d0f72: movs r3,#0x5
  000d0f74: blx 0x00074398
  000d0f78: b.w 0x000d3ad8
  000d0f7c: movs r0,#0xc
  000d0f7e: blx 0x0006eb24
  000d0f82: mov r5,r0
  000d0f84: blx 0x00074380
  000d0f88: movs r0,#0x17
  000d0f8a: mov r1,r5
  000d0f8c: str r5,[r4,#0x0]
  000d0f8e: blx 0x0007438c
  000d0f92: movs r0,#0x28
  000d0f94: blx 0x0006eb24
  000d0f98: mov r5,r0
  000d0f9a: movw r0,#0x5dc
  000d0f9e: movw r1,#0x684
  000d0fa2: str r0,[sp,#0x0]
  000d0fa4: mov r0,r5
  000d0fa6: movs r2,#0x11
  000d0fa8: movs r3,#0x5
  000d0faa: blx 0x00074398
  000d0fae: ldr r0,[r4,#0x0]
  000d0fb0: ldr r0,[r0,#0x4]
  000d0fb2: str r5,[r0,#0x0]
  000d0fb4: movs r0,#0x28
  000d0fb6: blx 0x0006eb24
  000d0fba: mov r5,r0
  000d0fbc: movs r0,#0x0
  000d0fbe: movw r1,#0x685
  000d0fc2: str r0,[sp,#0x0]
  000d0fc4: mov r0,r5
  000d0fc6: movs r2,#0x0
  000d0fc8: movs r3,#0x6
  000d0fca: blx 0x00074398
  000d0fce: ldr r0,[r4,#0x0]
  000d0fd0: ldr r0,[r0,#0x4]
  000d0fd2: str r5,[r0,#0x4]
  000d0fd4: movs r0,#0x28
  000d0fd6: blx 0x0006eb24
  000d0fda: mov r5,r0
  000d0fdc: movs r0,#0x1
  000d0fde: movw r1,#0x686
  000d0fe2: str r0,[sp,#0x0]
  000d0fe4: mov r0,r5
  000d0fe6: movs r2,#0x0
  000d0fe8: movs r3,#0x6
  000d0fea: blx 0x00074398
  000d0fee: ldr r0,[r4,#0x0]
  000d0ff0: ldr r0,[r0,#0x4]
  000d0ff2: str r5,[r0,#0x8]
  000d0ff4: movs r0,#0x28
  000d0ff6: blx 0x0006eb24
  000d0ffa: mov r5,r0
  000d0ffc: movs r0,#0x2
  000d0ffe: movw r1,#0x687
  000d1002: str r0,[sp,#0x0]
  000d1004: mov r0,r5
  000d1006: movs r2,#0xa
  000d1008: movs r3,#0x6
  000d100a: blx 0x00074398
  000d100e: ldr r0,[r4,#0x0]
  000d1010: ldr r0,[r0,#0x4]
  000d1012: str r5,[r0,#0xc]
  000d1014: movs r0,#0x28
  000d1016: blx 0x0006eb24
  000d101a: mov r5,r0
  000d101c: movs r0,#0x3
  000d101e: mov.w r1,#0x688
  000d1022: str r0,[sp,#0x0]
  000d1024: mov r0,r5
  000d1026: movs r2,#0xb
  000d1028: movs r3,#0x6
  000d102a: blx 0x00074398
  000d102e: ldr r0,[r4,#0x0]
  000d1030: ldr r0,[r0,#0x4]
  000d1032: str r5,[r0,#0x10]
  000d1034: movs r0,#0x28
  000d1036: blx 0x0006eb24
  000d103a: mov r5,r0
  000d103c: movs r0,#0x4
  000d103e: movw r1,#0x689
  000d1042: str r0,[sp,#0x0]
  000d1044: mov r0,r5
  000d1046: movs r2,#0x9
  000d1048: movs r3,#0x6
  000d104a: blx 0x00074398
  000d104e: ldr r0,[r4,#0x0]
  000d1050: ldr r0,[r0,#0x4]
  000d1052: str r5,[r0,#0x14]
  000d1054: movs r0,#0x28
  000d1056: blx 0x0006eb24
  000d105a: mov r5,r0
  000d105c: movs r0,#0x5
  000d105e: movw r1,#0x68a
  000d1062: str r0,[sp,#0x0]
  000d1064: mov r0,r5
  000d1066: movs r2,#0x9
  000d1068: movs r3,#0x6
  000d106a: movs r6,#0x6
  000d106c: blx 0x00074398
  000d1070: ldr r0,[r4,#0x0]
  000d1072: ldr r0,[r0,#0x4]
  000d1074: str r5,[r0,#0x18]
  000d1076: movs r0,#0x28
  000d1078: blx 0x0006eb24
  000d107c: mov r5,r0
  000d107e: movw r1,#0x68b
  000d1082: movs r2,#0x9
  000d1084: movs r3,#0x6
  000d1086: str r6,[sp,#0x0]
  000d1088: blx 0x00074398
  000d108c: ldr r0,[r4,#0x0]
  000d108e: ldr r0,[r0,#0x4]
  000d1090: str r5,[r0,#0x1c]
  000d1092: movs r0,#0x28
  000d1094: blx 0x0006eb24
  000d1098: mov r5,r0
  000d109a: movs r0,#0x7
  000d109c: movw r1,#0x68c
  000d10a0: str r0,[sp,#0x0]
  000d10a2: mov r0,r5
  000d10a4: movs r2,#0x0
  000d10a6: movs r3,#0x6
  000d10a8: movs r6,#0x0
  000d10aa: blx 0x00074398
  000d10ae: ldr r0,[r4,#0x0]
  000d10b0: ldr r0,[r0,#0x4]
  000d10b2: str r5,[r0,#0x20]
  000d10b4: movs r0,#0x28
  000d10b6: blx 0x0006eb24
  000d10ba: mov r5,r0
  000d10bc: movs r0,#0x3
  000d10be: movw r1,#0x693
  000d10c2: strd r6,r0,[sp,#0x0]
  000d10c6: mov r0,r5
  000d10c8: movs r2,#0x0
  000d10ca: movs r3,#0x9
  000d10cc: movs r6,#0x9
  000d10ce: blx 0x000743a4
  000d10d2: ldr r0,[r4,#0x0]
  000d10d4: ldr r0,[r0,#0x4]
  000d10d6: str r5,[r0,#0x24]
  000d10d8: movs r0,#0x28
  000d10da: blx 0x0006eb24
  000d10de: mov r5,r0
  000d10e0: movw r1,#0x694
  000d10e4: movs r2,#0x0
  000d10e6: movs r3,#0x6
  000d10e8: str r6,[sp,#0x0]
  000d10ea: blx 0x00074398
  000d10ee: ldr r0,[r4,#0x0]
  000d10f0: ldr r0,[r0,#0x4]
  000d10f2: str r5,[r0,#0x28]
  000d10f4: movs r0,#0x28
  000d10f6: blx 0x0006eb24
  000d10fa: mov r5,r0
  000d10fc: movs r0,#0xa
  000d10fe: movw r1,#0x695
  000d1102: str r0,[sp,#0x0]
  000d1104: mov r0,r5
  000d1106: movs r2,#0x0
  000d1108: movs r3,#0x6
  000d110a: blx 0x00074398
  000d110e: ldr r0,[r4,#0x0]
  000d1110: ldr r0,[r0,#0x4]
  000d1112: str r5,[r0,#0x2c]
  000d1114: movs r0,#0x28
  000d1116: blx 0x0006eb24
  000d111a: mov r5,r0
  000d111c: movs r0,#0xb
  000d111e: movw r1,#0x696
  000d1122: str r0,[sp,#0x0]
  000d1124: mov r0,r5
  000d1126: movs r2,#0xf
  000d1128: movs r3,#0x6
  000d112a: blx 0x00074398
  000d112e: ldr r0,[r4,#0x0]
  000d1130: ldr r0,[r0,#0x4]
  000d1132: str r5,[r0,#0x30]
  000d1134: movs r0,#0x28
  000d1136: blx 0x0006eb24
  000d113a: mov r5,r0
  000d113c: movs r0,#0xc
  000d113e: movw r1,#0x697
  000d1142: str r0,[sp,#0x0]
  000d1144: mov r0,r5
  000d1146: movs r2,#0x0
  000d1148: movs r3,#0x6
  000d114a: blx 0x00074398
  000d114e: ldr r0,[r4,#0x0]
  000d1150: ldr r0,[r0,#0x4]
  000d1152: str r5,[r0,#0x34]
  000d1154: movs r0,#0x28
  000d1156: blx 0x0006eb24
  000d115a: mov r5,r0
  000d115c: movs r0,#0xd
  000d115e: mov.w r1,#0x698
  000d1162: str r0,[sp,#0x0]
  000d1164: mov r0,r5
  000d1166: movs r2,#0x0
  000d1168: movs r3,#0x6
  000d116a: blx 0x00074398
  000d116e: ldr r0,[r4,#0x0]
  000d1170: ldr r0,[r0,#0x4]
  000d1172: str r5,[r0,#0x38]
  000d1174: movs r0,#0x28
  000d1176: blx 0x0006eb24
  000d117a: mov r5,r0
  000d117c: movs r0,#0xe
  000d117e: movw r1,#0x699
  000d1182: str r0,[sp,#0x0]
  000d1184: mov r0,r5
  000d1186: movs r2,#0x0
  000d1188: movs r3,#0x6
  000d118a: blx 0x00074398
  000d118e: ldr r0,[r4,#0x0]
  000d1190: ldr r0,[r0,#0x4]
  000d1192: str r5,[r0,#0x3c]
  000d1194: movs r0,#0x28
  000d1196: blx 0x0006eb24
  000d119a: mov r5,r0
  000d119c: movs r0,#0xc
  000d119e: movw r1,#0x69a
  000d11a2: str r0,[sp,#0x0]
  000d11a4: mov r0,r5
  000d11a6: movs r2,#0x0
  000d11a8: movs r3,#0x1b
  000d11aa: blx 0x00074398
  000d11ae: ldr r0,[r4,#0x0]
  000d11b0: ldr r0,[r0,#0x4]
  000d11b2: str r5,[r0,#0x40]
  000d11b4: movs r0,#0x28
  000d11b6: blx 0x0006eb24
  000d11ba: mov r5,r0
  000d11bc: movs r0,#0x10
  000d11be: movw r1,#0x69b
  000d11c2: str r0,[sp,#0x0]
  000d11c4: mov r0,r5
  000d11c6: movs r2,#0xf
  000d11c8: movs r3,#0x6
  000d11ca: blx 0x00074398
  000d11ce: ldr r0,[r4,#0x0]
  000d11d0: ldr r0,[r0,#0x4]
  000d11d2: str r5,[r0,#0x44]
  000d11d4: movs r0,#0x28
  000d11d6: blx 0x0006eb24
  000d11da: mov r5,r0
  000d11dc: movs r0,#0x11
  000d11de: movw r1,#0x69c
  000d11e2: str r0,[sp,#0x0]
  000d11e4: mov r0,r5
  000d11e6: movs r2,#0x0
  000d11e8: movs r3,#0x6
  000d11ea: blx 0x00074398
  000d11ee: ldr r0,[r4,#0x0]
  000d11f0: ldr r0,[r0,#0x4]
  000d11f2: str r5,[r0,#0x48]
  000d11f4: movs r0,#0x28
  000d11f6: blx 0x0006eb24
  000d11fa: mov r5,r0
  000d11fc: movs r0,#0x12
  000d11fe: movw r1,#0x69d
  000d1202: str r0,[sp,#0x0]
  000d1204: mov r0,r5
  000d1206: movs r2,#0xf
  000d1208: movs r3,#0x6
  000d120a: blx 0x00074398
  000d120e: ldr r0,[r4,#0x0]
  000d1210: ldr r0,[r0,#0x4]
  000d1212: str r5,[r0,#0x4c]
  000d1214: movs r0,#0x28
  000d1216: blx 0x0006eb24
  000d121a: mov r5,r0
  000d121c: movs r0,#0x13
  000d121e: movw r1,#0x69e
  000d1222: str r0,[sp,#0x0]
  000d1224: mov r0,r5
  000d1226: movs r2,#0x0
  000d1228: movs r3,#0x6
  000d122a: blx 0x00074398
  000d122e: ldr r0,[r4,#0x0]
  000d1230: ldr r0,[r0,#0x4]
  000d1232: str r5,[r0,#0x50]
  000d1234: movs r0,#0x28
  000d1236: blx 0x0006eb24
  000d123a: mov r5,r0
  000d123c: movs r0,#0x14
  000d123e: movw r1,#0x69f
  000d1242: str r0,[sp,#0x0]
  000d1244: mov r0,r5
  000d1246: movs r2,#0x0
  000d1248: movs r3,#0x6
  000d124a: blx 0x00074398
  000d124e: ldr r0,[r4,#0x0]
  000d1250: ldr r0,[r0,#0x4]
  000d1252: str r5,[r0,#0x54]
  000d1254: movs r0,#0x28
  000d1256: blx 0x0006eb24
  000d125a: mov r5,r0
  000d125c: movs r0,#0x15
  000d125e: mov.w r1,#0x6a0
  000d1262: str r0,[sp,#0x0]
  000d1264: mov r0,r5
  000d1266: movs r2,#0xf
  000d1268: movs r3,#0x6
  000d126a: blx 0x00074398
  000d126e: ldr r0,[r4,#0x0]
  000d1270: ldr r0,[r0,#0x4]
  000d1272: str r5,[r0,#0x58]
  000d1274: b.w 0x000d3ade
  000d1278: movs r0,#0xc
  000d127a: blx 0x0006eb24
  000d127e: mov r5,r0
  000d1280: blx 0x00074380
  000d1284: movs r0,#0x3
  000d1286: mov r1,r5
  000d1288: str r5,[r4,#0x0]
  000d128a: blx 0x0007438c
  000d128e: movs r0,#0x28
  000d1290: blx 0x0006eb24
  000d1294: mov r5,r0
  000d1296: movw r0,#0x2710
  000d129a: movw r1,#0x6a1
  000d129e: str r0,[sp,#0x0]
  000d12a0: mov r0,r5
  000d12a2: movs r2,#0x2
  000d12a4: movs r3,#0x5
  000d12a6: blx 0x00074398
  000d12aa: ldr r0,[r4,#0x0]
  000d12ac: ldr r0,[r0,#0x4]
  000d12ae: str r5,[r0,#0x0]
  000d12b0: movs r0,#0x28
  000d12b2: blx 0x0006eb24
  000d12b6: mov r5,r0
  000d12b8: movs r0,#0x0
  000d12ba: movw r1,#0x6a2
  000d12be: str r0,[sp,#0x0]
  000d12c0: mov r0,r5
  000d12c2: movs r2,#0x2
  000d12c4: movs r3,#0x6
  000d12c6: blx 0x00074398
  000d12ca: ldr r0,[r4,#0x0]
  000d12cc: ldr r0,[r0,#0x4]
  000d12ce: str r5,[r0,#0x4]
  000d12d0: movs r0,#0x28
  000d12d2: blx 0x0006eb24
  000d12d6: mov r5,r0
  000d12d8: movs r0,#0x1
  000d12da: movw r1,#0x6a3
  000d12de: str r0,[sp,#0x0]
  000d12e0: mov r0,r5
  000d12e2: movs r2,#0x2
  000d12e4: movs r3,#0x6
  000d12e6: blx 0x00074398
  000d12ea: b.w 0x000d31a4
  000d12ee: movs r0,#0xc
  000d12f0: blx 0x0006eb24
  000d12f4: mov r5,r0
  000d12f6: blx 0x00074380
  000d12fa: movs r0,#0x2
  000d12fc: mov r1,r5
  000d12fe: str r5,[r4,#0x0]
  000d1300: blx 0x0007438c
  000d1304: movs r0,#0x28
  000d1306: blx 0x0006eb24
  000d130a: mov r5,r0
  000d130c: movs r6,#0x0
  000d130e: movw r1,#0x6dc
  000d1312: movs r2,#0x2
  000d1314: movs r3,#0x10
  000d1316: str r6,[sp,#0x0]
  000d1318: blx 0x00074398
  000d131c: ldr r0,[r4,#0x0]
  000d131e: ldr r0,[r0,#0x4]
  000d1320: str r5,[r0,#0x0]
  000d1322: movs r0,#0x28
  000d1324: blx 0x0006eb24
  000d1328: mov r5,r0
  000d132a: movw r1,#0x6dd
  000d132e: movs r2,#0x0
  000d1330: movs r3,#0x6
  000d1332: str r6,[sp,#0x0]
  000d1334: blx 0x00074398
  000d1338: b.w 0x000d3560
  000d133c: movs r0,#0xc
  000d133e: blx 0x0006eb24
  000d1342: mov r5,r0
  000d1344: blx 0x00074380
  000d1348: movs r0,#0x5
  000d134a: mov r1,r5
  000d134c: str r5,[r4,#0x0]
  000d134e: blx 0x0007438c
  000d1352: movs r0,#0x28
  000d1354: blx 0x0006eb24
  000d1358: mov r5,r0
  000d135a: movw r0,#0x2710
  000d135e: movw r1,#0x71c
  000d1362: str r0,[sp,#0x0]
  000d1364: mov r0,r5
  000d1366: movs r2,#0x12
  000d1368: movs r3,#0x5
  000d136a: blx 0x00074398
  000d136e: ldr r0,[r4,#0x0]
  000d1370: ldr r0,[r0,#0x4]
  000d1372: str r5,[r0,#0x0]
  000d1374: movs r0,#0x28
  000d1376: blx 0x0006eb24
  000d137a: mov r5,r0
  000d137c: movs r6,#0x1
  000d137e: movw r1,#0x71d
  000d1382: movs r2,#0x12
  000d1384: movs r3,#0x14
  000d1386: str r6,[sp,#0x0]
  000d1388: blx 0x00074398
  000d138c: ldr r0,[r4,#0x0]
  000d138e: ldr r0,[r0,#0x4]
  000d1390: str r5,[r0,#0x4]
  000d1392: movs r0,#0x28
  000d1394: blx 0x0006eb24
  000d1398: mov r5,r0
  000d139a: movw r1,#0x71e
  000d139e: movs r2,#0x0
  000d13a0: movs r3,#0x6
  000d13a2: str r6,[sp,#0x0]
  000d13a4: blx 0x00074398
  000d13a8: ldr r0,[r4,#0x0]
  000d13aa: ldr r0,[r0,#0x4]
  000d13ac: str r5,[r0,#0x8]
  000d13ae: movs r0,#0x28
  000d13b0: blx 0x0006eb24
  000d13b4: mov r5,r0
  000d13b6: movs r0,#0x2
  000d13b8: movw r1,#0x71f
  000d13bc: str r0,[sp,#0x0]
  000d13be: mov r0,r5
  000d13c0: movs r2,#0x12
  000d13c2: movs r3,#0x6
  000d13c4: blx 0x00074398
  000d13c8: ldr r0,[r4,#0x0]
  000d13ca: ldr r0,[r0,#0x4]
  000d13cc: str r5,[r0,#0xc]
  000d13ce: movs r0,#0x28
  000d13d0: blx 0x0006eb24
  000d13d4: mov r5,r0
  000d13d6: movs r0,#0x3
  000d13d8: mov.w r1,#0x720
  000d13dc: str r0,[sp,#0x0]
  000d13de: mov r0,r5
  000d13e0: movs r2,#0x11
  000d13e2: movs r3,#0x6
  000d13e4: blx 0x00074398
  000d13e8: b.w 0x000d346e
  000d13ec: movs r0,#0xc
  000d13ee: blx 0x0006eb24
  000d13f2: mov r5,r0
  000d13f4: blx 0x00074380
  000d13f8: movs r0,#0x5
  000d13fa: mov r1,r5
  000d13fc: str r5,[r4,#0x0]
  000d13fe: blx 0x0007438c
  000d1402: movs r0,#0x28
  000d1404: blx 0x0006eb24
  000d1408: mov r5,r0
  000d140a: movw r0,#0x2ee0
  000d140e: movw r1,#0x77d
  000d1412: str r0,[sp,#0x0]
  000d1414: mov r0,r5
  000d1416: movs r2,#0x13
  000d1418: movs r3,#0x5
  000d141a: blx 0x00074398
  000d141e: ldr r0,[r4,#0x0]
  000d1420: ldr r0,[r0,#0x4]
  000d1422: str r5,[r0,#0x0]
  000d1424: movs r0,#0x28
  000d1426: blx 0x0006eb24
  000d142a: mov r5,r0
  000d142c: movs r0,#0x0
  000d142e: movw r1,#0x77e
  000d1432: str r0,[sp,#0x0]
  000d1434: mov r0,r5
  000d1436: movs r2,#0x6
  000d1438: movs r3,#0x6
  000d143a: blx 0x00074398
  000d143e: ldr r0,[r4,#0x0]
  000d1440: ldr r0,[r0,#0x4]
  000d1442: str r5,[r0,#0x4]
  000d1444: movs r0,#0x28
  000d1446: blx 0x0006eb24
  000d144a: mov r5,r0
  000d144c: movs r0,#0x1
  000d144e: movw r1,#0x77f
  000d1452: str r0,[sp,#0x0]
  000d1454: mov r0,r5
  000d1456: movs r2,#0x0
  000d1458: movs r3,#0x6
  000d145a: blx 0x00074398
  000d145e: ldr r0,[r4,#0x0]
  000d1460: ldr r0,[r0,#0x4]
  000d1462: str r5,[r0,#0x8]
  000d1464: movs r0,#0x28
  000d1466: blx 0x0006eb24
  000d146a: mov r5,r0
  000d146c: movs r6,#0x3
  000d146e: mov.w r1,#0x780
  000d1472: movs r2,#0x6
  000d1474: movs r3,#0x16
  000d1476: str r6,[sp,#0x0]
  000d1478: blx 0x00074398
  000d147c: ldr r0,[r4,#0x0]
  000d147e: ldr r0,[r0,#0x4]
  000d1480: str r5,[r0,#0xc]
  000d1482: movs r0,#0x28
  000d1484: blx 0x0006eb24
  000d1488: mov r5,r0
  000d148a: movw r1,#0x781
  000d148e: movs r2,#0x6
  000d1490: movs r3,#0x6
  000d1492: str r6,[sp,#0x0]
  000d1494: blx 0x00074398
  000d1498: b.w 0x000d346e
  000d149c: movs r0,#0xc
  000d149e: blx 0x0006eb24
  000d14a2: mov r5,r0
  000d14a4: blx 0x00074380
  000d14a8: movs r0,#0x3
  000d14aa: mov r1,r5
  000d14ac: str r5,[r4,#0x0]
  000d14ae: blx 0x0007438c
  000d14b2: movs r0,#0x28
  000d14b4: blx 0x0006eb24
  000d14b8: mov r5,r0
  000d14ba: movw r0,#0x4e20
  000d14be: movw r1,#0x785
  000d14c2: str r0,[sp,#0x0]
  000d14c4: mov r0,r5
  000d14c6: movs r2,#0x0
  000d14c8: movs r3,#0x5
  000d14ca: movs r6,#0x0
  000d14cc: blx 0x00074398
  000d14d0: ldr r0,[r4,#0x0]
  000d14d2: ldr r0,[r0,#0x4]
  000d14d4: str r5,[r0,#0x0]
  000d14d6: movs r0,#0x28
  000d14d8: blx 0x0006eb24
  000d14dc: mov r5,r0
  000d14de: movw r1,#0x786
  000d14e2: movs r2,#0x6
  000d14e4: movs r3,#0x6
  000d14e6: str r6,[sp,#0x0]
  000d14e8: blx 0x00074398
  000d14ec: ldr r0,[r4,#0x0]
  000d14ee: ldr r0,[r0,#0x4]
  000d14f0: str r5,[r0,#0x4]
  000d14f2: movs r0,#0x28
  000d14f4: blx 0x0006eb24
  000d14f8: mov r5,r0
  000d14fa: movs r0,#0x1
  000d14fc: movw r1,#0x787
  000d1500: str r0,[sp,#0x0]
  000d1502: mov r0,r5
  000d1504: movs r2,#0x0
  000d1506: movs r3,#0x6
  000d1508: blx 0x00074398
  000d150c: b.w 0x000d31a4
  000d1510: movs r0,#0xc
  000d1512: blx 0x0006eb24
  000d1516: mov r5,r0
  000d1518: blx 0x00074380
  000d151c: movs r0,#0x3
  000d151e: mov r1,r5
  000d1520: str r5,[r4,#0x0]
  000d1522: blx 0x0007438c
  000d1526: movs r0,#0x28
  000d1528: blx 0x0006eb24
  000d152c: mov r5,r0
  000d152e: movw r0,#0x4e20
  000d1532: mov.w r1,#0x798
  000d1536: str r0,[sp,#0x0]
  000d1538: mov r0,r5
  000d153a: movs r2,#0x0
  000d153c: movs r3,#0x5
  000d153e: movs r6,#0x0
  000d1540: blx 0x00074398
  000d1544: ldr r0,[r4,#0x0]
  000d1546: ldr r0,[r0,#0x4]
  000d1548: str r5,[r0,#0x0]
  000d154a: movs r0,#0x28
  000d154c: blx 0x0006eb24
  000d1550: mov r5,r0
  000d1552: movw r1,#0x799
  000d1556: movs r2,#0x13
  000d1558: movs r3,#0x6
  000d155a: str r6,[sp,#0x0]
  000d155c: blx 0x00074398
  000d1560: ldr r0,[r4,#0x0]
  000d1562: ldr r0,[r0,#0x4]
  000d1564: str r5,[r0,#0x4]
  000d1566: movs r0,#0x28
  000d1568: blx 0x0006eb24
  000d156c: mov r5,r0
  000d156e: movs r0,#0x1
  000d1570: movw r1,#0x79a
  000d1574: str r0,[sp,#0x0]
  000d1576: mov r0,r5
  000d1578: movs r2,#0x0
  000d157a: movs r3,#0x6
  000d157c: blx 0x00074398
  000d1580: b.w 0x000d31a4
  000d1584: movs r0,#0xc
  000d1586: blx 0x0006eb24
  000d158a: mov r5,r0
  000d158c: blx 0x00074380
  000d1590: movs r0,#0x1
  000d1592: mov r1,r5
  000d1594: str r5,[r4,#0x0]
  000d1596: blx 0x0007438c
  000d159a: movs r0,#0x28
  000d159c: blx 0x0006eb24
  000d15a0: mov r5,r0
  000d15a2: movw r0,#0x3a98
  000d15a6: movw r1,#0x7ed
  000d15aa: str r0,[sp,#0x0]
  000d15ac: mov r0,r5
  000d15ae: movs r2,#0x15
  000d15b0: movs r3,#0x5
  000d15b2: blx 0x00074398
  000d15b6: b.w 0x000d3ad8
  000d15ba: movs r0,#0xc
  000d15bc: blx 0x0006eb24
  000d15c0: mov r5,r0
  000d15c2: blx 0x00074380
  000d15c6: movs r0,#0x7
  000d15c8: mov r1,r5
  000d15ca: str r5,[r4,#0x0]
  000d15cc: blx 0x0007438c
  000d15d0: movs r0,#0x28
  000d15d2: blx 0x0006eb24
  000d15d6: mov r5,r0
  000d15d8: movw r0,#0x2710
  000d15dc: movw r1,#0x7fa
  000d15e0: str r0,[sp,#0x0]
  000d15e2: mov r0,r5
  000d15e4: movs r2,#0x0
  000d15e6: movs r3,#0x5
  000d15e8: movs r6,#0x0
  000d15ea: blx 0x00074398
  000d15ee: ldr r0,[r4,#0x0]
  000d15f0: ldr r0,[r0,#0x4]
  000d15f2: str r5,[r0,#0x0]
  000d15f4: movs r0,#0x28
  000d15f6: blx 0x0006eb24
  000d15fa: mov r5,r0
  000d15fc: movw r1,#0x7fb
  000d1600: movs r2,#0x8
  000d1602: movs r3,#0x6
  000d1604: str r6,[sp,#0x0]
  000d1606: blx 0x00074398
  000d160a: ldr r0,[r4,#0x0]
  000d160c: ldr r0,[r0,#0x4]
  000d160e: str r5,[r0,#0x4]
  000d1610: movs r0,#0x28
  000d1612: blx 0x0006eb24
  000d1616: mov r5,r0
  000d1618: movs r0,#0x1
  000d161a: movw r1,#0x7fc
  000d161e: str r0,[sp,#0x0]
  000d1620: mov r0,r5
  000d1622: movs r2,#0x0
  000d1624: movs r3,#0x6
  000d1626: blx 0x00074398
  000d162a: ldr r0,[r4,#0x0]
  000d162c: ldr r0,[r0,#0x4]
  000d162e: str r5,[r0,#0x8]
  000d1630: movs r0,#0x28
  000d1632: blx 0x0006eb24
  000d1636: mov r5,r0
  000d1638: movw r0,#0x9c40
  000d163c: movw r1,#0x7fd
  000d1640: str r0,[sp,#0x0]
  000d1642: mov r0,r5
  000d1644: movs r2,#0x7
  000d1646: movs r3,#0x5
  000d1648: blx 0x00074398
  000d164c: ldr r0,[r4,#0x0]
  000d164e: ldr r0,[r0,#0x4]
  000d1650: str r5,[r0,#0xc]
  000d1652: movs r0,#0x28
  000d1654: blx 0x0006eb24
  000d1658: mov r5,r0
  000d165a: movs r0,#0x3
  000d165c: movw r1,#0x7fe
  000d1660: str r0,[sp,#0x0]
  000d1662: mov r0,r5
  000d1664: movs r2,#0x0
  000d1666: movs r3,#0x6
  000d1668: movs r6,#0x0
  000d166a: blx 0x00074398
  000d166e: ldr r0,[r4,#0x0]
  000d1670: ldr r0,[r0,#0x4]
  000d1672: str r5,[r0,#0x10]
  000d1674: movs r0,#0x28
  000d1676: blx 0x0006eb24
  000d167a: mov r5,r0
  000d167c: movw r1,#0x7ff
  000d1680: movs r2,#0x7
  000d1682: movs r3,#0xc
  000d1684: str r6,[sp,#0x0]
  000d1686: blx 0x00074398
  000d168a: ldr r0,[r4,#0x0]
  000d168c: ldr r0,[r0,#0x4]
  000d168e: str r5,[r0,#0x14]
  000d1690: movs r0,#0x28
  000d1692: blx 0x0006eb24
  000d1696: mov r5,r0
  000d1698: movs r0,#0x0
  000d169a: mov.w r1,#0x800
  000d169e: str r0,[sp,#0x0]
  000d16a0: mov r0,r5
  000d16a2: movs r2,#0x0
  000d16a4: movs r3,#0x18
  000d16a6: blx 0x00074398
  000d16aa: b.w 0x000d1fce
  000d16ae: movs r0,#0xc
  000d16b0: blx 0x0006eb24
  000d16b4: mov r5,r0
  000d16b6: blx 0x00074380
  000d16ba: movs r0,#0x8
  000d16bc: mov r1,r5
  000d16be: str r5,[r4,#0x0]
  000d16c0: blx 0x0007438c
  000d16c4: movs r0,#0x28
  000d16c6: blx 0x0006eb24
  000d16ca: mov r5,r0
  000d16cc: ldr r0,[0x000d19e0]
  000d16ce: str r0,[sp,#0x0]
  000d16d0: mov r0,r5
  000d16d2: movw r1,#0x804
  000d16d6: movs r2,#0x0
  000d16d8: movs r3,#0x5
  000d16da: movs r6,#0x0
  000d16dc: blx 0x00074398
  000d16e0: ldr r0,[r4,#0x0]
  000d16e2: ldr r0,[r0,#0x4]
  000d16e4: str r5,[r0,#0x0]
  000d16e6: movs r0,#0x28
  000d16e8: blx 0x0006eb24
  000d16ec: mov r5,r0
  000d16ee: movw r1,#0x805
  000d16f2: movs r2,#0x7
  000d16f4: movs r3,#0x6
  000d16f6: str r6,[sp,#0x0]
  000d16f8: blx 0x00074398
  000d16fc: ldr r0,[r4,#0x0]
  000d16fe: ldr r0,[r0,#0x4]
  000d1700: str r5,[r0,#0x4]
  000d1702: movs r0,#0x28
  000d1704: blx 0x0006eb24
  000d1708: mov r5,r0
  000d170a: movs r0,#0x1
  000d170c: movw r1,#0x806
  000d1710: str r0,[sp,#0x0]
  000d1712: mov r0,r5
  000d1714: movs r2,#0x0
  000d1716: movs r3,#0x6
  000d1718: blx 0x00074398
  000d171c: ldr r0,[r4,#0x0]
  000d171e: ldr r0,[r0,#0x4]
  000d1720: str r5,[r0,#0x8]
  000d1722: movs r0,#0x28
  000d1724: blx 0x0006eb24
  000d1728: mov r5,r0
  000d172a: movs r0,#0x2
  000d172c: movw r1,#0x807
  000d1730: str r0,[sp,#0x0]
  000d1732: mov r0,r5
  000d1734: movs r2,#0x7
  000d1736: movs r3,#0x6
  000d1738: blx 0x00074398
  000d173c: ldr r0,[r4,#0x0]
  000d173e: ldr r0,[r0,#0x4]
  000d1740: str r5,[r0,#0xc]
  000d1742: movs r0,#0x28
  000d1744: blx 0x0006eb24
  000d1748: mov r5,r0
  000d174a: ldr r0,[0x000d19e4]
  000d174c: str r0,[sp,#0x0]
  000d174e: mov r0,r5
  000d1750: movw r1,#0x808
  000d1754: movs r2,#0x7
  000d1756: movs r3,#0x1a
  000d1758: blx 0x00074398
  000d175c: ldr r0,[r4,#0x0]
  000d175e: ldr r0,[r0,#0x4]
  000d1760: str r5,[r0,#0x10]
  000d1762: movs r0,#0x28
  000d1764: blx 0x0006eb24
  000d1768: mov r5,r0
  000d176a: movs r0,#0x4
  000d176c: movw r1,#0x809
  000d1770: str r0,[sp,#0x0]
  000d1772: mov r0,r5
  000d1774: movs r2,#0x7
  000d1776: movs r3,#0x6
  000d1778: blx 0x00074398
  000d177c: ldr r0,[r4,#0x0]
  000d177e: ldr r0,[r0,#0x4]
  000d1780: str r5,[r0,#0x14]
  000d1782: movs r0,#0x28
  000d1784: blx 0x0006eb24
  000d1788: mov r5,r0
  000d178a: movs r0,#0x0
  000d178c: movw r1,#0x80f
  000d1790: str r0,[sp,#0x0]
  000d1792: mov r0,r5
  000d1794: movs r2,#0x0
  000d1796: movs r3,#0x1
  000d1798: blx 0x00074398
  000d179c: ldr r0,[r4,#0x0]
  000d179e: ldr r0,[r0,#0x4]
  000d17a0: str r5,[r0,#0x18]
  000d17a2: movs r0,#0x28
  000d17a4: blx 0x0006eb24
  000d17a8: mov r5,r0
  000d17aa: movs r0,#0x6
  000d17ac: mov.w r1,#0x810
  000d17b0: str r0,[sp,#0x0]
  000d17b2: mov r0,r5
  000d17b4: movs r2,#0x0
  000d17b6: movs r3,#0x6
  000d17b8: blx 0x00074398
  000d17bc: b.w 0x000d25b4
  000d17c0: movs r0,#0xc
  000d17c2: blx 0x0006eb24
  000d17c6: mov r5,r0
  000d17c8: blx 0x00074380
  000d17cc: movs r0,#0x1
  000d17ce: mov r1,r5
  000d17d0: str r5,[r4,#0x0]
  000d17d2: blx 0x0007438c
  000d17d6: movs r0,#0x28
  000d17d8: blx 0x0006eb24
  000d17dc: mov r5,r0
  000d17de: mov.w r0,#0x1f40
  000d17e2: movw r1,#0x848
  000d17e6: str r0,[sp,#0x0]
  000d17e8: mov r0,r5
  000d17ea: movs r2,#0x0
  000d17ec: movs r3,#0x5
  000d17ee: blx 0x00074398
  000d17f2: b.w 0x000d3ad8
  000d17f6: movs r0,#0xc
  000d17f8: blx 0x0006eb24
  000d17fc: mov r5,r0
  000d17fe: blx 0x00074380
  000d1802: movs r0,#0x6
  000d1804: mov r1,r5
  000d1806: str r5,[r4,#0x0]
  000d1808: blx 0x0007438c
  000d180c: movs r0,#0x28
  000d180e: blx 0x0006eb24
  000d1812: mov r5,r0
  000d1814: mov.w r0,#0x1f40
  000d1818: movw r1,#0x86a
  000d181c: str r0,[sp,#0x0]
  000d181e: mov r0,r5
  000d1820: movs r2,#0x1b
  000d1822: movs r3,#0x5
  000d1824: blx 0x00074398
  000d1828: ldr r0,[r4,#0x0]
  000d182a: ldr r0,[r0,#0x4]
  000d182c: str r5,[r0,#0x0]
  000d182e: movs r0,#0x28
  000d1830: blx 0x0006eb24
  000d1834: mov r5,r0
  000d1836: movs r6,#0x0
  000d1838: movw r1,#0x86b
  000d183c: movs r2,#0x0
  000d183e: movs r3,#0x6
  000d1840: str r6,[sp,#0x0]
  000d1842: blx 0x00074398
  000d1846: ldr r0,[r4,#0x0]
  000d1848: ldr r0,[r0,#0x4]
  000d184a: str r5,[r0,#0x4]
  000d184c: movs r0,#0x28
  000d184e: blx 0x0006eb24
  000d1852: mov r5,r0
  000d1854: movw r1,#0x86c
  000d1858: movs r2,#0x1c
  000d185a: movs r3,#0x10
  000d185c: str r6,[sp,#0x0]
  000d185e: blx 0x00074398
  000d1862: ldr r0,[r4,#0x0]
  000d1864: ldr r0,[r0,#0x4]
  000d1866: str r5,[r0,#0x8]
  000d1868: movs r0,#0x28
  000d186a: blx 0x0006eb24
  000d186e: mov r5,r0
  000d1870: movs r6,#0x3
  000d1872: movw r1,#0x86d
  000d1876: movs r2,#0x0
  000d1878: movs r3,#0x14
  000d187a: str r6,[sp,#0x0]
  000d187c: blx 0x00074398
  000d1880: ldr r0,[r4,#0x0]
  000d1882: ldr r0,[r0,#0x4]
  000d1884: str r5,[r0,#0xc]
  000d1886: movs r0,#0x28
  000d1888: blx 0x0006eb24
  000d188c: mov r5,r0
  000d188e: movw r1,#0x86e
  000d1892: movs r2,#0x0
  000d1894: movs r3,#0x6
  000d1896: str r6,[sp,#0x0]
  000d1898: movs r6,#0x0
  000d189a: blx 0x00074398
  000d189e: ldr r0,[r4,#0x0]
  000d18a0: ldr r0,[r0,#0x4]
  000d18a2: str r5,[r0,#0x10]
  000d18a4: movs r0,#0x28
  000d18a6: blx 0x0006eb24
  000d18aa: mov r5,r0
  000d18ac: movw r1,#0x86f
  000d18b0: movs r2,#0x1b
  000d18b2: movs r3,#0x1c
  000d18b4: str r6,[sp,#0x0]
  000d18b6: blx 0x00074398
  000d18ba: b.w 0x000d2f6e
  000d18be: movs r0,#0xc
  000d18c0: blx 0x0006eb24
  000d18c4: mov r5,r0
  000d18c6: blx 0x00074380
  000d18ca: movs r0,#0x1
  000d18cc: mov r1,r5
  000d18ce: str r5,[r4,#0x0]
  000d18d0: blx 0x0007438c
  000d18d4: movs r0,#0x28
  000d18d6: blx 0x0006eb24
  000d18da: mov r5,r0
  000d18dc: mov.w r0,#0x1f40
  000d18e0: movw r1,#0x8a1
  000d18e4: str r0,[sp,#0x0]
  000d18e6: mov r0,r5
  000d18e8: movs r2,#0x0
  000d18ea: movs r3,#0x5
  000d18ec: blx 0x00074398
  000d18f0: b.w 0x000d3ad8
  000d18f4: movs r0,#0xc
  000d18f6: blx 0x0006eb24
  000d18fa: mov r5,r0
  000d18fc: blx 0x00074380
  000d1900: movs r0,#0x7
  000d1902: mov r1,r5
  000d1904: str r5,[r4,#0x0]
  000d1906: blx 0x0007438c
  000d190a: movs r0,#0x28
  000d190c: blx 0x0006eb24
  000d1910: mov r5,r0
  000d1912: mov.w r0,#0x1f40
  000d1916: movw r1,#0x8b4
  000d191a: str r0,[sp,#0x0]
  000d191c: mov r0,r5
  000d191e: movs r2,#0x0
  000d1920: movs r3,#0x5
  000d1922: movs r6,#0x0
  000d1924: blx 0x00074398
  000d1928: ldr r0,[r4,#0x0]
  000d192a: ldr r0,[r0,#0x4]
  000d192c: str r5,[r0,#0x0]
  000d192e: movs r0,#0x28
  000d1930: blx 0x0006eb24
  000d1934: mov r5,r0
  000d1936: movw r1,#0x8b5
  000d193a: movs r2,#0x14
  000d193c: movs r3,#0x6
  000d193e: str r6,[sp,#0x0]
  000d1940: blx 0x00074398
  000d1944: ldr r0,[r4,#0x0]
  000d1946: ldr r0,[r0,#0x4]
  000d1948: str r5,[r0,#0x4]
  000d194a: movs r0,#0x28
  000d194c: blx 0x0006eb24
  000d1950: mov r5,r0
  000d1952: movs r0,#0x1
  000d1954: movw r1,#0x8b6
  000d1958: str r0,[sp,#0x0]
  000d195a: mov r0,r5
  000d195c: movs r2,#0x0
  000d195e: movs r3,#0x6
  000d1960: blx 0x00074398
  000d1964: ldr r0,[r4,#0x0]
  000d1966: ldr r0,[r0,#0x4]
  000d1968: str r5,[r0,#0x8]
  000d196a: movs r0,#0x28
  000d196c: blx 0x0006eb24
  000d1970: mov r5,r0
  000d1972: movs r0,#0x2
  000d1974: movw r1,#0x8b7
  000d1978: str r0,[sp,#0x0]
  000d197a: mov r0,r5
  000d197c: movs r2,#0x1e
  000d197e: movs r3,#0x6
  000d1980: blx 0x00074398
  000d1984: ldr r0,[r4,#0x0]
  000d1986: ldr r0,[r0,#0x4]
  000d1988: str r5,[r0,#0xc]
  000d198a: movs r0,#0x28
  000d198c: blx 0x0006eb24
  000d1990: mov r5,r0
  000d1992: movs r0,#0x3
  000d1994: movw r1,#0x8b8
  000d1998: str r0,[sp,#0x0]
  000d199a: mov r0,r5
  000d199c: movs r2,#0x0
  000d199e: movs r3,#0x6
  000d19a0: blx 0x00074398
  000d19a4: ldr r0,[r4,#0x0]
  000d19a6: ldr r0,[r0,#0x4]
  000d19a8: str r5,[r0,#0x10]
  000d19aa: movs r0,#0x28
  000d19ac: blx 0x0006eb24
  000d19b0: mov r5,r0
  000d19b2: movs r6,#0x5
  000d19b4: movw r1,#0x8b9
  000d19b8: movs r2,#0x1e
  000d19ba: movs r3,#0x14
  000d19bc: str r6,[sp,#0x0]
  000d19be: blx 0x00074398
  000d19c2: ldr r0,[r4,#0x0]
  000d19c4: ldr r0,[r0,#0x4]
  000d19c6: str r5,[r0,#0x14]
  000d19c8: movs r0,#0x28
  000d19ca: blx 0x0006eb24
  000d19ce: mov r5,r0
  000d19d0: movw r1,#0x8ba
  000d19d4: movs r2,#0x0
  000d19d6: movs r3,#0x6
  000d19d8: str r6,[sp,#0x0]
  000d19da: blx 0x00074398
  000d19de: b 0x000d1fce
  000d19e8: movs r0,#0xc
  000d19ea: blx 0x0006eb24
  000d19ee: mov r5,r0
  000d19f0: blx 0x00074380
  000d19f4: movs r0,#0xc
  000d19f6: mov r1,r5
  000d19f8: str r5,[r4,#0x0]
  000d19fa: blx 0x0007438c
  000d19fe: movs r0,#0x28
  000d1a00: blx 0x0006eb24
  000d1a04: mov r5,r0
  000d1a06: movs r6,#0x0
  000d1a08: movw r1,#0x8ef
  000d1a0c: movs r2,#0x0
  000d1a0e: movs r3,#0x10
  000d1a10: str r6,[sp,#0x0]
  000d1a12: blx 0x00074398
  000d1a16: ldr r0,[r4,#0x0]
  000d1a18: ldr r0,[r0,#0x4]
  000d1a1a: str r5,[r0,#0x0]
  000d1a1c: movs r0,#0x28
  000d1a1e: blx 0x0006eb24
  000d1a22: mov r5,r0
  000d1a24: mov.w r1,#0x8f0
  000d1a28: movs r2,#0x1f
  000d1a2a: movs r3,#0x6
  000d1a2c: str r6,[sp,#0x0]
  000d1a2e: blx 0x00074398
  000d1a32: ldr r0,[r4,#0x0]
  000d1a34: ldr r0,[r0,#0x4]
  000d1a36: str r5,[r0,#0x4]
  000d1a38: movs r0,#0x28
  000d1a3a: blx 0x0006eb24
  000d1a3e: mov r5,r0
  000d1a40: movs r0,#0x1
  000d1a42: movw r1,#0x8f1
  000d1a46: str r0,[sp,#0x0]
  000d1a48: mov r0,r5
  000d1a4a: movs r2,#0x1e
  000d1a4c: movs r3,#0x6
  000d1a4e: blx 0x00074398
  000d1a52: ldr r0,[r4,#0x0]
  000d1a54: ldr r0,[r0,#0x4]
  000d1a56: str r5,[r0,#0x8]
  000d1a58: movs r0,#0x28
  000d1a5a: blx 0x0006eb24
  000d1a5e: mov r5,r0
  000d1a60: movs r0,#0x2
  000d1a62: movw r1,#0x8f2
  000d1a66: str r0,[sp,#0x0]
  000d1a68: mov r0,r5
  000d1a6a: movs r2,#0x0
  000d1a6c: movs r3,#0x14
  000d1a6e: blx 0x00074398
  000d1a72: ldr r0,[r4,#0x0]
  000d1a74: ldr r0,[r0,#0x4]
  000d1a76: str r5,[r0,#0xc]
  000d1a78: movs r0,#0x28
  000d1a7a: blx 0x0006eb24
  000d1a7e: mov r5,r0
  000d1a80: movs r0,#0x3
  000d1a82: movw r1,#0x8f3
  000d1a86: str r0,[sp,#0x0]
  000d1a88: mov r0,r5
  000d1a8a: movs r2,#0x1f
  000d1a8c: movs r3,#0x6
  000d1a8e: blx 0x00074398
  000d1a92: ldr r0,[r4,#0x0]
  000d1a94: ldr r0,[r0,#0x4]
  000d1a96: str r5,[r0,#0x10]
  000d1a98: movs r0,#0x28
  000d1a9a: blx 0x0006eb24
  000d1a9e: mov r5,r0
  000d1aa0: movs r0,#0x4
  000d1aa2: movw r1,#0x8f4
  000d1aa6: str r0,[sp,#0x0]
  000d1aa8: mov r0,r5
  000d1aaa: movs r2,#0x0
  000d1aac: movs r3,#0x6
  000d1aae: blx 0x00074398
  000d1ab2: ldr r0,[r4,#0x0]
  000d1ab4: ldr r0,[r0,#0x4]
  000d1ab6: str r5,[r0,#0x14]
  000d1ab8: movs r0,#0x28
  000d1aba: blx 0x0006eb24
  000d1abe: mov r5,r0
  000d1ac0: movs r0,#0x8
  000d1ac2: movw r1,#0x8f5
  000d1ac6: str r0,[sp,#0x0]
  000d1ac8: mov r0,r5
  000d1aca: movs r2,#0x0
  000d1acc: movs r3,#0x14
  000d1ace: blx 0x00074398
  000d1ad2: ldr r0,[r4,#0x0]
  000d1ad4: ldr r0,[r0,#0x4]
  000d1ad6: str r5,[r0,#0x18]
  000d1ad8: movs r0,#0x28
  000d1ada: blx 0x0006eb24
  000d1ade: mov r5,r0
  000d1ae0: movs r0,#0x6
  000d1ae2: movw r1,#0x8f6
  000d1ae6: str r0,[sp,#0x0]
  000d1ae8: mov r0,r5
  000d1aea: movs r2,#0x1f
  000d1aec: movs r3,#0x6
  000d1aee: blx 0x00074398
  000d1af2: ldr r0,[r4,#0x0]
  000d1af4: ldr r0,[r0,#0x4]
  000d1af6: str r5,[r0,#0x1c]
  000d1af8: movs r0,#0x28
  000d1afa: blx 0x0006eb24
  000d1afe: mov r5,r0
  000d1b00: movs r0,#0x7
  000d1b02: movw r1,#0x8f7
  000d1b06: str r0,[sp,#0x0]
  000d1b08: mov r0,r5
  000d1b0a: movs r2,#0x0
  000d1b0c: movs r3,#0x6
  000d1b0e: blx 0x00074398
  000d1b12: ldr r0,[r4,#0x0]
  000d1b14: ldr r0,[r0,#0x4]
  000d1b16: str r5,[r0,#0x20]
  000d1b18: movs r0,#0x28
  000d1b1a: blx 0x0006eb24
  000d1b1e: mov r5,r0
  000d1b20: movs r0,#0x8
  000d1b22: movw r1,#0x8f8
  000d1b26: str r0,[sp,#0x0]
  000d1b28: mov r0,r5
  000d1b2a: movs r2,#0x1f
  000d1b2c: movs r3,#0x6
  000d1b2e: blx 0x00074398
  000d1b32: ldr r0,[r4,#0x0]
  000d1b34: ldr r0,[r0,#0x4]
  000d1b36: str r5,[r0,#0x24]
  000d1b38: movs r0,#0x28
  000d1b3a: blx 0x0006eb24
  000d1b3e: mov r5,r0
  000d1b40: movs r0,#0x9
  000d1b42: movw r1,#0x8f9
  000d1b46: str r0,[sp,#0x0]
  000d1b48: mov r0,r5
  000d1b4a: movs r2,#0x0
  000d1b4c: movs r3,#0x6
  000d1b4e: blx 0x00074398
  000d1b52: ldr r0,[r4,#0x0]
  000d1b54: ldr r0,[r0,#0x4]
  000d1b56: str r5,[r0,#0x28]
  000d1b58: movs r0,#0x28
  000d1b5a: blx 0x0006eb24
  000d1b5e: mov r5,r0
  000d1b60: movs r0,#0xa
  000d1b62: movw r1,#0x8fa
  000d1b66: str r0,[sp,#0x0]
  000d1b68: mov r0,r5
  000d1b6a: movs r2,#0x1f
  000d1b6c: movs r3,#0x6
  000d1b6e: blx 0x00074398
  000d1b72: b.w 0x000d36f8
  000d1b76: movs r0,#0xc
  000d1b78: blx 0x0006eb24
  000d1b7c: mov r5,r0
  000d1b7e: blx 0x00074380
  000d1b82: movs r0,#0x2
  000d1b84: mov r1,r5
  000d1b86: str r5,[r4,#0x0]
  000d1b88: blx 0x0007438c
  000d1b8c: movs r0,#0x28
  000d1b8e: blx 0x0006eb24
  000d1b92: mov r5,r0
  000d1b94: mov.w r0,#0x1f40
  000d1b98: movw r1,#0x90e
  000d1b9c: str r0,[sp,#0x0]
  000d1b9e: mov r0,r5
  000d1ba0: movs r2,#0x0
  000d1ba2: movs r3,#0x5
  000d1ba4: movs r6,#0x0
  000d1ba6: blx 0x00074398
  000d1baa: ldr r0,[r4,#0x0]
  000d1bac: ldr r0,[r0,#0x4]
  000d1bae: str r5,[r0,#0x0]
  000d1bb0: movs r0,#0x28
  000d1bb2: blx 0x0006eb24
  000d1bb6: mov r5,r0
  000d1bb8: movw r1,#0x90f
  000d1bbc: movs r2,#0x0
  000d1bbe: movs r3,#0x6
  000d1bc0: str r6,[sp,#0x0]
  000d1bc2: blx 0x00074398
  000d1bc6: b.w 0x000d3560
  000d1bca: movs r0,#0xc
  000d1bcc: blx 0x0006eb24
  000d1bd0: mov r5,r0
  000d1bd2: blx 0x00074380
  000d1bd6: movs r0,#0x4
  000d1bd8: mov r1,r5
  000d1bda: str r5,[r4,#0x0]
  000d1bdc: blx 0x0007438c
  000d1be0: movs r0,#0x28
  000d1be2: blx 0x0006eb24
  000d1be6: mov r5,r0
  000d1be8: mov.w r0,#0x1f40
  000d1bec: mov.w r1,#0x910
  000d1bf0: str r0,[sp,#0x0]
  000d1bf2: mov r0,r5
  000d1bf4: movs r2,#0x0
  000d1bf6: movs r3,#0x5
  000d1bf8: movs r6,#0x0
  000d1bfa: blx 0x00074398
  000d1bfe: ldr r0,[r4,#0x0]
  000d1c00: ldr r0,[r0,#0x4]
  000d1c02: str r5,[r0,#0x0]
  000d1c04: movs r0,#0x28
  000d1c06: blx 0x0006eb24
  000d1c0a: mov r5,r0
  000d1c0c: movw r1,#0x911
  000d1c10: movs r2,#0x0
  000d1c12: movs r3,#0x6
  000d1c14: str r6,[sp,#0x0]
  000d1c16: blx 0x00074398
  000d1c1a: ldr r0,[r4,#0x0]
  000d1c1c: ldr r0,[r0,#0x4]
  000d1c1e: str r5,[r0,#0x4]
  000d1c20: movs r0,#0x28
  000d1c22: blx 0x0006eb24
  000d1c26: mov r5,r0
  000d1c28: movs r0,#0x1
  000d1c2a: movw r1,#0x912
  000d1c2e: str r0,[sp,#0x0]
  000d1c30: mov r0,r5
  000d1c32: movs r2,#0x22
  000d1c34: movs r3,#0x6
  000d1c36: blx 0x00074398
  000d1c3a: ldr r0,[r4,#0x0]
  000d1c3c: ldr r0,[r0,#0x4]
  000d1c3e: str r5,[r0,#0x8]
  000d1c40: movs r0,#0x28
  000d1c42: blx 0x0006eb24
  000d1c46: mov r5,r0
  000d1c48: movs r0,#0x2
  000d1c4a: movw r1,#0x913
  000d1c4e: str r0,[sp,#0x0]
  000d1c50: mov r0,r5
  000d1c52: movs r2,#0x0
  000d1c54: movs r3,#0x6
  000d1c56: blx 0x00074398
  000d1c5a: b.w 0x000d3508
  000d1c5e: movs r0,#0xc
  000d1c60: blx 0x0006eb24
  000d1c64: mov r5,r0
  000d1c66: blx 0x00074380
  000d1c6a: movs r0,#0x8
  000d1c6c: mov r1,r5
  000d1c6e: str r5,[r4,#0x0]
  000d1c70: blx 0x0007438c
  000d1c74: movs r0,#0x28
  000d1c76: blx 0x0006eb24
  000d1c7a: mov r5,r0
  000d1c7c: mov.w r0,#0x1f40
  000d1c80: movw r1,#0x92b
  000d1c84: str r0,[sp,#0x0]
  000d1c86: mov r0,r5
  000d1c88: movs r2,#0x0
  000d1c8a: movs r3,#0x5
  000d1c8c: movs r6,#0x0
  000d1c8e: blx 0x00074398
  000d1c92: ldr r0,[r4,#0x0]
  000d1c94: ldr r0,[r0,#0x4]
  000d1c96: str r5,[r0,#0x0]
  000d1c98: movs r0,#0x28
  000d1c9a: blx 0x0006eb24
  000d1c9e: mov r5,r0
  000d1ca0: movw r1,#0x92c
  000d1ca4: movs r2,#0x0
  000d1ca6: movs r3,#0x10
  000d1ca8: str r6,[sp,#0x0]
  000d1caa: blx 0x00074398
  000d1cae: ldr r0,[r4,#0x0]
  000d1cb0: ldr r0,[r0,#0x4]
  000d1cb2: str r5,[r0,#0x4]
  000d1cb4: movs r0,#0x28
  000d1cb6: blx 0x0006eb24
  000d1cba: mov r5,r0
  000d1cbc: movs r0,#0x1
  000d1cbe: movw r1,#0x92d
  000d1cc2: str r0,[sp,#0x0]
  000d1cc4: mov r0,r5
  000d1cc6: movs r2,#0xb
  000d1cc8: movs r3,#0x6
  000d1cca: blx 0x00074398
  000d1cce: ldr r0,[r4,#0x0]
  000d1cd0: ldr r0,[r0,#0x4]
  000d1cd2: str r5,[r0,#0x8]
  000d1cd4: movs r0,#0x28
  000d1cd6: blx 0x0006eb24
  000d1cda: mov r5,r0
  000d1cdc: movs r0,#0x2
  000d1cde: movw r1,#0x92e
  000d1ce2: str r0,[sp,#0x0]
  000d1ce4: mov r0,r5
  000d1ce6: movs r2,#0x0
  000d1ce8: movs r3,#0x6
  000d1cea: blx 0x00074398
  000d1cee: ldr r0,[r4,#0x0]
  000d1cf0: ldr r0,[r0,#0x4]
  000d1cf2: str r5,[r0,#0xc]
  000d1cf4: movs r0,#0x28
  000d1cf6: blx 0x0006eb24
  000d1cfa: mov r5,r0
  000d1cfc: movs r0,#0x1
  000d1cfe: movw r1,#0x92f
  000d1d02: str r0,[sp,#0x0]
  000d1d04: mov r0,r5
  000d1d06: movs r2,#0x0
  000d1d08: movs r3,#0x1b
  000d1d0a: blx 0x00074398
  000d1d0e: ldr r0,[r4,#0x0]
  000d1d10: ldr r0,[r0,#0x4]
  000d1d12: str r5,[r0,#0x10]
  000d1d14: movs r0,#0x28
  000d1d16: blx 0x0006eb24
  000d1d1a: mov r5,r0
  000d1d1c: movs r0,#0x2
  000d1d1e: mov.w r1,#0x930
  000d1d22: str r0,[sp,#0x0]
  000d1d24: mov r0,r5
  000d1d26: movs r2,#0x21
  000d1d28: movs r3,#0x1b
  000d1d2a: blx 0x00074398
  000d1d2e: ldr r0,[r4,#0x0]
  000d1d30: ldr r0,[r0,#0x4]
  000d1d32: str r5,[r0,#0x14]
  000d1d34: movs r0,#0x28
  000d1d36: blx 0x0006eb24
  000d1d3a: mov r5,r0
  000d1d3c: movs r0,#0x3
  000d1d3e: movw r1,#0x931
  000d1d42: str r0,[sp,#0x0]
  000d1d44: mov r0,r5
  000d1d46: movs r2,#0x0
  000d1d48: movs r3,#0x1b
  000d1d4a: blx 0x00074398
  000d1d4e: ldr r0,[r4,#0x0]
  000d1d50: ldr r0,[r0,#0x4]
  000d1d52: str r5,[r0,#0x18]
  000d1d54: movs r0,#0x28
  000d1d56: blx 0x0006eb24
  000d1d5a: mov r5,r0
  000d1d5c: movs r0,#0x6
  000d1d5e: movw r1,#0x932
  000d1d62: str r0,[sp,#0x0]
  000d1d64: mov r0,r5
  000d1d66: movs r2,#0x0
  000d1d68: movs r3,#0x6
  000d1d6a: blx 0x00074398
  000d1d6e: b.w 0x000d25b4
  000d1d72: movs r0,#0xc
  000d1d74: blx 0x0006eb24
  000d1d78: mov r5,r0
  000d1d7a: blx 0x00074380
  000d1d7e: movs r0,#0x4
  000d1d80: mov r1,r5
  000d1d82: str r5,[r4,#0x0]
  000d1d84: blx 0x0007438c
  000d1d88: movs r0,#0x28
  000d1d8a: blx 0x0006eb24
  000d1d8e: mov r5,r0
  000d1d90: mov.w r0,#0x3e80
  000d1d94: mov.w r1,#0x980
  000d1d98: str r0,[sp,#0x0]
  000d1d9a: mov r0,r5
  000d1d9c: movs r2,#0x1a
  000d1d9e: movs r3,#0x5
  000d1da0: blx 0x00074398
  000d1da4: ldr r0,[r4,#0x0]
  000d1da6: ldr r0,[r0,#0x4]
  000d1da8: str r5,[r0,#0x0]
  000d1daa: movs r0,#0x28
  000d1dac: blx 0x0006eb24
  000d1db0: mov r5,r0
  000d1db2: movs r0,#0x0
  000d1db4: movw r1,#0x981
  000d1db8: str r0,[sp,#0x0]
  000d1dba: mov r0,r5
  000d1dbc: movs r2,#0x1f
  000d1dbe: movs r3,#0x6
  000d1dc0: blx 0x00074398
  000d1dc4: ldr r0,[r4,#0x0]
  000d1dc6: ldr r0,[r0,#0x4]
  000d1dc8: str r5,[r0,#0x4]
  000d1dca: movs r0,#0x28
  000d1dcc: blx 0x0006eb24
  000d1dd0: mov r5,r0
  000d1dd2: movs r0,#0x1
  000d1dd4: movw r1,#0x982
  000d1dd8: str r0,[sp,#0x0]
  000d1dda: mov r0,r5
  000d1ddc: movs r2,#0x1a
  000d1dde: movs r3,#0x6
  000d1de0: blx 0x00074398
  000d1de4: ldr r0,[r4,#0x0]
  000d1de6: ldr r0,[r0,#0x4]
  000d1de8: str r5,[r0,#0x8]
  000d1dea: movs r0,#0x28
  000d1dec: blx 0x0006eb24
  000d1df0: mov r5,r0
  000d1df2: movs r0,#0x2
  000d1df4: movw r1,#0x983
  000d1df8: str r0,[sp,#0x0]
  000d1dfa: mov r0,r5
  000d1dfc: movs r2,#0x1a
  000d1dfe: movs r3,#0x6
  000d1e00: blx 0x00074398
  000d1e04: b.w 0x000d3508
  000d1e08: movs r0,#0xc
  000d1e0a: blx 0x0006eb24
  000d1e0e: mov r5,r0
  000d1e10: blx 0x00074380
  000d1e14: movs r0,#0x6
  000d1e16: mov r1,r5
  000d1e18: str r5,[r4,#0x0]
  000d1e1a: blx 0x0007438c
  000d1e1e: movs r0,#0x28
  000d1e20: blx 0x0006eb24
  000d1e24: mov r5,r0
  000d1e26: movw r0,#0x5dc
  000d1e2a: mov.w r1,#0x9b0
  000d1e2e: str r0,[sp,#0x0]
  000d1e30: mov r0,r5
  000d1e32: movs r2,#0x6
  000d1e34: movs r3,#0x5
  000d1e36: blx 0x00074398
  000d1e3a: ldr r0,[r4,#0x0]
  000d1e3c: ldr r0,[r0,#0x4]
  000d1e3e: str r5,[r0,#0x0]
  000d1e40: movs r0,#0x28
  000d1e42: blx 0x0006eb24
  000d1e46: mov r5,r0
  000d1e48: movs r0,#0x0
  000d1e4a: movw r1,#0x9b1
  000d1e4e: str r0,[sp,#0x0]
  000d1e50: mov r0,r5
  000d1e52: movs r2,#0x0
  000d1e54: movs r3,#0x6
  000d1e56: blx 0x00074398
  000d1e5a: ldr r0,[r4,#0x0]
  000d1e5c: ldr r0,[r0,#0x4]
  000d1e5e: str r5,[r0,#0x4]
  000d1e60: movs r0,#0x28
  000d1e62: blx 0x0006eb24
  000d1e66: mov r5,r0
  000d1e68: movs r0,#0x1
  000d1e6a: movw r1,#0x9b2
  000d1e6e: str r0,[sp,#0x0]
  000d1e70: mov r0,r5
  000d1e72: movs r2,#0x6
  000d1e74: movs r3,#0x6
  000d1e76: blx 0x00074398
  000d1e7a: ldr r0,[r4,#0x0]
  000d1e7c: ldr r0,[r0,#0x4]
  000d1e7e: str r5,[r0,#0x8]
  000d1e80: movs r0,#0x28
  000d1e82: blx 0x0006eb24
  000d1e86: mov r5,r0
  000d1e88: movs r0,#0x2
  000d1e8a: movw r1,#0x9b3
  000d1e8e: str r0,[sp,#0x0]
  000d1e90: mov r0,r5
  000d1e92: movs r2,#0x0
  000d1e94: movs r3,#0x6
  000d1e96: blx 0x00074398
  000d1e9a: ldr r0,[r4,#0x0]
  000d1e9c: ldr r0,[r0,#0x4]
  000d1e9e: str r5,[r0,#0xc]
  000d1ea0: movs r0,#0x28
  000d1ea2: blx 0x0006eb24
  000d1ea6: mov r5,r0
  000d1ea8: movs r0,#0x3
  000d1eaa: movw r1,#0x9b4
  000d1eae: str r0,[sp,#0x0]
  000d1eb0: mov r0,r5
  000d1eb2: movs r2,#0x6
  000d1eb4: movs r3,#0x6
  000d1eb6: blx 0x00074398
  000d1eba: ldr r0,[r4,#0x0]
  000d1ebc: ldr r0,[r0,#0x4]
  000d1ebe: str r5,[r0,#0x10]
  000d1ec0: movs r0,#0x28
  000d1ec2: blx 0x0006eb24
  000d1ec6: mov r5,r0
  000d1ec8: movs r0,#0x4
  000d1eca: movw r1,#0x9b5
  000d1ece: str r0,[sp,#0x0]
  000d1ed0: mov r0,r5
  000d1ed2: movs r2,#0x0
  000d1ed4: movs r3,#0x6
  000d1ed6: blx 0x00074398
  000d1eda: b.w 0x000d2f6e
  000d1ede: movs r0,#0xc
  000d1ee0: blx 0x0006eb24
  000d1ee4: mov r5,r0
  000d1ee6: blx 0x00074380
  000d1eea: movs r0,#0x7
  000d1eec: mov r1,r5
  000d1eee: str r5,[r4,#0x0]
  000d1ef0: blx 0x0007438c
  000d1ef4: movs r0,#0x28
  000d1ef6: blx 0x0006eb24
  000d1efa: mov r5,r0
  000d1efc: movw r0,#0x5dc
  000d1f00: movw r1,#0x9c8
  000d1f04: str r0,[sp,#0x0]
  000d1f06: mov r0,r5
  000d1f08: movs r2,#0x0
  000d1f0a: movs r3,#0x5
  000d1f0c: movs r6,#0x0
  000d1f0e: blx 0x00074398
  000d1f12: ldr r0,[r4,#0x0]
  000d1f14: ldr r0,[r0,#0x4]
  000d1f16: str r5,[r0,#0x0]
  000d1f18: movs r0,#0x28
  000d1f1a: blx 0x0006eb24
  000d1f1e: mov r5,r0
  000d1f20: movw r1,#0x9c9
  000d1f24: movs r2,#0x0
  000d1f26: movs r3,#0x6
  000d1f28: str r6,[sp,#0x0]
  000d1f2a: blx 0x00074398
  000d1f2e: ldr r0,[r4,#0x0]
  000d1f30: ldr r0,[r0,#0x4]
  000d1f32: str r5,[r0,#0x4]
  000d1f34: movs r0,#0x28
  000d1f36: blx 0x0006eb24
  000d1f3a: mov r5,r0
  000d1f3c: movs r0,#0x1
  000d1f3e: movw r1,#0x9ca
  000d1f42: str r0,[sp,#0x0]
  000d1f44: mov r0,r5
  000d1f46: movs r2,#0x0
  000d1f48: movs r3,#0x6
  000d1f4a: blx 0x00074398
  000d1f4e: ldr r0,[r4,#0x0]
  000d1f50: ldr r0,[r0,#0x4]
  000d1f52: str r5,[r0,#0x8]
  000d1f54: movs r0,#0x28
  000d1f56: blx 0x0006eb24
  000d1f5a: mov r5,r0
  000d1f5c: movs r0,#0x2
  000d1f5e: movw r1,#0x9cb
  000d1f62: str r0,[sp,#0x0]
  000d1f64: mov r0,r5
  000d1f66: movs r2,#0x3c
  000d1f68: movs r3,#0x6
  000d1f6a: blx 0x00074398
  000d1f6e: ldr r0,[r4,#0x0]
  000d1f70: ldr r0,[r0,#0x4]
  000d1f72: str r5,[r0,#0xc]
  000d1f74: movs r0,#0x28
  000d1f76: blx 0x0006eb24
  000d1f7a: mov r5,r0
  000d1f7c: movs r0,#0x3
  000d1f7e: movw r1,#0x9cc
  000d1f82: str r0,[sp,#0x0]
  000d1f84: mov r0,r5
  000d1f86: movs r2,#0x0
  000d1f88: movs r3,#0x6
  000d1f8a: blx 0x00074398
  000d1f8e: ldr r0,[r4,#0x0]
  000d1f90: ldr r0,[r0,#0x4]
  000d1f92: str r5,[r0,#0x10]
  000d1f94: movs r0,#0x28
  000d1f96: blx 0x0006eb24
  000d1f9a: mov r5,r0
  000d1f9c: movs r0,#0x2
  000d1f9e: movw r1,#0x9cd
  000d1fa2: str r0,[sp,#0x0]
  000d1fa4: mov r0,r5
  000d1fa6: movs r2,#0x0
  000d1fa8: movs r3,#0x1b
  000d1faa: blx 0x00074398
  000d1fae: ldr r0,[r4,#0x0]
  000d1fb0: ldr r0,[r0,#0x4]
  000d1fb2: str r5,[r0,#0x14]
  000d1fb4: movs r0,#0x28
  000d1fb6: blx 0x0006eb24
  000d1fba: mov r5,r0
  000d1fbc: movs r0,#0x4
  000d1fbe: movw r1,#0x9ce
  000d1fc2: str r0,[sp,#0x0]
  000d1fc4: mov r0,r5
  000d1fc6: movs r2,#0x3c
  000d1fc8: movs r3,#0x1b
  000d1fca: blx 0x00074398
  000d1fce: ldr r0,[r4,#0x0]
  000d1fd0: ldr r0,[r0,#0x4]
  000d1fd2: str r5,[r0,#0x18]
  000d1fd4: b.w 0x000d3ade
  000d1fd8: movs r0,#0xc
  000d1fda: blx 0x0006eb24
  000d1fde: mov r5,r0
  000d1fe0: blx 0x00074380
  000d1fe4: movs r0,#0xa
  000d1fe6: mov r1,r5
  000d1fe8: str r5,[r4,#0x0]
  000d1fea: blx 0x0007438c
  000d1fee: movs r0,#0x28
  000d1ff0: blx 0x0006eb24
  000d1ff4: mov r5,r0
  000d1ff6: movw r0,#0x5dc
  000d1ffa: movw r1,#0x9d1
  000d1ffe: str r0,[sp,#0x0]
  000d2000: mov r0,r5
  000d2002: movs r2,#0x3b
  000d2004: movs r3,#0x5
  000d2006: blx 0x00074398
  000d200a: ldr r0,[r4,#0x0]
  000d200c: ldr r0,[r0,#0x4]
  000d200e: str r5,[r0,#0x0]
  000d2010: movs r0,#0x28
  000d2012: blx 0x0006eb24
  000d2016: mov r5,r0
  000d2018: movs r0,#0x0
  000d201a: movw r1,#0x9d2
  000d201e: str r0,[sp,#0x0]
  000d2020: mov r0,r5
  000d2022: movs r2,#0x0
  000d2024: movs r3,#0x6
  000d2026: blx 0x00074398
  000d202a: ldr r0,[r4,#0x0]
  000d202c: ldr r0,[r0,#0x4]
  000d202e: str r5,[r0,#0x4]
  000d2030: movs r0,#0x28
  000d2032: blx 0x0006eb24
  000d2036: mov r5,r0
  000d2038: movs r0,#0x1
  000d203a: movw r1,#0x9d3
  000d203e: str r0,[sp,#0x0]
  000d2040: mov r0,r5
  000d2042: movs r2,#0x3b
  000d2044: movs r3,#0x6
  000d2046: blx 0x00074398
  000d204a: ldr r0,[r4,#0x0]
  000d204c: ldr r0,[r0,#0x4]
  000d204e: str r5,[r0,#0x8]
  000d2050: movs r0,#0x28
  000d2052: blx 0x0006eb24
  000d2056: mov r5,r0
  000d2058: movs r6,#0x2
  000d205a: movw r1,#0x9d4
  000d205e: movs r2,#0x0
  000d2060: movs r3,#0x6
  000d2062: str r6,[sp,#0x0]
  000d2064: blx 0x00074398
  000d2068: ldr r0,[r4,#0x0]
  000d206a: ldr r0,[r0,#0x4]
  000d206c: str r5,[r0,#0xc]
  000d206e: movs r0,#0x28
  000d2070: blx 0x0006eb24
  000d2074: mov r5,r0
  000d2076: movw r1,#0x9d5
  000d207a: movs r2,#0x0
  000d207c: movs r3,#0x1b
  000d207e: str r6,[sp,#0x0]
  000d2080: blx 0x00074398
  000d2084: ldr r0,[r4,#0x0]
  000d2086: ldr r0,[r0,#0x4]
  000d2088: str r5,[r0,#0x10]
  000d208a: movs r0,#0x28
  000d208c: blx 0x0006eb24
  000d2090: mov r5,r0
  000d2092: movs r0,#0x4
  000d2094: movw r1,#0x9d6
  000d2098: str r0,[sp,#0x0]
  000d209a: mov r0,r5
  000d209c: movs r2,#0x0
  000d209e: movs r3,#0x6
  000d20a0: movs r6,#0x6
  000d20a2: blx 0x00074398
  000d20a6: ldr r0,[r4,#0x0]
  000d20a8: ldr r0,[r0,#0x4]
  000d20aa: str r5,[r0,#0x14]
  000d20ac: movs r0,#0x28
  000d20ae: blx 0x0006eb24
  000d20b2: mov r5,r0
  000d20b4: movw r1,#0x9d7
  000d20b8: movs r2,#0x0
  000d20ba: movs r3,#0x1b
  000d20bc: str r6,[sp,#0x0]
  000d20be: blx 0x00074398
  000d20c2: ldr r0,[r4,#0x0]
  000d20c4: ldr r0,[r0,#0x4]
  000d20c6: str r5,[r0,#0x18]
  000d20c8: movs r0,#0x28
  000d20ca: blx 0x0006eb24
  000d20ce: mov r5,r0
  000d20d0: movs r0,#0x6
  000d20d2: movw r1,#0x9d8
  000d20d6: str r0,[sp,#0x0]
  000d20d8: mov r0,r5
  000d20da: movs r2,#0x0
  000d20dc: movs r3,#0x6
  000d20de: movs r6,#0x0
  000d20e0: blx 0x00074398
  000d20e4: ldr r0,[r4,#0x0]
  000d20e6: ldr r0,[r0,#0x4]
  000d20e8: str r5,[r0,#0x1c]
  000d20ea: movs r0,#0x28
  000d20ec: blx 0x0006eb24
  000d20f0: mov r5,r0
  000d20f2: movs r0,#0x3
  000d20f4: movw r1,#0x9d9
  000d20f8: strd r6,r0,[sp,#0x0]
  000d20fc: mov r0,r5
  000d20fe: movs r2,#0x0
  000d2100: movs r3,#0x9
  000d2102: blx 0x000743a4
  000d2106: ldr r0,[r4,#0x0]
  000d2108: ldr r0,[r0,#0x4]
  000d210a: str r5,[r0,#0x20]
  000d210c: movs r0,#0x28
  000d210e: blx 0x0006eb24
  000d2112: mov r5,r0
  000d2114: movs r0,#0xa
  000d2116: movw r1,#0x9da
  000d211a: str r0,[sp,#0x0]
  000d211c: mov r0,r5
  000d211e: movs r2,#0x3b
  000d2120: movs r3,#0x1b
  000d2122: blx 0x00074398
  000d2126: ldr r0,[r4,#0x0]
  000d2128: ldr r0,[r0,#0x4]
  000d212a: str r5,[r0,#0x24]
  000d212c: b.w 0x000d3ade
  000d2130: movs r0,#0xc
  000d2132: blx 0x0006eb24
  000d2136: mov r5,r0
  000d2138: blx 0x00074380
  000d213c: movs r0,#0x1
  000d213e: mov r1,r5
  000d2140: str r5,[r4,#0x0]
  000d2142: movs r6,#0x1
  000d2144: blx 0x0007438c
  000d2148: movs r0,#0x28
  000d214a: blx 0x0006eb24
  000d214e: mov r5,r0
  000d2150: movw r1,#0x9fb
  000d2154: movs r2,#0x11
  000d2156: movs r3,#0x1b
  000d2158: str r6,[sp,#0x0]
  000d215a: blx 0x00074398
  000d215e: b.w 0x000d3ad8
  000d2162: movs r0,#0xc
  000d2164: blx 0x0006eb24
  000d2168: mov r5,r0
  000d216a: blx 0x00074380
  000d216e: movs r0,#0x3
  000d2170: mov r1,r5
  000d2172: str r5,[r4,#0x0]
  000d2174: blx 0x0007438c
  000d2178: movs r0,#0x28
  000d217a: blx 0x0006eb24
  000d217e: mov r5,r0
  000d2180: movw r0,#0x5dc
  000d2184: movw r1,#0xa16
  000d2188: str r0,[sp,#0x0]
  000d218a: mov r0,r5
  000d218c: movs r2,#0x0
  000d218e: movs r3,#0x5
  000d2190: movs r6,#0x0
  000d2192: blx 0x00074398
  000d2196: ldr r0,[r4,#0x0]
  000d2198: ldr r0,[r0,#0x4]
  000d219a: str r5,[r0,#0x0]
  000d219c: movs r0,#0x28
  000d219e: blx 0x0006eb24
  000d21a2: mov r5,r0
  000d21a4: movw r1,#0xa17
  000d21a8: movs r2,#0x16
  000d21aa: movs r3,#0x6
  000d21ac: str r6,[sp,#0x0]
  000d21ae: blx 0x00074398
  000d21b2: ldr r0,[r4,#0x0]
  000d21b4: ldr r0,[r0,#0x4]
  000d21b6: str r5,[r0,#0x4]
  000d21b8: movs r0,#0x28
  000d21ba: blx 0x0006eb24
  000d21be: mov r5,r0
  000d21c0: movs r0,#0x1
  000d21c2: movw r1,#0xa18
  000d21c6: str r0,[sp,#0x0]
  000d21c8: mov r0,r5
  000d21ca: movs r2,#0x0
  000d21cc: movs r3,#0x6
  000d21ce: blx 0x00074398
  000d21d2: b.w 0x000d31a4
  000d21d6: movs r0,#0xc
  000d21d8: blx 0x0006eb24
  000d21dc: mov r5,r0
  000d21de: blx 0x00074380
  000d21e2: movs r0,#0x1
  000d21e4: mov r1,r5
  000d21e6: str r5,[r4,#0x0]
  000d21e8: movs r6,#0x1
  000d21ea: blx 0x0007438c
  000d21ee: movs r0,#0x28
  000d21f0: blx 0x0006eb24
  000d21f4: mov r5,r0
  000d21f6: movw r1,#0xa28
  000d21fa: movs r2,#0x11
  000d21fc: movs r3,#0x1b
  000d21fe: str r6,[sp,#0x0]
  000d2200: blx 0x00074398
  000d2204: b.w 0x000d3ad8
  000d2208: movs r0,#0xc
  000d220a: blx 0x0006eb24
  000d220e: mov r5,r0
  000d2210: blx 0x00074380
  000d2214: movs r0,#0x9
  000d2216: mov r1,r5
  000d2218: str r5,[r4,#0x0]
  000d221a: blx 0x0007438c
  000d221e: movs r0,#0x28
  000d2220: blx 0x0006eb24
  000d2224: mov r5,r0
  000d2226: movs r0,#0x0
  000d2228: movw r1,#0xa61
  000d222c: str r0,[sp,#0x0]
  000d222e: mov r0,r5
  000d2230: movs r2,#0x14
  000d2232: movs r3,#0x5
  000d2234: blx 0x00074398
  000d2238: ldr r0,[r4,#0x0]
  000d223a: ldr r0,[r0,#0x4]
  000d223c: str r5,[r0,#0x0]
  000d223e: movs r0,#0x28
  000d2240: blx 0x0006eb24
  000d2244: mov r5,r0
  000d2246: movs r0,#0x2
  000d2248: movw r1,#0xa62
  000d224c: str r0,[sp,#0x0]
  000d224e: mov r0,r5
  000d2250: movs r2,#0x3d
  000d2252: movs r3,#0x1b
  000d2254: blx 0x00074398
  000d2258: ldr r0,[r4,#0x0]
  000d225a: ldr r0,[r0,#0x4]
  000d225c: str r5,[r0,#0x4]
  000d225e: movs r0,#0x28
  000d2260: blx 0x0006eb24
  000d2264: mov r5,r0
  000d2266: movs r0,#0x5
  000d2268: movw r1,#0xa63
  000d226c: str r0,[sp,#0x0]
  000d226e: mov r0,r5
  000d2270: movs r2,#0x0
  000d2272: movs r3,#0x1b
  000d2274: blx 0x00074398
  000d2278: ldr r0,[r4,#0x0]
  000d227a: ldr r0,[r0,#0x4]
  000d227c: str r5,[r0,#0x8]
  000d227e: movs r0,#0x28
  000d2280: blx 0x0006eb24
  000d2284: mov r5,r0
  000d2286: movs r0,#0x6
  000d2288: movw r1,#0xa64
  000d228c: str r0,[sp,#0x0]
  000d228e: mov r0,r5
  000d2290: movs r2,#0x14
  000d2292: movs r3,#0x1b
  000d2294: blx 0x00074398
  000d2298: ldr r0,[r4,#0x0]
  000d229a: ldr r0,[r0,#0x4]
  000d229c: str r5,[r0,#0xc]
  000d229e: movs r0,#0x28
  000d22a0: blx 0x0006eb24
  000d22a4: mov r5,r0
  000d22a6: movs r0,#0x3
  000d22a8: movw r1,#0xa65
  000d22ac: str r0,[sp,#0x0]
  000d22ae: mov r0,r5
  000d22b0: movs r2,#0x0
  000d22b2: movs r3,#0x6
  000d22b4: blx 0x00074398
  000d22b8: ldr r0,[r4,#0x0]
  000d22ba: ldr r0,[r0,#0x4]
  000d22bc: str r5,[r0,#0x10]
  000d22be: movs r0,#0x28
  000d22c0: blx 0x0006eb24
  000d22c4: mov r5,r0
  000d22c6: movs r0,#0x7
  000d22c8: movw r1,#0xa66
  000d22cc: str r0,[sp,#0x0]
  000d22ce: mov r0,r5
  000d22d0: movs r2,#0x0
  000d22d2: movs r3,#0x1b
  000d22d4: blx 0x00074398
  000d22d8: ldr r0,[r4,#0x0]
  000d22da: ldr r0,[r0,#0x4]
  000d22dc: str r5,[r0,#0x14]
  000d22de: movs r0,#0x28
  000d22e0: blx 0x0006eb24
  000d22e4: mov r5,r0
  000d22e6: movs r0,#0x5
  000d22e8: movw r1,#0xa67
  000d22ec: str r0,[sp,#0x0]
  000d22ee: mov r0,r5
  000d22f0: movs r2,#0x14
  000d22f2: movs r3,#0x6
  000d22f4: blx 0x00074398
  000d22f8: ldr r0,[r4,#0x0]
  000d22fa: ldr r0,[r0,#0x4]
  000d22fc: str r5,[r0,#0x18]
  000d22fe: movs r0,#0x28
  000d2300: blx 0x0006eb24
  000d2304: mov r5,r0
  000d2306: movs r0,#0x9
  000d2308: movw r1,#0xa68
  000d230c: str r0,[sp,#0x0]
  000d230e: mov r0,r5
  000d2310: movs r2,#0x0
  000d2312: movs r3,#0x1b
  000d2314: blx 0x00074398
  000d2318: ldr r0,[r4,#0x0]
  000d231a: ldr r0,[r0,#0x4]
  000d231c: str r5,[r0,#0x1c]
  000d231e: movs r0,#0x28
  000d2320: blx 0x0006eb24
  000d2324: mov r5,r0
  000d2326: movs r0,#0xa
  000d2328: movw r1,#0xa69
  000d232c: str r0,[sp,#0x0]
  000d232e: mov r0,r5
  000d2330: movs r2,#0x0
  000d2332: movs r3,#0x1b
  000d2334: blx 0x00074398
  000d2338: b.w 0x000d30fc
  000d233c: movs r0,#0xc
  000d233e: blx 0x0006eb24
  000d2342: mov r5,r0
  000d2344: blx 0x00074380
  000d2348: movs r0,#0x6
  000d234a: mov r1,r5
  000d234c: str r5,[r4,#0x0]
  000d234e: blx 0x0007438c
  000d2352: movs r0,#0x28
  000d2354: blx 0x0006eb24
  000d2358: mov r5,r0
  000d235a: movw r0,#0x5dc
  000d235e: movw r1,#0xa6a
  000d2362: str r0,[sp,#0x0]
  000d2364: mov r0,r5
  000d2366: movs r2,#0x0
  000d2368: movs r3,#0x5
  000d236a: movs r6,#0x0
  000d236c: blx 0x00074398
  000d2370: ldr r0,[r4,#0x0]
  000d2372: ldr r0,[r0,#0x4]
  000d2374: str r5,[r0,#0x0]
  000d2376: movs r0,#0x28
  000d2378: blx 0x0006eb24
  000d237c: mov r5,r0
  000d237e: movw r1,#0xa6b
  000d2382: movs r2,#0x14
  000d2384: movs r3,#0x6
  000d2386: str r6,[sp,#0x0]
  000d2388: blx 0x00074398
  000d238c: ldr r0,[r4,#0x0]
  000d238e: ldr r0,[r0,#0x4]
  000d2390: str r5,[r0,#0x4]
  000d2392: movs r0,#0x28
  000d2394: blx 0x0006eb24
  000d2398: mov r5,r0
  000d239a: movs r0,#0x1
  000d239c: movw r1,#0xa6c
  000d23a0: str r0,[sp,#0x0]
  000d23a2: mov r0,r5
  000d23a4: movs r2,#0x14
  000d23a6: movs r3,#0x6
  000d23a8: blx 0x00074398
  000d23ac: ldr r0,[r4,#0x0]
  000d23ae: ldr r0,[r0,#0x4]
  000d23b0: str r5,[r0,#0x8]
  000d23b2: movs r0,#0x28
  000d23b4: blx 0x0006eb24
  000d23b8: mov r5,r0
  000d23ba: movs r0,#0x2
  000d23bc: movw r1,#0xa6d
  000d23c0: str r0,[sp,#0x0]
  000d23c2: mov r0,r5
  000d23c4: movs r2,#0x0
  000d23c6: movs r3,#0x6
  000d23c8: blx 0x00074398
  000d23cc: ldr r0,[r4,#0x0]
  000d23ce: ldr r0,[r0,#0x4]
  000d23d0: str r5,[r0,#0xc]
  000d23d2: movs r0,#0x28
  000d23d4: blx 0x0006eb24
  000d23d8: mov r5,r0
  000d23da: movs r0,#0x3
  000d23dc: movw r1,#0xa6e
  000d23e0: str r0,[sp,#0x0]
  000d23e2: mov r0,r5
  000d23e4: movs r2,#0x0
  000d23e6: movs r3,#0x6
  000d23e8: movs r6,#0x6
  000d23ea: blx 0x00074398
  000d23ee: ldr r0,[r4,#0x0]
  000d23f0: ldr r0,[r0,#0x4]
  000d23f2: str r5,[r0,#0x10]
  000d23f4: movs r0,#0x28
  000d23f6: blx 0x0006eb24
  000d23fa: mov r5,r0
  000d23fc: movw r1,#0xa6f
  000d2400: movs r2,#0x0
  000d2402: movs r3,#0x1b
  000d2404: str r6,[sp,#0x0]
  000d2406: blx 0x00074398
  000d240a: b.w 0x000d2f6e
  000d240e: movs r0,#0xc
  000d2410: blx 0x0006eb24
  000d2414: mov r5,r0
  000d2416: blx 0x00074380
  000d241a: movs r0,#0x4
  000d241c: mov r1,r5
  000d241e: str r5,[r4,#0x0]
  000d2420: blx 0x0007438c
  000d2424: movs r0,#0x28
  000d2426: blx 0x0006eb24
  000d242a: mov r5,r0
  000d242c: mov.w r0,#0x1f40
  000d2430: movw r1,#0xae1
  000d2434: str r0,[sp,#0x0]
  000d2436: mov r0,r5
  000d2438: movs r2,#0x16
  000d243a: movs r3,#0x5
  000d243c: blx 0x00074398
  000d2440: ldr r0,[r4,#0x0]
  000d2442: ldr r0,[r0,#0x4]
  000d2444: str r5,[r0,#0x0]
  000d2446: movs r0,#0x28
  000d2448: blx 0x0006eb24
  000d244c: mov r5,r0
  000d244e: movs r0,#0x0
  000d2450: movw r1,#0xae2
  000d2454: str r0,[sp,#0x0]
  000d2456: mov r0,r5
  000d2458: movs r2,#0x26
  000d245a: movs r3,#0x6
  000d245c: blx 0x00074398
  000d2460: ldr r0,[r4,#0x0]
  000d2462: ldr r0,[r0,#0x4]
  000d2464: str r5,[r0,#0x4]
  000d2466: movs r0,#0x28
  000d2468: blx 0x0006eb24
  000d246c: mov r5,r0
  000d246e: movs r0,#0x1
  000d2470: movw r1,#0xae3
  000d2474: str r0,[sp,#0x0]
  000d2476: mov r0,r5
  000d2478: movs r2,#0x16
  000d247a: movs r3,#0x6
  000d247c: blx 0x00074398
  000d2480: ldr r0,[r4,#0x0]
  000d2482: ldr r0,[r0,#0x4]
  000d2484: str r5,[r0,#0x8]
  000d2486: movs r0,#0x28
  000d2488: blx 0x0006eb24
  000d248c: mov r5,r0
  000d248e: movs r0,#0x2
  000d2490: movw r1,#0xae4
  000d2494: str r0,[sp,#0x0]
  000d2496: mov r0,r5
  000d2498: movs r2,#0x0
  000d249a: movs r3,#0x6
  000d249c: blx 0x00074398
  000d24a0: b.w 0x000d3508
  000d24a4: movs r0,#0xc
  000d24a6: blx 0x0006eb24
  000d24aa: mov r5,r0
  000d24ac: blx 0x00074380
  000d24b0: movs r0,#0x8
  000d24b2: mov r1,r5
  000d24b4: str r5,[r4,#0x0]
  000d24b6: blx 0x0007438c
  000d24ba: movs r0,#0x28
  000d24bc: blx 0x0006eb24
  000d24c0: mov r5,r0
  000d24c2: movw r0,#0x5dc
  000d24c6: movw r1,#0xaf4
  000d24ca: str r0,[sp,#0x0]
  000d24cc: mov r0,r5
  000d24ce: movs r2,#0x0
  000d24d0: movs r3,#0x5
  000d24d2: movs r6,#0x0
  000d24d4: blx 0x00074398
  000d24d8: ldr r0,[r4,#0x0]
  000d24da: ldr r0,[r0,#0x4]
  000d24dc: str r5,[r0,#0x0]
  000d24de: movs r0,#0x28
  000d24e0: blx 0x0006eb24
  000d24e4: mov r5,r0
  000d24e6: movw r1,#0xafe
  000d24ea: movs r2,#0x0
  000d24ec: movs r3,#0x6
  000d24ee: str r6,[sp,#0x0]
  000d24f0: blx 0x00074398
  000d24f4: ldr r0,[r4,#0x0]
  000d24f6: ldr r0,[r0,#0x4]
  000d24f8: str r5,[r0,#0x4]
  000d24fa: movs r0,#0x28
  000d24fc: blx 0x0006eb24
  000d2500: mov r5,r0
  000d2502: movs r0,#0x2
  000d2504: movw r1,#0xaff
  000d2508: str r0,[sp,#0x0]
  000d250a: mov r0,r5
  000d250c: movs r2,#0x0
  000d250e: movs r3,#0x1b
  000d2510: blx 0x00074398
  000d2514: ldr r0,[r4,#0x0]
  000d2516: ldr r0,[r0,#0x4]
  000d2518: str r5,[r0,#0x8]
  000d251a: movs r0,#0x28
  000d251c: blx 0x0006eb24
  000d2520: mov r5,r0
  000d2522: movs r0,#0x3
  000d2524: mov.w r1,#0xb00
  000d2528: str r0,[sp,#0x0]
  000d252a: mov r0,r5
  000d252c: movs r2,#0xa
  000d252e: movs r3,#0x1b
  000d2530: blx 0x00074398
  000d2534: ldr r0,[r4,#0x0]
  000d2536: ldr r0,[r0,#0x4]
  000d2538: str r5,[r0,#0xc]
  000d253a: movs r0,#0x28
  000d253c: blx 0x0006eb24
  000d2540: mov r5,r0
  000d2542: movs r0,#0x4
  000d2544: movw r1,#0xb01
  000d2548: str r0,[sp,#0x0]
  000d254a: mov r0,r5
  000d254c: movs r2,#0x0
  000d254e: movs r3,#0x1b
  000d2550: blx 0x00074398
  000d2554: ldr r0,[r4,#0x0]
  000d2556: ldr r0,[r0,#0x4]
  000d2558: str r5,[r0,#0x10]
  000d255a: movs r0,#0x28
  000d255c: blx 0x0006eb24
  000d2560: mov r5,r0
  000d2562: movs r0,#0x5
  000d2564: movw r1,#0xb02
  000d2568: str r0,[sp,#0x0]
  000d256a: mov r0,r5
  000d256c: movs r2,#0x0
  000d256e: movs r3,#0x1b
  000d2570: blx 0x00074398
  000d2574: ldr r0,[r4,#0x0]
  000d2576: ldr r0,[r0,#0x4]
  000d2578: str r5,[r0,#0x14]
  000d257a: movs r0,#0x28
  000d257c: blx 0x0006eb24
  000d2580: mov r5,r0
  000d2582: movs r0,#0x6
  000d2584: movw r1,#0xb04
  000d2588: str r0,[sp,#0x0]
  000d258a: mov r0,r5
  000d258c: movs r2,#0x0
  000d258e: movs r3,#0x1b
  000d2590: blx 0x00074398
  000d2594: ldr r0,[r4,#0x0]
  000d2596: ldr r0,[r0,#0x4]
  000d2598: str r5,[r0,#0x18]
  000d259a: movs r0,#0x28
  000d259c: blx 0x0006eb24
  000d25a0: mov r5,r0
  000d25a2: movs r0,#0x7
  000d25a4: movw r1,#0xb05
  000d25a8: str r0,[sp,#0x0]
  000d25aa: mov r0,r5
  000d25ac: movs r2,#0x0
  000d25ae: movs r3,#0x1b
  000d25b0: blx 0x00074398
  000d25b4: ldr r0,[r4,#0x0]
  000d25b6: ldr r0,[r0,#0x4]
  000d25b8: str r5,[r0,#0x1c]
  000d25ba: b.w 0x000d3ade
  000d25be: movs r0,#0xc
  000d25c0: blx 0x0006eb24
  000d25c4: mov r5,r0
  000d25c6: blx 0x00074380
  000d25ca: movs r0,#0x1
  000d25cc: mov r1,r5
  000d25ce: str r5,[r4,#0x0]
  000d25d0: movs r6,#0x1
  000d25d2: blx 0x0007438c
  000d25d6: movs r0,#0x28
  000d25d8: blx 0x0006eb24
  000d25dc: mov r5,r0
  000d25de: movw r1,#0xb33
  000d25e2: movs r2,#0x11
  000d25e4: movs r3,#0x1b
  000d25e6: str r6,[sp,#0x0]
  000d25e8: blx 0x00074398
  000d25ec: b.w 0x000d3ad8
  000d25f0: movs r0,#0xc
  000d25f2: blx 0x0006eb24
  000d25f6: mov r5,r0
  000d25f8: blx 0x00074380
  000d25fc: movs r0,#0x6
  000d25fe: mov r1,r5
  000d2600: str r5,[r4,#0x0]
  000d2602: blx 0x0007438c
  000d2606: movs r0,#0x28
  000d2608: blx 0x0006eb24
  000d260c: mov r5,r0
  000d260e: mov.w r0,#0x1f40
  000d2612: movw r1,#0xb6c
  000d2616: str r0,[sp,#0x0]
  000d2618: mov r0,r5
  000d261a: movs r2,#0x0
  000d261c: movs r3,#0x5
  000d261e: blx 0x00074398
  000d2622: ldr r0,[r4,#0x0]
  000d2624: ldr r0,[r0,#0x4]
  000d2626: str r5,[r0,#0x0]
  000d2628: movs r0,#0x28
  000d262a: blx 0x0006eb24
  000d262e: mov r5,r0
  000d2630: movs r6,#0x1
  000d2632: movw r1,#0xb6d
  000d2636: movs r2,#0x0
  000d2638: movs r3,#0x1b
  000d263a: str r6,[sp,#0x0]
  000d263c: blx 0x00074398
  000d2640: ldr r0,[r4,#0x0]
  000d2642: ldr r0,[r0,#0x4]
  000d2644: str r5,[r0,#0x4]
  000d2646: movs r0,#0x28
  000d2648: blx 0x0006eb24
  000d264c: mov r5,r0
  000d264e: movw r1,#0xb6e
  000d2652: movs r2,#0x32
  000d2654: movs r3,#0x6
  000d2656: str r6,[sp,#0x0]
  000d2658: blx 0x00074398
  000d265c: ldr r0,[r4,#0x0]
  000d265e: ldr r0,[r0,#0x4]
  000d2660: str r5,[r0,#0x8]
  000d2662: movs r0,#0x28
  000d2664: blx 0x0006eb24
  000d2668: mov r5,r0
  000d266a: movs r6,#0x2
  000d266c: movw r1,#0xb6f
  000d2670: movs r2,#0x32
  000d2672: movs r3,#0x6
  000d2674: str r6,[sp,#0x0]
  000d2676: blx 0x00074398
  000d267a: ldr r0,[r4,#0x0]
  000d267c: ldr r0,[r0,#0x4]
  000d267e: str r5,[r0,#0xc]
  000d2680: movs r0,#0x28
  000d2682: blx 0x0006eb24
  000d2686: mov r5,r0
  000d2688: mov.w r1,#0xb70
  000d268c: movs r2,#0x0
  000d268e: movs r3,#0x1b
  000d2690: str r6,[sp,#0x0]
  000d2692: blx 0x00074398
  000d2696: ldr r0,[r4,#0x0]
  000d2698: ldr r0,[r0,#0x4]
  000d269a: str r5,[r0,#0x10]
  000d269c: movs r0,#0x28
  000d269e: blx 0x0006eb24
  000d26a2: mov r5,r0
  000d26a4: movs r0,#0x3
  000d26a6: movw r1,#0xb71
  000d26aa: str r0,[sp,#0x0]
  000d26ac: mov r0,r5
  000d26ae: movs r2,#0x0
  000d26b0: movs r3,#0x1b
  000d26b2: blx 0x00074398
  000d26b6: b.w 0x000d2f6e
  000d26ba: movs r0,#0xc
  000d26bc: blx 0x0006eb24
  000d26c0: mov r5,r0
  000d26c2: blx 0x00074380
  000d26c6: movs r0,#0x3
  000d26c8: mov r1,r5
  000d26ca: str r5,[r4,#0x0]
  000d26cc: blx 0x0007438c
  000d26d0: movs r0,#0x28
  000d26d2: blx 0x0006eb24
  000d26d6: mov r5,r0
  000d26d8: movw r0,#0x1b58
  000d26dc: movw r1,#0xb9c
  000d26e0: str r0,[sp,#0x0]
  000d26e2: mov r0,r5
  000d26e4: movs r2,#0x0
  000d26e6: movs r3,#0x5
  000d26e8: movs r6,#0x0
  000d26ea: blx 0x00074398
  000d26ee: ldr r0,[r4,#0x0]
  000d26f0: ldr r0,[r0,#0x4]
  000d26f2: str r5,[r0,#0x0]
  000d26f4: movs r0,#0x28
  000d26f6: blx 0x0006eb24
  000d26fa: mov r5,r0
  000d26fc: movw r1,#0xb9d
  000d2700: movs r2,#0x27
  000d2702: movs r3,#0x6
  000d2704: str r6,[sp,#0x0]
  000d2706: blx 0x00074398
  000d270a: ldr r0,[r4,#0x0]
  000d270c: ldr r0,[r0,#0x4]
  000d270e: str r5,[r0,#0x4]
  000d2710: movs r0,#0x28
  000d2712: blx 0x0006eb24
  000d2716: mov r5,r0
  000d2718: movs r0,#0x5
  000d271a: movw r1,#0xb9e
  000d271e: str r0,[sp,#0x0]
  000d2720: mov r0,r5
  000d2722: movs r2,#0x27
  000d2724: movs r3,#0x1b
  000d2726: blx 0x00074398
  000d272a: b.w 0x000d31a4
  000d272e: movs r0,#0xc
  000d2730: blx 0x0006eb24
  000d2734: mov r5,r0
  000d2736: blx 0x00074380
  000d273a: movs r0,#0x1
  000d273c: mov r1,r5
  000d273e: str r5,[r4,#0x0]
  000d2740: movs r6,#0x1
  000d2742: blx 0x0007438c
  000d2746: movs r0,#0x28
  000d2748: blx 0x0006eb24
  000d274c: mov r5,r0
  000d274e: movw r1,#0xc31
  000d2752: movs r2,#0x11
  000d2754: movs r3,#0x1b
  000d2756: str r6,[sp,#0x0]
  000d2758: blx 0x00074398
  000d275c: b.w 0x000d3ad8
  000d2760: movs r0,#0xc
  000d2762: blx 0x0006eb24
  000d2766: mov r5,r0
  000d2768: blx 0x00074380
  000d276c: movs r0,#0x5
  000d276e: mov r1,r5
  000d2770: str r5,[r4,#0x0]
  000d2772: blx 0x0007438c
  000d2776: movs r0,#0x28
  000d2778: blx 0x0006eb24
  000d277c: mov r5,r0
  000d277e: movw r0,#0x2710
  000d2782: movw r1,#0x72e
  000d2786: str r0,[sp,#0x0]
  000d2788: mov r0,r5
  000d278a: movs r2,#0x13
  000d278c: movs r3,#0x5
  000d278e: blx 0x00074398
  000d2792: ldr r0,[r4,#0x0]
  000d2794: ldr r0,[r0,#0x4]
  000d2796: str r5,[r0,#0x0]
  000d2798: movs r0,#0x28
  000d279a: blx 0x0006eb24
  000d279e: mov r5,r0
  000d27a0: movs r6,#0x0
  000d27a2: movw r1,#0x72f
  000d27a6: movs r2,#0x0
  000d27a8: movs r3,#0x6
  000d27aa: str r6,[sp,#0x0]
  000d27ac: blx 0x00074398
  000d27b0: ldr r0,[r4,#0x0]
  000d27b2: ldr r0,[r0,#0x4]
  000d27b4: str r5,[r0,#0x4]
  000d27b6: movs r0,#0x28
  000d27b8: blx 0x0006eb24
  000d27bc: mov r5,r0
  000d27be: movs r0,#0x3
  000d27c0: mov.w r1,#0x730
  000d27c4: strd r6,r0,[sp,#0x0]
  000d27c8: mov r0,r5
  000d27ca: movs r2,#0x0
  000d27cc: movs r3,#0x9
  000d27ce: blx 0x000743a4
  000d27d2: ldr r0,[r4,#0x0]
  000d27d4: ldr r0,[r0,#0x4]
  000d27d6: str r5,[r0,#0x8]
  000d27d8: movs r0,#0x28
  000d27da: blx 0x0006eb24
  000d27de: mov r5,r0
  000d27e0: movs r0,#0x2
  000d27e2: movw r1,#0x731
  000d27e6: str r0,[sp,#0x0]
  000d27e8: mov r0,r5
  000d27ea: movs r2,#0x1
  000d27ec: movs r3,#0x6
  000d27ee: blx 0x00074398
  000d27f2: ldr r0,[r4,#0x0]
  000d27f4: ldr r0,[r0,#0x4]
  000d27f6: str r5,[r0,#0xc]
  000d27f8: movs r0,#0x28
  000d27fa: blx 0x0006eb24
  000d27fe: mov r5,r0
  000d2800: movs r0,#0x3
  000d2802: movw r1,#0x732
  000d2806: str r0,[sp,#0x0]
  000d2808: mov r0,r5
  000d280a: movs r2,#0x0
  000d280c: movs r3,#0x6
  000d280e: blx 0x00074398
  000d2812: b.w 0x000d346e
  000d2816: movs r0,#0xc
  000d2818: blx 0x0006eb24
  000d281c: mov r5,r0
  000d281e: blx 0x00074380
  000d2822: movs r0,#0x5
  000d2824: mov r1,r5
  000d2826: str r5,[r4,#0x0]
  000d2828: blx 0x0007438c
  000d282c: movs r0,#0x28
  000d282e: blx 0x0006eb24
  000d2832: mov r5,r0
  000d2834: movs r6,#0x0
  000d2836: movw r1,#0x759
  000d283a: movs r2,#0xa
  000d283c: movs r3,#0x10
  000d283e: str r6,[sp,#0x0]
  000d2840: blx 0x00074398
  000d2844: ldr r0,[r4,#0x0]
  000d2846: ldr r0,[r0,#0x4]
  000d2848: str r5,[r0,#0x0]
  000d284a: movs r0,#0x28
  000d284c: blx 0x0006eb24
  000d2850: mov r5,r0
  000d2852: movw r1,#0x75a
  000d2856: movs r2,#0x0
  000d2858: movs r3,#0x6
  000d285a: str r6,[sp,#0x0]
  000d285c: blx 0x00074398
  000d2860: ldr r0,[r4,#0x0]
  000d2862: ldr r0,[r0,#0x4]
  000d2864: str r5,[r0,#0x4]
  000d2866: movs r0,#0x28
  000d2868: blx 0x0006eb24
  000d286c: mov r5,r0
  000d286e: movs r0,#0x2
  000d2870: movw r1,#0x75b
  000d2874: str r0,[sp,#0x0]
  000d2876: mov r0,r5
  000d2878: movs r2,#0xa
  000d287a: movs r3,#0x19
  000d287c: blx 0x00074398
  000d2880: ldr r0,[r4,#0x0]
  000d2882: ldr r0,[r0,#0x4]
  000d2884: str r5,[r0,#0x8]
  000d2886: movs r0,#0x28
  000d2888: blx 0x0006eb24
  000d288c: mov r5,r0
  000d288e: movs r6,#0x0
  000d2890: movw r1,#0x75c
  000d2894: movs r2,#0xe
  000d2896: movs r3,#0x8
  000d2898: str r6,[sp,#0x0]
  000d289a: blx 0x00074398
  000d289e: ldr r0,[r4,#0x0]
  000d28a0: ldr r0,[r0,#0x4]
  000d28a2: str r5,[r0,#0xc]
  000d28a4: movs r0,#0x28
  000d28a6: blx 0x0006eb24
  000d28aa: mov r5,r0
  000d28ac: movw r1,#0x75d
  000d28b0: movs r2,#0xe
  000d28b2: movs r3,#0x15
  000d28b4: str r6,[sp,#0x0]
  000d28b6: blx 0x00074398
  000d28ba: b.w 0x000d346e
  000d28be: movs r0,#0xc
  000d28c0: blx 0x0006eb24
  000d28c4: mov r5,r0
  000d28c6: blx 0x00074380
  000d28ca: movs r0,#0x6
  000d28cc: mov r1,r5
  000d28ce: str r5,[r4,#0x0]
  000d28d0: blx 0x0007438c
  000d28d4: movs r0,#0x28
  000d28d6: blx 0x0006eb24
  000d28da: mov r5,r0
  000d28dc: movs r6,#0x0
  000d28de: movw r1,#0x79c
  000d28e2: movs r2,#0x0
  000d28e4: movs r3,#0x17
  000d28e6: str r6,[sp,#0x0]
  000d28e8: blx 0x00074398
  000d28ec: ldr r0,[r4,#0x0]
  000d28ee: ldr r0,[r0,#0x4]
  000d28f0: str r5,[r0,#0x0]
  000d28f2: movs r0,#0x28
  000d28f4: blx 0x0006eb24
  000d28f8: mov r5,r0
  000d28fa: movw r1,#0x79d
  000d28fe: movs r2,#0x0
  000d2900: movs r3,#0x6
  000d2902: str r6,[sp,#0x0]
  000d2904: blx 0x00074398
  000d2908: ldr r0,[r4,#0x0]
  000d290a: ldr r0,[r0,#0x4]
  000d290c: str r5,[r0,#0x4]
  000d290e: movs r0,#0x28
  000d2910: blx 0x0006eb24
  000d2914: mov r5,r0
  000d2916: movs r0,#0x1
  000d2918: movw r1,#0x79e
  000d291c: str r0,[sp,#0x0]
  000d291e: mov r0,r5
  000d2920: movs r2,#0x0
  000d2922: movs r3,#0x6
  000d2924: blx 0x00074398
  000d2928: ldr r0,[r4,#0x0]
  000d292a: ldr r0,[r0,#0x4]
  000d292c: str r5,[r0,#0x8]
  000d292e: movs r0,#0x28
  000d2930: blx 0x0006eb24
  000d2934: mov r5,r0
  000d2936: movs r0,#0x2
  000d2938: movw r1,#0x79f
  000d293c: str r0,[sp,#0x0]
  000d293e: mov r0,r5
  000d2940: movs r2,#0x0
  000d2942: movs r3,#0x6
  000d2944: blx 0x00074398
  000d2948: ldr r0,[r4,#0x0]
  000d294a: ldr r0,[r0,#0x4]
  000d294c: str r5,[r0,#0xc]
  000d294e: movs r0,#0x28
  000d2950: blx 0x0006eb24
  000d2954: mov r5,r0
  000d2956: ldr r0,[0x000d2cd0]
  000d2958: str r0,[sp,#0x0]
  000d295a: mov r0,r5
  000d295c: mov.w r1,#0x7a0
  000d2960: movs r2,#0x13
  000d2962: movs r3,#0x5
  000d2964: blx 0x00074398
  000d2968: ldr r0,[r4,#0x0]
  000d296a: ldr r0,[r0,#0x4]
  000d296c: str r5,[r0,#0x10]
  000d296e: movs r0,#0x28
  000d2970: blx 0x0006eb24
  000d2974: mov r5,r0
  000d2976: movs r0,#0x4
  000d2978: movw r1,#0x7a1
  000d297c: str r0,[sp,#0x0]
  000d297e: mov r0,r5
  000d2980: movs r2,#0x0
  000d2982: movs r3,#0x6
  000d2984: blx 0x00074398
  000d2988: b 0x000d2f6e
  000d298a: movs r0,#0xc
  000d298c: blx 0x0006eb24
  000d2990: mov r5,r0
  000d2992: blx 0x00074380
  000d2996: movs r0,#0x4
  000d2998: mov r1,r5
  000d299a: str r5,[r4,#0x0]
  000d299c: blx 0x0007438c
  000d29a0: movs r0,#0x28
  000d29a2: blx 0x0006eb24
  000d29a6: mov r5,r0
  000d29a8: mov.w r0,#0x1f40
  000d29ac: movw r1,#0x849
  000d29b0: str r0,[sp,#0x0]
  000d29b2: mov r0,r5
  000d29b4: movs r2,#0x3f
  000d29b6: movs r3,#0x5
  000d29b8: blx 0x00074398
  000d29bc: ldr r0,[r4,#0x0]
  000d29be: ldr r0,[r0,#0x4]
  000d29c0: str r5,[r0,#0x0]
  000d29c2: movs r0,#0x28
  000d29c4: blx 0x0006eb24
  000d29c8: mov r5,r0
  000d29ca: movs r0,#0x0
  000d29cc: movw r1,#0x84a
  000d29d0: str r0,[sp,#0x0]
  000d29d2: mov r0,r5
  000d29d4: movs r2,#0x0
  000d29d6: movs r3,#0x6
  000d29d8: blx 0x00074398
  000d29dc: ldr r0,[r4,#0x0]
  000d29de: ldr r0,[r0,#0x4]
  000d29e0: str r5,[r0,#0x4]
  000d29e2: movs r0,#0x28
  000d29e4: blx 0x0006eb24
  000d29e8: mov r5,r0
  000d29ea: movs r0,#0x1
  000d29ec: movw r1,#0x84b
  000d29f0: str r0,[sp,#0x0]
  000d29f2: mov r0,r5
  000d29f4: movs r2,#0x3f
  000d29f6: movs r3,#0x6
  000d29f8: blx 0x00074398
  000d29fc: ldr r0,[r4,#0x0]
  000d29fe: ldr r0,[r0,#0x4]
  000d2a00: str r5,[r0,#0x8]
  000d2a02: movs r0,#0x28
  000d2a04: blx 0x0006eb24
  000d2a08: mov r5,r0
  000d2a0a: movs r0,#0x2
  000d2a0c: movw r1,#0x84c
  000d2a10: str r0,[sp,#0x0]
  000d2a12: mov r0,r5
  000d2a14: movs r2,#0x0
  000d2a16: movs r3,#0x6
  000d2a18: blx 0x00074398
  000d2a1c: b.w 0x000d3508
  000d2a20: movs r0,#0xc
  000d2a22: blx 0x0006eb24
  000d2a26: mov r5,r0
  000d2a28: blx 0x00074380
  000d2a2c: movs r0,#0x2
  000d2a2e: mov r1,r5
  000d2a30: str r5,[r4,#0x0]
  000d2a32: blx 0x0007438c
  000d2a36: movs r0,#0x28
  000d2a38: blx 0x0006eb24
  000d2a3c: mov r5,r0
  000d2a3e: mov.w r0,#0x1f40
  000d2a42: movw r1,#0x84d
  000d2a46: str r0,[sp,#0x0]
  000d2a48: mov r0,r5
  000d2a4a: movs r2,#0x3f
  000d2a4c: movs r3,#0x5
  000d2a4e: blx 0x00074398
  000d2a52: ldr r0,[r4,#0x0]
  000d2a54: ldr r0,[r0,#0x4]
  000d2a56: str r5,[r0,#0x0]
  000d2a58: movs r0,#0x28
  000d2a5a: blx 0x0006eb24
  000d2a5e: mov r5,r0
  000d2a60: movs r0,#0x0
  000d2a62: movw r1,#0x84e
  000d2a66: str r0,[sp,#0x0]
  000d2a68: mov r0,r5
  000d2a6a: movs r2,#0x0
  000d2a6c: movs r3,#0x6
  000d2a6e: blx 0x00074398
  000d2a72: b.w 0x000d3560
  000d2a76: movs r0,#0xc
  000d2a78: blx 0x0006eb24
  000d2a7c: mov r5,r0
  000d2a7e: blx 0x00074380
  000d2a82: movs r0,#0x2
  000d2a84: mov r1,r5
  000d2a86: str r5,[r4,#0x0]
  000d2a88: blx 0x0007438c
  000d2a8c: movs r0,#0x28
  000d2a8e: blx 0x0006eb24
  000d2a92: mov r5,r0
  000d2a94: mov.w r0,#0x1f40
  000d2a98: movw r1,#0x84f
  000d2a9c: str r0,[sp,#0x0]
  000d2a9e: mov r0,r5
  000d2aa0: movs r2,#0x3f
  000d2aa2: movs r3,#0x5
  000d2aa4: blx 0x00074398
  000d2aa8: ldr r0,[r4,#0x0]
  000d2aaa: ldr r0,[r0,#0x4]
  000d2aac: str r5,[r0,#0x0]
  000d2aae: movs r0,#0x28
  000d2ab0: blx 0x0006eb24
  000d2ab4: mov r5,r0
  000d2ab6: movs r0,#0x0
  000d2ab8: mov.w r1,#0x850
  000d2abc: str r0,[sp,#0x0]
  000d2abe: mov r0,r5
  000d2ac0: movs r2,#0x0
  000d2ac2: movs r3,#0x6
  000d2ac4: blx 0x00074398
  000d2ac8: b.w 0x000d3560
  000d2acc: movs r0,#0xc
  000d2ace: blx 0x0006eb24
  000d2ad2: mov r5,r0
  000d2ad4: blx 0x00074380
  000d2ad8: movs r0,#0x2
  000d2ada: mov r1,r5
  000d2adc: str r5,[r4,#0x0]
  000d2ade: blx 0x0007438c
  000d2ae2: movs r0,#0x28
  000d2ae4: blx 0x0006eb24
  000d2ae8: mov r5,r0
  000d2aea: mov.w r0,#0x1f40
  000d2aee: movw r1,#0x85a
  000d2af2: str r0,[sp,#0x0]
  000d2af4: mov r0,r5
  000d2af6: movs r2,#0x0
  000d2af8: movs r3,#0x5
  000d2afa: movs r6,#0x0
  000d2afc: blx 0x00074398
  000d2b00: ldr r0,[r4,#0x0]
  000d2b02: ldr r0,[r0,#0x4]
  000d2b04: str r5,[r0,#0x0]
  000d2b06: movs r0,#0x28
  000d2b08: blx 0x0006eb24
  000d2b0c: mov r5,r0
  000d2b0e: movw r1,#0x85b
  000d2b12: movs r2,#0x0
  000d2b14: movs r3,#0x6
  000d2b16: str r6,[sp,#0x0]
  000d2b18: blx 0x00074398
  000d2b1c: b.w 0x000d3560
  000d2b20: movs r0,#0xc
  000d2b22: blx 0x0006eb24
  000d2b26: mov r5,r0
  000d2b28: blx 0x00074380
  000d2b2c: movs r0,#0x3
  000d2b2e: mov r1,r5
  000d2b30: str r5,[r4,#0x0]
  000d2b32: blx 0x0007438c
  000d2b36: movs r0,#0x28
  000d2b38: blx 0x0006eb24
  000d2b3c: mov r5,r0
  000d2b3e: mov.w r0,#0x1f40
  000d2b42: movw r1,#0x8af
  000d2b46: str r0,[sp,#0x0]
  000d2b48: mov r0,r5
  000d2b4a: movs r2,#0x0
  000d2b4c: movs r3,#0x5
  000d2b4e: movs r6,#0x0
  000d2b50: blx 0x00074398
  000d2b54: ldr r0,[r4,#0x0]
  000d2b56: ldr r0,[r0,#0x4]
  000d2b58: str r5,[r0,#0x0]
  000d2b5a: movs r0,#0x28
  000d2b5c: blx 0x0006eb24
  000d2b60: mov r5,r0
  000d2b62: mov.w r1,#0x8b0
  000d2b66: movs r2,#0x0
  000d2b68: movs r3,#0x6
  000d2b6a: str r6,[sp,#0x0]
  000d2b6c: blx 0x00074398
  000d2b70: ldr r0,[r4,#0x0]
  000d2b72: ldr r0,[r0,#0x4]
  000d2b74: str r5,[r0,#0x4]
  000d2b76: movs r0,#0x28
  000d2b78: blx 0x0006eb24
  000d2b7c: mov r5,r0
  000d2b7e: movs r0,#0x1
  000d2b80: movw r1,#0x8b1
  000d2b84: str r0,[sp,#0x0]
  000d2b86: mov r0,r5
  000d2b88: movs r2,#0x1d
  000d2b8a: movs r3,#0x6
  000d2b8c: blx 0x00074398
  000d2b90: b 0x000d31a4
  000d2b92: movs r0,#0xc
  000d2b94: blx 0x0006eb24
  000d2b98: mov r5,r0
  000d2b9a: blx 0x00074380
  000d2b9e: movs r0,#0x3
  000d2ba0: mov r1,r5
  000d2ba2: str r5,[r4,#0x0]
  000d2ba4: blx 0x0007438c
  000d2ba8: movs r0,#0x28
  000d2baa: blx 0x0006eb24
  000d2bae: mov r5,r0
  000d2bb0: movw r0,#0x2ee0
  000d2bb4: movw r1,#0x8cb
  000d2bb8: str r0,[sp,#0x0]
  000d2bba: mov r0,r5
  000d2bbc: movs r2,#0x0
  000d2bbe: movs r3,#0x5
  000d2bc0: movs r6,#0x0
  000d2bc2: blx 0x00074398
  000d2bc6: ldr r0,[r4,#0x0]
  000d2bc8: ldr r0,[r0,#0x4]
  000d2bca: str r5,[r0,#0x0]
  000d2bcc: movs r0,#0x28
  000d2bce: blx 0x0006eb24
  000d2bd2: mov r5,r0
  000d2bd4: movw r1,#0x8cc
  000d2bd8: movs r2,#0x14
  000d2bda: movs r3,#0x6
  000d2bdc: str r6,[sp,#0x0]
  000d2bde: blx 0x00074398
  000d2be2: ldr r0,[r4,#0x0]
  000d2be4: ldr r0,[r0,#0x4]
  000d2be6: str r5,[r0,#0x4]
  000d2be8: movs r0,#0x28
  000d2bea: blx 0x0006eb24
  000d2bee: mov r5,r0
  000d2bf0: movs r0,#0x1
  000d2bf2: movw r1,#0x8cd
  000d2bf6: str r0,[sp,#0x0]
  000d2bf8: mov r0,r5
  000d2bfa: movs r2,#0x0
  000d2bfc: movs r3,#0x6
  000d2bfe: blx 0x00074398
  000d2c02: b 0x000d31a4
  000d2c04: movs r0,#0xc
  000d2c06: blx 0x0006eb24
  000d2c0a: mov r5,r0
  000d2c0c: blx 0x00074380
  000d2c10: movs r0,#0x4
  000d2c12: mov r1,r5
  000d2c14: str r5,[r4,#0x0]
  000d2c16: blx 0x0007438c
  000d2c1a: movs r0,#0x28
  000d2c1c: blx 0x0006eb24
  000d2c20: mov r5,r0
  000d2c22: mov.w r0,#0x7d0
  000d2c26: movw r1,#0x969
  000d2c2a: str r0,[sp,#0x0]
  000d2c2c: mov r0,r5
  000d2c2e: movs r2,#0x0
  000d2c30: movs r3,#0x5
  000d2c32: blx 0x00074398
  000d2c36: ldr r0,[r4,#0x0]
  000d2c38: ldr r0,[r0,#0x4]
  000d2c3a: str r5,[r0,#0x0]
  000d2c3c: movs r0,#0x28
  000d2c3e: blx 0x0006eb24
  000d2c42: mov r5,r0
  000d2c44: movs r0,#0x9
  000d2c46: movw r1,#0x96a
  000d2c4a: str r0,[sp,#0x0]
  000d2c4c: mov r0,r5
  000d2c4e: movs r2,#0x0
  000d2c50: movs r3,#0x1b
  000d2c52: blx 0x00074398
  000d2c56: ldr r0,[r4,#0x0]
  000d2c58: ldr r0,[r0,#0x4]
  000d2c5a: str r5,[r0,#0x4]
  000d2c5c: movs r0,#0x28
  000d2c5e: blx 0x0006eb24
  000d2c62: mov r5,r0
  000d2c64: movs r0,#0xb
  000d2c66: movw r1,#0x96b
  000d2c6a: str r0,[sp,#0x0]
  000d2c6c: mov r0,r5
  000d2c6e: movs r2,#0x0
  000d2c70: movs r3,#0x1b
  000d2c72: blx 0x00074398
  000d2c76: ldr r0,[r4,#0x0]
  000d2c78: ldr r0,[r0,#0x4]
  000d2c7a: str r5,[r0,#0x8]
  000d2c7c: movs r0,#0x28
  000d2c7e: blx 0x0006eb24
  000d2c82: mov r5,r0
  000d2c84: movs r0,#0x2
  000d2c86: movw r1,#0x96c
  000d2c8a: str r0,[sp,#0x0]
  000d2c8c: mov r0,r5
  000d2c8e: movs r2,#0xb
  000d2c90: movs r3,#0x6
  000d2c92: blx 0x00074398
  000d2c96: b.w 0x000d3508
  000d2c9a: movs r0,#0xc
  000d2c9c: blx 0x0006eb24
  000d2ca0: mov r5,r0
  000d2ca2: blx 0x00074380
  000d2ca6: movs r0,#0x1
  000d2ca8: mov r1,r5
  000d2caa: str r5,[r4,#0x0]
  000d2cac: blx 0x0007438c
  000d2cb0: movs r0,#0x28
  000d2cb2: blx 0x0006eb24
  000d2cb6: mov r5,r0
  000d2cb8: movw r0,#0x1770
  000d2cbc: movw r1,#0x96d
  000d2cc0: str r0,[sp,#0x0]
  000d2cc2: mov r0,r5
  000d2cc4: movs r2,#0x0
  000d2cc6: movs r3,#0x5
  000d2cc8: blx 0x00074398
  000d2ccc: b.w 0x000d3ad8
  000d2cd4: movs r0,#0xc
  000d2cd6: blx 0x0006eb24
  000d2cda: mov r5,r0
  000d2cdc: blx 0x00074380
  000d2ce0: movs r0,#0xc
  000d2ce2: mov r1,r5
  000d2ce4: str r5,[r4,#0x0]
  000d2ce6: blx 0x0007438c
  000d2cea: movs r0,#0x28
  000d2cec: blx 0x0006eb24
  000d2cf0: mov r5,r0
  000d2cf2: mov.w r0,#0x1f40
  000d2cf6: movw r1,#0x96e
  000d2cfa: str r0,[sp,#0x0]
  000d2cfc: mov r0,r5
  000d2cfe: movs r2,#0x0
  000d2d00: movs r3,#0x5
  000d2d02: blx 0x00074398
  000d2d06: ldr r0,[r4,#0x0]
  000d2d08: ldr r0,[r0,#0x4]
  000d2d0a: str r5,[r0,#0x0]
  000d2d0c: movs r0,#0x28
  000d2d0e: blx 0x0006eb24
  000d2d12: mov r5,r0
  000d2d14: movw r0,#0x61a8
  000d2d18: movw r1,#0x96f
  000d2d1c: str r0,[sp,#0x0]
  000d2d1e: mov r0,r5
  000d2d20: movs r2,#0x6
  000d2d22: movs r3,#0x5
  000d2d24: blx 0x00074398
  000d2d28: ldr r0,[r4,#0x0]
  000d2d2a: ldr r0,[r0,#0x4]
  000d2d2c: str r5,[r0,#0x4]
  000d2d2e: movs r0,#0x28
  000d2d30: blx 0x0006eb24
  000d2d34: mov r5,r0
  000d2d36: movs r0,#0x1
  000d2d38: mov.w r1,#0x970
  000d2d3c: str r0,[sp,#0x0]
  000d2d3e: mov r0,r5
  000d2d40: movs r2,#0x1a
  000d2d42: movs r3,#0x6
  000d2d44: blx 0x00074398
  000d2d48: ldr r0,[r4,#0x0]
  000d2d4a: ldr r0,[r0,#0x4]
  000d2d4c: str r5,[r0,#0x8]
  000d2d4e: movs r0,#0x28
  000d2d50: blx 0x0006eb24
  000d2d54: mov r5,r0
  000d2d56: movs r0,#0x2
  000d2d58: movw r1,#0x971
  000d2d5c: str r0,[sp,#0x0]
  000d2d5e: mov r0,r5
  000d2d60: movs r2,#0x0
  000d2d62: movs r3,#0x6
  000d2d64: blx 0x00074398
  000d2d68: ldr r0,[r4,#0x0]
  000d2d6a: ldr r0,[r0,#0x4]
  000d2d6c: str r5,[r0,#0xc]
  000d2d6e: movs r0,#0x28
  000d2d70: blx 0x0006eb24
  000d2d74: mov r5,r0
  000d2d76: movs r0,#0x3
  000d2d78: movw r1,#0x972
  000d2d7c: str r0,[sp,#0x0]
  000d2d7e: mov r0,r5
  000d2d80: movs r2,#0x6
  000d2d82: movs r3,#0x6
  000d2d84: blx 0x00074398
  000d2d88: ldr r0,[r4,#0x0]
  000d2d8a: ldr r0,[r0,#0x4]
  000d2d8c: str r5,[r0,#0x10]
  000d2d8e: movs r0,#0x28
  000d2d90: blx 0x0006eb24
  000d2d94: mov r5,r0
  000d2d96: movs r0,#0x4
  000d2d98: movw r1,#0x973
  000d2d9c: str r0,[sp,#0x0]
  000d2d9e: mov r0,r5
  000d2da0: movs r2,#0x1a
  000d2da2: movs r3,#0x6
  000d2da4: blx 0x00074398
  000d2da8: ldr r0,[r4,#0x0]
  000d2daa: ldr r0,[r0,#0x4]
  000d2dac: str r5,[r0,#0x14]
  000d2dae: movs r0,#0x28
  000d2db0: blx 0x0006eb24
  000d2db4: mov r5,r0
  000d2db6: movs r0,#0x5
  000d2db8: movw r1,#0x974
  000d2dbc: str r0,[sp,#0x0]
  000d2dbe: mov r0,r5
  000d2dc0: movs r2,#0x6
  000d2dc2: movs r3,#0x6
  000d2dc4: movs r6,#0x6
  000d2dc6: blx 0x00074398
  000d2dca: ldr r0,[r4,#0x0]
  000d2dcc: ldr r0,[r0,#0x4]
  000d2dce: str r5,[r0,#0x18]
  000d2dd0: movs r0,#0x28
  000d2dd2: blx 0x0006eb24
  000d2dd6: mov r5,r0
  000d2dd8: movw r1,#0x975
  000d2ddc: movs r2,#0x1a
  000d2dde: movs r3,#0x6
  000d2de0: str r6,[sp,#0x0]
  000d2de2: blx 0x00074398
  000d2de6: ldr r0,[r4,#0x0]
  000d2de8: ldr r0,[r0,#0x4]
  000d2dea: str r5,[r0,#0x1c]
  000d2dec: movs r0,#0x28
  000d2dee: blx 0x0006eb24
  000d2df2: mov r5,r0
  000d2df4: movs r0,#0x7
  000d2df6: movw r1,#0x976
  000d2dfa: str r0,[sp,#0x0]
  000d2dfc: mov r0,r5
  000d2dfe: movs r2,#0x0
  000d2e00: movs r3,#0x6
  000d2e02: blx 0x00074398
  000d2e06: ldr r0,[r4,#0x0]
  000d2e08: ldr r0,[r0,#0x4]
  000d2e0a: str r5,[r0,#0x20]
  000d2e0c: movs r0,#0x28
  000d2e0e: blx 0x0006eb24
  000d2e12: mov r5,r0
  000d2e14: movs r0,#0x12
  000d2e16: movs r1,#0x1
  000d2e18: strd r1,r0,[sp,#0x0]
  000d2e1c: mov r0,r5
  000d2e1e: movw r1,#0x977
  000d2e22: movs r2,#0x1a
  000d2e24: movs r3,#0x9
  000d2e26: blx 0x000743a4
  000d2e2a: ldr r0,[r4,#0x0]
  000d2e2c: ldr r0,[r0,#0x4]
  000d2e2e: str r5,[r0,#0x24]
  000d2e30: movs r0,#0x28
  000d2e32: blx 0x0006eb24
  000d2e36: mov r5,r0
  000d2e38: movs r0,#0xc
  000d2e3a: movw r1,#0x978
  000d2e3e: str r0,[sp,#0x0]
  000d2e40: mov r0,r5
  000d2e42: movs r2,#0x1a
  000d2e44: movs r3,#0x1b
  000d2e46: blx 0x00074398
  000d2e4a: ldr r0,[r4,#0x0]
  000d2e4c: ldr r0,[r0,#0x4]
  000d2e4e: str r5,[r0,#0x28]
  000d2e50: movs r0,#0x28
  000d2e52: blx 0x0006eb24
  000d2e56: mov r5,r0
  000d2e58: movs r0,#0xa
  000d2e5a: movw r1,#0x979
  000d2e5e: str r0,[sp,#0x0]
  000d2e60: mov r0,r5
  000d2e62: movs r2,#0x1a
  000d2e64: movs r3,#0x6
  000d2e66: blx 0x00074398
  000d2e6a: b.w 0x000d36f8
  000d2e6e: movs r0,#0xc
  000d2e70: blx 0x0006eb24
  000d2e74: mov r5,r0
  000d2e76: blx 0x00074380
  000d2e7a: movs r0,#0x1
  000d2e7c: mov r1,r5
  000d2e7e: str r5,[r4,#0x0]
  000d2e80: blx 0x0007438c
  000d2e84: movs r0,#0x28
  000d2e86: blx 0x0006eb24
  000d2e8a: mov r5,r0
  000d2e8c: mov.w r0,#0x7d0
  000d2e90: movw r1,#0x9be
  000d2e94: str r0,[sp,#0x0]
  000d2e96: mov r0,r5
  000d2e98: movs r2,#0x11
  000d2e9a: movs r3,#0x5
  000d2e9c: blx 0x00074398
  000d2ea0: b.w 0x000d3ad8
  000d2ea4: movs r0,#0xc
  000d2ea6: blx 0x0006eb24
  000d2eaa: mov r5,r0
  000d2eac: blx 0x00074380
  000d2eb0: movs r0,#0x6
  000d2eb2: mov r1,r5
  000d2eb4: str r5,[r4,#0x0]
  000d2eb6: blx 0x0007438c
  000d2eba: movs r0,#0x28
  000d2ebc: blx 0x0006eb24
  000d2ec0: mov r5,r0
  000d2ec2: movw r0,#0x5dc
  000d2ec6: movw r1,#0x9ed
  000d2eca: str r0,[sp,#0x0]
  000d2ecc: mov r0,r5
  000d2ece: movs r2,#0x0
  000d2ed0: movs r3,#0x5
  000d2ed2: movs r6,#0x0
  000d2ed4: blx 0x00074398
  000d2ed8: ldr r0,[r4,#0x0]
  000d2eda: ldr r0,[r0,#0x4]
  000d2edc: str r5,[r0,#0x0]
  000d2ede: movs r0,#0x28
  000d2ee0: blx 0x0006eb24
  000d2ee4: mov r5,r0
  000d2ee6: movw r1,#0x9ee
  000d2eea: movs r2,#0x3b
  000d2eec: movs r3,#0x6
  000d2eee: str r6,[sp,#0x0]
  000d2ef0: blx 0x00074398
  000d2ef4: ldr r0,[r4,#0x0]
  000d2ef6: ldr r0,[r0,#0x4]
  000d2ef8: str r5,[r0,#0x4]
  000d2efa: movs r0,#0x28
  000d2efc: blx 0x0006eb24
  000d2f00: mov r5,r0
  000d2f02: movs r6,#0x1
  000d2f04: movw r1,#0x9ef
  000d2f08: movs r2,#0x0
  000d2f0a: movs r3,#0x6
  000d2f0c: str r6,[sp,#0x0]
  000d2f0e: blx 0x00074398
  000d2f12: ldr r0,[r4,#0x0]
  000d2f14: ldr r0,[r0,#0x4]
  000d2f16: str r5,[r0,#0x8]
  000d2f18: movs r0,#0x28
  000d2f1a: blx 0x0006eb24
  000d2f1e: mov r5,r0
  000d2f20: mov.w r1,#0x9f0
  000d2f24: movs r2,#0x0
  000d2f26: movs r3,#0x1b
  000d2f28: str r6,[sp,#0x0]
  000d2f2a: blx 0x00074398
  000d2f2e: ldr r0,[r4,#0x0]
  000d2f30: ldr r0,[r0,#0x4]
  000d2f32: str r5,[r0,#0xc]
  000d2f34: movs r0,#0x28
  000d2f36: blx 0x0006eb24
  000d2f3a: mov r5,r0
  000d2f3c: movs r0,#0x3
  000d2f3e: movw r1,#0x9f1
  000d2f42: str r0,[sp,#0x0]
  000d2f44: mov r0,r5
  000d2f46: movs r2,#0x0
  000d2f48: movs r3,#0x6
  000d2f4a: blx 0x00074398
  000d2f4e: ldr r0,[r4,#0x0]
  000d2f50: ldr r0,[r0,#0x4]
  000d2f52: str r5,[r0,#0x10]
  000d2f54: movs r0,#0x28
  000d2f56: blx 0x0006eb24
  000d2f5a: mov r5,r0
  000d2f5c: movs r0,#0x4
  000d2f5e: movw r1,#0x9f2
  000d2f62: str r0,[sp,#0x0]
  000d2f64: mov r0,r5
  000d2f66: movs r2,#0x0
  000d2f68: movs r3,#0x6
  000d2f6a: blx 0x00074398
  000d2f6e: ldr r0,[r4,#0x0]
  000d2f70: ldr r0,[r0,#0x4]
  000d2f72: str r5,[r0,#0x14]
  000d2f74: b.w 0x000d3ade
  000d2f78: movs r0,#0xc
  000d2f7a: blx 0x0006eb24
  000d2f7e: mov r5,r0
  000d2f80: blx 0x00074380
  000d2f84: movs r0,#0x2
  000d2f86: mov r1,r5
  000d2f88: str r5,[r4,#0x0]
  000d2f8a: blx 0x0007438c
  000d2f8e: movs r0,#0x28
  000d2f90: blx 0x0006eb24
  000d2f94: mov r5,r0
  000d2f96: movw r0,#0x5dc
  000d2f9a: movw r1,#0xa35
  000d2f9e: str r0,[sp,#0x0]
  000d2fa0: mov r0,r5
  000d2fa2: movs r2,#0x0
  000d2fa4: movs r3,#0x5
  000d2fa6: movs r6,#0x0
  000d2fa8: blx 0x00074398
  000d2fac: ldr r0,[r4,#0x0]
  000d2fae: ldr r0,[r0,#0x4]
  000d2fb0: str r5,[r0,#0x0]
  000d2fb2: movs r0,#0x28
  000d2fb4: blx 0x0006eb24
  000d2fb8: mov r5,r0
  000d2fba: movw r1,#0xa36
  000d2fbe: movs r2,#0x0
  000d2fc0: movs r3,#0x6
  000d2fc2: str r6,[sp,#0x0]
  000d2fc4: blx 0x00074398
  000d2fc8: b 0x000d3560
  000d2fca: movs r0,#0xc
  000d2fcc: blx 0x0006eb24
  000d2fd0: mov r5,r0
  000d2fd2: blx 0x00074380
  000d2fd6: movs r0,#0x9
  000d2fd8: mov r1,r5
  000d2fda: str r5,[r4,#0x0]
  000d2fdc: blx 0x0007438c
  000d2fe0: movs r0,#0x28
  000d2fe2: blx 0x0006eb24
  000d2fe6: mov r5,r0
  000d2fe8: mov.w r0,#0x1f40
  000d2fec: movw r1,#0xa41
  000d2ff0: str r0,[sp,#0x0]
  000d2ff2: mov r0,r5
  000d2ff4: movs r2,#0x12
  000d2ff6: movs r3,#0x5
  000d2ff8: blx 0x00074398
  000d2ffc: ldr r0,[r4,#0x0]
  000d2ffe: ldr r0,[r0,#0x4]
  000d3000: str r5,[r0,#0x0]
  000d3002: movs r0,#0x28
  000d3004: blx 0x0006eb24
  000d3008: mov r5,r0
  000d300a: movs r0,#0x0
  000d300c: movw r1,#0xa42
  000d3010: str r0,[sp,#0x0]
  000d3012: mov r0,r5
  000d3014: movs r2,#0x12
  000d3016: movs r3,#0x6
  000d3018: blx 0x00074398
  000d301c: ldr r0,[r4,#0x0]
  000d301e: ldr r0,[r0,#0x4]
  000d3020: str r5,[r0,#0x4]
  000d3022: movs r0,#0x28
  000d3024: blx 0x0006eb24
  000d3028: mov r5,r0
  000d302a: movs r0,#0x1
  000d302c: movw r1,#0xa43
  000d3030: str r0,[sp,#0x0]
  000d3032: mov r0,r5
  000d3034: movs r2,#0x12
  000d3036: movs r3,#0x6
  000d3038: blx 0x00074398
  000d303c: ldr r0,[r4,#0x0]
  000d303e: ldr r0,[r0,#0x4]
  000d3040: str r5,[r0,#0x8]
  000d3042: movs r0,#0x28
  000d3044: blx 0x0006eb24
  000d3048: mov r5,r0
  000d304a: movs r0,#0x3
  000d304c: movw r1,#0xa44
  000d3050: str r0,[sp,#0x0]
  000d3052: mov r0,r5
  000d3054: movs r2,#0x12
  000d3056: movs r3,#0x1b
  000d3058: blx 0x00074398
  000d305c: ldr r0,[r4,#0x0]
  000d305e: ldr r0,[r0,#0x4]
  000d3060: str r5,[r0,#0xc]
  000d3062: movs r0,#0x28
  000d3064: blx 0x0006eb24
  000d3068: mov r5,r0
  000d306a: movs r0,#0x1
  000d306c: movw r1,#0xa45
  000d3070: str r0,[sp,#0x0]
  000d3072: mov r0,r5
  000d3074: movs r2,#0x0
  000d3076: movs r3,#0x1e
  000d3078: blx 0x00074398
  000d307c: ldr r0,[r4,#0x0]
  000d307e: ldr r0,[r0,#0x4]
  000d3080: str r5,[r0,#0x10]
  000d3082: movs r0,#0x28
  000d3084: blx 0x0006eb24
  000d3088: mov r5,r0
  000d308a: movs r0,#0x2
  000d308c: movw r1,#0xa46
  000d3090: str r0,[sp,#0x0]
  000d3092: mov r0,r5
  000d3094: movs r2,#0x0
  000d3096: movs r3,#0x1e
  000d3098: blx 0x00074398
  000d309c: ldr r0,[r4,#0x0]
  000d309e: ldr r0,[r0,#0x4]
  000d30a0: str r5,[r0,#0x14]
  000d30a2: movs r0,#0x28
  000d30a4: blx 0x0006eb24
  000d30a8: mov r5,r0
  000d30aa: movs r0,#0x3
  000d30ac: movw r1,#0xa47
  000d30b0: str r0,[sp,#0x0]
  000d30b2: mov r0,r5
  000d30b4: movs r2,#0x0
  000d30b6: movs r3,#0x1e
  000d30b8: blx 0x00074398
  000d30bc: ldr r0,[r4,#0x0]
  000d30be: ldr r0,[r0,#0x4]
  000d30c0: str r5,[r0,#0x18]
  000d30c2: movs r0,#0x28
  000d30c4: blx 0x0006eb24
  000d30c8: mov r5,r0
  000d30ca: movs r0,#0x5
  000d30cc: movw r1,#0xa48
  000d30d0: str r0,[sp,#0x0]
  000d30d2: mov r0,r5
  000d30d4: movs r2,#0x12
  000d30d6: movs r3,#0x1b
  000d30d8: blx 0x00074398
  000d30dc: ldr r0,[r4,#0x0]
  000d30de: ldr r0,[r0,#0x4]
  000d30e0: str r5,[r0,#0x1c]
  000d30e2: movs r0,#0x28
  000d30e4: blx 0x0006eb24
  000d30e8: mov r5,r0
  000d30ea: movs r0,#0x7
  000d30ec: movw r1,#0xa49
  000d30f0: str r0,[sp,#0x0]
  000d30f2: mov r0,r5
  000d30f4: movs r2,#0x0
  000d30f6: movs r3,#0x1b
  000d30f8: blx 0x00074398
  000d30fc: ldr r0,[r4,#0x0]
  000d30fe: ldr r0,[r0,#0x4]
  000d3100: str r5,[r0,#0x20]
  000d3102: b.w 0x000d3ade
  000d3106: movs r0,#0xc
  000d3108: blx 0x0006eb24
  000d310c: mov r5,r0
  000d310e: blx 0x00074380
  000d3112: movs r0,#0x1
  000d3114: mov r1,r5
  000d3116: str r5,[r4,#0x0]
  000d3118: movs r6,#0x1
  000d311a: blx 0x0007438c
  000d311e: movs r0,#0x28
  000d3120: blx 0x0006eb24
  000d3124: mov r5,r0
  000d3126: movw r1,#0xa7f
  000d312a: movs r2,#0x11
  000d312c: movs r3,#0x1b
  000d312e: str r6,[sp,#0x0]
  000d3130: blx 0x00074398
  000d3134: b.w 0x000d3ad8
  000d3138: movs r0,#0xc
  000d313a: blx 0x0006eb24
  000d313e: mov r5,r0
  000d3140: blx 0x00074380
  000d3144: movs r0,#0x3
  000d3146: mov r1,r5
  000d3148: str r5,[r4,#0x0]
  000d314a: blx 0x0007438c
  000d314e: movs r0,#0x28
  000d3150: blx 0x0006eb24
  000d3154: mov r5,r0
  000d3156: mov.w r0,#0x1f40
  000d315a: movw r1,#0xa94
  000d315e: str r0,[sp,#0x0]
  000d3160: mov r0,r5
  000d3162: movs r2,#0x0
  000d3164: movs r3,#0x5
  000d3166: blx 0x00074398
  000d316a: ldr r0,[r4,#0x0]
  000d316c: ldr r0,[r0,#0x4]
  000d316e: str r5,[r0,#0x0]
  000d3170: movs r0,#0x28
  000d3172: blx 0x0006eb24
  000d3176: mov r5,r0
  000d3178: movs r6,#0x1
  000d317a: movw r1,#0xa95
  000d317e: movs r2,#0xa
  000d3180: movs r3,#0x1b
  000d3182: str r6,[sp,#0x0]
  000d3184: blx 0x00074398
  000d3188: ldr r0,[r4,#0x0]
  000d318a: ldr r0,[r0,#0x4]
  000d318c: str r5,[r0,#0x4]
  000d318e: movs r0,#0x28
  000d3190: blx 0x0006eb24
  000d3194: mov r5,r0
  000d3196: movw r1,#0xa96
  000d319a: movs r2,#0x0
  000d319c: movs r3,#0x6
  000d319e: str r6,[sp,#0x0]
  000d31a0: blx 0x00074398
  000d31a4: ldr r0,[r4,#0x0]
  000d31a6: ldr r0,[r0,#0x4]
  000d31a8: str r5,[r0,#0x8]
  000d31aa: b.w 0x000d3ade
  000d31ae: movs r0,#0xc
  000d31b0: blx 0x0006eb24
  000d31b4: mov r5,r0
  000d31b6: blx 0x00074380
  000d31ba: movs r0,#0x1
  000d31bc: mov r1,r5
  000d31be: str r5,[r4,#0x0]
  000d31c0: movs r6,#0x1
  000d31c2: blx 0x0007438c
  000d31c6: movs r0,#0x28
  000d31c8: blx 0x0006eb24
  000d31cc: mov r5,r0
  000d31ce: movw r1,#0xab9
  000d31d2: movs r2,#0x11
  000d31d4: movs r3,#0x1b
  000d31d6: str r6,[sp,#0x0]
  000d31d8: blx 0x00074398
  000d31dc: b.w 0x000d3ad8
  000d31e0: movs r0,#0xc
  000d31e2: blx 0x0006eb24
  000d31e6: mov r5,r0
  000d31e8: blx 0x00074380
  000d31ec: movs r0,#0x1
  000d31ee: mov r1,r5
  000d31f0: str r5,[r4,#0x0]
  000d31f2: blx 0x0007438c
  000d31f6: movs r0,#0x28
  000d31f8: blx 0x0006eb24
  000d31fc: mov r5,r0
  000d31fe: movw r0,#0x5dc
  000d3202: movw r1,#0xac5
  000d3206: str r0,[sp,#0x0]
  000d3208: mov r0,r5
  000d320a: movs r2,#0x0
  000d320c: movs r3,#0x5
  000d320e: blx 0x00074398
  000d3212: b.w 0x000d3ad8
  000d3216: movs r0,#0xc
  000d3218: blx 0x0006eb24
  000d321c: mov r5,r0
  000d321e: blx 0x00074380
  000d3222: movs r0,#0x1
  000d3224: mov r1,r5
  000d3226: str r5,[r4,#0x0]
  000d3228: movs r6,#0x1
  000d322a: blx 0x0007438c
  000d322e: movs r0,#0x28
  000d3230: blx 0x0006eb24
  000d3234: mov r5,r0
  000d3236: movw r1,#0xb0c
  000d323a: movs r2,#0x11
  000d323c: movs r3,#0x1b
  000d323e: str r6,[sp,#0x0]
  000d3240: blx 0x00074398
  000d3244: b.w 0x000d3ad8
  000d3248: movs r0,#0xc
  000d324a: blx 0x0006eb24
  000d324e: mov r5,r0
  000d3250: blx 0x00074380
  000d3254: movs r0,#0x1
  000d3256: mov r1,r5
  000d3258: str r5,[r4,#0x0]
  000d325a: blx 0x0007438c
  000d325e: movs r0,#0x28
  000d3260: blx 0x0006eb24
  000d3264: mov r5,r0
  000d3266: movs r0,#0x2
  000d3268: movw r1,#0xb2b
  000d326c: str r0,[sp,#0x0]
  000d326e: mov r0,r5
  000d3270: movs r2,#0x0
  000d3272: movs r3,#0x1b
  000d3274: blx 0x00074398
  000d3278: b.w 0x000d3ad8
  000d327c: movs r0,#0xc
  000d327e: blx 0x0006eb24
  000d3282: mov r5,r0
  000d3284: blx 0x00074380
  000d3288: movs r0,#0x4
  000d328a: mov r1,r5
  000d328c: str r5,[r4,#0x0]
  000d328e: blx 0x0007438c
  000d3292: movs r0,#0x28
  000d3294: blx 0x0006eb24
  000d3298: mov r5,r0
  000d329a: movs r0,#0x1
  000d329c: movw r1,#0xb43
  000d32a0: str r0,[sp,#0x0]
  000d32a2: mov r0,r5
  000d32a4: movs r2,#0x31
  000d32a6: movs r3,#0x1b
  000d32a8: blx 0x00074398
  000d32ac: ldr r0,[r4,#0x0]
  000d32ae: ldr r0,[r0,#0x4]
  000d32b0: str r5,[r0,#0x0]
  000d32b2: movs r0,#0x28
  000d32b4: blx 0x0006eb24
  000d32b8: mov r5,r0
  000d32ba: movs r0,#0x0
  000d32bc: movw r1,#0xb44
  000d32c0: str r0,[sp,#0x0]
  000d32c2: mov r0,r5
  000d32c4: movs r2,#0x0
  000d32c6: movs r3,#0x6
  000d32c8: blx 0x00074398
  000d32cc: ldr r0,[r4,#0x0]
  000d32ce: ldr r0,[r0,#0x4]
  000d32d0: str r5,[r0,#0x4]
  000d32d2: movs r0,#0x28
  000d32d4: blx 0x0006eb24
  000d32d8: mov r5,r0
  000d32da: movs r0,#0x2
  000d32dc: movw r1,#0xb45
  000d32e0: str r0,[sp,#0x0]
  000d32e2: mov r0,r5
  000d32e4: movs r2,#0x0
  000d32e6: movs r3,#0x1b
  000d32e8: blx 0x00074398
  000d32ec: ldr r0,[r4,#0x0]
  000d32ee: ldr r0,[r0,#0x4]
  000d32f0: str r5,[r0,#0x8]
  000d32f2: movs r0,#0x28
  000d32f4: blx 0x0006eb24
  000d32f8: mov r5,r0
  000d32fa: movs r0,#0x3
  000d32fc: movw r1,#0xb46
  000d3300: str r0,[sp,#0x0]
  000d3302: mov r0,r5
  000d3304: movs r2,#0x0
  000d3306: movs r3,#0x1b
  000d3308: blx 0x00074398
  000d330c: b 0x000d3508
  000d330e: movs r0,#0xc
  000d3310: blx 0x0006eb24
  000d3314: mov r5,r0
  000d3316: blx 0x00074380
  000d331a: movs r0,#0x5
  000d331c: mov r1,r5
  000d331e: str r5,[r4,#0x0]
  000d3320: blx 0x0007438c
  000d3324: movs r0,#0x28
  000d3326: blx 0x0006eb24
  000d332a: mov r5,r0
  000d332c: movw r0,#0x5dc
  000d3330: movw r1,#0xb4f
  000d3334: str r0,[sp,#0x0]
  000d3336: mov r0,r5
  000d3338: movs r2,#0x32
  000d333a: movs r3,#0x5
  000d333c: blx 0x00074398
  000d3340: ldr r0,[r4,#0x0]
  000d3342: ldr r0,[r0,#0x4]
  000d3344: str r5,[r0,#0x0]
  000d3346: movs r0,#0x28
  000d3348: blx 0x0006eb24
  000d334c: mov r5,r0
  000d334e: movs r0,#0x0
  000d3350: mov.w r1,#0xb50
  000d3354: str r0,[sp,#0x0]
  000d3356: mov r0,r5
  000d3358: movs r2,#0x0
  000d335a: movs r3,#0x6
  000d335c: blx 0x00074398
  000d3360: ldr r0,[r4,#0x0]
  000d3362: ldr r0,[r0,#0x4]
  000d3364: str r5,[r0,#0x4]
  000d3366: movs r0,#0x28
  000d3368: blx 0x0006eb24
  000d336c: mov r5,r0
  000d336e: movs r0,#0x1
  000d3370: movw r1,#0xb51
  000d3374: str r0,[sp,#0x0]
  000d3376: mov r0,r5
  000d3378: movs r2,#0x32
  000d337a: movs r3,#0x6
  000d337c: blx 0x00074398
  000d3380: ldr r0,[r4,#0x0]
  000d3382: ldr r0,[r0,#0x4]
  000d3384: str r5,[r0,#0x8]
  000d3386: movs r0,#0x28
  000d3388: blx 0x0006eb24
  000d338c: mov r5,r0
  000d338e: movs r0,#0x2
  000d3390: movw r1,#0xb52
  000d3394: str r0,[sp,#0x0]
  000d3396: mov r0,r5
  000d3398: movs r2,#0x0
  000d339a: movs r3,#0x6
  000d339c: blx 0x00074398
  000d33a0: ldr r0,[r4,#0x0]
  000d33a2: ldr r0,[r0,#0x4]
  000d33a4: str r5,[r0,#0xc]
  000d33a6: movs r0,#0x28
  000d33a8: blx 0x0006eb24
  000d33ac: mov r5,r0
  000d33ae: movs r0,#0x3
  000d33b0: movw r1,#0xb53
  000d33b4: str r0,[sp,#0x0]
  000d33b6: mov r0,r5
  000d33b8: movs r2,#0x32
  000d33ba: movs r3,#0x6
  000d33bc: blx 0x00074398
  000d33c0: b 0x000d346e
  000d33c2: movs r0,#0xc
  000d33c4: blx 0x0006eb24
  000d33c8: mov r5,r0
  000d33ca: blx 0x00074380
  000d33ce: movs r0,#0x5
  000d33d0: mov r1,r5
  000d33d2: str r5,[r4,#0x0]
  000d33d4: blx 0x0007438c
  000d33d8: movs r0,#0x28
  000d33da: blx 0x0006eb24
  000d33de: mov r5,r0
  000d33e0: movw r0,#0x5dc
  000d33e4: movw r1,#0xb88
  000d33e8: str r0,[sp,#0x0]
  000d33ea: mov r0,r5
  000d33ec: movs r2,#0x2
  000d33ee: movs r3,#0x5
  000d33f0: blx 0x00074398
  000d33f4: ldr r0,[r4,#0x0]
  000d33f6: ldr r0,[r0,#0x4]
  000d33f8: str r5,[r0,#0x0]
  000d33fa: movs r0,#0x28
  000d33fc: blx 0x0006eb24
  000d3400: mov r5,r0
  000d3402: movs r6,#0x1
  000d3404: movw r1,#0xb89
  000d3408: movs r2,#0x2
  000d340a: movs r3,#0x1b
  000d340c: str r6,[sp,#0x0]
  000d340e: blx 0x00074398
  000d3412: ldr r0,[r4,#0x0]
  000d3414: ldr r0,[r0,#0x4]
  000d3416: str r5,[r0,#0x4]
  000d3418: movs r0,#0x28
  000d341a: blx 0x0006eb24
  000d341e: mov r5,r0
  000d3420: movw r1,#0xb8a
  000d3424: movs r2,#0x0
  000d3426: movs r3,#0x6
  000d3428: str r6,[sp,#0x0]
  000d342a: blx 0x00074398
  000d342e: ldr r0,[r4,#0x0]
  000d3430: ldr r0,[r0,#0x4]
  000d3432: str r5,[r0,#0x8]
  000d3434: movs r0,#0x28
  000d3436: blx 0x0006eb24
  000d343a: mov r5,r0
  000d343c: movs r0,#0x2
  000d343e: movw r1,#0xb8b
  000d3442: str r0,[sp,#0x0]
  000d3444: mov r0,r5
  000d3446: movs r2,#0x2
  000d3448: movs r3,#0x6
  000d344a: blx 0x00074398
  000d344e: ldr r0,[r4,#0x0]
  000d3450: ldr r0,[r0,#0x4]
  000d3452: str r5,[r0,#0xc]
  000d3454: movs r0,#0x28
  000d3456: blx 0x0006eb24
  000d345a: mov r5,r0
  000d345c: movs r0,#0x3
  000d345e: movw r1,#0xb8c
  000d3462: str r0,[sp,#0x0]
  000d3464: mov r0,r5
  000d3466: movs r2,#0x2
  000d3468: movs r3,#0x1b
  000d346a: blx 0x00074398
  000d346e: ldr r0,[r4,#0x0]
  000d3470: ldr r0,[r0,#0x4]
  000d3472: str r5,[r0,#0x10]
  000d3474: b 0x000d3ade
  000d3476: movs r0,#0xc
  000d3478: blx 0x0006eb24
  000d347c: mov r5,r0
  000d347e: blx 0x00074380
  000d3482: movs r0,#0x4
  000d3484: mov r1,r5
  000d3486: str r5,[r4,#0x0]
  000d3488: blx 0x0007438c
  000d348c: movs r0,#0x28
  000d348e: blx 0x0006eb24
  000d3492: mov r5,r0
  000d3494: movw r0,#0x1b58
  000d3498: movw r1,#0xb98
  000d349c: str r0,[sp,#0x0]
  000d349e: mov r0,r5
  000d34a0: movs r2,#0x27
  000d34a2: movs r3,#0x5
  000d34a4: blx 0x00074398
  000d34a8: ldr r0,[r4,#0x0]
  000d34aa: ldr r0,[r0,#0x4]
  000d34ac: str r5,[r0,#0x0]
  000d34ae: movs r0,#0x28
  000d34b0: blx 0x0006eb24
  000d34b4: mov r5,r0
  000d34b6: movs r0,#0x0
  000d34b8: movw r1,#0xb99
  000d34bc: str r0,[sp,#0x0]
  000d34be: mov r0,r5
  000d34c0: movs r2,#0x27
  000d34c2: movs r3,#0x6
  000d34c4: blx 0x00074398
  000d34c8: ldr r0,[r4,#0x0]
  000d34ca: ldr r0,[r0,#0x4]
  000d34cc: str r5,[r0,#0x4]
  000d34ce: movs r0,#0x28
  000d34d0: blx 0x0006eb24
  000d34d4: mov r5,r0
  000d34d6: movs r0,#0x1
  000d34d8: movw r1,#0xb9a
  000d34dc: str r0,[sp,#0x0]
  000d34de: mov r0,r5
  000d34e0: movs r2,#0x0
  000d34e2: movs r3,#0x6
  000d34e4: blx 0x00074398
  000d34e8: ldr r0,[r4,#0x0]
  000d34ea: ldr r0,[r0,#0x4]
  000d34ec: str r5,[r0,#0x8]
  000d34ee: movs r0,#0x28
  000d34f0: blx 0x0006eb24
  000d34f4: mov r5,r0
  000d34f6: movs r0,#0x2
  000d34f8: movw r1,#0xb9b
  000d34fc: str r0,[sp,#0x0]
  000d34fe: mov r0,r5
  000d3500: movs r2,#0x27
  000d3502: movs r3,#0x6
  000d3504: blx 0x00074398
  000d3508: ldr r0,[r4,#0x0]
  000d350a: ldr r0,[r0,#0x4]
  000d350c: str r5,[r0,#0xc]
  000d350e: b 0x000d3ade
  000d3510: movs r0,#0xc
  000d3512: blx 0x0006eb24
  000d3516: mov r5,r0
  000d3518: blx 0x00074380
  000d351c: movs r0,#0x2
  000d351e: mov r1,r5
  000d3520: str r5,[r4,#0x0]
  000d3522: blx 0x0007438c
  000d3526: movs r0,#0x28
  000d3528: blx 0x0006eb24
  000d352c: mov r5,r0
  000d352e: movw r0,#0x1b58
  000d3532: movw r1,#0xbac
  000d3536: str r0,[sp,#0x0]
  000d3538: mov r0,r5
  000d353a: movs r2,#0x0
  000d353c: movs r3,#0x5
  000d353e: movs r6,#0x0
  000d3540: blx 0x00074398
  000d3544: ldr r0,[r4,#0x0]
  000d3546: ldr r0,[r0,#0x4]
  000d3548: str r5,[r0,#0x0]
  000d354a: movs r0,#0x28
  000d354c: blx 0x0006eb24
  000d3550: mov r5,r0
  000d3552: movw r1,#0xbad
  000d3556: movs r2,#0x0
  000d3558: movs r3,#0x6
  000d355a: str r6,[sp,#0x0]
  000d355c: blx 0x00074398
  000d3560: ldr r0,[r4,#0x0]
  000d3562: ldr r0,[r0,#0x4]
  000d3564: str r5,[r0,#0x4]
  000d3566: b 0x000d3ade
  000d3568: movs r0,#0xc
  000d356a: blx 0x0006eb24
  000d356e: mov r5,r0
  000d3570: blx 0x00074380
  000d3574: movs r0,#0xc
  000d3576: mov r1,r5
  000d3578: str r5,[r4,#0x0]
  000d357a: blx 0x0007438c
  000d357e: movs r0,#0x28
  000d3580: blx 0x0006eb24
  000d3584: mov r5,r0
  000d3586: movw r0,#0x2328
  000d358a: movw r1,#0xbdf
  000d358e: str r0,[sp,#0x0]
  000d3590: mov r0,r5
  000d3592: movs r2,#0x0
  000d3594: movs r3,#0x5
  000d3596: movs r6,#0x0
  000d3598: blx 0x00074398
  000d359c: ldr r0,[r4,#0x0]
  000d359e: ldr r0,[r0,#0x4]
  000d35a0: str r5,[r0,#0x0]
  000d35a2: movs r0,#0x28
  000d35a4: blx 0x0006eb24
  000d35a8: mov r5,r0
  000d35aa: mov.w r1,#0xbe0
  000d35ae: movs r2,#0x1a
  000d35b0: movs r3,#0x6
  000d35b2: str r6,[sp,#0x0]
  000d35b4: blx 0x00074398
  000d35b8: ldr r0,[r4,#0x0]
  000d35ba: ldr r0,[r0,#0x4]
  000d35bc: str r5,[r0,#0x4]
  000d35be: movs r0,#0x28
  000d35c0: blx 0x0006eb24
  000d35c4: mov r5,r0
  000d35c6: movs r0,#0x1
  000d35c8: movw r1,#0xbe1
  000d35cc: str r0,[sp,#0x0]
  000d35ce: mov r0,r5
  000d35d0: movs r2,#0x0
  000d35d2: movs r3,#0x6
  000d35d4: blx 0x00074398
  000d35d8: ldr r0,[r4,#0x0]
  000d35da: ldr r0,[r0,#0x4]
  000d35dc: str r5,[r0,#0x8]
  000d35de: movs r0,#0x28
  000d35e0: blx 0x0006eb24
  000d35e4: mov r5,r0
  000d35e6: movs r0,#0x2
  000d35e8: movw r1,#0xbe2
  000d35ec: str r0,[sp,#0x0]
  000d35ee: mov r0,r5
  000d35f0: movs r2,#0x1a
  000d35f2: movs r3,#0x6
  000d35f4: blx 0x00074398
  000d35f8: ldr r0,[r4,#0x0]
  000d35fa: ldr r0,[r0,#0x4]
  000d35fc: str r5,[r0,#0xc]
  000d35fe: movs r0,#0x28
  000d3600: blx 0x0006eb24
  000d3604: mov r5,r0
  000d3606: movs r0,#0x3
  000d3608: movw r1,#0xbe3
  000d360c: str r0,[sp,#0x0]
  000d360e: mov r0,r5
  000d3610: movs r2,#0x0
  000d3612: movs r3,#0x6
  000d3614: blx 0x00074398
  000d3618: ldr r0,[r4,#0x0]
  000d361a: ldr r0,[r0,#0x4]
  000d361c: str r5,[r0,#0x10]
  000d361e: movs r0,#0x28
  000d3620: blx 0x0006eb24
  000d3624: mov r5,r0
  000d3626: movs r0,#0x4
  000d3628: movw r1,#0xbe4
  000d362c: str r0,[sp,#0x0]
  000d362e: mov r0,r5
  000d3630: movs r2,#0x1a
  000d3632: movs r3,#0x6
  000d3634: blx 0x00074398
  000d3638: ldr r0,[r4,#0x0]
  000d363a: ldr r0,[r0,#0x4]
  000d363c: str r5,[r0,#0x14]
  000d363e: movs r0,#0x28
  000d3640: blx 0x0006eb24
  000d3644: mov r5,r0
  000d3646: movs r0,#0x5
  000d3648: movw r1,#0xbe5
  000d364c: str r0,[sp,#0x0]
  000d364e: mov r0,r5
  000d3650: movs r2,#0x0
  000d3652: movs r3,#0x6
  000d3654: blx 0x00074398
  000d3658: ldr r0,[r4,#0x0]
  000d365a: ldr r0,[r0,#0x4]
  000d365c: str r5,[r0,#0x18]
  000d365e: movs r0,#0x28
  000d3660: blx 0x0006eb24
  000d3664: mov r5,r0
  000d3666: movs r0,#0x9
  000d3668: movw r1,#0xbe6
  000d366c: str r0,[sp,#0x0]
  000d366e: mov r0,r5
  000d3670: movs r2,#0x1a
  000d3672: movs r3,#0x1b
  000d3674: blx 0x00074398
  000d3678: ldr r0,[r4,#0x0]
  000d367a: ldr r0,[r0,#0x4]
  000d367c: str r5,[r0,#0x1c]
  000d367e: movs r0,#0x28
  000d3680: blx 0x0006eb24
  000d3684: mov r5,r0
  000d3686: movs r0,#0xa
  000d3688: movw r1,#0xbe7
  000d368c: str r0,[sp,#0x0]
  000d368e: mov r0,r5
  000d3690: movs r2,#0x0
  000d3692: movs r3,#0x1b
  000d3694: blx 0x00074398
  000d3698: ldr r0,[r4,#0x0]
  000d369a: ldr r0,[r0,#0x4]
  000d369c: str r5,[r0,#0x20]
  000d369e: movs r0,#0x28
  000d36a0: blx 0x0006eb24
  000d36a4: mov r5,r0
  000d36a6: movs r0,#0x8
  000d36a8: movw r1,#0xbe8
  000d36ac: str r0,[sp,#0x0]
  000d36ae: mov r0,r5
  000d36b0: movs r2,#0x37
  000d36b2: movs r3,#0x6
  000d36b4: blx 0x00074398
  000d36b8: ldr r0,[r4,#0x0]
  000d36ba: ldr r0,[r0,#0x4]
  000d36bc: str r5,[r0,#0x24]
  000d36be: movs r0,#0x28
  000d36c0: blx 0x0006eb24
  000d36c4: mov r5,r0
  000d36c6: movs r0,#0x9
  000d36c8: movw r1,#0xbe9
  000d36cc: str r0,[sp,#0x0]
  000d36ce: mov r0,r5
  000d36d0: movs r2,#0x0
  000d36d2: movs r3,#0x6
  000d36d4: blx 0x00074398
  000d36d8: ldr r0,[r4,#0x0]
  000d36da: ldr r0,[r0,#0x4]
  000d36dc: str r5,[r0,#0x28]
  000d36de: movs r0,#0x28
  000d36e0: blx 0x0006eb24
  000d36e4: mov r5,r0
  000d36e6: movs r0,#0xb
  000d36e8: movw r1,#0xbea
  000d36ec: str r0,[sp,#0x0]
  000d36ee: mov r0,r5
  000d36f0: movs r2,#0x0
  000d36f2: movs r3,#0x1b
  000d36f4: blx 0x00074398
  000d36f8: ldr r0,[r4,#0x0]
  000d36fa: ldr r0,[r0,#0x4]
  000d36fc: str r5,[r0,#0x2c]
  000d36fe: b 0x000d3ade
  000d3700: movs r0,#0xc
  000d3702: blx 0x0006eb24
  000d3706: mov r5,r0
  000d3708: blx 0x00074380
  000d370c: movs r0,#0x11
  000d370e: mov r1,r5
  000d3710: str r5,[r4,#0x0]
  000d3712: blx 0x0007438c
  000d3716: movs r0,#0x28
  000d3718: blx 0x0006eb24
  000d371c: mov r5,r0
  000d371e: movw r0,#0x1b58
  000d3722: movw r1,#0xc05
  000d3726: str r0,[sp,#0x0]
  000d3728: mov r0,r5
  000d372a: movs r2,#0x0
  000d372c: movs r3,#0x5
  000d372e: movs r6,#0x0
  000d3730: blx 0x00074398
  000d3734: ldr r0,[r4,#0x0]
  000d3736: ldr r0,[r0,#0x4]
  000d3738: str r5,[r0,#0x0]
  000d373a: movs r0,#0x28
  000d373c: blx 0x0006eb24
  000d3740: mov r5,r0
  000d3742: movw r1,#0xc06
  000d3746: movs r2,#0x26
  000d3748: movs r3,#0x6
  000d374a: str r6,[sp,#0x0]
  000d374c: blx 0x00074398
  000d3750: ldr r0,[r4,#0x0]
  000d3752: ldr r0,[r0,#0x4]
  000d3754: str r5,[r0,#0x4]
  000d3756: movs r0,#0x28
  000d3758: blx 0x0006eb24
  000d375c: mov r5,r0
  000d375e: movs r0,#0x1
  000d3760: movw r1,#0xc07
  000d3764: str r0,[sp,#0x0]
  000d3766: mov r0,r5
  000d3768: movs r2,#0x0
  000d376a: movs r3,#0x6
  000d376c: blx 0x00074398
  000d3770: ldr r0,[r4,#0x0]
  000d3772: ldr r0,[r0,#0x4]
  000d3774: str r5,[r0,#0x8]
  000d3776: movs r0,#0x28
  000d3778: blx 0x0006eb24
  000d377c: mov r5,r0
  000d377e: movs r0,#0x2
  000d3780: movw r1,#0xc08
  000d3784: str r0,[sp,#0x0]
  000d3786: mov r0,r5
  000d3788: movs r2,#0x27
  000d378a: movs r3,#0x6
  000d378c: blx 0x00074398
  000d3790: ldr r0,[r4,#0x0]
  000d3792: ldr r0,[r0,#0x4]
  000d3794: str r5,[r0,#0xc]
  000d3796: movs r0,#0x28
  000d3798: blx 0x0006eb24
  000d379c: mov r5,r0
  000d379e: movs r0,#0x5
  000d37a0: movw r1,#0xc09
  000d37a4: str r0,[sp,#0x0]
  000d37a6: mov r0,r5
  000d37a8: movs r2,#0x1
  000d37aa: movs r3,#0x1b
  000d37ac: blx 0x00074398
  000d37b0: ldr r0,[r4,#0x0]
  000d37b2: ldr r0,[r0,#0x4]
  000d37b4: str r5,[r0,#0x10]
  000d37b6: movs r0,#0x28
  000d37b8: blx 0x0006eb24
  000d37bc: mov r5,r0
  000d37be: movs r0,#0x4
  000d37c0: movw r1,#0xc0a
  000d37c4: str r0,[sp,#0x0]
  000d37c6: mov r0,r5
  000d37c8: movs r2,#0x0
  000d37ca: movs r3,#0x6
  000d37cc: blx 0x00074398
  000d37d0: ldr r0,[r4,#0x0]
  000d37d2: ldr r0,[r0,#0x4]
  000d37d4: str r5,[r0,#0x14]
  000d37d6: movs r0,#0x28
  000d37d8: blx 0x0006eb24
  000d37dc: mov r5,r0
  000d37de: movs r0,#0x15
  000d37e0: movw r1,#0xc0b
  000d37e4: str r0,[sp,#0x0]
  000d37e6: mov r0,r5
  000d37e8: movs r2,#0x6
  000d37ea: movs r3,#0xc
  000d37ec: movs r6,#0x6
  000d37ee: blx 0x00074398
  000d37f2: ldr r0,[r4,#0x0]
  000d37f4: ldr r0,[r0,#0x4]
  000d37f6: str r5,[r0,#0x18]
  000d37f8: movs r0,#0x28
  000d37fa: blx 0x0006eb24
  000d37fe: mov r5,r0
  000d3800: movw r1,#0xc0c
  000d3804: movs r2,#0x0
  000d3806: movs r3,#0x6
  000d3808: str r6,[sp,#0x0]
  000d380a: blx 0x00074398
  000d380e: ldr r0,[r4,#0x0]
  000d3810: ldr r0,[r0,#0x4]
  000d3812: str r5,[r0,#0x1c]
  000d3814: movs r0,#0x28
  000d3816: blx 0x0006eb24
  000d381a: mov r5,r0
  000d381c: movs r0,#0x6
  000d381e: movw r1,#0xc0d
  000d3822: str r0,[sp,#0x0]
  000d3824: mov r0,r5
  000d3826: movs r2,#0x6
  000d3828: movs r3,#0x1b
  000d382a: blx 0x00074398
  000d382e: ldr r0,[r4,#0x0]
  000d3830: ldr r0,[r0,#0x4]
  000d3832: str r5,[r0,#0x20]
  000d3834: movs r0,#0x28
  000d3836: blx 0x0006eb24
  000d383a: mov r5,r0
  000d383c: movs r0,#0x8
  000d383e: movw r1,#0xc0e
  000d3842: str r0,[sp,#0x0]
  000d3844: mov r0,r5
  000d3846: movs r2,#0x1a
  000d3848: movs r3,#0x6
  000d384a: blx 0x00074398
  000d384e: ldr r0,[r4,#0x0]
  000d3850: ldr r0,[r0,#0x4]
  000d3852: str r5,[r0,#0x24]
  000d3854: movs r0,#0x28
  000d3856: blx 0x0006eb24
  000d385a: mov r5,r0
  000d385c: movs r0,#0x9
  000d385e: movw r1,#0xc0f
  000d3862: str r0,[sp,#0x0]
  000d3864: mov r0,r5
  000d3866: movs r2,#0x6
  000d3868: movs r3,#0x6
  000d386a: blx 0x00074398
  000d386e: ldr r0,[r4,#0x0]
  000d3870: ldr r0,[r0,#0x4]
  000d3872: str r5,[r0,#0x28]
  000d3874: movs r0,#0x28
  000d3876: blx 0x0006eb24
  000d387a: mov r5,r0
  000d387c: movs r0,#0xa
  000d387e: mov.w r1,#0xc10
  000d3882: str r0,[sp,#0x0]
  000d3884: mov r0,r5
  000d3886: movs r2,#0x1
  000d3888: movs r3,#0x6
  000d388a: blx 0x00074398
  000d388e: ldr r0,[r4,#0x0]
  000d3890: ldr r0,[r0,#0x4]
  000d3892: str r5,[r0,#0x2c]
  000d3894: movs r0,#0x28
  000d3896: blx 0x0006eb24
  000d389a: mov r5,r0
  000d389c: movs r0,#0x8
  000d389e: movw r1,#0xc11
  000d38a2: str r0,[sp,#0x0]
  000d38a4: mov r0,r5
  000d38a6: movs r2,#0x27
  000d38a8: movs r3,#0x1b
  000d38aa: blx 0x00074398
  000d38ae: ldr r0,[r4,#0x0]
  000d38b0: ldr r0,[r0,#0x4]
  000d38b2: str r5,[r0,#0x30]
  000d38b4: movs r0,#0x28
  000d38b6: blx 0x0006eb24
  000d38ba: mov r5,r0
  000d38bc: movs r0,#0x9
  000d38be: movw r1,#0xc12
  000d38c2: str r0,[sp,#0x0]
  000d38c4: mov r0,r5
  000d38c6: movs r2,#0x0
  000d38c8: movs r3,#0x1b
  000d38ca: blx 0x00074398
  000d38ce: ldr r0,[r4,#0x0]
  000d38d0: ldr r0,[r0,#0x4]
  000d38d2: str r5,[r0,#0x34]
  000d38d4: movs r0,#0x28
  000d38d6: blx 0x0006eb24
  000d38da: mov r5,r0
  000d38dc: movs r0,#0xd
  000d38de: movw r1,#0xc13
  000d38e2: str r0,[sp,#0x0]
  000d38e4: mov r0,r5
  000d38e6: movs r2,#0x1a
  000d38e8: movs r3,#0x6
  000d38ea: blx 0x00074398
  000d38ee: ldr r0,[r4,#0x0]
  000d38f0: ldr r0,[r0,#0x4]
  000d38f2: str r5,[r0,#0x38]
  000d38f4: movs r0,#0x28
  000d38f6: blx 0x0006eb24
  000d38fa: mov r5,r0
  000d38fc: movs r0,#0xe
  000d38fe: movw r1,#0xc14
  000d3902: str r0,[sp,#0x0]
  000d3904: mov r0,r5
  000d3906: movs r2,#0x0
  000d3908: movs r3,#0x6
  000d390a: blx 0x00074398
  000d390e: ldr r0,[r4,#0x0]
  000d3910: ldr r0,[r0,#0x4]
  000d3912: str r5,[r0,#0x3c]
  000d3914: movs r0,#0x28
  000d3916: blx 0x0006eb24
  000d391a: mov r5,r0
  000d391c: movs r0,#0xf
  000d391e: movw r1,#0xc15
  000d3922: str r0,[sp,#0x0]
  000d3924: mov r0,r5
  000d3926: movs r2,#0x1a
  000d3928: movs r3,#0x6
  000d392a: blx 0x00074398
  000d392e: ldr r0,[r4,#0x0]
  000d3930: ldr r0,[r0,#0x4]
  000d3932: str r5,[r0,#0x40]
  000d3934: b 0x000d3ade
  000d3936: movs r0,#0xc
  000d3938: blx 0x0006eb24
  000d393c: mov r5,r0
  000d393e: blx 0x00074380
  000d3942: movs r0,#0xb
  000d3944: mov r1,r5
  000d3946: str r5,[r4,#0x0]
  000d3948: blx 0x0007438c
  000d394c: movs r0,#0x28
  000d394e: blx 0x0006eb24
  000d3952: mov r5,r0
  000d3954: movw r0,#0x2710
  000d3958: movw r1,#0xc16
  000d395c: str r0,[sp,#0x0]
  000d395e: mov r0,r5
  000d3960: movs r2,#0x0
  000d3962: movs r3,#0x5
  000d3964: movs r6,#0x0
  000d3966: blx 0x00074398
  000d396a: ldr r0,[r4,#0x0]
  000d396c: ldr r0,[r0,#0x4]
  000d396e: str r5,[r0,#0x0]
  000d3970: movs r0,#0x28
  000d3972: blx 0x0006eb24
  000d3976: mov r5,r0
  000d3978: movw r1,#0xc17
  000d397c: movs r2,#0x0
  000d397e: movs r3,#0x6
  000d3980: str r6,[sp,#0x0]
  000d3982: blx 0x00074398
  000d3986: ldr r0,[r4,#0x0]
  000d3988: ldr r0,[r0,#0x4]
  000d398a: str r5,[r0,#0x4]
  000d398c: movs r0,#0x28
  000d398e: blx 0x0006eb24
  000d3992: mov r5,r0
  000d3994: movs r0,#0x1
  000d3996: movw r1,#0xc18
  000d399a: str r0,[sp,#0x0]
  000d399c: mov r0,r5
  000d399e: movs r2,#0x27
  000d39a0: movs r3,#0x6
  000d39a2: blx 0x00074398
  000d39a6: ldr r0,[r4,#0x0]
  000d39a8: ldr r0,[r0,#0x4]
  000d39aa: str r5,[r0,#0x8]
  000d39ac: movs r0,#0x28
  000d39ae: blx 0x0006eb24
  000d39b2: mov r5,r0
  000d39b4: movs r0,#0x2
  000d39b6: movw r1,#0xc19
  000d39ba: str r0,[sp,#0x0]
  000d39bc: mov r0,r5
  000d39be: movs r2,#0x0
  000d39c0: movs r3,#0x6
  000d39c2: blx 0x00074398
  000d39c6: ldr r0,[r4,#0x0]
  000d39c8: ldr r0,[r0,#0x4]
  000d39ca: str r5,[r0,#0xc]
  000d39cc: movs r0,#0x28
  000d39ce: blx 0x0006eb24
  000d39d2: mov r5,r0
  000d39d4: movs r0,#0x3
  000d39d6: movw r1,#0xc1a
  000d39da: str r0,[sp,#0x0]
  000d39dc: mov r0,r5
  000d39de: movs r2,#0x27
  000d39e0: movs r3,#0x6
  000d39e2: blx 0x00074398
  000d39e6: ldr r0,[r4,#0x0]
  000d39e8: ldr r0,[r0,#0x4]
  000d39ea: str r5,[r0,#0x10]
  000d39ec: movs r0,#0x28
  000d39ee: blx 0x0006eb24
  000d39f2: mov r5,r0
  000d39f4: movs r0,#0x0
  000d39f6: movw r1,#0xc1b
  000d39fa: str r0,[sp,#0x0]
  000d39fc: mov r0,r5
  000d39fe: movs r2,#0x27
  000d3a00: movs r3,#0x1f
  000d3a02: blx 0x00074398
  000d3a06: ldr r0,[r4,#0x0]
  000d3a08: ldr r0,[r0,#0x4]
  000d3a0a: str r5,[r0,#0x14]
  000d3a0c: movs r0,#0x28
  000d3a0e: blx 0x0006eb24
  000d3a12: mov r5,r0
  000d3a14: movs r0,#0x5
  000d3a16: movw r1,#0xc1c
  000d3a1a: str r0,[sp,#0x0]
  000d3a1c: mov r0,r5
  000d3a1e: movs r2,#0x0
  000d3a20: movs r3,#0x6
  000d3a22: movs r6,#0x0
  000d3a24: blx 0x00074398
  000d3a28: ldr r0,[r4,#0x0]
  000d3a2a: ldr r0,[r0,#0x4]
  000d3a2c: str r5,[r0,#0x18]
  000d3a2e: movs r0,#0x28
  000d3a30: blx 0x0006eb24
  000d3a34: mov r5,r0
  000d3a36: movw r1,#0xc1d
  000d3a3a: movs r2,#0x27
  000d3a3c: movs r3,#0xc
  000d3a3e: str r6,[sp,#0x0]
  000d3a40: blx 0x00074398
  000d3a44: ldr r0,[r4,#0x0]
  000d3a46: ldr r0,[r0,#0x4]
  000d3a48: str r5,[r0,#0x1c]
  000d3a4a: movs r0,#0x28
  000d3a4c: blx 0x0006eb24
  000d3a50: mov r5,r0
  000d3a52: movs r0,#0x7
  000d3a54: movw r1,#0xc1e
  000d3a58: str r0,[sp,#0x0]
  000d3a5a: mov r0,r5
  000d3a5c: movs r2,#0x0
  000d3a5e: movs r3,#0x6
  000d3a60: movs r6,#0x0
  000d3a62: blx 0x00074398
  000d3a66: ldr r0,[r4,#0x0]
  000d3a68: ldr r0,[r0,#0x4]
  000d3a6a: str r5,[r0,#0x20]
  000d3a6c: movs r0,#0x28
  000d3a6e: blx 0x0006eb24
  000d3a72: mov r5,r0
  000d3a74: movw r1,#0xc1f
  000d3a78: movs r2,#0x27
  000d3a7a: movs r3,#0x13
  000d3a7c: str r6,[sp,#0x0]
  000d3a7e: blx 0x00074398
  000d3a82: ldr r0,[r4,#0x0]
  000d3a84: ldr r0,[r0,#0x4]
  000d3a86: str r5,[r0,#0x24]
  000d3a88: movs r0,#0x28
  000d3a8a: blx 0x0006eb24
  000d3a8e: mov r5,r0
  000d3a90: movs r0,#0x0
  000d3a92: mov.w r1,#0xc20
  000d3a96: str r0,[sp,#0x0]
  000d3a98: mov r0,r5
  000d3a9a: movs r2,#0x0
  000d3a9c: movs r3,#0x1
  000d3a9e: blx 0x00074398
  000d3aa2: ldr r0,[r4,#0x0]
  000d3aa4: ldr r0,[r0,#0x4]
  000d3aa6: str r5,[r0,#0x28]
  000d3aa8: b 0x000d3ade
  000d3aaa: movs r0,#0xc
  000d3aac: blx 0x0006eb24
  000d3ab0: mov r5,r0
  000d3ab2: blx 0x00074380
  000d3ab6: movs r0,#0x1
  000d3ab8: mov r1,r5
  000d3aba: str r5,[r4,#0x0]
  000d3abc: movs r6,#0x1
  000d3abe: blx 0x0007438c
  000d3ac2: movs r0,#0x28
  000d3ac4: blx 0x0006eb24
  000d3ac8: mov r5,r0
  000d3aca: movw r1,#0xc35
  000d3ace: movs r2,#0x11
  000d3ad0: movs r3,#0x1b
  000d3ad2: str r6,[sp,#0x0]
  000d3ad4: blx 0x00074398
  000d3ad8: ldr r0,[r4,#0x0]
  000d3ada: ldr r0,[r0,#0x4]
  000d3adc: str r5,[r0,#0x0]
  000d3ade: ldr r0,[r4,#0x0]
  000d3ae0: add sp,#0x8
  000d3ae2: pop.w r11
  000d3ae6: pop {r4,r5,r6,r7,pc}
```
