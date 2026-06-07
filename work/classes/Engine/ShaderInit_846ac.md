# Engine::ShaderInit
elf 0x846ac body 794
Sig: undefined __thiscall ShaderInit(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ShaderInit() */

undefined4 __thiscall AbyssEngine::Engine::ShaderInit(Engine *this)

{
  TextureShader *this_00;
  SimpleShader *this_01;
  TextureVtxColorShader *this_02;
  NoTexShader *this_03;
  NoTexVtxColorShader *this_04;
  TextureAlphaTestShader *this_05;
  CubeMapping *this_06;
  CubeNormalMapping *this_07;
  BumpMapping *this_08;
  SandboxShader *this_09;
  GenericShader *this_10;
  GenericShader1 *this_11;
  GenericShader2 *this_12;
  TexOnlyShader *this_13;
  BumpShaderV2 *this_14;
  BumpShaderV3 *this_15;
  VertexColorShader *this_16;
  VertexColorAlphaTextureShader *this_17;
  PulseShader *this_18;
  BumpShader *this_19;
  BumpShaderV4 *this_20;
  BloomShader *this_21;
  BlurShader *this_22;
  PostBWShader *this_23;
  TextureLightShader *this_24;
  BumpRimCubeShader *this_25;
  BumpShaderCloak *this_26;
  SpecCubeMapping *this_27;
  SpecCubeAlphaMapping *this_28;
  EnergyShield *this_29;
  SimpleRefractionShader *this_30;
  GlowShader *this_31;
  GlowPPShader *this_32;
  DrawFBOShader *this_33;
  ColorMixAdd *this_34;
  BumpShaderParticle *this_35;
  DNSShader *this_36;
  BumpShaderRefract *this_37;
  BumpRimCubeShader_new *this_38;
  
  this_00 = operator_new(0x8c);
  TextureShader::TextureShader(this_00);
  ShaderRegister(this,(ShaderBaseStruct *)this_00);
  this_01 = operator_new(0x2c);
  SimpleShader::SimpleShader(this_01);
  ShaderRegister(this,(ShaderBaseStruct *)this_01);
  this_02 = operator_new(0x94);
  TextureVtxColorShader::TextureVtxColorShader(this_02);
  ShaderRegister(this,(ShaderBaseStruct *)this_02);
  this_03 = operator_new(0x2c);
  NoTexShader::NoTexShader(this_03);
  ShaderRegister(this,(ShaderBaseStruct *)this_03);
  this_04 = operator_new(0x30);
  NoTexVtxColorShader::NoTexVtxColorShader(this_04);
  ShaderRegister(this,(ShaderBaseStruct *)this_04);
  this_05 = operator_new(0x74);
  TextureAlphaTestShader::TextureAlphaTestShader(this_05);
  ShaderRegister(this,(ShaderBaseStruct *)this_05);
  this_06 = operator_new(0x5c);
  CubeMapping::CubeMapping(this_06);
  ShaderRegister(this,(ShaderBaseStruct *)this_06);
  this_07 = operator_new(0x68);
  CubeNormalMapping::CubeNormalMapping(this_07);
  ShaderRegister(this,(ShaderBaseStruct *)this_07);
  this_08 = operator_new(0x44);
  BumpMapping::BumpMapping(this_08);
  ShaderRegister(this,(ShaderBaseStruct *)this_08);
  this_09 = operator_new(0x50);
  SandboxShader::SandboxShader(this_09);
  ShaderRegister(this,(ShaderBaseStruct *)this_09);
  this_10 = operator_new(100);
  GenericShader::GenericShader(this_10);
  ShaderRegister(this,(ShaderBaseStruct *)this_10);
  this_11 = operator_new(0x58);
  GenericShader1::GenericShader1(this_11);
  ShaderRegister(this,(ShaderBaseStruct *)this_11);
  this_12 = operator_new(0x58);
  GenericShader2::GenericShader2(this_12);
  ShaderRegister(this,(ShaderBaseStruct *)this_12);
  this_13 = operator_new(0x30);
  TexOnlyShader::TexOnlyShader(this_13);
  ShaderRegister(this,(ShaderBaseStruct *)this_13);
  this_14 = operator_new(0x60);
  BumpShaderV2::BumpShaderV2(this_14);
  ShaderRegister(this,(ShaderBaseStruct *)this_14);
  this_15 = operator_new(0x84);
  BumpShaderV3::BumpShaderV3(this_15);
  ShaderRegister(this,(ShaderBaseStruct *)this_15);
  this_16 = operator_new(0x5c);
  VertexColorShader::VertexColorShader(this_16);
  ShaderRegister(this,(ShaderBaseStruct *)this_16);
  this_17 = operator_new(0x5c);
  VertexColorAlphaTextureShader::VertexColorAlphaTextureShader(this_17);
  ShaderRegister(this,(ShaderBaseStruct *)this_17);
  this_18 = operator_new(0x5c);
  PulseShader::PulseShader(this_18);
  ShaderRegister(this,(ShaderBaseStruct *)this_18);
  this_19 = operator_new(0x84);
  BumpShader::BumpShader(this_19);
  ShaderRegister(this,(ShaderBaseStruct *)this_19);
  this_20 = operator_new(0x5c);
  BumpShaderV4::BumpShaderV4(this_20);
  ShaderRegister(this,(ShaderBaseStruct *)this_20);
  this_21 = operator_new(0xa0);
  BloomShader::BloomShader(this_21);
  ShaderRegister(this,(ShaderBaseStruct *)this_21);
  this_22 = operator_new(100);
  BlurShader::BlurShader(this_22);
  ShaderRegister(this,(ShaderBaseStruct *)this_22);
  this_23 = operator_new(0x34);
  PostBWShader::PostBWShader(this_23);
  ShaderRegister(this,(ShaderBaseStruct *)this_23);
  this_24 = operator_new(0x70);
  TextureLightShader::TextureLightShader(this_24);
  ShaderRegister(this,(ShaderBaseStruct *)this_24);
  this_25 = operator_new(0x98);
  BumpRimCubeShader::BumpRimCubeShader(this_25);
  ShaderRegister(this,(ShaderBaseStruct *)this_25);
  this_26 = operator_new(0x98);
  BumpShaderCloak::BumpShaderCloak(this_26);
  ShaderRegister(this,(ShaderBaseStruct *)this_26);
  this_27 = operator_new(0x5c);
  SpecCubeMapping::SpecCubeMapping(this_27);
  ShaderRegister(this,(ShaderBaseStruct *)this_27);
  this_28 = operator_new(0x5c);
  SpecCubeAlphaMapping::SpecCubeAlphaMapping(this_28);
  ShaderRegister(this,(ShaderBaseStruct *)this_28);
  this_29 = operator_new(0x60);
  EnergyShield::EnergyShield(this_29);
  ShaderRegister(this,(ShaderBaseStruct *)this_29);
  this_30 = operator_new(0x58);
  SimpleRefractionShader::SimpleRefractionShader(this_30);
  ShaderRegister(this,(ShaderBaseStruct *)this_30);
  this_31 = operator_new(0x34);
  GlowShader::GlowShader(this_31);
  ShaderRegister(this,(ShaderBaseStruct *)this_31);
  this_32 = operator_new(0xa8);
  GlowPPShader::GlowPPShader(this_32);
  ShaderRegister(this,(ShaderBaseStruct *)this_32);
  this_33 = operator_new(0x60);
  DrawFBOShader::DrawFBOShader(this_33);
  ShaderRegister(this,(ShaderBaseStruct *)this_33);
  this_34 = operator_new(0x40);
  ColorMixAdd::ColorMixAdd(this_34);
  ShaderRegister(this,(ShaderBaseStruct *)this_34);
  this_35 = operator_new(0x6c);
  BumpShaderParticle::BumpShaderParticle(this_35);
  ShaderRegister(this,(ShaderBaseStruct *)this_35);
  this_36 = operator_new(100);
  DNSShader::DNSShader(this_36);
  ShaderRegister(this,(ShaderBaseStruct *)this_36);
  this_37 = operator_new(0x4c);
  BumpShaderRefract::BumpShaderRefract(this_37);
  ShaderRegister(this,(ShaderBaseStruct *)this_37);
  this_38 = operator_new(0x98);
  BumpRimCubeShader_new::BumpRimCubeShader_new(this_38);
  ShaderRegister(this,(ShaderBaseStruct *)this_38);
  glGetError();
  return 1;
}

```

## target disasm
```
  000946ac: push {r4,r5,r7,lr}
  000946ae: add r7,sp,#0x8
  000946b0: mov r4,r0
  000946b2: movs r0,#0x8c
  000946b4: blx 0x0006eb24
  000946b8: mov r5,r0
  000946ba: blx 0x00070558
  000946be: mov r0,r4
  000946c0: mov r1,r5
  000946c2: blx 0x00070564
  000946c6: movs r0,#0x2c
  000946c8: blx 0x0006eb24
  000946cc: mov r5,r0
  000946ce: blx 0x00070570
  000946d2: mov r0,r4
  000946d4: mov r1,r5
  000946d6: blx 0x00070564
  000946da: movs r0,#0x94
  000946dc: blx 0x0006eb24
  000946e0: mov r5,r0
  000946e2: blx 0x0007057c
  000946e6: mov r0,r4
  000946e8: mov r1,r5
  000946ea: blx 0x00070564
  000946ee: movs r0,#0x2c
  000946f0: blx 0x0006eb24
  000946f4: mov r5,r0
  000946f6: blx 0x00070588
  000946fa: mov r0,r4
  000946fc: mov r1,r5
  000946fe: blx 0x00070564
  00094702: movs r0,#0x30
  00094704: blx 0x0006eb24
  00094708: mov r5,r0
  0009470a: blx 0x00070594
  0009470e: mov r0,r4
  00094710: mov r1,r5
  00094712: blx 0x00070564
  00094716: movs r0,#0x74
  00094718: blx 0x0006eb24
  0009471c: mov r5,r0
  0009471e: blx 0x000705a0
  00094722: mov r0,r4
  00094724: mov r1,r5
  00094726: blx 0x00070564
  0009472a: movs r0,#0x5c
  0009472c: blx 0x0006eb24
  00094730: mov r5,r0
  00094732: blx 0x000705ac
  00094736: mov r0,r4
  00094738: mov r1,r5
  0009473a: blx 0x00070564
  0009473e: movs r0,#0x68
  00094740: blx 0x0006eb24
  00094744: mov r5,r0
  00094746: blx 0x000705b8
  0009474a: mov r0,r4
  0009474c: mov r1,r5
  0009474e: blx 0x00070564
  00094752: movs r0,#0x44
  00094754: blx 0x0006eb24
  00094758: mov r5,r0
  0009475a: blx 0x000705c4
  0009475e: mov r0,r4
  00094760: mov r1,r5
  00094762: blx 0x00070564
  00094766: movs r0,#0x50
  00094768: blx 0x0006eb24
  0009476c: mov r5,r0
  0009476e: blx 0x000705d0
  00094772: mov r0,r4
  00094774: mov r1,r5
  00094776: blx 0x00070564
  0009477a: movs r0,#0x64
  0009477c: blx 0x0006eb24
  00094780: mov r5,r0
  00094782: blx 0x000705dc
  00094786: mov r0,r4
  00094788: mov r1,r5
  0009478a: blx 0x00070564
  0009478e: movs r0,#0x58
  00094790: blx 0x0006eb24
  00094794: mov r5,r0
  00094796: blx 0x000705e8
  0009479a: mov r0,r4
  0009479c: mov r1,r5
  0009479e: blx 0x00070564
  000947a2: movs r0,#0x58
  000947a4: blx 0x0006eb24
  000947a8: mov r5,r0
  000947aa: blx 0x000705f4
  000947ae: mov r0,r4
  000947b0: mov r1,r5
  000947b2: blx 0x00070564
  000947b6: movs r0,#0x30
  000947b8: blx 0x0006eb24
  000947bc: mov r5,r0
  000947be: blx 0x00070600
  000947c2: mov r0,r4
  000947c4: mov r1,r5
  000947c6: blx 0x00070564
  000947ca: movs r0,#0x60
  000947cc: blx 0x0006eb24
  000947d0: mov r5,r0
  000947d2: blx 0x0007060c
  000947d6: mov r0,r4
  000947d8: mov r1,r5
  000947da: blx 0x00070564
  000947de: movs r0,#0x84
  000947e0: blx 0x0006eb24
  000947e4: mov r5,r0
  000947e6: blx 0x00070618
  000947ea: mov r0,r4
  000947ec: mov r1,r5
  000947ee: blx 0x00070564
  000947f2: movs r0,#0x5c
  000947f4: blx 0x0006eb24
  000947f8: mov r5,r0
  000947fa: blx 0x00070624
  000947fe: mov r0,r4
  00094800: mov r1,r5
  00094802: blx 0x00070564
  00094806: movs r0,#0x5c
  00094808: blx 0x0006eb24
  0009480c: mov r5,r0
  0009480e: blx 0x00070630
  00094812: mov r0,r4
  00094814: mov r1,r5
  00094816: blx 0x00070564
  0009481a: movs r0,#0x5c
  0009481c: blx 0x0006eb24
  00094820: mov r5,r0
  00094822: blx 0x0007063c
  00094826: mov r0,r4
  00094828: mov r1,r5
  0009482a: blx 0x00070564
  0009482e: movs r0,#0x84
  00094830: blx 0x0006eb24
  00094834: mov r5,r0
  00094836: blx 0x00070648
  0009483a: mov r0,r4
  0009483c: mov r1,r5
  0009483e: blx 0x00070564
  00094842: movs r0,#0x5c
  00094844: blx 0x0006eb24
  00094848: mov r5,r0
  0009484a: blx 0x00070654
  0009484e: mov r0,r4
  00094850: mov r1,r5
  00094852: blx 0x00070564
  00094856: movs r0,#0xa0
  00094858: blx 0x0006eb24
  0009485c: mov r5,r0
  0009485e: blx 0x00070660
  00094862: mov r0,r4
  00094864: mov r1,r5
  00094866: blx 0x00070564
  0009486a: movs r0,#0x64
  0009486c: blx 0x0006eb24
  00094870: mov r5,r0
  00094872: blx 0x0007066c
  00094876: mov r0,r4
  00094878: mov r1,r5
  0009487a: blx 0x00070564
  0009487e: movs r0,#0x34
  00094880: blx 0x0006eb24
  00094884: mov r5,r0
  00094886: blx 0x00070678
  0009488a: mov r0,r4
  0009488c: mov r1,r5
  0009488e: blx 0x00070564
  00094892: movs r0,#0x70
  00094894: blx 0x0006eb24
  00094898: mov r5,r0
  0009489a: blx 0x00070684
  0009489e: mov r0,r4
  000948a0: mov r1,r5
  000948a2: blx 0x00070564
  000948a6: movs r0,#0x98
  000948a8: blx 0x0006eb24
  000948ac: mov r5,r0
  000948ae: blx 0x00070690
  000948b2: mov r0,r4
  000948b4: mov r1,r5
  000948b6: blx 0x00070564
  000948ba: movs r0,#0x98
  000948bc: blx 0x0006eb24
  000948c0: mov r5,r0
  000948c2: blx 0x0007069c
  000948c6: mov r0,r4
  000948c8: mov r1,r5
  000948ca: blx 0x00070564
  000948ce: movs r0,#0x5c
  000948d0: blx 0x0006eb24
  000948d4: mov r5,r0
  000948d6: blx 0x000706a8
  000948da: mov r0,r4
  000948dc: mov r1,r5
  000948de: blx 0x00070564
  000948e2: movs r0,#0x5c
  000948e4: blx 0x0006eb24
  000948e8: mov r5,r0
  000948ea: blx 0x000706b4
  000948ee: mov r0,r4
  000948f0: mov r1,r5
  000948f2: blx 0x00070564
  000948f6: movs r0,#0x60
  000948f8: blx 0x0006eb24
  000948fc: mov r5,r0
  000948fe: blx 0x000706c0
  00094902: mov r0,r4
  00094904: mov r1,r5
  00094906: blx 0x00070564
  0009490a: movs r0,#0x58
  0009490c: blx 0x0006eb24
  00094910: mov r5,r0
  00094912: blx 0x000706cc
  00094916: mov r0,r4
  00094918: mov r1,r5
  0009491a: blx 0x00070564
  0009491e: movs r0,#0x34
  00094920: blx 0x0006eb24
  00094924: mov r5,r0
  00094926: blx 0x000706d8
  0009492a: mov r0,r4
  0009492c: mov r1,r5
  0009492e: blx 0x00070564
  00094932: movs r0,#0xa8
  00094934: blx 0x0006eb24
  00094938: mov r5,r0
  0009493a: blx 0x000706e4
  0009493e: mov r0,r4
  00094940: mov r1,r5
  00094942: blx 0x00070564
  00094946: movs r0,#0x60
  00094948: blx 0x0006eb24
  0009494c: mov r5,r0
  0009494e: blx 0x000706f0
  00094952: mov r0,r4
  00094954: mov r1,r5
  00094956: blx 0x00070564
  0009495a: movs r0,#0x40
  0009495c: blx 0x0006eb24
  00094960: mov r5,r0
  00094962: blx 0x000706fc
  00094966: mov r0,r4
  00094968: mov r1,r5
  0009496a: blx 0x00070564
  0009496e: movs r0,#0x6c
  00094970: blx 0x0006eb24
  00094974: mov r5,r0
  00094976: blx 0x00070708
  0009497a: mov r0,r4
  0009497c: mov r1,r5
  0009497e: blx 0x00070564
  00094982: movs r0,#0x64
  00094984: blx 0x0006eb24
  00094988: mov r5,r0
  0009498a: blx 0x00070714
  0009498e: mov r0,r4
  00094990: mov r1,r5
  00094992: blx 0x00070564
  00094996: movs r0,#0x4c
  00094998: blx 0x0006eb24
  0009499c: mov r5,r0
  0009499e: blx 0x00070720
  000949a2: mov r0,r4
  000949a4: mov r1,r5
  000949a6: blx 0x00070564
  000949aa: movs r0,#0x98
  000949ac: blx 0x0006eb24
  000949b0: mov r5,r0
  000949b2: blx 0x0007072c
  000949b6: mov r0,r4
  000949b8: mov r1,r5
  000949ba: blx 0x00070564
  000949be: blx 0x0006ed70
  000949c2: movs r0,#0x1
  000949c4: pop {r4,r5,r7,pc}
```
