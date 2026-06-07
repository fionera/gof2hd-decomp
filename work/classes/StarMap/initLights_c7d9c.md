# StarMap::initLights
elf 0xc7d9c body 54
Sig: undefined __thiscall initLights(StarMap * this)

## decompile
```c

/* StarMap::initLights() */

void __thiscall StarMap::initLights(StarMap *this)

{
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  
  AbyssEngine::ApplicationManager::GetEngine();
  AbyssEngine::Engine::LightSetMaterialColorAmbient(extraout_s0,extraout_s1,extraout_s2);
  (*(code *)(DAT_001ac054 + 0x1ac058))(**(undefined4 **)(DAT_000d7dd4 + 0xd7dc6),0,1);
  return;
}

```

## target disasm
```
  000d7d9c: push {r7,lr}
  000d7d9e: mov r7,sp
  000d7da0: ldr r0,[0x000d7dd0]
  000d7da2: add r0,pc
  000d7da4: ldr r0,[r0,#0x0]
  000d7da6: ldr r0,[r0,#0x0]
  000d7da8: blx 0x0007258c
  000d7dac: mov.w r1,#0x3f000000
  000d7db0: mov.w r2,#0x3f000000
  000d7db4: mov.w r3,#0x3f000000
  000d7db8: blx 0x000745cc
  000d7dbc: ldr r0,[0x000d7dd4]
  000d7dbe: movs r1,#0x0
  000d7dc0: movs r2,#0x1
  000d7dc2: add r0,pc
  000d7dc4: ldr r0,[r0,#0x0]
  000d7dc6: ldr r0,[r0,#0x0]
  000d7dc8: pop.w {r7,lr}
  000d7dcc: b.w 0x001ac048
  001ac048: bx pc
```
