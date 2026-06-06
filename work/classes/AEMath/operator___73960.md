# AbyssEngine::AEMath::Vector::operator[]
elf 0x73960 body 11
Sig: undefined __stdcall operator[](int param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0017f402) overlaps instruction at (ram,0x0017f400)
    */
/* WARNING: Removing unreachable block (ram,0x0117f280) */
/* WARNING: Removing unreachable block (ram,0x000ff010) */
/* WARNING: Removing unreachable block (ram,0x000ff018) */
/* WARNING: Removing unreachable block (ram,0x001d0830) */
/* WARNING: Removing unreachable block (ram,0x001d0834) */
/* WARNING: Removing unreachable block (ram,0x001d0838) */
/* WARNING: Removing unreachable block (ram,0x001d083c) */
/* WARNING: Removing unreachable block (ram,0x001d0840) */
/* WARNING: Removing unreachable block (ram,0x001d0844) */
/* WARNING: Removing unreachable block (ram,0x001d0848) */
/* WARNING: Removing unreachable block (ram,0x001d084c) */
/* WARNING: Removing unreachable block (ram,0x001d0850) */
/* WARNING: Removing unreachable block (ram,0x000bf050) */
/* WARNING: Removing unreachable block (ram,0x000bf054) */
/* WARNING: Removing unreachable block (ram,0x000ff058) */
/* WARNING: Removing unreachable block (ram,0x000bf2a8) */
/* WARNING: Removing unreachable block (ram,0x000ff030) */
/* WARNING: Removing unreachable block (ram,0x00083a14) */
/* WARNING: Removing unreachable block (ram,0x00083a18) */
/* WARNING: Removing unreachable block (ram,0x0013f060) */
/* WARNING: Removing unreachable block (ram,0x0013f064) */
/* WARNING: Removing unreachable block (ram,0x00083a1c) */
/* WARNING: Removing unreachable block (ram,0x000bf024) */
/* WARNING: Removing unreachable block (ram,0x00083a20) */
/* WARNING: Removing unreachable block (ram,0x000ff06c) */
/* WARNING: Removing unreachable block (ram,0x000ff070) */
/* WARNING: Removing unreachable block (ram,0x00083a24) */
/* WARNING: Removing unreachable block (ram,0x00083a28) */
/* WARNING: Removing unreachable block (ram,0x00083a2c) */
/* WARNING: Removing unreachable block (ram,0x0013f078) */
/* WARNING: Removing unreachable block (ram,0x0013f07c) */
/* WARNING: Removing unreachable block (ram,0x0013f080) */
/* WARNING: Removing unreachable block (ram,0x00083a30) */
/* WARNING: Removing unreachable block (ram,0x00083a34) */
/* WARNING: Removing unreachable block (ram,0x0013f03c) */
/* WARNING: Removing unreachable block (ram,0x0013f040) */
/* WARNING: Removing unreachable block (ram,0x00083a38) */
/* WARNING: Removing unreachable block (ram,0x00083a60) */
/* WARNING: Removing unreachable block (ram,0x00083a64) */
/* WARNING: Removing unreachable block (ram,0x000bf0ac) */
/* WARNING: Removing unreachable block (ram,0x00083a68) */
/* WARNING: Removing unreachable block (ram,0x000bf0b4) */
/* WARNING: Removing unreachable block (ram,0x000bf304) */
/* WARNING: Removing unreachable block (ram,0x000bf308) */
/* WARNING: Removing unreachable block (ram,0x00083a6c) */
/* WARNING: Removing unreachable block (ram,0x000ff0b4) */
/* WARNING: Removing unreachable block (ram,0x000ff090) */
/* WARNING: Removing unreachable block (ram,0x000ff08c) */
/* WARNING: Removing unreachable block (ram,0x000ff094) */
/* WARNING: Removing unreachable block (ram,0x000ff098) */
/* WARNING: Removing unreachable block (ram,0x0043ad20) */
/* WARNING: Removing unreachable block (ram,0x0013f318) */
/* WARNING: Removing unreachable block (ram,0x0013f31c) */
/* WARNING: Removing unreachable block (ram,0x001baba4) */
/* WARNING: Removing unreachable block (ram,0x004aa3ac) */
/* WARNING: Removing unreachable block (ram,0x0013f320) */
/* WARNING: Removing unreachable block (ram,0xff17ae1c) */
/* WARNING: Removing unreachable block (ram,0x0013f324) */
/* WARNING: Removing unreachable block (ram,0x0013f328) */
/* WARNING: Removing unreachable block (ram,0x001baba8) */
/* WARNING: Removing unreachable block (ram,0x001babac) */
/* WARNING: Removing unreachable block (ram,0x001babb4) */
/* WARNING: Removing unreachable block (ram,0x004e23bc) */
/* WARNING: Removing unreachable block (ram,0x001babb8) */
/* WARNING: Removing unreachable block (ram,0x001babbc) */
/* WARNING: Removing unreachable block (ram,0x001babc0) */
/* WARNING: Removing unreachable block (ram,0x001babc4) */
/* WARNING: Removing unreachable block (ram,0x001babc8) */
/* WARNING: Removing unreachable block (ram,0x001ba6f0) */
/* WARNING: Removing unreachable block (ram,0x0013f0a0) */
/* WARNING: Removing unreachable block (ram,0x0013f0a4) */
/* WARNING: Removing unreachable block (ram,0x0013f0a8) */
/* WARNING: Removing unreachable block (ram,0x0117a974) */
/* WARNING: Removing unreachable block (ram,0x0013f098) */
/* WARNING: Removing unreachable block (ram,0x0013f09c) */
/* WARNING: Removing unreachable block (ram,0x0017a6ec) */
/* WARNING: Removing unreachable block (ram,0x0017a6f8) */
/* WARNING: Removing unreachable block (ram,0x0017a6fc) */
/* WARNING: Removing unreachable block (ram,0x0017a700) */
/* WARNING: Removing unreachable block (ram,0x0017a704) */
/* WARNING: Removing unreachable block (ram,0x012361cc) */
/* WARNING: Removing unreachable block (ram,0xff1faba8) */
/* WARNING: Removing unreachable block (ram,0x0013f0c8) */
/* WARNING: Removing unreachable block (ram,0x0013f0ac) */
/* WARNING: Removing unreachable block (ram,0x0013f0b0) */
/* WARNING: Removing unreachable block (ram,0xff1babac) */
/* WARNING: Removing unreachable block (ram,0x0013f0b4) */
/* WARNING: Removing unreachable block (ram,0x0013f0b8) */
/* WARNING: Removing unreachable block (ram,0x0013f0bc) */
/* WARNING: Removing unreachable block (ram,0x0013f0c0) */
/* WARNING: Removing unreachable block (ram,0x0140fb0c) */
/* WARNING: Removing unreachable block (ram,0x0013f0c4) */
/* WARNING: Removing unreachable block (ram,0x0057a90c) */
/* WARNING: Removing unreachable block (ram,0x0057a900) */
/* WARNING: Removing unreachable block (ram,0x001ba6f4) */
/* WARNING: Removing unreachable block (ram,0x0017a708) */
/* WARNING: Removing unreachable block (ram,0x0017a70c) */
/* WARNING: Removing unreachable block (ram,0x0017a710) */
/* WARNING: Removing unreachable block (ram,0x0017a714) */
/* WARNING: Removing unreachable block (ram,0x0017a718) */
/* WARNING: Removing unreachable block (ram,0x0017a71c) */
/* WARNING: Removing unreachable block (ram,0xff17ab80) */
/* WARNING: Removing unreachable block (ram,0x000bf0f0) */
/* WARNING: Removing unreachable block (ram,0x0013f100) */
/* WARNING: Removing unreachable block (ram,0x0013f0f8) */
/* WARNING: Removing unreachable block (ram,0x0013f0fc) */
/* WARNING: Removing unreachable block (ram,0x0013f104) */
/* WARNING: Removing unreachable block (ram,0x004fa8d4) */
/* WARNING: Removing unreachable block (ram,0x000bf0e4) */
/* WARNING: Removing unreachable block (ram,0x000bf0e8) */
/* WARNING: Removing unreachable block (ram,0x0013a720) */
/* WARNING: Removing unreachable block (ram,0x0013a724) */
/* WARNING: Removing unreachable block (ram,0x0013a728) */
/* WARNING: Removing unreachable block (ram,0x0013f0cc) */
/* WARNING: Removing unreachable block (ram,0x0013f0d0) */
/* WARNING: Removing unreachable block (ram,0x0013f0d4) */
/* WARNING: Removing unreachable block (ram,0x0118fae4) */
/* WARNING: Removing unreachable block (ram,0x0013f0d8) */
/* WARNING: Removing unreachable block (ram,0x0013f130) */
/* WARNING: Removing unreachable block (ram,0x0013f134) */
/* WARNING: Removing unreachable block (ram,0x000bf1b4) */
/* WARNING: Removing unreachable block (ram,0x000bf1ac) */
/* WARNING: Removing unreachable block (ram,0x000bf1b0) */
/* WARNING: Removing unreachable block (ram,0xfed0dc78) */
/* WARNING: Removing unreachable block (ram,0x000bf1b8) */
/* WARNING: Removing unreachable block (ram,0x000bf1bc) */
/* WARNING: Removing unreachable block (ram,0x000bf1c0) */
/* WARNING: Removing unreachable block (ram,0x000bf1c4) */
/* WARNING: Removing unreachable block (ram,0x000bf1c8) */
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
/* WARNING: Removing unreachable block (ram,0x000bf18c) */
/* WARNING: Removing unreachable block (ram,0x000bf204) */
/* WARNING: Removing unreachable block (ram,0x000ff20c) */
/* WARNING: Removing unreachable block (ram,0x0013f5f0) */
/* WARNING: Removing unreachable block (ram,0x000bf128) */
/* WARNING: Removing unreachable block (ram,0x00083b1c) */
/* WARNING: Removing unreachable block (ram,0x00083b20) */
/* WARNING: Removing unreachable block (ram,0x00083b24) */
/* WARNING: Removing unreachable block (ram,0x000ff12c) */
/* WARNING: Removing unreachable block (ram,0x000bf260) */
/* WARNING: Removing unreachable block (ram,0x0013f26c) */
/* WARNING: Removing unreachable block (ram,0x00083c1c) */
/* WARNING: Removing unreachable block (ram,0x00083c20) */
/* WARNING: Removing unreachable block (ram,0x0017f628) */
/* WARNING: Removing unreachable block (ram,0x00083c24) */
/* WARNING: Removing unreachable block (ram,0x0017f630) */
/* WARNING: Removing unreachable block (ram,0x0017f634) */
/* WARNING: Removing unreachable block (ram,0x0017f638) */
/* WARNING: Removing unreachable block (ram,0x0017f63c) */
/* WARNING: Removing unreachable block (ram,0x0017f640) */
/* WARNING: Removing unreachable block (ram,0x0017f644) */
/* WARNING: Removing unreachable block (ram,0x0017f648) */
/* WARNING: Removing unreachable block (ram,0x0017f64c) */
/* WARNING: Removing unreachable block (ram,0xfebe5a6c) */
/* WARNING: Removing unreachable block (ram,0x0017f654) */
/* WARNING: Removing unreachable block (ram,0x0017f658) */
/* WARNING: Removing unreachable block (ram,0x0013f244) */
/* WARNING: Removing unreachable block (ram,0x0013f23c) */
/* WARNING: Removing unreachable block (ram,0x0013f240) */
/* WARNING: Removing unreachable block (ram,0x0013f248) */
/* WARNING: Removing unreachable block (ram,0x00083c44) */
/* WARNING: Removing unreachable block (ram,0x010bf71c) */
/* WARNING: Removing unreachable block (ram,0x00083c48) */
/* WARNING: Removing unreachable block (ram,0x00083c4c) */
/* WARNING: Removing unreachable block (ram,0x00083c50) */
/* WARNING: Removing unreachable block (ram,0x00083c54) */
/* WARNING: Removing unreachable block (ram,0x00083c58) */
/* WARNING: Removing unreachable block (ram,0x00083c5c) */
/* WARNING: Removing unreachable block (ram,0x010bf734) */
/* WARNING: Removing unreachable block (ram,0x00083c60) */
/* WARNING: Removing unreachable block (ram,0x000ff2a8) */
/* WARNING: Removing unreachable block (ram,0x000ff2ac) */
/* WARNING: Removing unreachable block (ram,0x00083c28) */
/* WARNING: Removing unreachable block (ram,0x00083c2c) */
/* WARNING: Removing unreachable block (ram,0x00083c30) */
/* WARNING: Removing unreachable block (ram,0x000ff238) */
/* WARNING: Removing unreachable block (ram,0x00083c34) */
/* WARNING: Removing unreachable block (ram,0x00083c38) */
/* WARNING: Removing unreachable block (ram,0x00083c3c) */
/* WARNING: Removing unreachable block (ram,0x00083c40) */
/* WARNING: Removing unreachable block (ram,0x000bf288) */
/* WARNING: Removing unreachable block (ram,0x000bf234) */
/* WARNING: Removing unreachable block (ram,0x00083b28) */
/* WARNING: Removing unreachable block (ram,0x00083b2c) */
/* WARNING: Removing unreachable block (ram,0x00083b5c) */
/* WARNING: Removing unreachable block (ram,0x00083b60) */
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
/* WARNING: Removing unreachable block (ram,0x00083b78) */
/* WARNING: Removing unreachable block (ram,0x00083b7c) */
/* WARNING: Removing unreachable block (ram,0x000bf184) */
/* WARNING: Removing unreachable block (ram,0x000bf188) */
/* WARNING: Removing unreachable block (ram,0x000bf190) */
/* WARNING: Removing unreachable block (ram,0x000bf198) */
/* WARNING: Removing unreachable block (ram,0x00083b80) */
/* WARNING: Removing unreachable block (ram,0x000ff188) */
/* WARNING: Removing unreachable block (ram,0x00083b84) */
/* WARNING: Removing unreachable block (ram,0x0013f18c) */
/* WARNING: Removing unreachable block (ram,0x0013f190) */
/* WARNING: Removing unreachable block (ram,0x00083b88) */
/* WARNING: Removing unreachable block (ram,0x00083bb0) */
/* WARNING: Removing unreachable block (ram,0x00083bb4) */
/* WARNING: Removing unreachable block (ram,0x00083bb8) */
/* WARNING: Removing unreachable block (ram,0x000bf208) */
/* WARNING: Removing unreachable block (ram,0x000bf20c) */
/* WARNING: Removing unreachable block (ram,0x00083bbc) */
/* WARNING: Removing unreachable block (ram,0x0017f408) */
/* WARNING: Removing unreachable block (ram,0x0017f40c) */
/* WARNING: Removing unreachable block (ram,0x00083bc0) */
/* WARNING: Removing unreachable block (ram,0x000ff210) */
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
/* WARNING: Removing unreachable block (ram,0x00083bd4) */
/* WARNING: Removing unreachable block (ram,0x00083bd8) */
/* WARNING: Removing unreachable block (ram,0x000bf1e0) */
/* WARNING: Removing unreachable block (ram,0x00083bdc) */
/* WARNING: Removing unreachable block (ram,0x000ff1e4) */
/* WARNING: Removing unreachable block (ram,0x00083be0) */
/* WARNING: Removing unreachable block (ram,0x0013f1e8) */
/* WARNING: Removing unreachable block (ram,0x00083be4) */
/* WARNING: Removing unreachable block (ram,0x00083c0c) */
/* WARNING: Removing unreachable block (ram,0x00083c10) */
/* WARNING: Removing unreachable block (ram,0x00083c14) */
/* WARNING: Removing unreachable block (ram,0x004bf428) */
/* WARNING: Removing unreachable block (ram,0x00083c18) */
/* WARNING: Removing unreachable block (ram,0x000ff268) */
/* WARNING: Removing unreachable block (ram,0x000bf1a0) */
/* WARNING: Removing unreachable block (ram,0x004bf378) */
/* WARNING: Removing unreachable block (ram,0x0113f3f8) */
/* WARNING: Removing unreachable block (ram,0x00083b08) */
/* WARNING: Removing unreachable block (ram,0x00083b0c) */
/* WARNING: Removing unreachable block (ram,0x00083b10) */
/* WARNING: Removing unreachable block (ram,0x00083b14) */
/* WARNING: Removing unreachable block (ram,0x0013f160) */
/* WARNING: Removing unreachable block (ram,0x0013f164) */
/* WARNING: Removing unreachable block (ram,0x00083b18) */
/* WARNING: Removing unreachable block (ram,0x010ff3f0) */
/* WARNING: Removing unreachable block (ram,0x010bf3e0) */
/* WARNING: Removing unreachable block (ram,0x00083ac4) */
/* WARNING: Removing unreachable block (ram,0x00083ac8) */
/* WARNING: Removing unreachable block (ram,0x00083af8) */
/* WARNING: Removing unreachable block (ram,0x00083afc) */
/* WARNING: Removing unreachable block (ram,0x00083b00) */
/* WARNING: Removing unreachable block (ram,0x000bf14c) */
/* WARNING: Removing unreachable block (ram,0x000bf150) */
/* WARNING: Removing unreachable block (ram,0x000bf154) */
/* WARNING: Removing unreachable block (ram,0x00083b04) */
/* WARNING: Removing unreachable block (ram,0x000ff154) */
/* WARNING: Removing unreachable block (ram,0x00083a70) */
/* WARNING: Removing unreachable block (ram,0x00083a74) */
/* WARNING: Removing unreachable block (ram,0x00083a78) */
/* WARNING: Removing unreachable block (ram,0x00083a7c) */
/* WARNING: Removing unreachable block (ram,0x00083a80) */
/* WARNING: Removing unreachable block (ram,0x00083a84) */
/* WARNING: Removing unreachable block (ram,0x00083a88) */
/* WARNING: Removing unreachable block (ram,0x00083a8c) */
/* WARNING: Removing unreachable block (ram,0x0013f314) */
/* WARNING: Removing unreachable block (ram,0xff1badfc) */
/* WARNING: Removing unreachable block (ram,0x00083a90) */
/* WARNING: Removing unreachable block (ram,0x00083a94) */
/* WARNING: Removing unreachable block (ram,0x00083a98) */
/* WARNING: Removing unreachable block (ram,0x00083a9c) */
/* WARNING: Removing unreachable block (ram,0x00083aa0) */
/* WARNING: Removing unreachable block (ram,0x00083aa4) */
/* WARNING: Removing unreachable block (ram,0x00083aa8) */
/* WARNING: Removing unreachable block (ram,0x000ff0f4) */
/* WARNING: Removing unreachable block (ram,0x000ff0f8) */
/* WARNING: Removing unreachable block (ram,0x00083aac) */
/* WARNING: Removing unreachable block (ram,0x00083ab0) */
/* WARNING: Removing unreachable block (ram,0x010bf57c) */
/* WARNING: Removing unreachable block (ram,0x00083ab4) */
/* WARNING: Removing unreachable block (ram,0x010ff580) */
/* WARNING: Removing unreachable block (ram,0x00083ab8) */
/* WARNING: Removing unreachable block (ram,0x0113f584) */
/* WARNING: Removing unreachable block (ram,0x00083abc) */
/* WARNING: Removing unreachable block (ram,0x000bf0c4) */
/* WARNING: Removing unreachable block (ram,0x01ac190c) */
/* WARNING: Removing unreachable block (ram,0x000bf0c8) */
/* WARNING: Removing unreachable block (ram,0x000bf0cc) */
/* WARNING: Removing unreachable block (ram,0x000bf0d0) */
/* WARNING: Removing unreachable block (ram,0xff13abb8) */
/* WARNING: Removing unreachable block (ram,0x000bf0d4) */
/* WARNING: Removing unreachable block (ram,0x000bf0d8) */
/* WARNING: Removing unreachable block (ram,0x000bf0dc) */
/* WARNING: Removing unreachable block (ram,0x000bf0e0) */
/* WARNING: Removing unreachable block (ram,0x000fa71c) */
/* WARNING: Removing unreachable block (ram,0x00083ac0) */
/* WARNING: Removing unreachable block (ram,0x000ff0c8) */
/* WARNING: Removing unreachable block (ram,0x000ff0cc) */
/* WARNING: Removing unreachable block (ram,0x000ff0d0) */
/* WARNING: Removing unreachable block (ram,0xff0fabb4) */
/* WARNING: Removing unreachable block (ram,0x000ff308) */
/* WARNING: Removing unreachable block (ram,0x000bf080) */
/* WARNING: Removing unreachable block (ram,0x000bf084) */
/* WARNING: Removing unreachable block (ram,0x000bf088) */
/* WARNING: Removing unreachable block (ram,0x00083a10) */
/* WARNING: Removing unreachable block (ram,0x000ff05c) */
/* WARNING: Removing unreachable block (ram,0x000ff060) */
/* WARNING: Removing unreachable block (ram,0x0013f2b8) */
/* WARNING: Removing unreachable block (ram,0x0013f2bc) */
/* WARNING: Removing unreachable block (ram,0x0013f2c0) */
/* WARNING: Removing unreachable block (ram,0x0013f2c4) */
/* WARNING: Removing unreachable block (ram,0x0017a8fc) */
/* WARNING: Removing unreachable block (ram,0x000839b8) */
/* WARNING: Removing unreachable block (ram,0x000839bc) */
/* WARNING: Removing unreachable block (ram,0x0013f004) */
/* WARNING: Removing unreachable block (ram,0x0013f008) */
/* WARNING: Removing unreachable block (ram,0x0013f00c) */
/* WARNING: Removing unreachable block (ram,0x0013f010) */
/* WARNING: Removing unreachable block (ram,0x0013f014) */
/* WARNING: Removing unreachable block (ram,0x0013f018) */
/* WARNING: Removing unreachable block (ram,0x0013f01c) */
/* WARNING: Removing unreachable block (ram,0x0013f020) */
/* WARNING: Removing unreachable block (ram,0x0013f024) */
/* WARNING: Removing unreachable block (ram,0x0013f028) */
/* WARNING: Removing unreachable block (ram,0x000839c0) */
/* WARNING: Removing unreachable block (ram,0x000839c4) */
/* WARNING: Removing unreachable block (ram,0x000839c8) */
/* WARNING: Removing unreachable block (ram,0x000839cc) */
/* WARNING: Removing unreachable block (ram,0x000839d0) */
/* WARNING: Removing unreachable block (ram,0x000839d4) */
/* WARNING: Removing unreachable block (ram,0x010bf2a4) */
/* WARNING: Removing unreachable block (ram,0x000839d8) */
/* WARNING: Removing unreachable block (ram,0x0013efe0) */
/* WARNING: Removing unreachable block (ram,0x000839dc) */
/* WARNING: Removing unreachable block (ram,0x00083a04) */
/* WARNING: Removing unreachable block (ram,0x00083a08) */
/* WARNING: Removing unreachable block (ram,0x00083a0c) */
/* WARNING: Removing unreachable block (ram,0x000bf058) */
/* WARNING: Removing unreachable block (ram,0x000fefd4) */
/* WARNING: Removing unreachable block (ram,0x010bf294) */
/* WARNING: Removing unreachable block (ram,0x000befc8) */
/* WARNING: Removing unreachable block (ram,0x0017d604) */
/* WARNING: Removing unreachable block (ram,0x0017ae08) */
/* WARNING: Removing unreachable block (ram,0x0017c8f0) */
/* WARNING: Removing unreachable block (ram,0x0017c8de) */
/* WARNING: Removing unreachable block (ram,0xff0faaec) */
/* WARNING: Removing unreachable block (ram,0x009fa64c) */
/* WARNING: Removing unreachable block (ram,0x004fa808) */
/* WARNING: Removing unreachable block (ram,0x009ba648) */
/* WARNING: Removing unreachable block (ram,0x000bf008) */
/* AbyssEngine::AEMath::Vector::operator[](int) */

void AbyssEngine::AEMath::Vector::operator[](int param_1)

{
  float *pfVar1;
  float *extraout_r1;
  int extraout_r2;
  uint *unaff_r8;
  undefined1 in_NG;
  undefined1 in_ZR;
  undefined1 in_CY;
  undefined4 in_cr4;
  float extraout_s2;
  float extraout_s4;
  
  pfVar1 = (float *)Engine::LightEnable(SUB41(param_1,0));
  if ((bool)in_NG) {
    pfVar1 = (float *)(uint)*(byte *)((int)pfVar1 + (int)extraout_r1 * -2);
  }
  coprocessor_load(7,in_cr4,pfVar1 + 0x70);
  pfVar1[2] = extraout_s4;
  pfVar1[1] = *extraout_r1 + *pfVar1 + extraout_s2;
  pfVar1[2] = extraout_r1[1] + extraout_s4;
  if ((bool)in_NG) {
    pfVar1 = (float *)(uint)*(byte *)(pfVar1 + extraout_r2 * -0x40000);
  }
  if (!(bool)in_ZR) {
    if (!(bool)in_CY) {
      if ((bool)in_NG) {
        *unaff_r8 = (uint)pfVar1;
      }
                    /* WARNING: Could not recover jumptable at 0x000bf008. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  00083960: addeq lr,r1,r0, lsl #0x16
  00083964: bl 0x0009572c
  00083968: ldrbmi r0,[r0,-r1,lsl #0x1]!
```
