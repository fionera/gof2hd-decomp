# CheatCode::Update
elf 0x743f0 body 88
Sig: undefined __thiscall Update(CheatCode * this, ushort param_1)

## decompile
```c

/* AbyssEngine::CheatCode::Update(unsigned short) */

bool __thiscall AbyssEngine::CheatCode::Update(CheatCode *this,ushort param_1)

{
  bool bVar1;
  uint uVar2;
  uint uVar3;
  ushort *puVar4;
  uint uVar5;
  
  uVar3 = *(uint *)(this + 8);
  uVar5 = **(uint **)this;
  if (uVar5 <= uVar3) {
    return false;
  }
  puVar4 = (ushort *)(*(uint **)this)[1];
  if (puVar4[uVar3] != param_1) {
    *(undefined4 *)(this + 8) = 0;
    bVar1 = *puVar4 == param_1;
    uVar3 = (uint)bVar1;
    *(uint *)(this + 8) = (uint)(bVar1 && uVar5 != uVar3);
    return uVar5 == uVar3;
  }
  uVar3 = uVar3 + 1;
  uVar2 = uVar3;
  if (uVar3 == uVar5) {
    uVar2 = 0;
  }
  *(uint *)(this + 8) = uVar2;
  return uVar3 == uVar5;
}

```

## target disasm
```
  000843f0: push {r7,lr}
  000843f2: mov r7,sp
  000843f4: ldr r2,[r0,#0x0]
  000843f6: ldr r3,[r0,#0x8]
  000843f8: ldr.w lr,[r2,#0x0]
  000843fc: cmp r3,lr
  000843fe: itt cs
  00084400: mov.cs r0,#0x0
  00084402: pop.cs {r7,pc}
  00084404: ldr.w r12,[r2,#0x4]
  00084408: ldrh.w r2,[r12,r3,lsl #0x1]
  0008440c: cmp r2,r1
  0008440e: bne 0x00084424
  00084410: adds r1,r3,#0x1
  00084412: subs.w r2,r1,lr
  00084416: it eq
  00084418: mov.eq r1,r2
  0008441a: str r1,[r0,#0x8]
  0008441c: clz r0,r2
  00084420: lsrs r0,r0,#0x5
  00084422: pop {r7,pc}
  00084424: movs r2,#0x0
  00084426: str r2,[r0,#0x8]
  00084428: ldrh.w r2,[r12,#0x0]
  0008442c: subs r1,r2,r1
  0008442e: clz r1,r1
  00084432: lsrs r1,r1,#0x5
  00084434: subs.w r2,lr,r1
  00084438: clz r3,r2
  0008443c: it ne
  0008443e: mov.ne r2,#0x1
  00084440: ands r1,r2
  00084442: str r1,[r0,#0x8]
  00084444: lsrs r0,r3,#0x5
  00084446: pop {r7,pc}
```
