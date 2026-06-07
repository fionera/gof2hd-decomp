# ParticleSystemMesh::setParticle
elf 0x186820 body 80
Sig: undefined __stdcall setParticle(Vector * param_1, float param_2, uint param_3, float param_4, float param_5, float param_6, float param_7, bool param_8, float param_9, float param_10, Vector * param_11)

## decompile
```c

/* ParticleSystemMesh::setParticle(AbyssEngine::AEMath::Vector const&, float, unsigned int, float,
   float, float, float, bool, float, float, AbyssEngine::AEMath::Vector const&) */

void ParticleSystemMesh::setParticle
               (Vector *param_1,float param_2,uint param_3,float param_4,float param_5,float param_6
               ,float param_7,bool param_8,float param_9,float param_10,Vector *param_11)

{
  bool in_stack_00000000;
  float in_stack_00000004;
  float in_stack_00000008;
  float in_stack_0000000c;
  float in_stack_00000014;
  
  setParticle(param_1,in_stack_00000014,param_3,param_4,in_stack_0000000c,param_6,in_stack_00000008,
              param_8,param_9,in_stack_00000004,param_11,in_stack_00000000);
  return;
}

```

## target disasm
```
  00196820: push {r4,r6,r7,lr}
  00196822: add r7,sp,#0x8
  00196824: sub sp,#0x28
  00196826: vldr.32 s0,[r7,#0x1c]
  0019682a: movs r4,#0x0
  0019682c: vldr.32 s2,[r7,#0x14]
  00196830: vldr.32 s4,[r7,#0x10]
  00196834: vldr.32 s6,[r7,#0xc]
  00196838: vldr.32 s8,[r7,#0x8]
  0019683c: vldr.32 s10,[r7,#0x20]
  00196840: ldr.w lr,[r7,#0x24]
  00196844: ldr.w r12,[r7,#0x18]
  00196848: strd lr,r4,[sp,#0x1c]
  0019684c: vstr.32 s0,[sp,#0x14]
  00196850: vstr.32 s10,[sp,#0x18]
  00196854: str.w r12,[sp,#0x10]
  00196858: vstr.32 s8,[sp]
  0019685c: vstr.32 s6,[sp,#0x4]
  00196860: vstr.32 s4,[sp,#0x8]
  00196864: vstr.32 s2,[sp,#0xc]
  00196868: blx 0x00078b68
  0019686c: add sp,#0x28
  0019686e: pop {r4,r6,r7,pc}
```
