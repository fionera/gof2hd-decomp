# Level::Level
elf 0xadac8 body 258
Sig: undefined __thiscall Level(Level * this, int param_1)

## decompile
```c

/* Level::Level(int) */

Level * __thiscall Level::Level(Level *this,int param_1)

{
  undefined4 *puVar1;
  code *pcVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  
  puVar1 = (undefined4 *)(DAT_000bdbcc + 0xbdae0);
  *(undefined4 *)(this + 0xb4) = 0;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  pcVar2 = (code *)*puVar1;
  *(undefined4 *)(this + 200) = 0;
  *(undefined4 *)(this + 0xcc) = 0;
  *(undefined4 *)(this + 0xd0) = 0;
  *(undefined4 *)(this + 0x18c) = 0;
  *(undefined4 *)(this + 400) = 0;
  *(undefined4 *)(this + 0x194) = 0;
  (*pcVar2)(this + 0x1d0);
  (*pcVar2)(this + 0x20c);
  (*pcVar2)(this + 0x248);
  uVar3 = 0;
  uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar6 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = uVar4;
  *(undefined4 *)(this + 0x24) = uVar5;
  *(undefined4 *)(this + 0x28) = uVar6;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x94) = uVar4;
  *(undefined4 *)(this + 0x98) = uVar5;
  *(undefined4 *)(this + 0x9c) = uVar6;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = uVar4;
  *(undefined4 *)(this + 0x8c) = uVar5;
  *(undefined4 *)(this + 0x90) = uVar6;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = uVar4;
  *(undefined4 *)(this + 0x7c) = uVar5;
  *(undefined4 *)(this + 0x80) = uVar6;
  *(undefined4 *)(this + 4) = 0xffffffff;
  *(undefined4 *)(this + 8) = 0xffffffff;
  *(undefined4 *)(this + 0xc) = 0xffffffff;
  *(undefined4 *)(this + 0x10) = 0xffffffff;
  *(int *)(this + 0xc0) = param_1;
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 0xb0) = 0;
  *(undefined4 *)(this + 0x140) = 0;
  *(undefined4 *)(this + 0x144) = 0;
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x150) = 0;
  *(undefined4 *)(this + 0x154) = 0;
  this[0x158] = (Level)0x0;
  *(undefined4 *)(this + 0x2d) = 0;
  *(undefined4 *)(this + 0x29) = 0;
  __aeabi_memclr4(this + 0xd8,0x65);
  *(undefined4 *)(this + 0x16c) = uVar3;
  *(undefined4 *)(this + 0x170) = uVar4;
  *(undefined4 *)(this + 0x174) = uVar5;
  *(undefined4 *)(this + 0x178) = uVar6;
  *(undefined4 *)(this + 0x15c) = uVar3;
  *(undefined4 *)(this + 0x160) = uVar4;
  *(undefined4 *)(this + 0x164) = uVar5;
  *(undefined4 *)(this + 0x168) = uVar6;
  this[0x18a] = (Level)0x0;
  *(undefined2 *)(this + 0x188) = 0;
  *(undefined4 *)(this + 100) = 0xffffffff;
  *(undefined4 *)(this + 0x284) = 0xffffffff;
  *(undefined4 *)(this + 0x34) = 0xffffffff;
  *(undefined4 *)(this + 0x38) = 0xffffffff;
  *(undefined4 *)(this + 0x3c) = 0xffffffff;
  *(undefined4 *)(this + 0x48) = 0xffffffff;
  *(undefined4 *)(this + 0x4c) = 0xffffffff;
  *(undefined4 *)(this + 0x17c) = 0;
  *(undefined4 *)(this + 0x180) = 0;
  *(undefined4 *)(this + 0x184) = 0;
  this[0x69] = (Level)0x0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined2 *)(this + 0x288) = 0;
  *(undefined4 *)(this + 0xa0) = 0;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = 0xffffffff;
  *(undefined4 *)(this + 0x28c) = 0;
  *(undefined4 *)(this + 0x290) = 0;
  *(undefined4 *)(this + 0x294) = 0;
  *(undefined2 *)(this + 0x29c) = 0;
  this[0x29e] = (Level)0x0;
  *(undefined4 *)(this + 0x1b4) = 0xffffffff;
  *(undefined4 *)(this + 0x1b8) = 0xffffffff;
  *(undefined4 *)(this + 0x1bc) = 0xffffffff;
  *(undefined4 *)(this + 0x1c0) = 0xffffffff;
  return this;
}

```
## target disasm
```
  000bdac8: push {r4,r5,r6,r7,lr}
  000bdaca: add r7,sp,#0xc
  000bdacc: push.w r8
  000bdad0: vpush {d8,d9}
  000bdad4: mov r4,r0
  000bdad6: ldr r0,[0x000bdbcc]
  000bdad8: movs r6,#0x0
  000bdada: mov r8,r1
  000bdadc: add r0,pc
  000bdade: strd r6,r6,[r4,#0xb4]
  000bdae2: str.w r6,[r4,#0xbc]
  000bdae6: ldr r5,[r0,#0x0]
  000bdae8: add.w r0,r4,#0x1d0
  000bdaec: strd r6,r6,[r4,#0xc8]
  000bdaf0: str.w r6,[r4,#0xd0]
  000bdaf4: strd r6,r6,[r4,#0x18c]
  000bdaf8: str.w r6,[r4,#0x194]
  000bdafc: blx r5
  000bdafe: add.w r0,r4,#0x20c
  000bdb02: blx r5
  000bdb04: add.w r0,r4,#0x248
  000bdb08: blx r5
  000bdb0a: vmov.i32 q4,#0x0
  000bdb0e: add.w r0,r4,#0x1c
  000bdb12: mov.w r5,#0xffffffff
  000bdb16: movs r1,#0x65
  000bdb18: vst1.32 {d8,d9},[r0]
  000bdb1c: add.w r0,r4,#0x90
  000bdb20: vst1.32 {d8,d9},[r0]
  000bdb24: add.w r0,r4,#0x84
  000bdb28: vst1.32 {d8,d9},[r0]
  000bdb2c: add.w r0,r4,#0x74
  000bdb30: vst1.32 {d8,d9},[r0]
  000bdb34: add.w r0,r4,#0xd8
  000bdb38: strd r5,r5,[r4,#0x4]
  000bdb3c: strd r5,r5,[r4,#0xc]
  000bdb40: strd r8,r6,[r4,#0xc0]
  000bdb44: str.w r6,[r4,#0xb0]
  000bdb48: strd r6,r6,[r4,#0x140]
  000bdb4c: str.w r6,[r4,#0x148]
  000bdb50: strd r6,r6,[r4,#0x150]
  000bdb54: strb.w r6,[r4,#0x158]
  000bdb58: str.w r6,[r4,#0x2d]
  000bdb5c: str.w r6,[r4,#0x29]
  000bdb60: blx 0x0006ec14
  000bdb64: add.w r0,r4,#0x16c
  000bdb68: vst1.32 {d8,d9},[r0]
  000bdb6c: add.w r0,r4,#0x15c
  000bdb70: vst1.32 {d8,d9},[r0]
  000bdb74: mov r0,r4
  000bdb76: strb.w r6,[r4,#0x18a]
  000bdb7a: strh.w r6,[r4,#0x188]
  000bdb7e: str r5,[r4,#0x64]
  000bdb80: str.w r5,[r4,#0x284]
  000bdb84: strd r5,r5,[r4,#0x34]
  000bdb88: str r5,[r4,#0x3c]
  000bdb8a: strd r5,r5,[r4,#0x48]
  000bdb8e: strd r6,r6,[r4,#0x17c]
  000bdb92: str.w r6,[r4,#0x184]
  000bdb96: strb.w r6,[r4,#0x69]
  000bdb9a: str r6,[r4,#0x6c]
  000bdb9c: strh.w r6,[r4,#0x288]
  000bdba0: strd r6,r6,[r4,#0xa0]
  000bdba4: strd r6,r5,[r4,#0xa8]
  000bdba8: strd r6,r6,[r4,#0x28c]
  000bdbac: str.w r6,[r4,#0x294]
  000bdbb0: strh.w r6,[r4,#0x29c]
  000bdbb4: strb.w r6,[r4,#0x29e]
  000bdbb8: strd r5,r5,[r4,#0x1b4]
  000bdbbc: strd r5,r5,[r4,#0x1bc]
  000bdbc0: vpop {d8,d9}
  000bdbc4: pop.w r8
  000bdbc8: pop {r4,r5,r6,r7,pc}
```
