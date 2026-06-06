# AbyssEngine::AEMath::operator/
elf 0x73c10 body 143
Sig: undefined __stdcall operator/(float param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x00083c28) */
/* WARNING: Removing unreachable block (ram,0x00083c2c) */
/* WARNING: Removing unreachable block (ram,0x000bf234) */
/* WARNING: Removing unreachable block (ram,0x00083c30) */
/* WARNING: Removing unreachable block (ram,0x000ff238) */
/* WARNING: Removing unreachable block (ram,0x00083c1c) */
/* WARNING: Removing unreachable block (ram,0x0013f26c) */
/* WARNING: Removing unreachable block (ram,0x00083c20) */
/* WARNING: Removing unreachable block (ram,0x00083c24) */
/* WARNING: Removing unreachable block (ram,0x0017f630) */
/* WARNING: Removing unreachable block (ram,0x0017f634) */
/* WARNING: Removing unreachable block (ram,0x0013f240) */
/* WARNING: Removing unreachable block (ram,0x0013f23c) */
/* WARNING: Removing unreachable block (ram,0x0013f244) */
/* WARNING: Removing unreachable block (ram,0x0013f248) */
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
/* WARNING: Removing unreachable block (ram,0x00083c34) */
/* WARNING: Removing unreachable block (ram,0x00083c38) */
/* WARNING: Removing unreachable block (ram,0x00083c3c) */
/* WARNING: Removing unreachable block (ram,0x00083c40) */
/* WARNING: Removing unreachable block (ram,0x000bf288) */
/* WARNING: Removing unreachable block (ram,0x0017f648) */
/* WARNING: Removing unreachable block (ram,0x0017f638) */
/* WARNING: Removing unreachable block (ram,0x0017f63c) */
/* WARNING: Removing unreachable block (ram,0x0017f640) */
/* WARNING: Removing unreachable block (ram,0x0017f644) */
/* WARNING: Removing unreachable block (ram,0x0017f64c) */
/* WARNING: Removing unreachable block (ram,0x0017f654) */
/* WARNING: Removing unreachable block (ram,0x0017f658) */
/* WARNING: Removing unreachable block (ram,0xfebe5a6c) */
/* WARNING: Removing unreachable block (ram,0x00083c18) */
/* WARNING: Removing unreachable block (ram,0x000ff268) */
/* WARNING: Removing unreachable block (ram,0x0017f628) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(float, AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::operator/(float param_1,Vector *param_2)

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
  00083c10: beq 0x000bf260
  00083c14: bne 0x004bf428
  00083c18: bne 0x000ff268
  00083c1c: bcs 0x0013f26c
  00083c20: beq 0x0017f628
  00083c24: bne 0x0017f630
  00083c28: bcs 0x0017f638
  00083c2c: beq 0x000bf234
  00083c30: bne 0x000ff238
  00083c34: bcs 0x0013f23c
  00083c38: ldc p7,cr4,[r1,#0x1c0]
  00083c3c: strmi r0,[r2],-r0,lsl #0x14
  00083c40: bne 0x000bf288
  00083c44: bne 0x010bf71c
  00083c48: blx 0x004bf814
  00083c4c: andcs r11,r0,r12, lsl pc
  00083c50: ldc p7,cr4,[r1,#0x1c0]
  00083c54: andcs r0,r0,r1, lsl #0x14
  00083c58: bne 0x000ff2a8
  00083c5c: bne 0x010bf734
  00083c60: blx 0x004bf82c
  000ff268: and lr,r1,r0, lsl #0x1c
  000ff2a8: eorhi r2,r1,r2
  0013f23c: swilt 0x89909
  0013f240: biceqs pc,r8,r4, lsl #0x2
  0013f244: ldccs p9,cr14,[pc],#-0x350
  0013f26c: ldmdb r0!,{r0,r1,r4,r5,r8,r9,r10,r12,sp,lr,pc}^
  0017f630: stmiage r2,{r1,r7,r8,r11,r12}^
  0017f638: stmiage r6!,{r1,r2,r3,r10,r11,sp,lr,pc}
  0017f63c: andcs r10,r0,#0x388000
  0017f640: ldcl p6,cr15,[r2],#0x3bc
  0017f644: ldrbtmi r4,[r9],#-0x93b
  0017f648: andcs r10,r0,#0xa30000
  0017f64c: bl 0xffabd210
  0017f650: bge 0xfebe5a6c
  0017f654: stmiage r5,{r12,pc}^
```
