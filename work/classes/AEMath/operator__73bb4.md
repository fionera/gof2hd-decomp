# AbyssEngine::AEMath::operator/
elf 0x73bb4 body 115
Sig: undefined __stdcall operator/(Vector * param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0013f5f0) */
/* WARNING: Removing unreachable block (ram,0x000bf1e0) */
/* WARNING: Removing unreachable block (ram,0x00083bd4) */
/* WARNING: Removing unreachable block (ram,0x00083bd8) */
/* WARNING: Removing unreachable block (ram,0x00083bdc) */
/* WARNING: Removing unreachable block (ram,0x000ff1e4) */
/* WARNING: Removing unreachable block (ram,0x00083be0) */
/* WARNING: Removing unreachable block (ram,0x00083be4) */
/* WARNING: Removing unreachable block (ram,0x00083c0c) */
/* WARNING: Removing unreachable block (ram,0x00083c10) */
/* WARNING: Removing unreachable block (ram,0x000bf260) */
/* WARNING: Removing unreachable block (ram,0x00083c14) */
/* WARNING: Removing unreachable block (ram,0x004bf428) */
/* WARNING: Removing unreachable block (ram,0x00083bcc) */
/* WARNING: Removing unreachable block (ram,0x00083bd0) */
/* WARNING: Removing unreachable block (ram,0x000ff5e8) */
/* WARNING: Removing unreachable block (ram,0x000ff5ec) */
/* WARNING: Removing unreachable block (ram,0x0017f638) */
/* WARNING: Removing unreachable block (ram,0x0017f63c) */
/* WARNING: Removing unreachable block (ram,0x0017f640) */
/* WARNING: Removing unreachable block (ram,0x0017f644) */
/* WARNING: Removing unreachable block (ram,0x0017f648) */
/* WARNING: Removing unreachable block (ram,0x0017f64c) */
/* WARNING: Removing unreachable block (ram,0x0017f654) */
/* WARNING: Removing unreachable block (ram,0x0017f658) */
/* WARNING: Removing unreachable block (ram,0xfebe5a6c) */
/* WARNING: Removing unreachable block (ram,0x0013f244) */
/* WARNING: Removing unreachable block (ram,0x0013f23c) */
/* WARNING: Removing unreachable block (ram,0x0013f240) */
/* WARNING: Removing unreachable block (ram,0x0013f248) */
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
/* WARNING: Removing unreachable block (ram,0x00083c5c) */
/* WARNING: Removing unreachable block (ram,0x00083c60) */
/* WARNING: Removing unreachable block (ram,0x010bf734) */
/* WARNING: Removing unreachable block (ram,0x000ff2a8) */
/* WARNING: Removing unreachable block (ram,0x000ff2ac) */
/* WARNING: Removing unreachable block (ram,0x00083c18) */
/* WARNING: Removing unreachable block (ram,0x000ff268) */
/* WARNING: Removing unreachable block (ram,0x00083c1c) */
/* WARNING: Removing unreachable block (ram,0x0013f26c) */
/* WARNING: Removing unreachable block (ram,0x00083c20) */
/* WARNING: Removing unreachable block (ram,0x0017f628) */
/* WARNING: Removing unreachable block (ram,0x00083c24) */
/* WARNING: Removing unreachable block (ram,0x0017f630) */
/* WARNING: Removing unreachable block (ram,0x0017f634) */
/* WARNING: Removing unreachable block (ram,0x00083c28) */
/* WARNING: Removing unreachable block (ram,0x00083c2c) */
/* WARNING: Removing unreachable block (ram,0x00083c30) */
/* WARNING: Removing unreachable block (ram,0x000ff238) */
/* WARNING: Removing unreachable block (ram,0x000bf234) */
/* WARNING: Removing unreachable block (ram,0x0013f1e8) */
/* WARNING: Removing unreachable block (ram,0x00083bc0) */
/* WARNING: Removing unreachable block (ram,0x00083bc4) */
/* WARNING: Removing unreachable block (ram,0x00083bc8) */
/* WARNING: Removing unreachable block (ram,0x0013f214) */
/* WARNING: Removing unreachable block (ram,0x0013f218) */
/* WARNING: Removing unreachable block (ram,0x000bf5e0) */
/* WARNING: Removing unreachable block (ram,0x000bf5e4) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::operator/(Vector *param_1,Vector *param_2)

{
  bool in_ZR;
  bool in_CY;
  
  if ((!in_ZR) && (in_CY)) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  00083bb4: beq 0x000bf204
  00083bb8: bcc 0x000bf204
  00083bbc: bne 0x000ff20c
  00083bc0: bmi 0x000ff20c
  00083bc4: bcs 0x0013f214
  00083bc8: bpl 0x0013f214
  00083bcc: beq 0x000bf5e0
  00083bd0: bne 0x000ff5e8
  00083bd4: bcs 0x0013f5f0
  00083bd8: beq 0x000bf1e0
  00083bdc: bne 0x000ff1e4
  00083be0: bcs 0x0013f1e8
  00083be4: ldc p7,cr4,[r1,#0x1c0]
  00083be8: vmla.f32 s0,s6,s0
  00083bec: vldr.32 s4,[r1,#0x40]
  00083bf0: vldr.32 s2,[r1,#0x4]
  00083bf4: vdiv.f32 s4,s0,s4
  00083bf8: vdiv.f32 s0,s2,s6
  00083bfc: vdiv.f32 s2,s4,s6
  00083c00: vstr.32 s4,[r0,#0xc]
  00083c04: vstr.32 s0,[r0]
  00083c08: vstr.32 s2,[r0,#0x4]
  00083c0c: ldrbmi r2,[r0,-r2,lsl #0x14]!
  000bf204: ldrtmi r6,[r0],-r4,lsl #0x10
  000bf208: andls r4,pc,r0, lsr #0xf
  000bf5e0: stmdavs r0,{r7,r11,sp,lr}^
  000ff20c: eorhi r2,r1,r2
  000ff5e8: eorgt r1,r3,#0x880
  0013f214: ldrtgt r8,[r11],r0
```
