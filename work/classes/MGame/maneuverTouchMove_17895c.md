# MGame::maneuverTouchMove
elf 0x17895c body 76
Sig: undefined __stdcall maneuverTouchMove(int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::maneuverTouchMove(int, int, void*) */

void MGame::maneuverTouchMove(int param_1,int param_2,void *param_3)

{
  int iVar1;
  uint in_fpscr;
  float fVar2;
  float fVar3;
  
  if (*(char *)(param_1 + 0x17c) != '\0') {
    fVar2 = (float)VectorSignedToFloat(**(undefined4 **)(DAT_001889b0 + 0x18896c),
                                       (byte)(in_fpscr >> 0x16) & 3);
    iVar1 = (int)param_3 - *(int *)(param_1 + 0x184);
    if (iVar1 < 0) {
      iVar1 = -iVar1;
    }
    fVar3 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    if ((int)((uint)((fVar2 / DAT_001889a8) * DAT_001889ac < fVar3) << 0x1f) < 0) {
      *(undefined4 *)(param_1 + 0x178) = 0;
      *(undefined1 *)(param_1 + 0x17c) = 0;
    }
  }
  return;
}

```

## target disasm
```
  0018895c: ldrb.w r1,[r0,#0x17c]
  00188960: cbz r1,0x001889a6
  00188962: ldr r1,[0x001889b0]
  00188964: vldr.32 s2,[pc,#0x40]
  00188968: add r1,pc
  0018896a: ldr r1,[r1,#0x0]
  0018896c: vldr.32 s0,[r1]
  00188970: vcvt.f32.s32 s0,s0
  00188974: ldr.w r1,[r0,#0x184]
  00188978: subs r1,r2,r1
  0018897a: it mi
  0018897c: rsb.mi r1,r1
  0018897e: vmov s4,r1
  00188982: vcvt.f32.s32 s4,s4
  00188986: vdiv.f32 s0,s0,s2
  0018898a: vldr.32 s2,[pc,#0x20]
  0018898e: vmul.f32 s0,s0,s2
  00188992: vcmpe.f32 s0,s4
  00188996: vmrs apsr,fpscr
  0018899a: ittt mi
  0018899c: mov.mi r1,#0x0
  0018899e: str.w.mi r1,[r0,#0x178]
  001889a2: strb.mi.w r1,[r0,#0x17c]
  001889a6: bx lr
```
