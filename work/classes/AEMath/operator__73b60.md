# AbyssEngine::AEMath::operator*
elf 0x73b60 body 163
Sig: undefined __stdcall operator*(Vector * param_1, float param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0017f402) overlaps instruction at (ram,0x0017f400)
    */
/* WARNING: Removing unreachable block (ram,0x000bf198) */
/* WARNING: Removing unreachable block (ram,0x00083b78) */
/* WARNING: Removing unreachable block (ram,0x00083b7c) */
/* WARNING: Removing unreachable block (ram,0x000bf184) */
/* WARNING: Removing unreachable block (ram,0x000bf188) */
/* WARNING: Removing unreachable block (ram,0x000bf18c) */
/* WARNING: Removing unreachable block (ram,0x000bf190) */
/* WARNING: Removing unreachable block (ram,0x000bf1a0) */
/* WARNING: Removing unreachable block (ram,0x00083b80) */
/* WARNING: Removing unreachable block (ram,0x000ff188) */
/* WARNING: Removing unreachable block (ram,0x0013f18c) */
/* WARNING: Removing unreachable block (ram,0x0013f190) */
/* WARNING: Removing unreachable block (ram,0x00083b84) */
/* WARNING: Removing unreachable block (ram,0x00083b88) */
/* WARNING: Removing unreachable block (ram,0x00083bb0) */
/* WARNING: Removing unreachable block (ram,0x00083bb4) */
/* WARNING: Removing unreachable block (ram,0x00083bb8) */
/* WARNING: Removing unreachable block (ram,0x000bf204) */
/* WARNING: Removing unreachable block (ram,0x000bf208) */
/* WARNING: Removing unreachable block (ram,0x000bf20c) */
/* WARNING: Removing unreachable block (ram,0x000bf1e0) */
/* WARNING: Removing unreachable block (ram,0x00083c0c) */
/* WARNING: Removing unreachable block (ram,0x00083be0) */
/* WARNING: Removing unreachable block (ram,0x00083be4) */
/* WARNING: Removing unreachable block (ram,0x00083c10) */
/* WARNING: Removing unreachable block (ram,0x000bf260) */
/* WARNING: Removing unreachable block (ram,0x00083c14) */
/* WARNING: Removing unreachable block (ram,0x004bf428) */
/* WARNING: Removing unreachable block (ram,0x00083c18) */
/* WARNING: Removing unreachable block (ram,0x000ff268) */
/* WARNING: Removing unreachable block (ram,0x0017f628) */
/* WARNING: Removing unreachable block (ram,0x0017f63c) */
/* WARNING: Removing unreachable block (ram,0x0017f638) */
/* WARNING: Removing unreachable block (ram,0x0017f640) */
/* WARNING: Removing unreachable block (ram,0x0017f644) */
/* WARNING: Removing unreachable block (ram,0x0017f648) */
/* WARNING: Removing unreachable block (ram,0x0017f64c) */
/* WARNING: Removing unreachable block (ram,0xfebe5a6c) */
/* WARNING: Removing unreachable block (ram,0x0017f654) */
/* WARNING: Removing unreachable block (ram,0x0017f658) */
/* WARNING: Removing unreachable block (ram,0x00083c44) */
/* WARNING: Removing unreachable block (ram,0x010bf71c) */
/* WARNING: Removing unreachable block (ram,0x00083c3c) */
/* WARNING: Removing unreachable block (ram,0x00083c34) */
/* WARNING: Removing unreachable block (ram,0x00083c38) */
/* WARNING: Removing unreachable block (ram,0x00083c40) */
/* WARNING: Removing unreachable block (ram,0x000bf288) */
/* WARNING: Removing unreachable block (ram,0x00083c48) */
/* WARNING: Removing unreachable block (ram,0x00083c4c) */
/* WARNING: Removing unreachable block (ram,0x00083c50) */
/* WARNING: Removing unreachable block (ram,0x00083c54) */
/* WARNING: Removing unreachable block (ram,0x00083c58) */
/* WARNING: Removing unreachable block (ram,0x000ff2a8) */
/* WARNING: Removing unreachable block (ram,0x000ff2ac) */
/* WARNING: Removing unreachable block (ram,0x00083c5c) */
/* WARNING: Removing unreachable block (ram,0x010bf734) */
/* WARNING: Removing unreachable block (ram,0x00083c60) */
/* WARNING: Removing unreachable block (ram,0x0013f23c) */
/* WARNING: Removing unreachable block (ram,0x0013f240) */
/* WARNING: Removing unreachable block (ram,0x0013f244) */
/* WARNING: Removing unreachable block (ram,0x0013f248) */
/* WARNING: Removing unreachable block (ram,0x00083c1c) */
/* WARNING: Removing unreachable block (ram,0x00083c20) */
/* WARNING: Removing unreachable block (ram,0x00083c24) */
/* WARNING: Removing unreachable block (ram,0x0017f630) */
/* WARNING: Removing unreachable block (ram,0x0017f634) */
/* WARNING: Removing unreachable block (ram,0x00083c28) */
/* WARNING: Removing unreachable block (ram,0x00083c2c) */
/* WARNING: Removing unreachable block (ram,0x000bf234) */
/* WARNING: Removing unreachable block (ram,0x00083c30) */
/* WARNING: Removing unreachable block (ram,0x000ff238) */
/* WARNING: Removing unreachable block (ram,0x0013f26c) */
/* WARNING: Removing unreachable block (ram,0x00083bd4) */
/* WARNING: Removing unreachable block (ram,0x00083bd8) */
/* WARNING: Removing unreachable block (ram,0x00083bdc) */
/* WARNING: Removing unreachable block (ram,0x000ff1e4) */
/* WARNING: Removing unreachable block (ram,0x0013f1e8) */
/* WARNING: Removing unreachable block (ram,0x0013f5f0) */
/* WARNING: Removing unreachable block (ram,0x00083bbc) */
/* WARNING: Removing unreachable block (ram,0x00083bc0) */
/* WARNING: Removing unreachable block (ram,0x000ff20c) */
/* WARNING: Removing unreachable block (ram,0x000ff210) */
/* WARNING: Removing unreachable block (ram,0x00083bc4) */
/* WARNING: Removing unreachable block (ram,0x00083bc8) */
/* WARNING: Removing unreachable block (ram,0x0013f214) */
/* WARNING: Removing unreachable block (ram,0x0013f218) */
/* WARNING: Removing unreachable block (ram,0x00083bcc) */
/* WARNING: Removing unreachable block (ram,0x00083bd0) */
/* WARNING: Removing unreachable block (ram,0x000ff5e8) */
/* WARNING: Removing unreachable block (ram,0x000ff5ec) */
/* WARNING: Removing unreachable block (ram,0x000bf5e0) */
/* WARNING: Removing unreachable block (ram,0x000bf5e4) */
/* WARNING: Removing unreachable block (ram,0x0017f404) */
/* WARNING: Removing unreachable block (ram,0x00083b6c) */
/* WARNING: Removing unreachable block (ram,0x00083b70) */
/* WARNING: Removing unreachable block (ram,0x00083b74) */
/* WARNING: Removing unreachable block (ram,0x0017f400) */
/* WARNING: Removing unreachable block (ram,0x0017f408) */
/* WARNING: Removing unreachable block (ram,0x0017f40c) */
/* WARNING: Removing unreachable block (ram,0x0017f3f8) */
/* WARNING: Removing unreachable block (ram,0x0017f40a) */
/* WARNING: Removing unreachable block (ram,0x0017f40e) */
/* WARNING: Removing unreachable block (ram,0x0017f406) */
/* WARNING: Removing unreachable block (ram,0x0013f1b8) */
/* WARNING: Removing unreachable block (ram,0x0013f1bc) */
/* WARNING: Removing unreachable block (ram,0x0057aa0c) */
/* WARNING: Removing unreachable block (ram,0x0013f1c0) */
/* WARNING: Removing unreachable block (ram,0x0013f1c4) */
/* WARNING: Removing unreachable block (ram,0x0013f1c8) */
/* WARNING: Removing unreachable block (ram,0x00acfc38) */
/* WARNING: Removing unreachable block (ram,0x0013f1cc) */
/* WARNING: Removing unreachable block (ram,0x0057aa14) */
/* WARNING: Removing unreachable block (ram,0x0013f1d0) */
/* WARNING: Removing unreachable block (ram,0xff1bacb4) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&, float) */

void AbyssEngine::AEMath::operator*(Vector *param_1,float param_2)

{
  uint uVar1;
  uint unaff_r4;
  uint unaff_r6;
  uint unaff_r8;
  bool in_ZR;
  bool in_CY;
  char in_OV;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  
  if (in_ZR) {
    if (in_CY) {
      bVar4 = SUB41((unaff_r8 << 0x1d) >> 0x1f,0);
      uVar1 = unaff_r6 & (unaff_r8 >> 3 | unaff_r8 << 0x1d);
      bVar2 = uVar1 == 0;
      if (bVar2) {
        bVar4 = (bool)((byte)(unaff_r6 >> 0x10) & 1);
      }
      bVar3 = (unaff_r4 & unaff_r6 >> 0x11) == 0;
      if (0x7fffffff < uVar1 == (bool)in_OV) {
        software_interrupt(0x3b5f0);
      }
      if (bVar2 && bVar3) {
        software_interrupt(0x80e92d);
      }
      if (bVar4 && (!bVar2 || !bVar3)) {
        FUN_001fa680();
      }
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_CY) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  00083b60: beq 0x000bf1ac
  00083b64: bcs 0x004bf378
  00083b68: bne 0x000ff1b4
  00083b6c: bcs 0x0013f1b8
  00083b70: beq 0x0017f3f8
  00083b74: bne 0x0017f400
  00083b78: bcs 0x0017f408
  00083b7c: beq 0x000bf184
  00083b80: bne 0x000ff188
  00083b84: bcs 0x0013f18c
  00083b88: ldc p7,cr4,[r2,#0x1c0]
  00083b8c: vmla.f32 s0,s6,s0
  00083b90: vldr.32 s2,[r2,#0x40]
  00083b94: vldr.32 s2,[r2,#0x4]
  00083b98: vmul.f32 s4,s0,s4
  00083b9c: vmul.f32 s0,s2,s6
  00083ba0: vmul.f32 s2,s4,s6
  00083ba4: vstr.32 s4,[r0,#0xc]
  00083ba8: vstr.32 s0,[r0]
  00083bac: vstr.32 s2,[r0,#0x4]
  00083bb0: ldrbmi r2,[r0,-r2,lsl #0x14]!
  000bf184: swilt 0xece8
  000bf188: swicc 0x800000
  000bf18c: andeq r0,r0,r0
  000bf190: swicc 0x800000
  000bf194: swicc 0x800000
  000bf198: andeq r9,r2,r0, lsl r8
  000bf19c: andeq r3,r1,r0, lsl #0x11
  000bf1ac: andeq r1,r1,r0, ror r1
  000bf1b0: bcc 0xfed0dc78
  000bf1b4: andeqs r1,r6,r8, ror #0x3
  000bf1b8: andeqs r12,r4,r6, lsr #0x11
  0013f1b8: beq 0xff1bacb4
  0013f1bc: bcc 0x0057aa0c
  0013f1c0: adcmis r1,r2,#0x5e000
  0013f1c4: strmi sp,[r11],#-0xd29
  0013f1c8: ble 0x00acfc38
  0013f1cc: beq 0x0057aa14
  0017f400: addmi lr,r5,#0x3f800
  0017f404: adchis pc,r10,r0, lsl #0x5
  0017f408: stmdbls r6,{r1,r3,r4,r5,r7,r11,lr}
```
