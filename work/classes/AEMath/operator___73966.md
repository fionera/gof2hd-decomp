# AbyssEngine::AEMath::Vector::operator[]
elf 0x73966 body 1
Sig: undefined __stdcall operator[](int param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0017f28c) overlaps instruction at (ram,0x0017f28a)
    */
/* WARNING: Possible PIC construction at 0x00083a96: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000fefa2) */
/* WARNING: Removing unreachable block (ram,0x0013efae) */
/* WARNING: Removing unreachable block (ram,0x0013efbe) */
/* WARNING: Removing unreachable block (ram,0x0013efb2) */
/* WARNING: Removing unreachable block (ram,0x0013efba) */
/* WARNING: Removing unreachable block (ram,0x0013efbc) */
/* WARNING: Removing unreachable block (ram,0x0013efc0) */
/* WARNING: Removing unreachable block (ram,0x0013f02e) */
/* WARNING: Removing unreachable block (ram,0x0013f03e) */
/* WARNING: Removing unreachable block (ram,0x0013f03a) */
/* WARNING: Removing unreachable block (ram,0x000ff25e) */
/* WARNING: Removing unreachable block (ram,0x000ff262) */
/* WARNING: Removing unreachable block (ram,0x000ff266) */
/* WARNING: Removing unreachable block (ram,0x000ff26a) */
/* WARNING: Removing unreachable block (ram,0x000ff26e) */
/* WARNING: Removing unreachable block (ram,0x000ff272) */
/* WARNING: Removing unreachable block (ram,0x000ff276) */
/* WARNING: Removing unreachable block (ram,0x001fa626) */
/* WARNING: Removing unreachable block (ram,0x001fa62a) */
/* WARNING: Removing unreachable block (ram,0x001fa62e) */
/* WARNING: Removing unreachable block (ram,0x001fa632) */
/* WARNING: Removing unreachable block (ram,0x001fa636) */
/* WARNING: Removing unreachable block (ram,0x001fa63a) */
/* WARNING: Removing unreachable block (ram,0x00c51b06) */
/* WARNING: Removing unreachable block (ram,0x001fa63e) */
/* WARNING: Removing unreachable block (ram,0x001fa646) */
/* WARNING: Removing unreachable block (ram,0x0013efea) */
/* WARNING: Removing unreachable block (ram,0x0013efee) */
/* WARNING: Removing unreachable block (ram,0x0023a62a) */
/* WARNING: Removing unreachable block (ram,0x0013eff2) */
/* WARNING: Removing unreachable block (ram,0x0013eff6) */
/* WARNING: Removing unreachable block (ram,0x0013effa) */
/* WARNING: Removing unreachable block (ram,0x0017f276) */
/* WARNING: Removing unreachable block (ram,0x000839e6) */
/* WARNING: Removing unreachable block (ram,0x000ff02e) */
/* WARNING: Removing unreachable block (ram,0x000839ea) */
/* WARNING: Removing unreachable block (ram,0x0013f032) */
/* WARNING: Removing unreachable block (ram,0x000839ee) */
/* WARNING: Removing unreachable block (ram,0x000839f2) */
/* WARNING: Removing unreachable block (ram,0x000839f6) */
/* WARNING: Removing unreachable block (ram,0x000839fa) */
/* WARNING: Removing unreachable block (ram,0x000bf002) */
/* WARNING: Removing unreachable block (ram,0x000839fe) */
/* WARNING: Removing unreachable block (ram,0x000ff006) */
/* WARNING: Removing unreachable block (ram,0x000ff00a) */
/* WARNING: Removing unreachable block (ram,0x000ff00e) */
/* WARNING: Removing unreachable block (ram,0x000ff012) */
/* WARNING: Removing unreachable block (ram,0x000ff01a) */
/* WARNING: Removing unreachable block (ram,0x0017f27e) */
/* WARNING: Removing unreachable block (ram,0x0017f282) */
/* WARNING: Removing unreachable block (ram,0x0017f286) */
/* WARNING: Removing unreachable block (ram,0x0017f28a) */
/* WARNING: Removing unreachable block (ram,0x0017f28e) */
/* WARNING: Removing unreachable block (ram,0x0017f294) */
/* WARNING: Removing unreachable block (ram,0x0017f2a4) */
/* WARNING: Removing unreachable block (ram,0x0017f2bc) */
/* WARNING: Removing unreachable block (ram,0x0017f2c6) */
/* WARNING: Removing unreachable block (ram,0x0017f2ce) */
/* WARNING: Removing unreachable block (ram,0x0017ff8c) */
/* WARNING: Removing unreachable block (ram,0x0017ff9c) */
/* WARNING: Removing unreachable block (ram,0x0017e506) */
/* WARNING: Removing unreachable block (ram,0x0017e524) */
/* WARNING: Removing unreachable block (ram,0x0017e510) */
/* WARNING: Removing unreachable block (ram,0x0017f28c) */
/* WARNING: Removing unreachable block (ram,0x00083986) */
/* WARNING: Removing unreachable block (ram,0x0008398a) */
/* WARNING: Removing unreachable block (ram,0x0013efd2) */
/* WARNING: Removing unreachable block (ram,0x0013efca) */
/* WARNING: Removing unreachable block (ram,0x0008398e) */
/* WARNING: Removing unreachable block (ram,0x00083992) */
/* WARNING: Removing unreachable block (ram,0x00083996) */
/* WARNING: Removing unreachable block (ram,0x0008399a) */
/* WARNING: Removing unreachable block (ram,0x0008399e) */
/* WARNING: Removing unreachable block (ram,0x000839a2) */
/* WARNING: Removing unreachable block (ram,0x000839a6) */
/* WARNING: Removing unreachable block (ram,0x000839aa) */
/* WARNING: Removing unreachable block (ram,0x000839da) */
/* WARNING: Removing unreachable block (ram,0x000839de) */
/* WARNING: Removing unreachable block (ram,0x000839e2) */
/* WARNING: Removing unreachable block (ram,0x004bf1f6) */
/* WARNING: Removing unreachable block (ram,0x00083a96) */
/* WARNING: Removing unreachable block (ram,0x000bf3ae) */
/* WARNING: Removing unreachable block (ram,0x00083aea) */
/* WARNING: Removing unreachable block (ram,0x0013f3c6) */
/* WARNING: Removing unreachable block (ram,0x0013f3ca) */
/* WARNING: Removing unreachable block (ram,0x0013f3de) */
/* WARNING: Removing unreachable block (ram,0x0013f3e6) */
/* WARNING: Removing unreachable block (ram,0x0013f3ea) */
/* WARNING: Removing unreachable block (ram,0x0013f3f0) */
/* WARNING: Removing unreachable block (ram,0x0013fe6c) */
/* WARNING: Removing unreachable block (ram,0x0013f3fa) */
/* WARNING: Removing unreachable block (ram,0x0013f91a) */
/* WARNING: Removing unreachable block (ram,0x0013fe74) */
/* WARNING: Removing unreachable block (ram,0x0013fe7a) */
/* WARNING: Removing unreachable block (ram,0x0013fe80) */
/* WARNING: Removing unreachable block (ram,0x0013fe8a) */
/* WARNING: Removing unreachable block (ram,0x0013fe90) */
/* WARNING: Removing unreachable block (ram,0x0013feb4) */
/* WARNING: Removing unreachable block (ram,0x0013fec6) */
/* WARNING: Removing unreachable block (ram,0x0013fed0) */
/* WARNING: Removing unreachable block (ram,0x0013ff8e) */
/* WARNING: Removing unreachable block (ram,0x0013fed4) */
/* WARNING: Removing unreachable block (ram,0x0013feda) */
/* WARNING: Removing unreachable block (ram,0x0013fef2) */
/* WARNING: Removing unreachable block (ram,0x00140086) */
/* WARNING: Removing unreachable block (ram,0x00140090) */
/* WARNING: Removing unreachable block (ram,0x001400b4) */
/* WARNING: Removing unreachable block (ram,0x001400fa) */
/* WARNING: Removing unreachable block (ram,0x00140104) */
/* WARNING: Removing unreachable block (ram,0x0014011a) */
/* WARNING: Removing unreachable block (ram,0x00140124) */
/* WARNING: Removing unreachable block (ram,0x0014013a) */
/* WARNING: Removing unreachable block (ram,0x00140146) */
/* WARNING: Removing unreachable block (ram,0x001405da) */
/* WARNING: Removing unreachable block (ram,0x00140152) */
/* WARNING: Removing unreachable block (ram,0x00140156) */
/* WARNING: Removing unreachable block (ram,0x0014015c) */
/* WARNING: Removing unreachable block (ram,0x0014016a) */
/* WARNING: Removing unreachable block (ram,0x0013fee0) */
/* WARNING: Removing unreachable block (ram,0x0013fefe) */
/* WARNING: Removing unreachable block (ram,0x0013ff06) */
/* WARNING: Removing unreachable block (ram,0x0013ff1c) */
/* WARNING: Removing unreachable block (ram,0x0013ff22) */
/* WARNING: Removing unreachable block (ram,0x0013ff28) */
/* WARNING: Removing unreachable block (ram,0x0013ff30) */
/* WARNING: Removing unreachable block (ram,0x0013ff94) */
/* WARNING: Removing unreachable block (ram,0x0013ff34) */
/* WARNING: Removing unreachable block (ram,0x0013ff3a) */
/* WARNING: Removing unreachable block (ram,0x0013ff42) */
/* WARNING: Removing unreachable block (ram,0x0013ffca) */
/* WARNING: Removing unreachable block (ram,0x0013ffd4) */
/* WARNING: Removing unreachable block (ram,0x0013ff48) */
/* WARNING: Removing unreachable block (ram,0x0013ffd6) */
/* WARNING: Removing unreachable block (ram,0x0013ffdc) */
/* WARNING: Removing unreachable block (ram,0x00140008) */
/* WARNING: Removing unreachable block (ram,0x00140012) */
/* WARNING: Removing unreachable block (ram,0x00140016) */
/* WARNING: Removing unreachable block (ram,0x00140536) */
/* WARNING: Removing unreachable block (ram,0x0013fffa) */
/* WARNING: Removing unreachable block (ram,0x00140022) */
/* WARNING: Removing unreachable block (ram,0x0014002a) */
/* WARNING: Removing unreachable block (ram,0x0014007e) */
/* WARNING: Removing unreachable block (ram,0x0013fe94) */
/* WARNING: Removing unreachable block (ram,0x0013fe9a) */
/* WARNING: Removing unreachable block (ram,0x0013fe9c) */
/* WARNING: Removing unreachable block (ram,0x0013fe9e) */
/* WARNING: Removing unreachable block (ram,0x0013fee4) */
/* WARNING: Removing unreachable block (ram,0x0013fea4) */
/* WARNING: Removing unreachable block (ram,0x00140080) */
/* WARNING: Removing unreachable block (ram,0x0013f40e) */
/* WARNING: Removing unreachable block (ram,0x0013f412) */
/* WARNING: Removing unreachable block (ram,0x0013f418) */
/* WARNING: Removing unreachable block (ram,0x0013f41c) */
/* WARNING: Removing unreachable block (ram,0x0013f424) */
/* WARNING: Removing unreachable block (ram,0x0013f8a6) */
/* WARNING: Removing unreachable block (ram,0x0013f8ac) */
/* WARNING: Removing unreachable block (ram,0x0013f8ae) */
/* WARNING: Removing unreachable block (ram,0x0013f426) */
/* WARNING: Removing unreachable block (ram,0x0013f430) */
/* WARNING: Removing unreachable block (ram,0x0013f81c) */
/* WARNING: Removing unreachable block (ram,0x0013f458) */
/* WARNING: Removing unreachable block (ram,0x0013f46c) */
/* WARNING: Removing unreachable block (ram,0x0013f480) */
/* WARNING: Removing unreachable block (ram,0x0013f488) */
/* WARNING: Removing unreachable block (ram,0x0013f5ac) */
/* WARNING: Removing unreachable block (ram,0x0013f5ba) */
/* WARNING: Removing unreachable block (ram,0x0013f5c6) */
/* WARNING: Removing unreachable block (ram,0x0013f5e0) */
/* WARNING: Removing unreachable block (ram,0x0013f816) */
/* WARNING: Removing unreachable block (ram,0x0013f824) */
/* WARNING: Removing unreachable block (ram,0x0013f82e) */
/* WARNING: Removing unreachable block (ram,0x0013f832) */
/* WARNING: Removing unreachable block (ram,0x0013f83a) */
/* WARNING: Removing unreachable block (ram,0x0013f83e) */
/* WARNING: Removing unreachable block (ram,0x0013f840) */
/* WARNING: Removing unreachable block (ram,0x0013f846) */
/* WARNING: Removing unreachable block (ram,0x0013f84c) */
/* WARNING: Removing unreachable block (ram,0x0013f850) */
/* WARNING: Removing unreachable block (ram,0x0013f89c) */
/* WARNING: Removing unreachable block (ram,0x0013f856) */
/* WARNING: Removing unreachable block (ram,0x0013f8b0) */
/* WARNING: Removing unreachable block (ram,0x0013f86c) */
/* WARNING: Removing unreachable block (ram,0x0013f872) */
/* WARNING: Removing unreachable block (ram,0x0013f884) */
/* WARNING: Removing unreachable block (ram,0x0013f892) */
/* WARNING: Removing unreachable block (ram,0x0013f8b2) */
/* WARNING: Removing unreachable block (ram,0x0013f8b8) */
/* WARNING: Removing unreachable block (ram,0x0013f8c8) */
/* WARNING: Removing unreachable block (ram,0x0013f920) */
/* WARNING: Removing unreachable block (ram,0x0013f8cc) */
/* WARNING: Removing unreachable block (ram,0x0013f912) */
/* WARNING: Removing unreachable block (ram,0x0013f922) */
/* WARNING: Removing unreachable block (ram,0x0013f95c) */
/* WARNING: Removing unreachable block (ram,0x0013f96e) */
/* WARNING: Removing unreachable block (ram,0x0013f968) */
/* WARNING: Removing unreachable block (ram,0x0013f972) */
/* WARNING: Removing unreachable block (ram,0x0013f974) */
/* WARNING: Removing unreachable block (ram,0x0013f97a) */
/* WARNING: Removing unreachable block (ram,0x0013f97e) */
/* WARNING: Removing unreachable block (ram,0x0013f986) */
/* WARNING: Removing unreachable block (ram,0x0013fdb4) */
/* WARNING: Removing unreachable block (ram,0x0013fdbe) */
/* WARNING: Removing unreachable block (ram,0x0013fdc2) */
/* WARNING: Removing unreachable block (ram,0x0013f98a) */
/* WARNING: Removing unreachable block (ram,0x0013f996) */
/* WARNING: Removing unreachable block (ram,0x0013f9c0) */
/* WARNING: Removing unreachable block (ram,0x0013f9d0) */
/* WARNING: Removing unreachable block (ram,0x0013f9da) */
/* WARNING: Removing unreachable block (ram,0x0013f99c) */
/* WARNING: Removing unreachable block (ram,0x0013fa7a) */
/* WARNING: Removing unreachable block (ram,0x0013f9a4) */
/* WARNING: Removing unreachable block (ram,0x0013fa7c) */
/* WARNING: Removing unreachable block (ram,0x0013fa94) */
/* WARNING: Removing unreachable block (ram,0x0013faa0) */
/* WARNING: Removing unreachable block (ram,0x0013faac) */
/* WARNING: Removing unreachable block (ram,0x0013fab4) */
/* WARNING: Removing unreachable block (ram,0x0013fabc) */
/* WARNING: Removing unreachable block (ram,0x0013fac8) */
/* WARNING: Removing unreachable block (ram,0x0013fad4) */
/* WARNING: Removing unreachable block (ram,0x0013faf0) */
/* WARNING: Removing unreachable block (ram,0x0013fae2) */
/* WARNING: Removing unreachable block (ram,0x0013fb0c) */
/* WARNING: Removing unreachable block (ram,0x0013faea) */
/* WARNING: Removing unreachable block (ram,0x0013fb14) */
/* WARNING: Removing unreachable block (ram,0x0013fc1e) */
/* WARNING: Removing unreachable block (ram,0x0013fc24) */
/* WARNING: Removing unreachable block (ram,0x0013fc4e) */
/* WARNING: Removing unreachable block (ram,0x0013fc54) */
/* WARNING: Removing unreachable block (ram,0x0013fc5a) */
/* WARNING: Removing unreachable block (ram,0x0013fc64) */
/* WARNING: Removing unreachable block (ram,0x0013fc84) */
/* WARNING: Removing unreachable block (ram,0x0013fc90) */
/* WARNING: Removing unreachable block (ram,0x0013fc92) */
/* WARNING: Removing unreachable block (ram,0x0013fc6a) */
/* WARNING: Removing unreachable block (ram,0x0013fc70) */
/* WARNING: Removing unreachable block (ram,0x0013fc7c) */
/* WARNING: Removing unreachable block (ram,0x0013fc94) */
/* WARNING: Removing unreachable block (ram,0x0013fc9a) */
/* WARNING: Removing unreachable block (ram,0x0013fcaa) */
/* WARNING: Removing unreachable block (ram,0x0013fb1e) */
/* WARNING: Removing unreachable block (ram,0x0013fb3c) */
/* WARNING: Removing unreachable block (ram,0x0013fb46) */
/* WARNING: Removing unreachable block (ram,0x0013fb4e) */
/* WARNING: Removing unreachable block (ram,0x0013fb54) */
/* WARNING: Removing unreachable block (ram,0x0013fcd8) */
/* WARNING: Removing unreachable block (ram,0x0013fb5e) */
/* WARNING: Removing unreachable block (ram,0x0013fb94) */
/* WARNING: Removing unreachable block (ram,0x0013fb9c) */
/* WARNING: Removing unreachable block (ram,0x0013fbac) */
/* WARNING: Removing unreachable block (ram,0x0013fbb4) */
/* WARNING: Removing unreachable block (ram,0x0013fbba) */
/* WARNING: Removing unreachable block (ram,0x0013fbc4) */
/* WARNING: Removing unreachable block (ram,0x0013fbca) */
/* WARNING: Removing unreachable block (ram,0x0013fbd6) */
/* WARNING: Removing unreachable block (ram,0x0013fce2) */
/* WARNING: Removing unreachable block (ram,0x0013fbd8) */
/* WARNING: Removing unreachable block (ram,0x0013fbda) */
/* WARNING: Removing unreachable block (ram,0x0013fbde) */
/* WARNING: Removing unreachable block (ram,0x0013fbe6) */
/* WARNING: Removing unreachable block (ram,0x0013fc0a) */
/* WARNING: Removing unreachable block (ram,0x0013fc10) */
/* WARNING: Removing unreachable block (ram,0x0013fcac) */
/* WARNING: Removing unreachable block (ram,0x0013fcb8) */
/* WARNING: Removing unreachable block (ram,0x0013fcbe) */
/* WARNING: Removing unreachable block (ram,0x0013fcc4) */
/* WARNING: Removing unreachable block (ram,0x0013fcca) */
/* WARNING: Removing unreachable block (ram,0x0013fcd2) */
/* WARNING: Removing unreachable block (ram,0x0013fcea) */
/* WARNING: Removing unreachable block (ram,0x0013fd6e) */
/* WARNING: Removing unreachable block (ram,0x0013fd06) */
/* WARNING: Removing unreachable block (ram,0x0013fd12) */
/* WARNING: Removing unreachable block (ram,0x0013fd66) */
/* WARNING: Removing unreachable block (ram,0x0013fd6a) */
/* WARNING: Removing unreachable block (ram,0x0013fd74) */
/* WARNING: Removing unreachable block (ram,0x0013fd7a) */
/* WARNING: Removing unreachable block (ram,0x0013fd8a) */
/* WARNING: Removing unreachable block (ram,0x0013fd90) */
/* WARNING: Removing unreachable block (ram,0x0013fd96) */
/* WARNING: Removing unreachable block (ram,0x0013fda0) */
/* WARNING: Removing unreachable block (ram,0x0013fdc4) */
/* WARNING: Removing unreachable block (ram,0x0013fda6) */
/* WARNING: Removing unreachable block (ram,0x0013fdcc) */
/* WARNING: Removing unreachable block (ram,0x0013fdac) */
/* WARNING: Removing unreachable block (ram,0x0013fdd0) */
/* WARNING: Removing unreachable block (ram,0x0013fdda) */
/* WARNING: Removing unreachable block (ram,0x0013fde0) */
/* WARNING: Removing unreachable block (ram,0x0013fde6) */
/* WARNING: Removing unreachable block (ram,0x0013fde8) */
/* WARNING: Removing unreachable block (ram,0x0013fdf8) */
/* WARNING: Removing unreachable block (ram,0x0013fdee) */
/* WARNING: Removing unreachable block (ram,0x0013fdfc) */
/* WARNING: Removing unreachable block (ram,0x0013fdf4) */
/* WARNING: Removing unreachable block (ram,0x0013fe04) */
/* WARNING: Removing unreachable block (ram,0x0013fe0a) */
/* WARNING: Removing unreachable block (ram,0x0013fe34) */
/* WARNING: Removing unreachable block (ram,0x0013fe4c) */
/* WARNING: Removing unreachable block (ram,0x0013ff4c) */
/* WARNING: Removing unreachable block (ram,0x0013ff56) */
/* WARNING: Removing unreachable block (ram,0x0013ff5c) */
/* WARNING: Removing unreachable block (ram,0x0013ff74) */
/* WARNING: Removing unreachable block (ram,0x0013ff86) */
/* WARNING: Removing unreachable block (ram,0x00140192) */
/* WARNING: Removing unreachable block (ram,0x0013fe50) */
/* WARNING: Removing unreachable block (ram,0x00140184) */
/* WARNING: Removing unreachable block (ram,0x0014019e) */
/* WARNING: Removing unreachable block (ram,0x0013fe5a) */
/* WARNING: Removing unreachable block (ram,0x0013ff8a) */
/* WARNING: Removing unreachable block (ram,0x001401a4) */
/* WARNING: Removing unreachable block (ram,0x001401a8) */
/* WARNING: Removing unreachable block (ram,0x001401ae) */
/* WARNING: Removing unreachable block (ram,0x001401b2) */
/* WARNING: Removing unreachable block (ram,0x001401ba) */
/* WARNING: Removing unreachable block (ram,0x001401bc) */
/* WARNING: Removing unreachable block (ram,0x0014021c) */
/* WARNING: Removing unreachable block (ram,0x001401c2) */
/* WARNING: Removing unreachable block (ram,0x00140214) */
/* WARNING: Removing unreachable block (ram,0x0014022a) */
/* WARNING: Removing unreachable block (ram,0x0013fe16) */
/* WARNING: Removing unreachable block (ram,0x0013fe1a) */
/* WARNING: Removing unreachable block (ram,0x0013fe1c) */
/* WARNING: Removing unreachable block (ram,0x0013fe1e) */
/* WARNING: Removing unreachable block (ram,0x0013fe2a) */
/* WARNING: Removing unreachable block (ram,0x0013fe22) */
/* WARNING: Removing unreachable block (ram,0x00140264) */
/* WARNING: Removing unreachable block (ram,0x00140278) */
/* WARNING: Removing unreachable block (ram,0x001402f0) */
/* WARNING: Removing unreachable block (ram,0x00140302) */
/* WARNING: Removing unreachable block (ram,0x00140310) */
/* WARNING: Removing unreachable block (ram,0x0014031e) */
/* WARNING: Removing unreachable block (ram,0x0014038e) */
/* WARNING: Removing unreachable block (ram,0x001403a0) */
/* WARNING: Removing unreachable block (ram,0x00140598) */
/* WARNING: Removing unreachable block (ram,0x001405a4) */
/* WARNING: Removing unreachable block (ram,0x001405b0) */
/* WARNING: Removing unreachable block (ram,0x001405ee) */
/* WARNING: Removing unreachable block (ram,0x0014060a) */
/* WARNING: Removing unreachable block (ram,0x0014066c) */
/* WARNING: Removing unreachable block (ram,0x0014068c) */
/* WARNING: Removing unreachable block (ram,0x00140678) */
/* WARNING: Removing unreachable block (ram,0x0014061a) */
/* WARNING: Removing unreachable block (ram,0x00140620) */
/* WARNING: Removing unreachable block (ram,0x0014062e) */
/* WARNING: Removing unreachable block (ram,0x0014064a) */
/* WARNING: Removing unreachable block (ram,0x00140662) */
/* WARNING: Removing unreachable block (ram,0x00140666) */
/* WARNING: Removing unreachable block (ram,0x0014066a) */
/* WARNING: Removing unreachable block (ram,0x00140680) */
/* WARNING: Removing unreachable block (ram,0x001405fe) */
/* WARNING: Removing unreachable block (ram,0x001405ce) */
/* WARNING: Removing unreachable block (ram,0x00140698) */
/* WARNING: Removing unreachable block (ram,0x001403b2) */
/* WARNING: Removing unreachable block (ram,0x00140280) */
/* WARNING: Removing unreachable block (ram,0x0014028e) */
/* WARNING: Removing unreachable block (ram,0x00140298) */
/* WARNING: Removing unreachable block (ram,0x0014029c) */
/* WARNING: Removing unreachable block (ram,0x001402a4) */
/* WARNING: Removing unreachable block (ram,0x001402b2) */
/* WARNING: Removing unreachable block (ram,0x001402be) */
/* WARNING: Removing unreachable block (ram,0x00140348) */
/* WARNING: Removing unreachable block (ram,0x00140364) */
/* WARNING: Removing unreachable block (ram,0x00140380) */
/* WARNING: Removing unreachable block (ram,0x00140442) */
/* WARNING: Removing unreachable block (ram,0x0014044e) */
/* WARNING: Removing unreachable block (ram,0x00140450) */
/* WARNING: Removing unreachable block (ram,0x00140452) */
/* WARNING: Removing unreachable block (ram,0x0014036c) */
/* WARNING: Removing unreachable block (ram,0x00140370) */
/* WARNING: Removing unreachable block (ram,0x0014053c) */
/* WARNING: Removing unreachable block (ram,0x00140350) */
/* WARNING: Removing unreachable block (ram,0x00140386) */
/* WARNING: Removing unreachable block (ram,0x00140358) */
/* WARNING: Removing unreachable block (ram,0x00140378) */
/* WARNING: Removing unreachable block (ram,0x0014035c) */
/* WARNING: Removing unreachable block (ram,0x001402c8) */
/* WARNING: Removing unreachable block (ram,0x001402d2) */
/* WARNING: Removing unreachable block (ram,0x00140326) */
/* WARNING: Removing unreachable block (ram,0x0014032c) */
/* WARNING: Removing unreachable block (ram,0x00140344) */
/* WARNING: Removing unreachable block (ram,0x00140334) */
/* WARNING: Removing unreachable block (ram,0x00083aee) */
/* WARNING: Removing unreachable block (ram,0x00083af2) */
/* WARNING: Removing unreachable block (ram,0x000ff0fa) */
/* WARNING: Removing unreachable block (ram,0x000ff0fe) */
/* WARNING: Removing unreachable block (ram,0x000ff102) */
/* WARNING: Removing unreachable block (ram,0x000ff106) */
/* WARNING: Removing unreachable block (ram,0x000ff10a) */
/* WARNING: Removing unreachable block (ram,0x000bf0f6) */
/* WARNING: Removing unreachable block (ram,0x0013f0fe) */
/* WARNING: Removing unreachable block (ram,0x000bf186) */
/* WARNING: Removing unreachable block (ram,0x000ff18e) */
/* WARNING: Removing unreachable block (ram,0x00083af6) */
/* WARNING: Removing unreachable block (ram,0x00083afa) */
/* WARNING: Removing unreachable block (ram,0x00083b2a) */
/* WARNING: Removing unreachable block (ram,0x00083b2e) */
/* WARNING: Removing unreachable block (ram,0x00083b32) */
/* WARNING: Removing unreachable block (ram,0x000bf17e) */
/* WARNING: Removing unreachable block (ram,0x00083b36) */
/* WARNING: Removing unreachable block (ram,0x000ff186) */
/* WARNING: Removing unreachable block (ram,0x000ff18a) */
/* WARNING: Removing unreachable block (ram,0x000ff192) */
/* WARNING: Removing unreachable block (ram,0x000ff196) */
/* WARNING: Removing unreachable block (ram,0x000ff19a) */
/* WARNING: Removing unreachable block (ram,0x000ff19e) */
/* WARNING: Removing unreachable block (ram,0x000ff1a2) */
/* WARNING: Removing unreachable block (ram,0x000ff1a6) */
/* WARNING: Removing unreachable block (ram,0x0013f19e) */
/* WARNING: Removing unreachable block (ram,0x00083b3a) */
/* WARNING: Removing unreachable block (ram,0x00083b3e) */
/* WARNING: Removing unreachable block (ram,0x000bf3d2) */
/* WARNING: Removing unreachable block (ram,0x000bf3d6) */
/* WARNING: Removing unreachable block (ram,0x000bf3da) */
/* WARNING: Removing unreachable block (ram,0x00083b42) */
/* WARNING: Removing unreachable block (ram,0x00083b46) */
/* WARNING: Removing unreachable block (ram,0x0013f192) */
/* WARNING: Removing unreachable block (ram,0x0013f196) */
/* WARNING: Removing unreachable block (ram,0x0127b2da) */
/* WARNING: Removing unreachable block (ram,0x0013f19a) */
/* WARNING: Removing unreachable block (ram,0x0013f1a2) */
/* WARNING: Removing unreachable block (ram,0x00083b4a) */
/* WARNING: Removing unreachable block (ram,0x000ff3e2) */
/* WARNING: Removing unreachable block (ram,0x00083b4e) */
/* WARNING: Removing unreachable block (ram,0x00083b52) */
/* WARNING: Removing unreachable block (ram,0x000bf15a) */
/* WARNING: Removing unreachable block (ram,0x000bf15e) */
/* WARNING: Removing unreachable block (ram,0x000bf162) */
/* WARNING: Removing unreachable block (ram,0x00083b56) */
/* WARNING: Removing unreachable block (ram,0x000ff15e) */
/* WARNING: Removing unreachable block (ram,0x000ff162) */
/* WARNING: Removing unreachable block (ram,0x000ff166) */
/* WARNING: Removing unreachable block (ram,0x000ff16a) */
/* WARNING: Removing unreachable block (ram,0x00083b5a) */
/* WARNING: Removing unreachable block (ram,0x0013f162) */
/* WARNING: Removing unreachable block (ram,0x0013f166) */
/* WARNING: Removing unreachable block (ram,0x0013f16a) */
/* WARNING: Removing unreachable block (ram,0x00083b5e) */
/* WARNING: Removing unreachable block (ram,0x00083b86) */
/* WARNING: Removing unreachable block (ram,0x00083b8a) */
/* WARNING: Removing unreachable block (ram,0x00083b8e) */
/* WARNING: Removing unreachable block (ram,0x004bf3a2) */
/* WARNING: Removing unreachable block (ram,0x00083b92) */
/* WARNING: Removing unreachable block (ram,0x000ff1e2) */
/* WARNING: Removing unreachable block (ram,0x000ff1e6) */
/* WARNING: Removing unreachable block (ram,0x00a3d522) */
/* WARNING: Removing unreachable block (ram,0x000ff1ea) */
/* WARNING: Removing unreachable block (ram,0x000ff1ee) */
/* WARNING: Removing unreachable block (ram,0x000ff1f2) */
/* WARNING: Removing unreachable block (ram,0x000ff1f6) */
/* WARNING: Removing unreachable block (ram,0x000ff1fa) */
/* WARNING: Removing unreachable block (ram,0x000ff1fe) */
/* WARNING: Removing unreachable block (ram,0x00083b96) */
/* WARNING: Removing unreachable block (ram,0x0013f1e6) */
/* WARNING: Removing unreachable block (ram,0x00083b9a) */
/* WARNING: Removing unreachable block (ram,0x0017f422) */
/* WARNING: Removing unreachable block (ram,0x0017f426) */
/* WARNING: Removing unreachable block (ram,0x00083b9e) */
/* WARNING: Removing unreachable block (ram,0x0017f42a) */
/* WARNING: Removing unreachable block (ram,0x0017f42e) */
/* WARNING: Removing unreachable block (ram,0x00083ba2) */
/* WARNING: Removing unreachable block (ram,0x0017f432) */
/* WARNING: Removing unreachable block (ram,0x0017f436) */
/* WARNING: Removing unreachable block (ram,0x0017f43a) */
/* WARNING: Removing unreachable block (ram,0x0017f43e) */
/* WARNING: Removing unreachable block (ram,0x00083ba6) */
/* WARNING: Removing unreachable block (ram,0x00083baa) */
/* WARNING: Removing unreachable block (ram,0x000ff1b2) */
/* WARNING: Removing unreachable block (ram,0x0093d4ee) */
/* WARNING: Removing unreachable block (ram,0x000ff1b6) */
/* WARNING: Removing unreachable block (ram,0x000ff1ba) */
/* WARNING: Removing unreachable block (ram,0x000ff1be) */
/* WARNING: Removing unreachable block (ram,0x000ff1c2) */
/* WARNING: Removing unreachable block (ram,0x000ff1c6) */
/* WARNING: Removing unreachable block (ram,0x000ff1ca) */
/* WARNING: Removing unreachable block (ram,0x000ff1ce) */
/* WARNING: Removing unreachable block (ram,0x000ff1d2) */
/* WARNING: Removing unreachable block (ram,0x000ff1d6) */
/* WARNING: Removing unreachable block (ram,0x000ff1da) */
/* WARNING: Removing unreachable block (ram,0x00083bae) */
/* WARNING: Removing unreachable block (ram,0x0013f1b6) */
/* WARNING: Removing unreachable block (ram,0x018cda06) */
/* WARNING: Removing unreachable block (ram,0x0013f1c2) */
/* WARNING: Removing unreachable block (ram,0x0013f1c6) */
/* WARNING: Removing unreachable block (ram,0x0013f1ca) */
/* WARNING: Removing unreachable block (ram,0x00083bb2) */
/* WARNING: Removing unreachable block (ram,0x00083be2) */
/* WARNING: Removing unreachable block (ram,0x00083be6) */
/* WARNING: Removing unreachable block (ram,0x000bf232) */
/* WARNING: Removing unreachable block (ram,0x000bf236) */
/* WARNING: Removing unreachable block (ram,0x000bf23a) */
/* WARNING: Removing unreachable block (ram,0x00083bea) */
/* WARNING: Removing unreachable block (ram,0x004bf3fe) */
/* WARNING: Removing unreachable block (ram,0x00083bee) */
/* WARNING: Removing unreachable block (ram,0x000ff23a) */
/* WARNING: Removing unreachable block (ram,0x000ff23e) */
/* WARNING: Removing unreachable block (ram,0x000ff242) */
/* WARNING: Removing unreachable block (ram,0x00083bf2) */
/* WARNING: Removing unreachable block (ram,0x0013f23e) */
/* WARNING: Removing unreachable block (ram,0x00083bf6) */
/* WARNING: Removing unreachable block (ram,0x0017f5fe) */
/* WARNING: Removing unreachable block (ram,0x00083bfa) */
/* WARNING: Removing unreachable block (ram,0x00083bfe) */
/* WARNING: Removing unreachable block (ram,0x00083c02) */
/* WARNING: Removing unreachable block (ram,0x000bf20a) */
/* WARNING: Removing unreachable block (ram,0x00083c06) */
/* WARNING: Removing unreachable block (ram,0x000ff20e) */
/* WARNING: Removing unreachable block (ram,0x00083c0a) */
/* WARNING: Removing unreachable block (ram,0x0013f212) */
/* WARNING: Removing unreachable block (ram,0x00083c0e) */
/* WARNING: Removing unreachable block (ram,0x00083c36) */
/* WARNING: Removing unreachable block (ram,0x00083c3a) */
/* WARNING: Removing unreachable block (ram,0x000bf286) */
/* WARNING: Removing unreachable block (ram,0x000bf28a) */
/* WARNING: Removing unreachable block (ram,0x00083c3e) */
/* WARNING: Removing unreachable block (ram,0x00083c4a) */
/* WARNING: Removing unreachable block (ram,0x00083c4e) */
/* WARNING: Removing unreachable block (ram,0x00083c52) */
/* WARNING: Removing unreachable block (ram,0x000ff29e) */
/* WARNING: Removing unreachable block (ram,0x00083c56) */
/* WARNING: Removing unreachable block (ram,0x00083c62) */
/* WARNING: Removing unreachable block (ram,0x00083c66) */
/* WARNING: Removing unreachable block (ram,0x00083c76) */
/* WARNING: Removing unreachable block (ram,0x00083c7a) */
/* WARNING: Removing unreachable block (ram,0x00083c7e) */
/* WARNING: Removing unreachable block (ram,0x00083c90) */
/* WARNING: Removing unreachable block (ram,0x00083ca6) */
/* WARNING: Removing unreachable block (ram,0x00083cba) */
/* WARNING: Removing unreachable block (ram,0x00083cbc) */
/* WARNING: Removing unreachable block (ram,0x00083ca2) */
/* WARNING: Removing unreachable block (ram,0x0017f606) */
/* WARNING: Removing unreachable block (ram,0x0017f60e) */
/* WARNING: Removing unreachable block (ram,0x000bf1ae) */
/* WARNING: Removing unreachable block (ram,0xfecbf1ba) */
/* WARNING: Removing unreachable block (ram,0x000bf1b2) */
/* WARNING: Removing unreachable block (ram,0x000bf1b6) */
/* WARNING: Removing unreachable block (ram,0x000bf1ba) */
/* WARNING: Removing unreachable block (ram,0x000bf1be) */
/* WARNING: Removing unreachable block (ram,0x000bf1c6) */
/* WARNING: Removing unreachable block (ram,0x000bf1ca) */
/* WARNING: Removing unreachable block (ram,0x000bf1ce) */
/* WARNING: Removing unreachable block (ram,0x000bf1da) */
/* WARNING: Removing unreachable block (ram,0x000bf1de) */
/* WARNING: Removing unreachable block (ram,0x000bf1e2) */
/* WARNING: Removing unreachable block (ram,0x000bf182) */
/* WARNING: Removing unreachable block (ram,0x000bf196) */
/* WARNING: Removing unreachable block (ram,0x000bf19a) */
/* WARNING: Removing unreachable block (ram,0x000bf19e) */
/* WARNING: Removing unreachable block (ram,0x00083a02) */
/* WARNING: Removing unreachable block (ram,0x00083a06) */
/* WARNING: Removing unreachable block (ram,0x00083a36) */
/* WARNING: Removing unreachable block (ram,0x00083a3a) */
/* WARNING: Removing unreachable block (ram,0x00083a3e) */
/* WARNING: Removing unreachable block (ram,0x004bf252) */
/* WARNING: Removing unreachable block (ram,0x00083a42) */
/* WARNING: Removing unreachable block (ram,0x000ff08a) */
/* WARNING: Removing unreachable block (ram,0x00083a46) */
/* WARNING: Removing unreachable block (ram,0x0013f08e) */
/* WARNING: Removing unreachable block (ram,0x0057a89a) */
/* WARNING: Removing unreachable block (ram,0x0013f0b6) */
/* WARNING: Removing unreachable block (ram,0x0013f0ba) */
/* WARNING: Removing unreachable block (ram,0x0013f0be) */
/* WARNING: Removing unreachable block (ram,0x0013f0c2) */
/* WARNING: Removing unreachable block (ram,0x00083a4a) */
/* WARNING: Removing unreachable block (ram,0x0017f452) */
/* WARNING: Removing unreachable block (ram,0x00083a4e) */
/* WARNING: Removing unreachable block (ram,0x0017f45a) */
/* WARNING: Removing unreachable block (ram,0x0017f45e) */
/* WARNING: Removing unreachable block (ram,0x00083a52) */
/* WARNING: Removing unreachable block (ram,0x00083a56) */
/* WARNING: Removing unreachable block (ram,0x000bf05e) */
/* WARNING: Removing unreachable block (ram,0x000bf062) */
/* WARNING: Removing unreachable block (ram,0x00083a5a) */
/* WARNING: Removing unreachable block (ram,0x000ff062) */
/* WARNING: Removing unreachable block (ram,0x00083a5e) */
/* WARNING: Removing unreachable block (ram,0x00083a62) */
/* WARNING: Removing unreachable block (ram,0x00083a92) */
/* WARNING: Removing unreachable block (ram,0x00083a9a) */
/* WARNING: Removing unreachable block (ram,0x00083a9e) */
/* WARNING: Removing unreachable block (ram,0x00083aa2) */
/* WARNING: Removing unreachable block (ram,0x00083ac6) */
/* WARNING: Removing unreachable block (ram,0x00083aca) */
/* WARNING: Removing unreachable block (ram,0x00083ace) */
/* WARNING: Removing unreachable block (ram,0x000bf11a) */
/* WARNING: Removing unreachable block (ram,0x000bf11e) */
/* WARNING: Removing unreachable block (ram,0x00083ad2) */
/* WARNING: Removing unreachable block (ram,0x00083ad6) */
/* WARNING: Removing unreachable block (ram,0x000ff122) */
/* WARNING: Removing unreachable block (ram,0x000ff126) */
/* WARNING: Removing unreachable block (ram,0x000ff12a) */
/* WARNING: Removing unreachable block (ram,0x000ff12e) */
/* WARNING: Removing unreachable block (ram,0x000ff132) */
/* WARNING: Removing unreachable block (ram,0x000ff136) */
/* WARNING: Removing unreachable block (ram,0x000ff13a) */
/* WARNING: Removing unreachable block (ram,0x000ff13e) */
/* WARNING: Removing unreachable block (ram,0x00083ada) */
/* WARNING: Removing unreachable block (ram,0x00083ade) */
/* WARNING: Removing unreachable block (ram,0x00083ae2) */
/* WARNING: Removing unreachable block (ram,0x0013f12e) */
/* WARNING: Removing unreachable block (ram,0x0013f134) */
/* WARNING: Removing unreachable block (ram,0x0013f130) */
/* WARNING: Removing unreachable block (ram,0x00083ae6) */
/* WARNING: Removing unreachable block (ram,0x000ff3be) */
/* WARNING: Removing unreachable block (ram,0x000ff3c2) */
/* WARNING: Removing unreachable block (ram,0x0013f066) */
/* WARNING: Removing unreachable block (ram,0x0013f06a) */
/* WARNING: Removing unreachable block (ram,0x0013f06e) */
/* WARNING: Removing unreachable block (ram,0x0017f462) */
/* WARNING: Removing unreachable block (ram,0x000bf082) */
/* WARNING: Removing unreachable block (ram,0x000bf086) */
/* WARNING: Removing unreachable block (ram,0x0013f00a) */
/* WARNING: Removing unreachable block (ram,0x000bf026) */
/* WARNING: Removing unreachable block (ram,0x0013f26a) */
/* WARNING: Removing unreachable block (ram,0x000bef96) */
/* WARNING: Removing unreachable block (ram,0x000bef98) */
/* WARNING: Removing unreachable block (ram,0x000bef9a) */
/* WARNING: Removing unreachable block (ram,0x000befb0) */
/* WARNING: Removing unreachable block (ram,0x000bed48) */
/* WARNING: Removing unreachable block (ram,0x000bed4e) */
/* WARNING: Removing unreachable block (ram,0x000bed5a) */
/* WARNING: Removing unreachable block (ram,0x000bed6a) */
/* WARNING: Removing unreachable block (ram,0x000bed76) */
/* WARNING: Removing unreachable block (ram,0x000bed9e) */
/* WARNING: Removing unreachable block (ram,0x000beda0) */
/* WARNING: Removing unreachable block (ram,0x000bedbe) */
/* WARNING: Removing unreachable block (ram,0x000bedc0) */
/* WARNING: Removing unreachable block (ram,0x000bee1c) */
/* WARNING: Removing unreachable block (ram,0x000bee4c) */
/* WARNING: Removing unreachable block (ram,0x000bee26) */
/* WARNING: Removing unreachable block (ram,0x000bee5a) */
/* WARNING: Removing unreachable block (ram,0x000beed8) */
/* WARNING: Removing unreachable block (ram,0x000beeda) */
/* WARNING: Removing unreachable block (ram,0x000bef06) */
/* WARNING: Removing unreachable block (ram,0x000bef14) */
/* WARNING: Removing unreachable block (ram,0x000bee2c) */
/* WARNING: Removing unreachable block (ram,0x000bee3a) */
/* WARNING: Removing unreachable block (ram,0x000bf25a) */
/* WARNING: Removing unreachable block (ram,0x000befca) */
/* AbyssEngine::AEMath::Vector::operator[](int) const */

undefined4 AbyssEngine::AEMath::Vector::operator[](int param_1)

{
  undefined2 *puVar1;
  ResourceTexture *this;
  Vector *pVVar2;
  int in_r1;
  int extraout_r1;
  int *unaff_r4;
  code *pcVar3;
  int unaff_r5;
  int iVar4;
  Galaxy *this_00;
  ScrollTouchWindow *pSVar5;
  Agent *pAVar6;
  int unaff_r6;
  int unaff_r7;
  int unaff_r8;
  Agent *unaff_r9;
  int *unaff_r10;
  SpaceLounge *unaff_r11;
  int unaff_lr;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  bool in_OV;
  undefined4 in_cr15;
  float extraout_s0;
  float extraout_s0_00;
  int *in_stack_00000014;
  Agent *in_stack_00000018;
  
  if (in_ZR) {
    unaff_lr = in_r1 + param_1 * 0x400000;
  }
  if (in_OV) {
    *unaff_r10 = unaff_lr;
    unaff_r10[-1] = (int)unaff_r10;
    unaff_r10[-2] = (int)unaff_r9;
    unaff_r10[-3] = unaff_r8;
    unaff_r10[-4] = unaff_r6;
    unaff_r10[-5] = unaff_r5;
    unaff_r10[-6] = (int)unaff_r4;
    *unaff_r10 = unaff_lr;
    unaff_r10[-1] = (int)register0x00000054;
    unaff_r10[-2] = in_r1;
    *(int *)unaff_r9 = in_r1;
    *(int *)(unaff_r9 + 4) = unaff_r6;
    *(BADSPACEBASE **)(unaff_r9 + 8) = register0x00000054;
    *(int *)(unaff_r9 + 0xc) = unaff_lr;
  }
  if (in_NG) {
    param_1 = param_1 + in_r1 * -2;
  }
  if (in_ZR) {
    coprocessor_store(8,in_cr15,unaff_r4);
    AERandom::reset();
    String::String((String *)(unaff_r7 + -0x68),(char *)(extraout_r1 + 0x17cea0),false);
    iVar4 = *unaff_r4;
    Agent::getIndex(unaff_r9);
    GameText::getText(iVar4);
    AbyssEngine::operator+((String *)(unaff_r7 + -0x5c),(String *)(unaff_r7 + -0x68));
    String::operator+=((String *)(unaff_r7 + -0x38),(String *)(unaff_r7 + -0x5c));
    String::~String((String *)(unaff_r7 + -0x5c));
    String::~String((String *)(unaff_r7 + -0x68));
    String::String((String *)(unaff_r7 + -0x68),(char *)(DAT_0017d0e8 + 0x17ceea),false);
    GameText::getText(*unaff_r4);
    AbyssEngine::operator+((String *)(unaff_r7 + -0x5c),(String *)(unaff_r7 + -0x68));
    String::operator+=((String *)(unaff_r7 + -0x38),(String *)(unaff_r7 + -0x5c));
    String::~String((String *)(unaff_r7 + -0x5c));
    String::~String((String *)(unaff_r7 + -0x68));
    this_00 = (Galaxy *)**(undefined4 **)(DAT_0017d0ec + 0x17cf2c);
    iVar4 = Agent::getSellSystemIndex(unaff_r9);
    Galaxy::getSystem(this_00,iVar4);
    iVar4 = *unaff_r10;
    String::String((String *)&stack0x000000f4,(String *)(unaff_r7 + -0x38),false);
    SolarSystem::getName();
    String::String((String *)&stack0x000000e8,(String *)(unaff_r7 + -0x68),false);
    String::String((String *)&stack0x000000dc,(char *)(DAT_0017d0f0 + 0x17cf6a),false);
    Status::replaceHash(unaff_r7 + -0x5c,iVar4,&stack0x000000f4,&stack0x000000e8);
    String::operator=((String *)(unaff_r7 + -0x38),(String *)(unaff_r7 + -0x5c));
    pcVar3 = *(code **)(DAT_0017d0f4 + 0x17cf92);
    (*pcVar3)(unaff_r7 + -0x5c);
    (*pcVar3)(&stack0x000000dc);
    (*pcVar3)(&stack0x000000e8);
    (*pcVar3)(unaff_r7 + -0x68);
    (*pcVar3)(&stack0x000000f4);
    iVar4 = *unaff_r10;
    String::String((String *)&stack0x000000d0,(String *)(unaff_r7 + -0x38),false);
    Agent::getSellItemPrice(unaff_r9);
    Layout::formatCredits(unaff_r7 + -0x68);
    String::String((String *)&stack0x000000c4,(String *)(unaff_r7 + -0x68),false);
    String::String((String *)&stack0x000000b8,(char *)(DAT_0017d0fc + 0x17cfe6),false);
    Status::replaceHash(unaff_r7 + -0x5c,iVar4,&stack0x000000d0,&stack0x000000c4);
    String::operator=((String *)(unaff_r7 + -0x38),(String *)(unaff_r7 + -0x5c));
    (*pcVar3)(unaff_r7 + -0x5c);
    (*pcVar3)(&stack0x000000b8);
    (*pcVar3)(&stack0x000000c4);
    (*pcVar3)(unaff_r7 + -0x68);
    (*pcVar3)(&stack0x000000d0);
    String::String((String *)(unaff_r7 + -0x68),(char *)(DAT_0017d7ac + 0x17d542),false);
    iVar4 = **(int **)(DAT_0017d7b0 + 0x17d552);
    AERandom::nextInt(**(int **)(DAT_0017d7b4 + 0x17d554));
    GameText::getText(iVar4);
    AbyssEngine::operator+((String *)(unaff_r7 + -0x5c),(String *)(unaff_r7 + -0x68));
    String::operator+=((String *)(unaff_r7 + -0x38),(String *)(unaff_r7 + -0x5c));
    String::~String((String *)(unaff_r7 + -0x5c));
    String::~String((String *)(unaff_r7 + -0x68));
    pSVar5 = *(ScrollTouchWindow **)(unaff_r11 + 0x60);
    Agent::getName();
    String::String((String *)&stack0x00000058,(String *)(unaff_r7 + -0x38),false);
    ScrollTouchWindow::setText(pSVar5,&stack0x00000064,&stack0x00000058);
    String::~String((String *)&stack0x00000058);
    String::~String((String *)&stack0x00000064);
    pAVar6 = *(Agent **)(*(int *)(*(int *)(unaff_r11 + 0x24) + 4) + *(int *)(unaff_r11 + 0x20) * 4);
    String::String((String *)&stack0x0000004c,(String *)(unaff_r7 + -0x38),false);
    Agent::setMissionString(pAVar6,&stack0x0000004c);
    String::~String((String *)&stack0x0000004c);
    *(undefined4 *)(unaff_r11 + 0x14) = 2;
    *(undefined4 *)(unaff_r11 + 0x2c) = 0;
    pVVar2 = (Vector *)SpaceLounge::getSoundId(unaff_r11,in_stack_00000018);
    if (-1 < (int)pVVar2) {
      FModSound::play(**(int **)(DAT_0017d7b8 + 0x17d60a),pVVar2,(Vector *)0x0,extraout_s0_00);
    }
    String::~String((String *)(unaff_r7 + -0x38));
    if (*in_stack_00000014 != *(int *)(unaff_r7 + -0x2c)) {
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
    return 0;
  }
  if (!in_CY) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  puVar1 = operator_new(param_1);
  this = operator_new(8);
  ResourceTexture::ResourceTexture(this,(char *)(DAT_000fefe8 + 0xfefe4),extraout_s0);
  *puVar1 = 0x27ab;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
```
