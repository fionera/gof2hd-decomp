# ParticleSystemMesh::setQuadEdge
elf 0x18608c body 246
Sig: undefined __stdcall setQuadEdge(Vector * param_1, int param_2, Vector * param_3)

## decompile
```c

/* ParticleSystemMesh::setQuadEdge(AbyssEngine::AEMath::Vector const&, int,
   AbyssEngine::AEMath::Vector const&) */

void ParticleSystemMesh::setQuadEdge(Vector *param_1,int param_2,Vector *param_3)

{
  code *pcVar1;
  int *piVar2;
  float extraout_s1;
  float extraout_s1_00;
  Vector local_40 [12];
  undefined4 local_34;
  float local_30;
  float local_2c;
  int local_28;
  
  piVar2 = *(int **)(DAT_00196184 + 0x1960a4);
  local_28 = *piVar2;
  AbyssEngine::AEMath::operator-((Vector *)&local_34,(Vector *)param_2);
  AbyssEngine::PaintCanvas::MeshSetPoint
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),local_30,extraout_s1,
             local_2c);
  AbyssEngine::AEMath::operator+(local_40,(Vector *)param_2);
  AbyssEngine::AEMath::Vector::operator=((Vector *)&local_34,local_40);
  if (param_1[0x74] == (Vector)0x0) {
    AbyssEngine::PaintCanvas::MeshSetPoint
              (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),local_30,
               extraout_s1_00,local_2c);
  }
  else {
    pcVar1 = *(code **)(DAT_00196188 + 0x196110);
    (*pcVar1)(*(uint *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),(uint)(param_3 + 1) & 0xffff,
              *(undefined4 *)param_2,*(undefined4 *)(param_2 + 4),*(undefined4 *)(param_2 + 8));
    (*pcVar1)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),
              (uint)(param_3 + 5) & 0xffff,*(undefined4 *)param_2,*(undefined4 *)(param_2 + 4),
              *(undefined4 *)(param_2 + 8));
    (*pcVar1)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),
              (uint)(param_3 + 4) & 0xffff,local_34,local_30,local_2c);
  }
  if (*piVar2 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0019608c: push {r4,r5,r6,r7,lr}
  0019608e: add r7,sp,#0xc
  00196090: push {r8,r9,r10,r11}
  00196094: sub sp,#0x24
  00196096: mov r5,r0
  00196098: ldr r0,[0x00196184]
  0019609a: add.w r9,sp,#0x14
  0019609e: mov r8,r2
  001960a0: add r0,pc
  001960a2: mov r2,r3
  001960a4: mov r4,r3
  001960a6: mov r6,r1
  001960a8: ldr.w r11,[r0,#0x0]
  001960ac: ldr.w r0,[r11,#0x0]
  001960b0: str r0,[sp,#0x20]
  001960b2: mov r0,r9
  001960b4: blx 0x0006ec38
  001960b8: ldr r0,[r5,#0x8]
  001960ba: uxth.w r2,r8
  001960be: ldr r1,[r5,#0x54]
  001960c0: ldr r3,[sp,#0x14]
  001960c2: vldr.32 s0,[sp,#0x18]
  001960c6: vldr.32 s2,[sp,#0x1c]
  001960ca: vstr.32 s0,[sp]
  001960ce: vstr.32 s2,[sp,#0x4]
  001960d2: blx 0x000777f4
  001960d6: add.w r10,sp,#0x8
  001960da: mov r1,r6
  001960dc: mov r2,r4
  001960de: mov r0,r10
  001960e0: blx 0x0006f1cc
  001960e4: mov r0,r9
  001960e6: mov r1,r10
  001960e8: blx 0x0006eb3c
  001960ec: ldrb.w r3,[r5,#0x74]
  001960f0: add.w r2,r8,#0x1
  001960f4: ldr r0,[r5,#0x8]
  001960f6: ldr r1,[r5,#0x54]
  001960f8: cbz r3,0x00196154
  001960fa: ldr r3,[r6,#0x0]
  001960fc: uxth r2,r2
  001960fe: vldr.32 s0,[r6,#0x4]
  00196102: vldr.32 s2,[r6,#0x8]
  00196106: ldr r4,[0x00196188]
  00196108: vstr.32 s0,[sp]
  0019610c: add r4,pc
  0019610e: vstr.32 s2,[sp,#0x4]
  00196112: ldr r4,[r4,#0x0]
  00196114: blx r4
  00196116: add.w r2,r8,#0x5
  0019611a: ldr r0,[r5,#0x8]
  0019611c: ldr r1,[r5,#0x54]
  0019611e: ldr r3,[r6,#0x0]
  00196120: uxth r2,r2
  00196122: vldr.32 s0,[r6,#0x4]
  00196126: vldr.32 s2,[r6,#0x8]
  0019612a: vstr.32 s0,[sp]
  0019612e: vstr.32 s2,[sp,#0x4]
  00196132: blx r4
  00196134: add.w r2,r8,#0x4
  00196138: ldr r0,[r5,#0x8]
  0019613a: ldr r1,[r5,#0x54]
  0019613c: ldr r3,[sp,#0x14]
  0019613e: uxth r2,r2
  00196140: vldr.32 s0,[sp,#0x18]
  00196144: vldr.32 s2,[sp,#0x1c]
  00196148: vstr.32 s0,[sp]
  0019614c: vstr.32 s2,[sp,#0x4]
  00196150: blx r4
  00196152: b 0x0019616c
  00196154: ldr r3,[sp,#0x14]
  00196156: uxth r2,r2
  00196158: vldr.32 s0,[sp,#0x18]
  0019615c: vldr.32 s2,[sp,#0x1c]
  00196160: vstr.32 s0,[sp]
  00196164: vstr.32 s2,[sp,#0x4]
  00196168: blx 0x000777f4
  0019616c: ldr r0,[sp,#0x20]
  0019616e: ldr.w r1,[r11,#0x0]
  00196172: subs r0,r1,r0
  00196174: ittt eq
  00196176: add.eq sp,#0x24
  00196178: pop.eq.w {r8,r9,r10,r11}
  0019617c: pop.eq {r4,r5,r6,r7,pc}
  0019617e: blx 0x0006e824
```
