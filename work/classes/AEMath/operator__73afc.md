# AbyssEngine::AEMath::operator-
elf 0x73afc body 107
Sig: undefined __stdcall operator-(Vector * param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0017f402) overlaps instruction at (ram,0x0017f400)
    */
/* WARNING: Removing unreachable block (ram,0x00083b1c) */
/* WARNING: Removing unreachable block (ram,0x0113f3f8) */
/* WARNING: Removing unreachable block (ram,0x00083b20) */
/* WARNING: Removing unreachable block (ram,0x000bf128) */
/* WARNING: Removing unreachable block (ram,0x00083b24) */
/* WARNING: Removing unreachable block (ram,0x000ff12c) */
/* WARNING: Removing unreachable block (ram,0x00083b18) */
/* WARNING: Removing unreachable block (ram,0x010ff3f0) */
/* WARNING: Removing unreachable block (ram,0x0013f130) */
/* WARNING: Removing unreachable block (ram,0x0013f134) */
/* WARNING: Removing unreachable block (ram,0x000bf1b4) */
/* WARNING: Removing unreachable block (ram,0x000bf1b8) */
/* WARNING: Removing unreachable block (ram,0x000bf1bc) */
/* WARNING: Removing unreachable block (ram,0x000bf1c0) */
/* WARNING: Removing unreachable block (ram,0x000bf1c4) */
/* WARNING: Removing unreachable block (ram,0x000bf1c8) */
/* WARNING: Removing unreachable block (ram,0x0017f3f8) */
/* WARNING: Removing unreachable block (ram,0x0017f40a) */
/* WARNING: Removing unreachable block (ram,0x0017f40e) */
/* WARNING: Removing unreachable block (ram,0x0017f406) */
/* WARNING: Removing unreachable block (ram,0x0017f404) */
/* WARNING: Removing unreachable block (ram,0x0017f400) */
/* WARNING: Removing unreachable block (ram,0x004bf378) */
/* WARNING: Removing unreachable block (ram,0x00083b28) */
/* WARNING: Removing unreachable block (ram,0x00083b2c) */
/* WARNING: Removing unreachable block (ram,0x00083b5c) */
/* WARNING: Removing unreachable block (ram,0x00083b60) */
/* WARNING: Removing unreachable block (ram,0x00083b64) */
/* WARNING: Removing unreachable block (ram,0x00083b68) */
/* WARNING: Removing unreachable block (ram,0x000ff1b4) */
/* WARNING: Removing unreachable block (ram,0x00083b6c) */
/* WARNING: Removing unreachable block (ram,0x00083b70) */
/* WARNING: Removing unreachable block (ram,0x00083b74) */
/* WARNING: Removing unreachable block (ram,0x0017f408) */
/* WARNING: Removing unreachable block (ram,0x0017f40c) */
/* WARNING: Removing unreachable block (ram,0x0013f18c) */
/* WARNING: Removing unreachable block (ram,0x0013f190) */
/* WARNING: Removing unreachable block (ram,0x000ff20c) */
/* WARNING: Removing unreachable block (ram,0x00083bc0) */
/* WARNING: Removing unreachable block (ram,0x00083bc4) */
/* WARNING: Removing unreachable block (ram,0x00083bc8) */
/* WARNING: Removing unreachable block (ram,0x0013f214) */
/* WARNING: Removing unreachable block (ram,0x0013f218) */
/* WARNING: Removing unreachable block (ram,0x00083bcc) */
/* WARNING: Removing unreachable block (ram,0x00083bd0) */
/* WARNING: Removing unreachable block (ram,0x000ff5e8) */
/* WARNING: Removing unreachable block (ram,0x000ff5ec) */
/* WARNING: Removing unreachable block (ram,0x00083c1c) */
/* WARNING: Removing unreachable block (ram,0x0013f26c) */
/* WARNING: Removing unreachable block (ram,0x00083c20) */
/* WARNING: Removing unreachable block (ram,0x0017f628) */
/* WARNING: Removing unreachable block (ram,0x00083c24) */
/* WARNING: Removing unreachable block (ram,0x0017f630) */
/* WARNING: Removing unreachable block (ram,0x0017f634) */
/* WARNING: Removing unreachable block (ram,0x00083c28) */
/* WARNING: Removing unreachable block (ram,0x00083c2c) */
/* WARNING: Removing unreachable block (ram,0x000bf234) */
/* WARNING: Removing unreachable block (ram,0x00083c30) */
/* WARNING: Removing unreachable block (ram,0x000ff238) */
/* WARNING: Removing unreachable block (ram,0x00083c18) */
/* WARNING: Removing unreachable block (ram,0x000ff268) */
/* WARNING: Removing unreachable block (ram,0x00083c34) */
/* WARNING: Removing unreachable block (ram,0x00083c38) */
/* WARNING: Removing unreachable block (ram,0x00083c3c) */
/* WARNING: Removing unreachable block (ram,0x00083c40) */
/* WARNING: Removing unreachable block (ram,0x000bf288) */
/* WARNING: Removing unreachable block (ram,0x00083c44) */
/* WARNING: Removing unreachable block (ram,0x010bf71c) */
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
/* WARNING: Removing unreachable block (ram,0x0013f240) */
/* WARNING: Removing unreachable block (ram,0x0013f23c) */
/* WARNING: Removing unreachable block (ram,0x0013f244) */
/* WARNING: Removing unreachable block (ram,0x0013f248) */
/* WARNING: Removing unreachable block (ram,0x0017f648) */
/* WARNING: Removing unreachable block (ram,0x0017f644) */
/* WARNING: Removing unreachable block (ram,0x0017f638) */
/* WARNING: Removing unreachable block (ram,0x0017f63c) */
/* WARNING: Removing unreachable block (ram,0x0017f640) */
/* WARNING: Removing unreachable block (ram,0x0017f64c) */
/* WARNING: Removing unreachable block (ram,0xfebe5a6c) */
/* WARNING: Removing unreachable block (ram,0x0017f654) */
/* WARNING: Removing unreachable block (ram,0x0017f658) */
/* WARNING: Removing unreachable block (ram,0x00083c0c) */
/* WARNING: Removing unreachable block (ram,0x00083be0) */
/* WARNING: Removing unreachable block (ram,0x00083be4) */
/* WARNING: Removing unreachable block (ram,0x00083c10) */
/* WARNING: Removing unreachable block (ram,0x00083c14) */
/* WARNING: Removing unreachable block (ram,0x004bf428) */
/* WARNING: Removing unreachable block (ram,0x000bf260) */
/* WARNING: Removing unreachable block (ram,0x0013f1e8) */
/* WARNING: Removing unreachable block (ram,0x00083bd4) */
/* WARNING: Removing unreachable block (ram,0x00083bd8) */
/* WARNING: Removing unreachable block (ram,0x000bf1e0) */
/* WARNING: Removing unreachable block (ram,0x00083bdc) */
/* WARNING: Removing unreachable block (ram,0x000ff1e4) */
/* WARNING: Removing unreachable block (ram,0x0013f5f0) */
/* WARNING: Removing unreachable block (ram,0x000bf5e0) */
/* WARNING: Removing unreachable block (ram,0x000bf5e4) */
/* WARNING: Removing unreachable block (ram,0x00083b78) */
/* WARNING: Removing unreachable block (ram,0x00083b7c) */
/* WARNING: Removing unreachable block (ram,0x00083b80) */
/* WARNING: Removing unreachable block (ram,0x000ff188) */
/* WARNING: Removing unreachable block (ram,0x00083b84) */
/* WARNING: Removing unreachable block (ram,0x00083b88) */
/* WARNING: Removing unreachable block (ram,0x00083bb0) */
/* WARNING: Removing unreachable block (ram,0x00083bb4) */
/* WARNING: Removing unreachable block (ram,0x00083bb8) */
/* WARNING: Removing unreachable block (ram,0x000bf204) */
/* WARNING: Removing unreachable block (ram,0x000bf208) */
/* WARNING: Removing unreachable block (ram,0x000bf20c) */
/* WARNING: Removing unreachable block (ram,0x00083bbc) */
/* WARNING: Removing unreachable block (ram,0x000ff210) */
/* WARNING: Removing unreachable block (ram,0x000bf184) */
/* WARNING: Removing unreachable block (ram,0x000bf188) */
/* WARNING: Removing unreachable block (ram,0x000bf18c) */
/* WARNING: Removing unreachable block (ram,0x000bf190) */
/* WARNING: Removing unreachable block (ram,0x000bf198) */
/* WARNING: Removing unreachable block (ram,0x000bf1a0) */
/* WARNING: Removing unreachable block (ram,0x0013f1b8) */
/* WARNING: Removing unreachable block (ram,0xff1bacb4) */
/* WARNING: Removing unreachable block (ram,0x0013f1bc) */
/* WARNING: Removing unreachable block (ram,0x0057aa0c) */
/* WARNING: Removing unreachable block (ram,0x0013f1c0) */
/* WARNING: Removing unreachable block (ram,0x0013f1c4) */
/* WARNING: Removing unreachable block (ram,0x0013f1c8) */
/* WARNING: Removing unreachable block (ram,0x00acfc38) */
/* WARNING: Removing unreachable block (ram,0x0013f1cc) */
/* WARNING: Removing unreachable block (ram,0x0057aa14) */
/* WARNING: Removing unreachable block (ram,0x0013f1d0) */
/* WARNING: Removing unreachable block (ram,0x000bf1ac) */
/* WARNING: Removing unreachable block (ram,0x000bf1b0) */
/* WARNING: Removing unreachable block (ram,0xfed0dc78) */
/* WARNING: Removing unreachable block (ram,0x00083b08) */
/* WARNING: Removing unreachable block (ram,0x00083b0c) */
/* WARNING: Removing unreachable block (ram,0x00083b10) */
/* WARNING: Removing unreachable block (ram,0x00083b14) */
/* WARNING: Removing unreachable block (ram,0x0013f160) */
/* WARNING: Removing unreachable block (ram,0x0013f164) */
/* WARNING: Removing unreachable block (ram,0x010bf3e0) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::operator-(Vector *param_1,Vector *param_2)

{
  undefined4 *unaff_r4;
  undefined4 in_r12;
  undefined4 unaff_lr;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  undefined4 in_cr15;
  
  if ((!in_ZR) && (in_CY)) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  coprocessor_loadlong(7,in_cr15,in_r12);
  if (in_NG) {
    *unaff_r4 = unaff_lr;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  00083afc: beq 0x000bf14c
  00083b00: bcc 0x000bf14c
  00083b04: bne 0x000ff154
  00083b08: bmi 0x000ff154
  00083b0c: beq 0x010bf3e0
  00083b10: bcs 0x0013f160
  00083b14: bpl 0x0013f160
  00083b18: bne 0x010ff3f0
  00083b1c: bcs 0x0113f3f8
  00083b20: beq 0x000bf128
  00083b24: bne 0x000ff12c
  00083b28: bcs 0x0013f130
  00083b2c: ldc p7,cr4,[r2,#0x1c0]
  00083b30: vldr.32 s0,[r1]
  00083b34: vldr.32 s6,[r2]
  00083b38: vldr.32 s2,[r1,#0x4]
  00083b3c: vmul.f32 s8,s6,s2
  00083b40: vldr.32 s0,[r2]
  00083b44: vldr.32 s4,[r1,#0x8]
  00083b48: vmul.f32 s10,s8,s4
  00083b4c: vmul.f32 s2,s10,s2
  00083b50: vstr.32 s4,[r0,#0x8]
  00083b54: vstr.32 s0,[r0]
  00083b58: vstr.32 s2,[r0,#0x4]
  00083b5c: ldrbmi r2,[r0,-r2,lsl #0x14]!
  000bf14c: ldcl p7,cr15,[r12],#0x2bc
  000bf150: strmi lr,[r4],-r6
```
