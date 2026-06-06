# Status::setStation
elf 0xa90b0 body 382
Sig: undefined __stdcall setStation(Station * param_1)

## decompile
```c

/* Status::setStation(Station*) */

void Status::setStation(Station *param_1)

{
  int iVar1;
  FileRead *this;
  Array *this_00;
  void *pvVar2;
  Array *pAVar3;
  String *this_01;
  undefined4 uVar4;
  uint *puVar5;
  Station *in_r1;
  Galaxy *this_02;
  int iVar6;
  Station *this_03;
  uint uVar7;
  int iVar8;
  uint uVar9;
  
  iVar1 = DAT_000b9248;
  if (*(Station **)(param_1 + 0x19c) != in_r1) {
    *(Station **)(param_1 + 0x19c) = in_r1;
    this_02 = (Galaxy *)**(undefined4 **)(iVar1 + 0xb90ce);
    iVar1 = Station::getSystem(in_r1);
    iVar1 = Galaxy::getSystem(this_02,iVar1);
    *(int *)(param_1 + 0x1a4) = iVar1;
    if (iVar1 != 0) {
      iVar6 = *(int *)(param_1 + 0xb4);
      iVar1 = Station::getSystem(*(Station **)(param_1 + 0x19c));
      *(undefined1 *)(*(int *)(iVar6 + 4) + iVar1) = 1;
      this = operator_new(1);
      FileRead::FileRead(this);
      this_00 = (Array *)FileRead::loadStationsBinary(this,*(SolarSystem **)(param_1 + 0x1a4));
      pvVar2 = (void *)FileRead::~FileRead(this);
      operator_delete(pvVar2);
      if (*(Array **)(param_1 + 0x1a8) != (Array *)0x0) {
        ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(param_1 + 0x1a8));
        if (*(Array<AbyssEngine::String*> **)(param_1 + 0x1a8) != (Array<AbyssEngine::String*> *)0x0
           ) {
          pvVar2 = (void *)Array<AbyssEngine::String*>::~Array
                                     (*(Array<AbyssEngine::String*> **)(param_1 + 0x1a8));
          operator_delete(pvVar2);
        }
      }
      *(undefined4 *)(param_1 + 0x1a8) = 0;
      pAVar3 = operator_new(0xc);
      Array<AbyssEngine::String*>::Array();
      *(Array **)(param_1 + 0x1a8) = pAVar3;
      ArraySetLength<AbyssEngine::String*>(*(uint *)this_00,pAVar3);
      if (*(Array **)(param_1 + 0x1ac) != (Array *)0x0) {
        ArrayRelease<int>(*(Array **)(param_1 + 0x1ac));
        if (*(Array<int> **)(param_1 + 0x1ac) != (Array<int> *)0x0) {
          pvVar2 = (void *)Array<int>::~Array(*(Array<int> **)(param_1 + 0x1ac));
          operator_delete(pvVar2);
        }
      }
      *(undefined4 *)(param_1 + 0x1ac) = 0;
      pAVar3 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(param_1 + 0x1ac) = pAVar3;
      ArraySetLength<int>(*(uint *)this_00,pAVar3);
      iVar1 = SolarSystem::getStations(*(SolarSystem **)(param_1 + 0x1a4));
      uVar9 = 0;
      do {
        puVar5 = (uint *)SolarSystem::getStations(*(SolarSystem **)(param_1 + 0x1a4));
        if (*puVar5 <= uVar9) {
          ArrayReleaseClasses<Station*>(this_00);
          Array<Station*>::~Array((Array<Station*> *)this_00);
          (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
          return;
        }
        uVar7 = 0;
        do {
          if (*(uint *)this_00 <= uVar7) goto LAB_000b91fe;
          this_03 = *(Station **)(*(int *)(this_00 + 4) + uVar7 * 4);
          iVar8 = *(int *)(*(int *)(iVar1 + 4) + uVar9 * 4);
          iVar6 = Station::getIndex(this_03);
          uVar7 = uVar7 + 1;
        } while (iVar8 != iVar6);
        iVar6 = *(int *)(param_1 + 0x1e8);
        this_01 = operator_new(0xc);
        if (iVar6 == 0) {
          AbyssEngine::String::String(this_01);
        }
        else {
          Station::getName();
        }
        *(String **)(*(int *)(*(int *)(param_1 + 0x1a8) + 4) + uVar9 * 4) = this_01;
        uVar4 = Station::getTextureIndex(this_03);
        *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x1ac) + 4) + uVar9 * 4) = uVar4;
LAB_000b91fe:
        uVar9 = uVar9 + 1;
      } while( true );
    }
  }
  return;
}

```
## target disasm
```
  000b90b0: push {r4,r5,r6,r7,lr}
  000b90b2: add r7,sp,#0xc
  000b90b4: push {r7,r8,r9,r10,r11}
  000b90b8: mov r9,r0
  000b90ba: ldr.w r0,[r0,#0x19c]
  000b90be: cmp r0,r1
  000b90c0: beq.w 0x000b9228
  000b90c4: ldr r0,[0x000b9248]
  000b90c6: str.w r1,[r9,#0x19c]
  000b90ca: add r0,pc
  000b90cc: ldr r0,[r0,#0x0]
  000b90ce: ldr r4,[r0,#0x0]
  000b90d0: mov r0,r1
  000b90d2: blx 0x0007189c
  000b90d6: mov r1,r0
  000b90d8: mov r0,r4
  000b90da: blx 0x00073684
  000b90de: cmp r0,#0x0
  000b90e0: str.w r0,[r9,#0x1a4]
  000b90e4: beq.w 0x000b9228
  000b90e8: ldr.w r0,[r9,#0x19c]
  000b90ec: ldr.w r4,[r9,#0xb4]
  000b90f0: blx 0x0007189c
  000b90f4: ldr r1,[r4,#0x4]
  000b90f6: movs r2,#0x1
  000b90f8: strb r2,[r1,r0]
  000b90fa: movs r0,#0x1
  000b90fc: blx 0x0006eb24
  000b9100: mov r6,r0
  000b9102: blx 0x00072124
  000b9106: ldr.w r1,[r9,#0x1a4]
  000b910a: mov r0,r6
  000b910c: blx 0x00073690
  000b9110: mov r11,r0
  000b9112: mov r0,r6
  000b9114: blx 0x0007213c
  000b9118: blx 0x0006eb48
  000b911c: ldr.w r0,[r9,#0x1a8]
  000b9120: cbz r0,0x000b9134
  000b9122: blx 0x0006facc
  000b9126: ldr.w r0,[r9,#0x1a8]
  000b912a: cbz r0,0x000b9134
  000b912c: blx 0x0006f64c
  000b9130: blx 0x0006eb48
  000b9134: movs r0,#0x0
  000b9136: str.w r0,[r9,#0x1a8]
  000b913a: movs r0,#0xc
  000b913c: blx 0x0006eb24
  000b9140: mov r6,r0
  000b9142: blx 0x0006f628
  000b9146: str.w r6,[r9,#0x1a8]
  000b914a: mov r1,r6
  000b914c: ldr.w r0,[r11,#0x0]
  000b9150: blx 0x0006fe08
  000b9154: ldr.w r0,[r9,#0x1ac]
  000b9158: cbz r0,0x000b916c
  000b915a: blx 0x0007369c
  000b915e: ldr.w r0,[r9,#0x1ac]
  000b9162: cbz r0,0x000b916c
  000b9164: blx 0x00070204
  000b9168: blx 0x0006eb48
  000b916c: movs r0,#0x0
  000b916e: str.w r0,[r9,#0x1ac]
  000b9172: movs r0,#0xc
  000b9174: blx 0x0006eb24
  000b9178: mov r6,r0
  000b917a: blx 0x000701f8
  000b917e: str.w r6,[r9,#0x1ac]
  000b9182: mov r1,r6
  000b9184: ldr.w r0,[r11,#0x0]
  000b9188: blx 0x00071a4c
  000b918c: ldr.w r0,[r9,#0x1a4]
  000b9190: blx 0x00071a88
  000b9194: mov r10,r0
  000b9196: mov.w r8,#0x0
  000b919a: b 0x000b9202
  000b919c: movs r5,#0x0
  000b919e: ldr.w r0,[r11,#0x0]
  000b91a2: cmp r5,r0
  000b91a4: bcs 0x000b91fe
  000b91a6: ldr.w r1,[r11,#0x4]
  000b91aa: ldr.w r0,[r10,#0x4]
  000b91ae: ldr.w r4,[r1,r5,lsl #0x2]
  000b91b2: ldr.w r6,[r0,r8,lsl #0x2]
  000b91b6: mov r0,r4
  000b91b8: blx 0x00071824
  000b91bc: adds r5,#0x1
  000b91be: cmp r6,r0
  000b91c0: bne 0x000b919e
  000b91c2: movs r0,#0xc
  000b91c4: mov r6,r9
  000b91c6: ldr.w r5,[r9,#0x1e8]
  000b91ca: blx 0x0006eb24
  000b91ce: mov r9,r0
  000b91d0: cbz r5,0x000b91dc
  000b91d2: mov r0,r9
  000b91d4: mov r1,r4
  000b91d6: blx 0x000736a8
  000b91da: b 0x000b91e2
  000b91dc: mov r0,r9
  000b91de: blx 0x0006efbc
  000b91e2: ldr.w r0,[r6,#0x1a8]
  000b91e6: ldr r0,[r0,#0x4]
  000b91e8: str.w r9,[r0,r8,lsl #0x2]
  000b91ec: mov r0,r4
  000b91ee: mov r9,r6
  000b91f0: blx 0x000736b4
  000b91f4: ldr.w r1,[r6,#0x1ac]
  000b91f8: ldr r1,[r1,#0x4]
  000b91fa: str.w r0,[r1,r8,lsl #0x2]
  000b91fe: add.w r8,r8,#0x1
  000b9202: ldr.w r0,[r9,#0x1a4]
  000b9206: blx 0x00071a88
  000b920a: ldr r0,[r0,#0x0]
  000b920c: cmp r8,r0
  000b920e: bcc 0x000b919c
  000b9210: mov r0,r11
  000b9212: blx 0x000735dc
  000b9216: mov r0,r11
  000b9218: blx 0x000735e8
  000b921c: pop.w {r3,r8,r9,r10,r11}
  000b9220: pop.w {r4,r5,r6,r7,lr}
  000b9224: b.w 0x001ab098
  000b9228: pop.w {r3,r8,r9,r10,r11}
  000b922c: pop {r4,r5,r6,r7,pc}
```
