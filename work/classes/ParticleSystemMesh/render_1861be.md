# ParticleSystemMesh::render
elf 0x1861be body 78
Sig: undefined __stdcall render(PaintCanvas * param_1, uint param_2, uint param_3, BlendMode param_4)

## decompile
```c

/* ParticleSystemMesh::render(AbyssEngine::PaintCanvas*, unsigned int, unsigned int,
   AbyssEngine::BlendMode) */

void ParticleSystemMesh::render(PaintCanvas *param_1,int param_2,uint param_3,undefined4 param_4)

{
  undefined4 uVar1;
  
  if (param_2 == -1) {
    return;
  }
  AbyssEngine::PaintCanvas::SetTexture((uint)param_1,param_3);
  AbyssEngine::PaintCanvas::SetBlendMode(param_1,param_4);
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  uVar1 = AbyssEngine::PaintCanvas::CameraGetLocal((uint)param_1);
  (*(code *)(DAT_001abda4 + 0x1abda8))(param_1,param_2,uVar1);
  return;
}

```

## target disasm
```
  001961be: push {r4,r5,r6,r7,lr}
  001961c0: add r7,sp,#0xc
  001961c2: push.w r11
  001961c6: mov r5,r0
  001961c8: mov r6,r3
  001961ca: mov r4,r1
  001961cc: adds r0,r1,#0x1
  001961ce: beq 0x00196204
  001961d0: mov r1,r2
  001961d2: mov r0,r5
  001961d4: mov.w r2,#0xffffffff
  001961d8: blx 0x0006fa6c
  001961dc: mov r0,r5
  001961de: mov r1,r6
  001961e0: blx 0x0006febc
  001961e4: mov r0,r5
  001961e6: blx 0x000717f4
  001961ea: mov r1,r0
  001961ec: mov r0,r5
  001961ee: blx 0x0006ff1c
  001961f2: mov r2,r0
  001961f4: mov r0,r5
  001961f6: mov r1,r4
  001961f8: pop.w r11
  001961fc: pop.w {r4,r5,r6,r7,lr}
  00196200: b.w 0x001abd98
  00196204: pop.w r11
  00196208: pop {r4,r5,r6,r7,pc}
  001abd98: bx pc
```
