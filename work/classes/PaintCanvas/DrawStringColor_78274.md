# PaintCanvas::DrawStringColor
elf 0x78274 body 308
Sig: undefined __thiscall DrawStringColor(PaintCanvas * this, uint param_1, String * param_2, int param_3, int param_4, bool param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawStringColor(unsigned int, AbyssEngine::String const&, int, int,
   bool) */

void __thiscall
AbyssEngine::PaintCanvas::DrawStringColor
          (PaintCanvas *this,uint param_1,String *param_2,int param_3,int param_4,bool param_5)

{
  bool bVar1;
  Array *this_00;
  int iVar2;
  ushort *puVar3;
  int iVar4;
  undefined4 uVar5;
  void *pvVar6;
  int *piVar7;
  ImageFont *pIVar8;
  uint uVar9;
  undefined4 local_44;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar7 = *(int **)(DAT_000883c8 + 0x8828a);
  local_28 = *piVar7;
  if (param_1 < *(uint *)(this + 0x140)) {
    SetTexture((uint)this,*(uint *)(*(int *)(*(int *)(this + 0x144) + param_1 * 4) + 8));
    GetColor(this);
    String::String(aSStack_34,param_2,false);
    String::String(aSStack_40,(char *)(DAT_000883cc + 0x882c4),false);
    this_00 = (Array *)String::SplitTags(aSStack_34,aSStack_40);
    String::~String(aSStack_40);
    if (this_00 != (Array *)0x0) {
      bVar1 = true;
      iVar2 = DAT_000883d0 + 0x882ee;
      for (uVar9 = 0; uVar9 < *(uint *)this_00; uVar9 = uVar9 + 1) {
        if (bVar1) {
          pIVar8 = *(ImageFont **)(*(int *)(this + 0x144) + param_1 * 4);
          puVar3 = String::operator_cast_to_unsigned_short_
                             (*(String **)(*(int *)(this_00 + 4) + uVar9 * 4));
          ImageFontDrawString(pIVar8,puVar3,
                              *(uint *)(*(int *)(*(int *)(this_00 + 4) + uVar9 * 4) + 8),param_3,
                              param_4,this,*(Engine **)(this + 0x34),param_5);
          iVar4 = GetTextWidth((uint)this,(String *)param_1);
          param_3 = param_3 + iVar4;
        }
        else if (*(int *)(*(int *)(*(int *)(this_00 + 4) + uVar9 * 4) + 8) == 0) {
          SetColor((uint)this);
        }
        else {
          local_44 = 0;
          uVar5 = String::GetAEChar();
          sscanf(uVar5,iVar2,&local_44);
          SetColor((uint)this);
        }
        bVar1 = (bool)(bVar1 ^ 1);
      }
      SetColor((uint)this);
      ArrayReleaseClasses<AbyssEngine::String*>(this_00);
      pvVar6 = (void *)Array<AbyssEngine::String*>::~Array((Array<AbyssEngine::String*> *)this_00);
      operator_delete(pvVar6);
    }
    String::~String(aSStack_34);
  }
  if (*piVar7 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00088274: push {r4,r5,r6,r7,lr}
  00088276: add r7,sp,#0xc
  00088278: push {r8,r9,r10,r11}
  0008827c: sub sp,#0x3c
  0008827e: mov r4,r0
  00088280: ldr r0,[0x000883c8]
  00088282: mov r9,r3
  00088284: mov r5,r2
  00088286: add r0,pc
  00088288: mov r11,r1
  0008828a: ldr r6,[r0,#0x0]
  0008828c: ldr r0,[r6,#0x0]
  0008828e: str r0,[sp,#0x38]
  00088290: ldr.w r0,[r4,#0x140]
  00088294: cmp r0,r1
  00088296: bls 0x00088394
  00088298: ldr.w r0,[r4,#0x144]
  0008829c: mov.w r2,#0xffffffff
  000882a0: ldr.w r0,[r0,r11,lsl #0x2]
  000882a4: ldr r1,[r0,#0x8]
  000882a6: mov r0,r4
  000882a8: blx 0x0006fa6c
  000882ac: mov r0,r4
  000882ae: blx 0x0006fa90
  000882b2: str r0,[sp,#0x14]
  000882b4: add r0,sp,#0x2c
  000882b6: mov r1,r5
  000882b8: movs r2,#0x0
  000882ba: blx 0x0006f028
  000882be: ldr r1,[0x000883cc]
  000882c0: add r1,pc
  000882c2: add r0,sp,#0x20
  000882c4: movs r2,#0x0
  000882c6: blx 0x0006ee18
  000882ca: add r0,sp,#0x2c
  000882cc: add r1,sp,#0x20
  000882ce: blx 0x0006fa9c
  000882d2: mov r5,r0
  000882d4: add r0,sp,#0x20
  000882d6: blx 0x0006ee30
  000882da: cmp r5,#0x0
  000882dc: beq 0x0008838e
  000882de: str r6,[sp,#0x10]
  000882e0: mov.w r8,#0x1
  000882e4: ldr r0,[0x000883d0]
  000882e6: mov.w r10,#0x0
  000882ea: add r0,pc
  000882ec: str r0,[sp,#0x18]
  000882ee: b 0x0008836e
  000882f0: lsls.w r0,r8,#0x1f
  000882f4: beq 0x0008833a
  000882f6: ldr.w r0,[r4,#0x144]
  000882fa: ldr r1,[r5,#0x4]
  000882fc: ldr.w r6,[r0,r11,lsl #0x2]
  00088300: ldr.w r0,[r1,r10,lsl #0x2]
  00088304: blx 0x0006f3ac
  00088308: mov r1,r0
  0008830a: ldr r0,[r5,#0x4]
  0008830c: ldr r3,[r4,#0x34]
  0008830e: ldr.w r0,[r0,r10,lsl #0x2]
  00088312: ldr r2,[r0,#0x8]
  00088314: ldr r0,[r7,#0x8]
  00088316: strd r0,r4,[sp,#0x0]
  0008831a: ldr r0,[r7,#0xc]
  0008831c: str r3,[sp,#0x8]
  0008831e: mov r3,r9
  00088320: str r0,[sp,#0xc]
  00088322: mov r0,r6
  00088324: blx 0x0006f520
  00088328: ldr r0,[r5,#0x4]
  0008832a: ldr.w r2,[r0,r10,lsl #0x2]
  0008832e: mov r0,r4
  00088330: mov r1,r11
  00088332: blx 0x0006faa8
  00088336: add r9,r0
  00088338: b 0x00088366
  0008833a: ldr r0,[r5,#0x4]
  0008833c: ldr.w r0,[r0,r10,lsl #0x2]
  00088340: ldr r1,[r0,#0x8]
  00088342: cbz r1,0x0008835e
  00088344: movs r1,#0x0
  00088346: str r1,[sp,#0x1c]
  00088348: blx 0x0006ef68
  0008834c: ldr r1,[sp,#0x18]
  0008834e: add r2,sp,#0x1c
  00088350: blx 0x0006fab4
  00088354: ldr r1,[sp,#0x1c]
  00088356: mov r0,r4
  00088358: blx 0x0006fac0
  0008835c: b 0x00088366
  0008835e: ldr r1,[sp,#0x14]
  00088360: mov r0,r4
  00088362: blx 0x0006fac0
  00088366: add.w r10,r10,#0x1
  0008836a: eor r8,r8,#0x1
  0008836e: ldr r0,[r5,#0x0]
  00088370: cmp r10,r0
  00088372: bcc 0x000882f0
  00088374: ldr r1,[sp,#0x14]
  00088376: mov r0,r4
  00088378: blx 0x0006fac0
  0008837c: mov r0,r5
  0008837e: blx 0x0006facc
  00088382: mov r0,r5
  00088384: blx 0x0006f64c
  00088388: blx 0x0006eb48
  0008838c: ldr r6,[sp,#0x10]
  0008838e: add r0,sp,#0x2c
  00088390: blx 0x0006ee30
  00088394: ldr r0,[sp,#0x38]
  00088396: ldr r1,[r6,#0x0]
  00088398: subs r0,r1,r0
  0008839a: ittt eq
  0008839c: add.eq sp,#0x3c
  0008839e: pop.eq.w {r8,r9,r10,r11}
  000883a2: pop.eq {r4,r5,r6,r7,pc}
  000883a4: blx 0x0006e824
```
