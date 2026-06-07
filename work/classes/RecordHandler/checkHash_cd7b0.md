# RecordHandler::checkHash
elf 0xcd7b0 body 250
Sig: undefined __stdcall checkHash(uint param_1)

## decompile
```c

/* RecordHandler::checkHash(unsigned int) */

void RecordHandler::checkHash(uint param_1)

{
  uchar *puVar1;
  int iVar2;
  uint uVar3;
  uchar *puVar4;
  Array<unsigned_char> *this;
  SHA256_CTX *c;
  uint in_r1;
  int iVar5;
  undefined4 in_r2;
  undefined4 in_r3;
  code *pcVar6;
  int *piVar7;
  undefined8 local_40;
  undefined4 uStack_38;
  undefined4 uStack_34;
  
  local_40 = CONCAT44(in_r1,param_1);
  piVar7 = *(int **)(DAT_000dd8b8 + 0xdd7c0);
  iVar2 = *piVar7;
  uStack_38 = in_r2;
  uStack_34 = in_r3;
  uVar3 = AEFile::GetFileSize(in_r1);
  if (-1 < (int)uVar3) {
    puVar4 = operator_new__(uVar3);
    AEFile::Read(uVar3,puVar4,in_r1);
    this = operator_new(0xc);
    Array<unsigned_char>::Array(this);
    ArrayAdd<unsigned_char>(puVar4,uVar3,(Array *)this);
    operator_delete__(puVar4);
    if (0x21 < *(uint *)this) {
      puVar4 = operator_new__(0x20);
      c = operator_new(0x70);
      SHA256_Init(c);
      pcVar6 = *(code **)(DAT_000dd8bc + 0xdd826);
      (*pcVar6)(c,*(undefined4 *)(this + 4),*(int *)this + -0x20);
      local_40 = *(undefined8 *)(DAT_000dd8c0 + 0xdd836);
      uStack_38._0_1_ = (undefined1)*(undefined8 *)(DAT_000dd8c0 + 0xdd83e);
      uStack_38._1_3_ = (undefined3)*(undefined8 *)(DAT_000dd8c0 + 0xdd83f);
      uStack_34 = (undefined4)((ulonglong)*(undefined8 *)(DAT_000dd8c0 + 0xdd83f) >> 0x18);
      (*pcVar6)(c,&local_40,0x19);
      (*pcVar6)(c,**(undefined4 **)(DAT_000dd8c4 + 0xdd854),0x10);
      SHA256_Final(puVar4,c);
      uVar3 = 0;
      do {
        if (0x1f < uVar3) break;
        puVar1 = puVar4 + uVar3;
        iVar5 = *(int *)this + *(int *)(this + 4) + uVar3;
        uVar3 = uVar3 + 1;
      } while (*puVar1 == *(uchar *)(iVar5 + -0x20));
      operator_delete__(puVar4);
      operator_delete(c);
    }
  }
  iVar2 = *piVar7 - iVar2;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  000dd7b0: push {r4,r5,r6,r7,lr}
  000dd7b2: add r7,sp,#0xc
  000dd7b4: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000dd7b8: ldr r0,[0x000dd8b8]
  000dd7ba: mov r4,r1
  000dd7bc: add r0,pc
  000dd7be: ldr.w r8,[r0,#0x0]
  000dd7c2: ldr.w r0,[r8,#0x0]
  000dd7c6: str r0,[sp,#0x1c]
  000dd7c8: mov r0,r1
  000dd7ca: blx 0x00071044
  000dd7ce: mov r5,r0
  000dd7d0: cmp r0,#0x0
  000dd7d2: blt 0x000dd880
  000dd7d4: mov r0,r5
  000dd7d6: blx 0x0006ec08
  000dd7da: mov r6,r0
  000dd7dc: mov r0,r5
  000dd7de: mov r1,r6
  000dd7e0: mov r2,r4
  000dd7e2: blx 0x0006eb90
  000dd7e6: movs r0,#0xc
  000dd7e8: blx 0x0006eb24
  000dd7ec: mov r4,r0
  000dd7ee: blx 0x0007492c
  000dd7f2: mov r0,r6
  000dd7f4: mov r1,r5
  000dd7f6: mov r2,r4
  000dd7f8: blx 0x00074938
  000dd7fc: mov r0,r6
  000dd7fe: blx 0x0006ebfc
  000dd802: ldr r0,[r4,#0x0]
  000dd804: cmp r0,#0x21
  000dd806: bls 0x000dd880
  000dd808: movs r0,#0x20
  000dd80a: blx 0x0006ec08
  000dd80e: mov r5,r0
  000dd810: movs r0,#0x70
  000dd812: blx 0x0006eb24
  000dd816: mov r9,r0
  000dd818: blx 0x00074944
  000dd81c: ldr r0,[0x000dd8bc]
  000dd81e: ldrd r2,r1,[r4,#0x0]
  000dd822: add r0,pc
  000dd824: subs r2,#0x20
  000dd826: ldr r6,[r0,#0x0]
  000dd828: mov r0,r9
  000dd82a: blx r6
  000dd82c: ldr r0,[0x000dd8c0]
  000dd82e: movs r2,#0x9
  000dd830: mov r1,sp
  000dd832: add r0,pc
  000dd834: vld1.64 {d16,d17},[r0],r2
  000dd838: vld1.8 {d18,d19},[r0]
  000dd83c: mov r0,r1
  000dd83e: vst1.64 {d16,d17},[r0],r2
  000dd842: movs r2,#0x19
  000dd844: vst1.8 {d18,d19},[r0]
  000dd848: mov r0,r9
  000dd84a: blx r6
  000dd84c: ldr r0,[0x000dd8c4]
  000dd84e: movs r2,#0x10
  000dd850: add r0,pc
  000dd852: ldr r0,[r0,#0x0]
  000dd854: ldr r1,[r0,#0x0]
  000dd856: mov r0,r9
  000dd858: blx r6
  000dd85a: mov r0,r5
  000dd85c: mov r1,r9
  000dd85e: blx 0x00074950
  000dd862: movs r0,#0x0
  000dd864: cmp r0,#0x1f
  000dd866: bhi 0x000dd884
  000dd868: ldrd r1,r2,[r4,#0x0]
  000dd86c: ldrb r3,[r5,r0]
  000dd86e: add r1,r2
  000dd870: add r1,r0
  000dd872: adds r0,#0x1
  000dd874: ldrb.w r1,[r1,#-0x20]
  000dd878: cmp r3,r1
  000dd87a: beq 0x000dd864
  000dd87c: movs r4,#0x0
  000dd87e: b 0x000dd886
  000dd880: movs r4,#0x0
  000dd882: b 0x000dd892
  000dd884: movs r4,#0x1
  000dd886: mov r0,r5
  000dd888: blx 0x0006ebfc
  000dd88c: mov r0,r9
  000dd88e: blx 0x0006eb48
  000dd892: ldr r0,[sp,#0x1c]
  000dd894: ldr.w r1,[r8,#0x0]
  000dd898: subs r0,r1,r0
  000dd89a: itttt eq
  000dd89c: mov.eq r0,r4
  000dd89e: add.eq sp,#0x20
  000dd8a0: pop.eq.w {r8,r9,r11}
  000dd8a4: pop.eq {r4,r5,r6,r7,pc}
  000dd8a6: blx 0x0006e824
```
