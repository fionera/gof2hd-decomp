# AbyssEngine::AEMath::operator-
elf 0x73aa4 body 163
Sig: undefined __thiscall operator-(AEMath * this, Vector * param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0017f402) overlaps instruction at (ram,0x0017f400)
    */
/* WARNING: Removing unreachable block (ram,0x0013f104) */
/* WARNING: Removing unreachable block (ram,0x0113f584) */
/* WARNING: Removing unreachable block (ram,0x004fa8d4) */
/* WARNING: Removing unreachable block (ram,0x000bf0c8) */
/* WARNING: Removing unreachable block (ram,0x000bf0cc) */
/* WARNING: Removing unreachable block (ram,0xff0fabb4) */
/* WARNING: Removing unreachable block (ram,0x000bf0d0) */
/* WARNING: Removing unreachable block (ram,0xff13abb8) */
/* WARNING: Removing unreachable block (ram,0x000bf128) */
/* WARNING: Removing unreachable block (ram,0x000bf1b4) */
/* WARNING: Removing unreachable block (ram,0x00083b1c) */
/* WARNING: Removing unreachable block (ram,0x0113f3f8) */
/* WARNING: Removing unreachable block (ram,0x00083b20) */
/* WARNING: Removing unreachable block (ram,0x00083b24) */
/* WARNING: Removing unreachable block (ram,0x000ff12c) */
/* WARNING: Removing unreachable block (ram,0x00083b28) */
/* WARNING: Removing unreachable block (ram,0x0013f130) */
/* WARNING: Removing unreachable block (ram,0x0013f134) */
/* WARNING: Removing unreachable block (ram,0x00083b2c) */
/* WARNING: Removing unreachable block (ram,0x00083b18) */
/* WARNING: Removing unreachable block (ram,0x010ff3f0) */
/* WARNING: Removing unreachable block (ram,0x010bf3e0) */
/* WARNING: Removing unreachable block (ram,0x00083b08) */
/* WARNING: Removing unreachable block (ram,0x00083b0c) */
/* WARNING: Removing unreachable block (ram,0x00083b10) */
/* WARNING: Removing unreachable block (ram,0x00083b14) */
/* WARNING: Removing unreachable block (ram,0x0013f160) */
/* WARNING: Removing unreachable block (ram,0x00083b04) */
/* WARNING: Removing unreachable block (ram,0x000ff154) */
/* WARNING: Removing unreachable block (ram,0x00083ac4) */
/* WARNING: Removing unreachable block (ram,0x00083ac8) */
/* WARNING: Removing unreachable block (ram,0x00083af8) */
/* WARNING: Removing unreachable block (ram,0x00083afc) */
/* WARNING: Removing unreachable block (ram,0x00083b00) */
/* WARNING: Removing unreachable block (ram,0x000bf14c) */
/* WARNING: Removing unreachable block (ram,0x000bf150) */
/* WARNING: Removing unreachable block (ram,0x000bf154) */
/* WARNING: Removing unreachable block (ram,0x00083b5c) */
/* WARNING: Removing unreachable block (ram,0x00083b60) */
/* WARNING: Removing unreachable block (ram,0x000bf1ac) */
/* WARNING: Removing unreachable block (ram,0x000bf1b0) */
/* WARNING: Removing unreachable block (ram,0xfed0dc78) */
/* WARNING: Removing unreachable block (ram,0x000bf1b8) */
/* WARNING: Removing unreachable block (ram,0x000bf1bc) */
/* WARNING: Removing unreachable block (ram,0x000bf1c0) */
/* WARNING: Removing unreachable block (ram,0x000bf1c4) */
/* WARNING: Removing unreachable block (ram,0x000bf1c8) */
/* WARNING: Removing unreachable block (ram,0x00083b64) */
/* WARNING: Removing unreachable block (ram,0x00083b68) */
/* WARNING: Removing unreachable block (ram,0x000ff1b4) */
/* WARNING: Removing unreachable block (ram,0x00083b6c) */
/* WARNING: Removing unreachable block (ram,0x00083b70) */
/* WARNING: Removing unreachable block (ram,0x0017f3f8) */
/* WARNING: Removing unreachable block (ram,0x0017f40a) */
/* WARNING: Removing unreachable block (ram,0x0017f40e) */
/* WARNING: Removing unreachable block (ram,0x0017f406) */
/* WARNING: Removing unreachable block (ram,0x00083b74) */
/* WARNING: Removing unreachable block (ram,0x0017f400) */
/* WARNING: Removing unreachable block (ram,0x0017f404) */
/* WARNING: Removing unreachable block (ram,0x00083bc0) */
/* WARNING: Removing unreachable block (ram,0x00083bc4) */
/* WARNING: Removing unreachable block (ram,0x00083bc8) */
/* WARNING: Removing unreachable block (ram,0x0013f214) */
/* WARNING: Removing unreachable block (ram,0x0013f218) */
/* WARNING: Removing unreachable block (ram,0x00083bcc) */
/* WARNING: Removing unreachable block (ram,0x000bf5e0) */
/* WARNING: Removing unreachable block (ram,0x000bf5e4) */
/* WARNING: Removing unreachable block (ram,0x00083bd0) */
/* WARNING: Removing unreachable block (ram,0x000ff5e8) */
/* WARNING: Removing unreachable block (ram,0x000ff5ec) */
/* WARNING: Removing unreachable block (ram,0x00083bbc) */
/* WARNING: Removing unreachable block (ram,0x000ff20c) */
/* WARNING: Removing unreachable block (ram,0x0013f5f0) */
/* WARNING: Removing unreachable block (ram,0x0013f1e8) */
/* WARNING: Removing unreachable block (ram,0x00083bd4) */
/* WARNING: Removing unreachable block (ram,0x00083bd8) */
/* WARNING: Removing unreachable block (ram,0x00083bdc) */
/* WARNING: Removing unreachable block (ram,0x000ff1e4) */
/* WARNING: Removing unreachable block (ram,0x0013f26c) */
/* WARNING: Removing unreachable block (ram,0x00083c28) */
/* WARNING: Removing unreachable block (ram,0x00083c2c) */
/* WARNING: Removing unreachable block (ram,0x000bf234) */
/* WARNING: Removing unreachable block (ram,0x00083c30) */
/* WARNING: Removing unreachable block (ram,0x000ff238) */
/* WARNING: Removing unreachable block (ram,0x00083c1c) */
/* WARNING: Removing unreachable block (ram,0x00083c20) */
/* WARNING: Removing unreachable block (ram,0x00083c24) */
/* WARNING: Removing unreachable block (ram,0x0017f630) */
/* WARNING: Removing unreachable block (ram,0x0017f634) */
/* WARNING: Removing unreachable block (ram,0x0013f240) */
/* WARNING: Removing unreachable block (ram,0x0013f23c) */
/* WARNING: Removing unreachable block (ram,0x0013f244) */
/* WARNING: Removing unreachable block (ram,0x0013f248) */
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
/* WARNING: Removing unreachable block (ram,0x00083c34) */
/* WARNING: Removing unreachable block (ram,0x00083c38) */
/* WARNING: Removing unreachable block (ram,0x00083c3c) */
/* WARNING: Removing unreachable block (ram,0x00083c40) */
/* WARNING: Removing unreachable block (ram,0x000bf288) */
/* WARNING: Removing unreachable block (ram,0x00083c44) */
/* WARNING: Removing unreachable block (ram,0x010bf71c) */
/* WARNING: Removing unreachable block (ram,0x0017f648) */
/* WARNING: Removing unreachable block (ram,0x0017f638) */
/* WARNING: Removing unreachable block (ram,0x0017f63c) */
/* WARNING: Removing unreachable block (ram,0x0017f640) */
/* WARNING: Removing unreachable block (ram,0x0017f644) */
/* WARNING: Removing unreachable block (ram,0x0017f64c) */
/* WARNING: Removing unreachable block (ram,0xfebe5a6c) */
/* WARNING: Removing unreachable block (ram,0x0017f654) */
/* WARNING: Removing unreachable block (ram,0x0017f658) */
/* WARNING: Removing unreachable block (ram,0x0017f628) */
/* WARNING: Removing unreachable block (ram,0x00083be0) */
/* WARNING: Removing unreachable block (ram,0x00083be4) */
/* WARNING: Removing unreachable block (ram,0x00083c0c) */
/* WARNING: Removing unreachable block (ram,0x00083c10) */
/* WARNING: Removing unreachable block (ram,0x000bf260) */
/* WARNING: Removing unreachable block (ram,0x00083c14) */
/* WARNING: Removing unreachable block (ram,0x004bf428) */
/* WARNING: Removing unreachable block (ram,0x00083c18) */
/* WARNING: Removing unreachable block (ram,0x000ff268) */
/* WARNING: Removing unreachable block (ram,0x000bf1e0) */
/* WARNING: Removing unreachable block (ram,0x00083bb0) */
/* WARNING: Removing unreachable block (ram,0x00083b84) */
/* WARNING: Removing unreachable block (ram,0x00083b88) */
/* WARNING: Removing unreachable block (ram,0x00083bb4) */
/* WARNING: Removing unreachable block (ram,0x00083bb8) */
/* WARNING: Removing unreachable block (ram,0x000bf204) */
/* WARNING: Removing unreachable block (ram,0x000ff210) */
/* WARNING: Removing unreachable block (ram,0x000bf208) */
/* WARNING: Removing unreachable block (ram,0x000bf20c) */
/* WARNING: Removing unreachable block (ram,0x0013f18c) */
/* WARNING: Removing unreachable block (ram,0x0013f190) */
/* WARNING: Removing unreachable block (ram,0x00083b78) */
/* WARNING: Removing unreachable block (ram,0x00083b7c) */
/* WARNING: Removing unreachable block (ram,0x000bf184) */
/* WARNING: Removing unreachable block (ram,0x000bf188) */
/* WARNING: Removing unreachable block (ram,0x000bf18c) */
/* WARNING: Removing unreachable block (ram,0x000bf190) */
/* WARNING: Removing unreachable block (ram,0x000bf198) */
/* WARNING: Removing unreachable block (ram,0x00083b80) */
/* WARNING: Removing unreachable block (ram,0x000ff188) */
/* WARNING: Removing unreachable block (ram,0x0017f408) */
/* WARNING: Removing unreachable block (ram,0x0017f40c) */
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
/* WARNING: Removing unreachable block (ram,0x004bf378) */
/* WARNING: Removing unreachable block (ram,0x00083ab8) */
/* WARNING: Removing unreachable block (ram,0x00083abc) */
/* WARNING: Removing unreachable block (ram,0x00083ac0) */
/* WARNING: Removing unreachable block (ram,0x000ff0c8) */
/* WARNING: Removing unreachable block (ram,0x000ff0cc) */
/* WARNING: Removing unreachable block (ram,0x000ff0d0) */
/* WARNING: Removing unreachable block (ram,0x000bf0c4) */
/* WARNING: Removing unreachable block (ram,0x01ac190c) */
/* WARNING: Removing unreachable block (ram,0x0013f164) */
/* WARNING: Removing unreachable block (ram,0x0013f0cc) */
/* WARNING: Removing unreachable block (ram,0x0013f0d0) */
/* WARNING: Removing unreachable block (ram,0x0013f0d4) */
/* WARNING: Removing unreachable block (ram,0x0118fae4) */
/* WARNING: Removing unreachable block (ram,0x0013f0d8) */
/* WARNING: Removing unreachable block (ram,0x000bf0e4) */
/* WARNING: Removing unreachable block (ram,0x000bf0e8) */
/* WARNING: Removing unreachable block (ram,0x0013a720) */
/* WARNING: Removing unreachable block (ram,0x0013a724) */
/* WARNING: Removing unreachable block (ram,0x0013a728) */
/* WARNING: Removing unreachable block (ram,0x000bf0d4) */
/* WARNING: Removing unreachable block (ram,0x000bf0d8) */
/* WARNING: Removing unreachable block (ram,0x000bf0dc) */
/* WARNING: Removing unreachable block (ram,0x000bf0e0) */
/* WARNING: Removing unreachable block (ram,0x000fa71c) */
/* WARNING: Removing unreachable block (ram,0x00083aac) */
/* WARNING: Removing unreachable block (ram,0x0013f0f8) */
/* WARNING: Removing unreachable block (ram,0x0013f0fc) */
/* WARNING: Removing unreachable block (ram,0x0013f100) */
/* WARNING: Removing unreachable block (ram,0x00083ab0) */
/* WARNING: Removing unreachable block (ram,0x00083ab4) */
/* WARNING: Removing unreachable block (ram,0x010ff580) */
/* WARNING: Removing unreachable block (ram,0x010bf57c) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&) */

void __thiscall AbyssEngine::AEMath::operator-(AEMath *this,Vector *param_1)

{
  bool in_ZR;
  
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  00083aa4: beq 0x000bf0f0
  00083aa8: bne 0x000ff0f4
  00083aac: bcs 0x0013f0f8
  00083ab0: beq 0x010bf57c
  00083ab4: bne 0x010ff580
  00083ab8: bcs 0x0113f584
  00083abc: beq 0x000bf0c4
  00083ac0: bne 0x000ff0c8
  00083ac4: bcs 0x0013f0cc
  00083ac8: ldc p7,cr4,[r2,#0x1c0]
  00083acc: vldr.32 s0,[r1]
  00083ad0: vldr.32 s6,[r2]
  00083ad4: vldr.32 s2,[r1,#0x4]
  00083ad8: vadd.f32 s8,s6,s2
  00083adc: vldr.32 s0,[r2]
  00083ae0: vldr.32 s4,[r1,#0x8]
  00083ae4: vadd.f32 s10,s8,s4
  00083ae8: vadd.f32 s2,s10,s2
  00083aec: vstr.32 s4,[r0,#0x8]
  00083af0: vstr.32 s0,[r0]
  00083af4: vstr.32 s2,[r0,#0x4]
  00083af8: ldrbmi r2,[r0,-r2,lsl #0x14]!
  000bf0c4: bne 0x01ac190c
  000bf0c8: beq 0x004fa8d4
  000bf0cc: beq 0xff0fabb4
  000bf0d0: bne 0xff13abb8
  000bf0d4: vadd.i8 d25,d4,d4
  000bf0d8: andls r2,r2,r2, ror #0x2
  000bf0dc: ldrtmi r4,[r2],-r0,asr #0xc
  000bf0e0: bne 0x000fa71c
  000bf0e4: beq 0x0013a720
  000bf0e8: mcr p7,0x2,pc,cr4,cr4,0x5
  000ff0c8: andeq r3,lr,r5, ror #0x2
  000ff0cc: swilt 0x800000
  000ff0f4: ldrbtmi r4,[r9],#-0x605
  0013f0cc: stclle p2,cr4,[r5,#-0x228]
  0013f0d0: strmi r0,[r8],#-0x40
  0013f0d4: ble 0x0118fae4
  0013f0f8: andcs r11,r0,r12, lsl pc
  0013f0fc: orreqs pc,r8,r4, asr #0x11
  0013f100: andhi pc,r8,r4, asr #0x11
```
