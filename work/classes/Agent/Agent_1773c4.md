# Agent::Agent
elf 0x1773c4 body 198
Sig: undefined __thiscall Agent(Agent * this, int param_1, String param_2, int param_3, int param_4, int param_5, bool param_6, int param_7, int param_8, int param_9, int param_10)

## decompile
```c

/* Agent::Agent(int, AbyssEngine::String, int, int, int, bool, int, int, int, int) */

Agent * __thiscall
Agent::Agent(Agent *this,uint param_1,String *param_3,undefined4 param_4,undefined4 param_5,
            undefined4 param_6,Agent param_7,int param_8,int param_9,int param_10,
            undefined4 param_11)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = AbyssEngine::String::String((String *)this);
  AbyssEngine::String::String((String *)(iVar1 + 0x18));
  AbyssEngine::String::String((String *)(this + 0x6c));
  AbyssEngine::String::String((String *)(this + 0x78));
  *(uint *)(this + 0x40) = param_1;
  AbyssEngine::String::operator=((String *)this,param_3);
  *(undefined4 *)(this + 0x44) = param_4;
  *(undefined4 *)(this + 0x48) = param_5;
  *(undefined4 *)(this + 0x4c) = param_6;
  this[0x50] = param_7;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x30) = 0xffffffff;
  *(undefined4 *)(this + 0x5c) = 0xffffffff;
  *(int *)(this + 100) = param_8;
  if (-1 < param_8) {
    *(undefined4 *)(this + 0x5c) = 4;
  }
  *(int *)(this + 0x68) = param_9;
  if (-1 < param_9) {
    *(undefined4 *)(this + 0x5c) = 3;
  }
  *(undefined2 *)(this + 0x84) = 0;
  *(undefined2 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = param_11;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x28) = 0xffffffff;
  *(undefined4 *)(this + 0x2c) = 0xffffffff;
  *(uint *)(this + 0x58) = param_1 >> 0x1f;
  if (-1 < param_10) {
    *(undefined4 *)(this + 0x5c) = 8;
  }
  if (param_1 == 0x19) {
    uVar2 = 9;
  }
  else {
    if (param_1 != 0x1a) goto LAB_0018747e;
    uVar2 = 10;
  }
  *(undefined4 *)(this + 0x5c) = uVar2;
LAB_0018747e:
  *(int *)(this + 0x94) = param_10;
  return this;
}

```

## target disasm
```
  001873c4: push {r4,r5,r6,r7,lr}
  001873c6: add r7,sp,#0xc
  001873c8: push {r7,r8,r9,r10,r11}
  001873cc: mov r9,r3
  001873ce: mov r11,r2
  001873d0: mov r5,r1
  001873d2: mov r4,r0
  001873d4: blx 0x0006efbc
  001873d8: add.w r8,r0,#0x18
  001873dc: mov r0,r8
  001873de: blx 0x0006efbc
  001873e2: add.w r10,r4,#0x6c
  001873e6: mov r0,r10
  001873e8: blx 0x0006efbc
  001873ec: add.w r6,r4,#0x78
  001873f0: mov r0,r6
  001873f2: blx 0x0006efbc
  001873f6: str r5,[r4,#0x40]
  001873f8: mov r0,r4
  001873fa: mov r1,r11
  001873fc: blx 0x0006f2b0
  00187400: ldrd r0,r2,[r7,#0x10]
  00187404: cmp r2,#0x0
  00187406: ldrd r6,r3,[r7,#0x8]
  0018740a: ldr r1,[r7,#0x18]
  0018740c: strd r9,r6,[r4,#0x44]
  00187410: str r3,[r4,#0x4c]
  00187412: strb.w r0,[r4,#0x50]
  00187416: mov.w r0,#0x0
  0018741a: str r0,[r4,#0x54]
  0018741c: mov.w r0,#0xffffffff
  00187420: str r0,[r4,#0x30]
  00187422: str r0,[r4,#0x5c]
  00187424: str r2,[r4,#0x64]
  00187426: blt 0x0018742c
  00187428: movs r0,#0x4
  0018742a: str r0,[r4,#0x5c]
  0018742c: ldrd r0,r2,[r7,#0x1c]
  00187430: cmp r1,#0x0
  00187432: str r1,[r4,#0x68]
  00187434: blt 0x0018743a
  00187436: movs r1,#0x3
  00187438: str r1,[r4,#0x5c]
  0018743a: movs r1,#0x0
  0018743c: cmp r0,#0x0
  0018743e: strh.w r1,[r4,#0x84]
  00187442: strh r1,[r4,#0x24]
  00187444: strd r1,r1,[r4,#0xc]
  00187448: str r1,[r4,#0x14]
  0018744a: strd r1,r1,[r4,#0x34]
  0018744e: str r2,[r4,#0x3c]
  00187450: str r1,[r4,#0x60]
  00187452: strd r1,r1,[r4,#0x88]
  00187456: str.w r1,[r4,#0x90]
  0018745a: mov.w r1,#0xffffffff
  0018745e: strd r1,r1,[r4,#0x28]
  00187462: lsr.w r1,r5,#0x1f
  00187466: str r1,[r4,#0x58]
  00187468: blt 0x0018746e
  0018746a: movs r1,#0x8
  0018746c: str r1,[r4,#0x5c]
  0018746e: cmp r5,#0x19
  00187470: beq 0x0018747a
  00187472: cmp r5,#0x1a
  00187474: bne 0x0018747e
  00187476: movs r1,#0xa
  00187478: b 0x0018747c
  0018747a: movs r1,#0x9
  0018747c: str r1,[r4,#0x5c]
  0018747e: str.w r0,[r4,#0x94]
  00187482: mov r0,r4
  00187484: pop.w {r3,r8,r9,r10,r11}
  00187488: pop {r4,r5,r6,r7,pc}
```
