# RocketGun::RocketGun
elf 0x15e8f8 body 180
Sig: undefined __thiscall RocketGun(RocketGun * this, int param_1, Gun * param_2, int param_3, int param_4, uint param_5, int param_6, bool param_7, Level * param_8)

## decompile
```c

/* RocketGun::RocketGun(int, Gun*, int, int, unsigned int, int, bool, Level*) */

RocketGun * __thiscall
RocketGun::RocketGun
          (RocketGun *this,int param_1,Gun *param_2,int param_3,int param_4,uint param_5,int param_6
          ,bool param_7,Level *param_8)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  AEGeometry *this_00;
  void *pvVar4;
  ushort uVar5;
  undefined4 *puVar6;
  
  ObjectGun::ObjectGun((ObjectGun *)this,param_1,param_2,param_3,param_5,param_8);
  uVar1 = DAT_0016e9c4;
  piVar2 = (int *)(DAT_0016e9c8 + 0x16e928);
  *(undefined4 *)(this + 0xd4) = 0;
  *(undefined4 *)(this + 0xd8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0xdc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xe0) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar3 = *piVar2;
  *(undefined4 *)(this + 0xb4) = 0;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  this[0xc0] = (RocketGun)param_7;
  *(undefined4 *)(this + 0xc4) = 0xffffffff;
  *(undefined4 *)(this + 200) = uVar1;
  *(undefined4 *)(this + 0xcc) = 0xffffffff;
  *(int *)(this + 0xd0) = param_6;
  *(undefined4 *)(this + 0xe4) = 0;
  *(int *)this = iVar3 + 8;
  *(undefined4 *)(this + 4) = 0xffffffff;
  iVar3 = 0x37a9;
  if (param_3 != 0x37a9) {
    iVar3 = 0x37a7;
  }
  if (param_3 == 0x37a9 || param_3 == iVar3) {
    this_00 = operator_new(0xc0);
    uVar5 = 0x37aa;
    puVar6 = *(undefined4 **)(DAT_0016e9cc + 0x16e978);
    if (param_1 == 0x37a7) {
      uVar5 = 0x37a8;
    }
    AEGeometry::AEGeometry(this_00,uVar5,(PaintCanvas *)*puVar6,false);
    AbyssEngine::PaintCanvas::TransformAddChild
              ((PaintCanvas *)*puVar6,*(uint *)(this + 0x10),*(uint *)(this_00 + 0xc));
    pvVar4 = (void *)AEGeometry::~AEGeometry(this_00);
    operator_delete(pvVar4);
  }
  return this;
}

```

## target disasm
```
  0016e8f8: push {r4,r5,r6,r7,lr}
  0016e8fa: add r7,sp,#0xc
  0016e8fc: push.w r8
  0016e900: sub sp,#0x8
  0016e902: mov r8,r1
  0016e904: ldr r1,[r7,#0x18]
  0016e906: mov r4,r0
  0016e908: ldr r0,[r7,#0xc]
  0016e90a: mov r6,r3
  0016e90c: strd r0,r1,[sp,#0x0]
  0016e910: mov r0,r4
  0016e912: mov r1,r8
  0016e914: blx 0x0007759c
  0016e918: ldr r0,[0x0016e9c8]
  0016e91a: vmov.i32 q8,#0x0
  0016e91e: add.w r5,r4,#0xd4
  0016e922: ldr r1,[0x0016e9c4]
  0016e924: add r0,pc
  0016e926: ldr.w r12,[r7,#0x10]
  0016e92a: vst1.32 {d16,d17},[r5]
  0016e92e: movs r5,#0x0
  0016e930: mov.w r3,#0xffffffff
  0016e934: ldr r0,[r0,#0x0]
  0016e936: ldr r2,[r7,#0x14]
  0016e938: adds r0,#0x8
  0016e93a: strd r5,r5,[r4,#0xb4]
  0016e93e: str.w r5,[r4,#0xbc]
  0016e942: strb.w r2,[r4,#0xc0]
  0016e946: strd r3,r1,[r4,#0xc4]
  0016e94a: strd r3,r12,[r4,#0xcc]
  0016e94e: str.w r5,[r4,#0xe4]
  0016e952: strd r0,r3,[r4,#0x0]
  0016e956: movw r0,#0x37a9
  0016e95a: cmp r6,r0
  0016e95c: itt ne
  0016e95e: movw.ne r0,#0x37a7
  0016e962: cmp.ne r6,r0
  0016e964: bne 0x0016e9a2
  0016e966: movs r0,#0xc0
  0016e968: blx 0x0006eb24
  0016e96c: mov r6,r0
  0016e96e: ldr r0,[0x0016e9cc]
  0016e970: movw r1,#0x37aa
  0016e974: add r0,pc
  0016e976: ldr r5,[r0,#0x0]
  0016e978: movw r0,#0x37a7
  0016e97c: cmp r8,r0
  0016e97e: ldr r2,[r5,#0x0]
  0016e980: it eq
  0016e982: movw.eq r1,#0x37a8
  0016e986: mov r0,r6
  0016e988: movs r3,#0x0
  0016e98a: blx 0x0007207c
  0016e98e: ldr r1,[r4,#0x10]
  0016e990: ldr r0,[r5,#0x0]
  0016e992: ldr r2,[r6,#0xc]
  0016e994: blx 0x000720d0
  0016e998: mov r0,r6
  0016e99a: blx 0x00071fc8
  0016e99e: blx 0x0006eb48
  0016e9a2: mov r0,r4
  0016e9a4: add sp,#0x8
  0016e9a6: pop.w r8
  0016e9aa: pop {r4,r5,r6,r7,pc}
```
