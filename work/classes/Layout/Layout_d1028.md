# Layout::Layout
elf 0xd1028 body 5478
Sig: undefined __thiscall Layout(Layout * this)

## decompile
```c

/* WARNING: Removing unreachable block (ram,0x000e2018) */
/* WARNING: Removing unreachable block (ram,0x000e201e) */
/* WARNING: Restarted to delay deadcode elimination for space: register */
/* Layout::Layout() */

void __thiscall Layout::Layout(Layout *this)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  ulonglong uVar4;
  char cVar5;
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  uint3 uVar8;
  bool bVar9;
  byte bVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  undefined8 uVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  undefined8 uVar23;
  undefined8 uVar24;
  undefined8 uVar25;
  undefined8 uVar26;
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined8 uVar31;
  undefined8 uVar32;
  undefined8 uVar33;
  undefined8 uVar34;
  undefined8 uVar35;
  undefined8 uVar36;
  undefined8 uVar37;
  undefined8 uVar38;
  undefined8 uVar39;
  undefined8 uVar40;
  undefined8 uVar41;
  bool bVar42;
  char cVar43;
  uint uVar44;
  undefined4 uVar45;
  char *pcVar46;
  undefined4 *puVar47;
  byte *pbVar48;
  undefined4 uVar49;
  undefined4 uVar50;
  float *pfVar51;
  undefined4 uVar52;
  undefined4 uVar53;
  float *pfVar54;
  undefined4 uVar55;
  undefined4 uVar56;
  char *pcVar57;
  undefined4 uVar58;
  char cVar59;
  char *pcVar60;
  uint uVar61;
  char cVar62;
  undefined4 uVar63;
  int iVar64;
  byte bVar65;
  undefined4 uVar66;
  undefined4 uVar67;
  bool bVar68;
  uint in_fpscr;
  float fVar69;
  float fVar70;
  undefined1 auVar71 [16];
  undefined1 auVar72 [16];
  undefined1 auVar73 [16];
  undefined1 auVar74 [16];
  undefined1 auVar75 [16];
  undefined1 auVar76 [16];
  undefined1 auVar77 [16];
  undefined1 auVar78 [16];
  undefined1 auVar79 [16];
  undefined1 auVar80 [16];
  undefined1 auVar81 [16];
  undefined1 auVar82 [16];
  undefined1 auVar83 [16];
  undefined1 auVar84 [16];
  undefined1 auVar85 [16];
  undefined1 auVar86 [16];
  undefined1 auVar87 [16];
  undefined1 auVar88 [16];
  undefined1 auVar89 [16];
  undefined1 auVar90 [16];
  undefined1 auVar91 [16];
  
  uVar41 = DAT_000e1538;
  uVar40 = DAT_000e1530;
  uVar39 = DAT_000e1528;
  uVar38 = DAT_000e1520;
  uVar37 = DAT_000e1518;
  uVar36 = DAT_000e1510;
  uVar35 = DAT_000e1508;
  uVar34 = DAT_000e1500;
  uVar33 = DAT_000e14f8;
  uVar32 = DAT_000e14f0;
  uVar31 = DAT_000e14e8;
  uVar30 = DAT_000e14e0;
  uVar29 = DAT_000e13a8;
  uVar28 = DAT_000e1398;
  uVar27 = DAT_000e1390;
  uVar26 = DAT_000e1388;
  uVar25 = DAT_000e1380;
  uVar24 = DAT_000e1378;
  uVar23 = DAT_000e1370;
  uVar22 = DAT_000e1368;
  uVar21 = DAT_000e1360;
  uVar20 = DAT_000e1358;
  uVar19 = DAT_000e1350;
  uVar18 = DAT_000e1348;
  uVar17 = DAT_000e1340;
  uVar16 = DAT_000e1338;
  uVar15 = DAT_000e1330;
  uVar14 = DAT_000e1328;
  uVar13 = DAT_000e1320;
  uVar12 = DAT_000e1310;
  uVar11 = DAT_000e1308;
  cVar5 = **(char **)(DAT_000e1300 + 0xe1036);
  if (cVar5 == '\0') {
    cVar43 = **(char **)(DAT_000e14dc + 0xe1272);
    if (cVar43 == '\0') {
      uVar50 = 0x3c;
      uVar45 = 0x1e;
      uVar58 = 0x32;
      uVar56 = 0x4a;
      uVar67 = 0x28;
      uVar49 = 0x3c;
      uVar55 = 0xe;
      pcVar46 = *(char **)(DAT_000e18d4 + 0xe1582);
      uVar53 = 0x3a;
      uVar63 = 10;
      if (*pcVar46 == '\0') {
        uVar50 = 0x1e;
        uVar45 = 0xf;
        uVar53 = 0x1d;
      }
      if (*pcVar46 == '\0') {
        uVar58 = 0x16;
        uVar56 = 0x25;
      }
      uVar52 = 0xe;
      if (*pcVar46 == '\0') {
        uVar52 = 9;
        uVar67 = 0x14;
      }
      uVar66 = 10;
      if (*pcVar46 == '\0') {
        uVar66 = 5;
      }
      *(undefined4 *)(this + 4) = uVar45;
      *(undefined4 *)(this + 8) = uVar50;
      *(undefined4 *)(this + 0xc) = uVar53;
      *(undefined4 *)(this + 0x10) = uVar56;
      *(undefined4 *)(this + 0x14) = uVar58;
      uVar45 = 0xc;
      uVar50 = 0x2b6;
      *(undefined4 *)(this + 0x18) = uVar52;
      *(undefined4 *)(this + 0x1c) = uVar67;
      *(undefined4 *)(this + 0x20) = uVar66;
      *(undefined4 *)(this + 0x24) = uVar66;
      *(undefined4 *)(this + 0x28) = uVar66;
      cVar59 = *pcVar46;
      if (cVar59 == '\0') {
        uVar49 = 0x1e;
      }
      uVar58 = 6;
      *(undefined4 *)(this + 0x30) = uVar49;
      if (cVar59 == '\0') {
        uVar58 = 3;
      }
      *(undefined4 *)(this + 0x2c) = uVar58;
      cVar59 = *pcVar46;
      if (cVar59 == '\0') {
        uVar55 = 7;
      }
      *(undefined4 *)(this + 0x38) = uVar55;
      if (cVar59 == '\0') {
        uVar45 = 6;
      }
      *(undefined4 *)(this + 0x34) = uVar45;
      uVar45 = 0x76;
      if (cVar59 == '\0') {
        uVar45 = 0x3b;
      }
      *(undefined4 *)(this + 0x3c) = uVar45;
      cVar59 = *pcVar46;
      uVar45 = 10;
      if (cVar59 == '\0') {
        uVar45 = 5;
      }
      *(undefined4 *)(this + 0x44) = uVar45;
      uVar45 = 200;
      if (cVar59 == '\0') {
        uVar45 = 100;
      }
      *(undefined4 *)(this + 0x40) = uVar45;
      iVar64 = DAT_000e18d8;
      cVar59 = *pcVar46;
      if (cVar59 == '\0') {
        uVar63 = 5;
      }
      uVar45 = 0x16;
      *(undefined4 *)(this + 0x4c) = uVar63;
      if (cVar59 == '\0') {
        uVar45 = 0xb;
      }
      *(undefined4 *)(this + 0x48) = uVar45;
      cVar59 = *pcVar46;
      pcVar57 = *(char **)(iVar64 + 0xe1668);
      uVar45 = 0xb2;
      if (cVar59 == '\0') {
        uVar50 = 0x15b;
      }
      *(undefined4 *)(this + 0x54) = uVar50;
      if (cVar59 == '\0') {
        uVar45 = 0x59;
      }
      *(undefined4 *)(this + 0x50) = uVar45;
      uVar45 = 0x1e6;
      if (cVar59 == '\0') {
        uVar45 = 0xf3;
      }
      *(undefined4 *)(this + 0x58) = uVar45;
      uVar11 = DAT_000e1558;
      if (*pcVar57 == '\0') {
        if (*pcVar46 == '\0') {
          uVar45 = 0x2d;
          cVar59 = *pcVar57;
          pcVar60 = *(char **)(DAT_000e18dc + 0xe16c2);
          if (*pcVar60 == '\0') {
            uVar45 = 0x1e;
          }
          *(undefined4 *)(this + 0x5c) = uVar45;
          if (cVar59 == '\0') {
            if (*pcVar46 == '\0') {
              uVar45 = 0x40;
              if (*pcVar60 == '\0') {
                uVar45 = 0x2c;
              }
            }
            else {
              uVar45 = 0x58;
            }
          }
          else {
            uVar45 = 0x80;
          }
        }
        else {
          uVar45 = 0x58;
          *(undefined4 *)(this + 0x5c) = 0x3c;
        }
        fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
        cVar59 = *pcVar57;
        *(int *)(this + 0x60) = (int)fVar69;
        if (cVar59 == '\0') {
          if (*pcVar46 == '\0') {
            uVar45 = 0x50;
            uVar50 = 0x193;
            bVar68 = **(char **)(DAT_000e18e0 + 0xe172a) == '\0';
            if (bVar68) {
              uVar45 = 0x3c;
            }
            *(undefined4 *)(this + 100) = uVar45;
            if (bVar68) {
              uVar50 = 0x12f;
            }
          }
          else {
            uVar50 = 0x25e;
            *(undefined4 *)(this + 100) = 0x78;
          }
          fVar69 = (float)VectorSignedToFloat(uVar50,(byte)(in_fpscr >> 0x16) & 3);
          *(int *)(this + 0x68) = (int)fVar69;
          if (*pcVar46 == '\0') {
            uVar45 = 0x88;
            if (**(char **)(DAT_000e18e4 + 0xe175e) == '\0') {
              uVar45 = 0x83;
            }
          }
          else {
            uVar45 = 0x106;
          }
        }
        else {
          *(undefined4 *)(this + 100) = 0xa0;
          *(undefined4 *)(this + 0x68) = 0x326;
          uVar45 = 0x110;
        }
      }
      else {
        uVar45 = 0x110;
        *(undefined8 *)(this + 0x5c) = DAT_000e1550;
        *(undefined8 *)(this + 100) = uVar11;
      }
      uVar50 = 0xfffffffc;
      fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x6c) = (int)fVar69;
      if (*pcVar46 == '\0') {
        uVar50 = 0xfffffffe;
      }
      *(undefined4 *)(this + 0x2c0) = uVar50;
      if (*pcVar57 == '\0') {
        cVar59 = *pcVar46;
        if (cVar59 == '\0') {
          uVar45 = 0x2d;
          if (**(char **)(DAT_000e18e8 + 0xe17aa) == '\0') {
            uVar45 = 0x1e;
          }
        }
        else {
          uVar45 = 0x3c;
        }
        fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
        *(int *)(this + 0x70) = (int)fVar69;
      }
      else {
        *(undefined4 *)(this + 0x70) = 0x5a;
        cVar59 = *pcVar46;
      }
      uVar45 = 0x20e;
      uVar50 = 0xe6;
      if (cVar59 == '\0') {
        uVar45 = 0x107;
        uVar50 = 0x73;
      }
      *(undefined4 *)(this + 0x74) = uVar50;
      *(undefined4 *)(this + 0x78) = uVar45;
      if (*pcVar57 == '\0') {
        if (*pcVar46 == '\0') {
          uVar45 = 0x5a;
          cVar59 = *pcVar46;
          pcVar60 = *(char **)(DAT_000e18ec + 0xe1800);
          if (*pcVar60 == '\0') {
            uVar45 = 0x3c;
          }
          *(undefined4 *)(this + 0x2c4) = uVar45;
          if (cVar59 == '\0') {
            uVar45 = 0x2d;
            if (*pcVar60 == '\0') {
              uVar45 = 0x1e;
            }
            goto LAB_000e1814;
          }
        }
        else {
          *(undefined4 *)(this + 0x2c4) = 0x78;
        }
        uVar45 = 0x3c;
      }
      else {
        uVar45 = 0x5a;
        *(undefined4 *)(this + 0x2c4) = 0xb4;
      }
LAB_000e1814:
      fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x2c8) = (int)fVar69;
      if (*pcVar57 == '\0') {
        if (*pcVar46 == '\0') {
          uVar45 = 0x5a;
          cVar59 = *pcVar46;
          pcVar46 = *(char **)(DAT_000e18f0 + 0xe184c);
          if (*pcVar46 == '\0') {
            uVar45 = 0x3c;
          }
          *(undefined4 *)(this + 0x2cc) = uVar45;
          if (cVar59 == '\0') {
            uVar45 = 0x2d;
            if (*pcVar46 == '\0') {
              uVar45 = 0x1e;
            }
            goto LAB_000e1860;
          }
        }
        else {
          *(undefined4 *)(this + 0x2cc) = 0x78;
        }
        uVar45 = 0x3c;
      }
      else {
        uVar45 = 0x5a;
        *(undefined4 *)(this + 0x2cc) = 0xb4;
      }
    }
    else {
      *(undefined4 *)(this + 0x74) = 0xcf;
      *(undefined4 *)(this + 0x78) = 0x1d9;
      uVar12 = DAT_000e1548;
      uVar11 = DAT_000e1540;
      *(undefined4 *)(this + 0x2c0) = 0xfffffffd;
      *(undefined4 *)(this + 0x2c4) = 0x6c;
      *(undefined4 *)(this + 0x2c8) = 0x2d;
      *(undefined4 *)(this + 0x2cc) = 0x6a;
      uVar45 = 0x35;
      *(undefined8 *)(this + 0x14) = uVar30;
      *(undefined8 *)(this + 0x1c) = uVar31;
      *(undefined8 *)(this + 4) = uVar32;
      *(undefined8 *)(this + 0xc) = uVar33;
      *(undefined8 *)(this + 0x24) = uVar34;
      *(undefined8 *)(this + 0x2c) = uVar35;
      *(undefined8 *)(this + 0x34) = uVar36;
      *(undefined8 *)(this + 0x3c) = uVar37;
      *(undefined8 *)(this + 0x44) = uVar38;
      *(undefined8 *)(this + 0x4c) = uVar39;
      *(undefined8 *)(this + 0x54) = uVar40;
      *(undefined8 *)(this + 0x5c) = uVar41;
      *(undefined8 *)(this + 100) = uVar11;
      *(undefined8 *)(this + 0x6c) = uVar12;
    }
LAB_000e1860:
    uVar53 = 200;
    uVar58 = 0xa0;
    fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
    pcVar46 = *(char **)(DAT_000e18f4 + 0xe1878);
    pbVar48 = *(byte **)(DAT_000e18f8 + 0xe187a);
    uVar50 = 0xa0;
    *(int *)(this + 0x2d0) = (int)fVar69;
    cVar59 = *pcVar46;
    uVar45 = 0x7d;
    uVar61 = (uint)*pbVar48;
    if (cVar59 == '\0') {
      uVar45 = 0x4b;
      uVar53 = 0x7d;
    }
    if (uVar61 == 0 && cVar43 == '\0') {
      uVar53 = uVar45;
    }
    uVar45 = 100;
    if (cVar59 == '\0') {
      uVar45 = 0x3c;
      uVar58 = 100;
    }
    if (uVar61 == 0 && cVar43 == '\0') {
      uVar58 = uVar45;
    }
    *(undefined4 *)(this + 0x2d4) = uVar58;
    *(undefined4 *)(this + 0x2d8) = uVar53;
    fVar69 = DAT_000e18fc;
    uVar11 = DAT_000e1568;
    if (cVar43 == '\0') {
      uVar58 = 0x14;
      uVar45 = 0x12;
      pfVar51 = (float *)&DAT_000e1d94;
      if (uVar61 == 0) {
        uVar58 = 10;
        uVar45 = 9;
        uVar50 = 0x50;
        pfVar51 = (float *)&DAT_000e1d98;
      }
      uVar53 = 0x3c;
      fVar69 = *pfVar51;
      if (uVar61 == 0) {
        uVar53 = 0x1e;
      }
      *(undefined4 *)(this + 0x7c) = uVar45;
      *(undefined4 *)(this + 0x80) = uVar58;
      *(undefined4 *)(this + 0x84) = uVar50;
      *(undefined4 *)(this + 0x88) = uVar53;
    }
    else {
      *(undefined8 *)(this + 0x7c) = DAT_000e1560;
      *(undefined8 *)(this + 0x84) = uVar11;
    }
    puVar47 = &DAT_000e2854;
    if ((cVar43 == '\0' && cVar59 == '\0') && uVar61 == 0) {
      puVar47 = &DAT_000e2858;
    }
    uVar45 = *puVar47;
    *(int *)(this + 0x8c) = (int)fVar69;
    *(undefined4 *)(this + 0x90) = uVar45;
    fVar69 = DAT_000e1900;
    if (cVar43 == '\0') {
      uVar45 = 0x348;
      pfVar51 = (float *)&DAT_000e285c;
      if (uVar61 == 0) {
        uVar45 = 0x1a4;
        pfVar51 = (float *)&DAT_000e2860;
      }
      uVar50 = 0x14;
      fVar69 = *pfVar51;
      if (uVar61 == 0) {
        uVar50 = 10;
      }
      *(undefined4 *)(this + 0x94) = uVar50;
    }
    else {
      uVar45 = 0x276;
      *(undefined4 *)(this + 0x94) = 0xf;
    }
    *(undefined4 *)(this + 0x98) = uVar45;
    pcVar57 = *(char **)(DAT_000e1d7c + 0xe19a8);
    *(int *)(this + 0x9c) = (int)fVar69;
    cVar62 = *pcVar57;
    if (cVar62 == '\0') {
      if (cVar43 == '\0' && cVar59 == '\0') {
        uVar45 = 0x52;
        if (uVar61 == 0) {
          uVar45 = 0x29;
        }
      }
      else {
        uVar45 = 0x3d;
      }
    }
    else {
      uVar45 = 0x7a;
    }
    *(undefined4 *)(this + 0xac) = uVar45;
    fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 0xa8) = (int)fVar69;
    uVar28 = DAT_000e2918;
    uVar27 = DAT_000e2910;
    uVar26 = DAT_000e2908;
    uVar25 = DAT_000e2900;
    uVar24 = DAT_000e28f8;
    uVar23 = DAT_000e28f0;
    uVar22 = DAT_000e28e8;
    uVar21 = DAT_000e28e0;
    uVar20 = DAT_000e28d8;
    uVar19 = DAT_000e28d0;
    uVar18 = DAT_000e28c8;
    uVar17 = DAT_000e28c0;
    uVar16 = DAT_000e28b8;
    uVar15 = DAT_000e28b0;
    uVar14 = DAT_000e28a8;
    uVar13 = DAT_000e28a0;
    uVar12 = DAT_000e2898;
    uVar11 = DAT_000e2890;
    uVar50 = DAT_000e1d84;
    uVar45 = DAT_000e1d80;
    if (cVar43 == '\0') {
      auVar80._8_8_ = SUB128(SUB1612((undefined1  [16])0x0,4),4);
      auVar80._0_8_ = 0x1f0000001f;
      auVar89._8_8_ = DAT_000e2928;
      auVar89._0_8_ = DAT_000e2920;
      auVar90._8_8_ = DAT_000e2938;
      auVar90._0_8_ = DAT_000e2930;
      uVar53 = 0x41a00000;
      uVar8 = CONCAT12(uVar61 == 0,(ushort)(uVar61 == 0));
      auVar88 = FloatVectorNeg(*(undefined1 (*) [16])(auVar80 << 0x40 | auVar80),1,4);
      auVar77._8_8_ = DAT_000e2948;
      auVar77._0_8_ = DAT_000e2940;
      uVar58 = 0x1e;
      auVar80 = VectorCopyLong((ulonglong)CONCAT34(uVar8,(uint)uVar8),2,1);
      auVar81._8_8_ = DAT_000e2958;
      auVar81._0_8_ = DAT_000e2950;
      uVar45 = 0x41200000;
      auVar84._8_8_ = DAT_000e2968;
      auVar84._0_8_ = DAT_000e2960;
      uVar50 = 0x28;
      if (uVar61 == 0) {
        uVar50 = 0x14;
      }
      auVar80 = VectorShiftLeft(auVar80,0x1f,0x20,0);
      auVar91._8_8_ = DAT_000e2978;
      auVar91._0_8_ = DAT_000e2970;
      uVar49 = 0x4a;
      uVar56 = 0x40000000;
      *(undefined4 *)(this + 0xd4) = uVar50;
      if (uVar61 == 0) {
        uVar49 = 0x25;
      }
      auVar80 = VectorShiftLeft(auVar80,auVar88,4,0);
      if (uVar61 == 0) {
        uVar45 = 0x41a00000;
        uVar56 = 0x3f800000;
        uVar53 = 0x41200000;
      }
      if (uVar61 == 0) {
        uVar58 = 0xf;
      }
      *(undefined4 *)(this + 0xd0) = uVar58;
      uVar50 = 0x32;
      uVar58 = 10;
      auVar88._8_8_ = DAT_000e2988;
      auVar88._0_8_ = DAT_000e2980;
      if (uVar61 == 0) {
        uVar50 = 0x19;
        uVar58 = 5;
      }
      auVar84 = VectorBitwiseSelect(auVar80,auVar91,auVar84);
      auVar71._8_8_ = DAT_000e2998;
      auVar71._0_8_ = DAT_000e2990;
      *(undefined4 *)(this + 0xa0) = uVar50;
      *(undefined4 *)(this + 0xa4) = uVar58;
      *(undefined4 *)(this + 0xd8) = uVar49;
      uVar49 = 4;
      if (uVar61 == 0) {
        uVar49 = 2;
      }
      uVar55 = 5;
      auVar6._8_4_ = (int)DAT_000e29a8;
      auVar6._0_8_ = DAT_000e29a0;
      auVar6._12_4_ = (int)((ulonglong)DAT_000e29a8 >> 0x20);
      auVar91 = VectorBitwiseSelect(auVar80,auVar6,auVar71);
      if (uVar61 == 0) {
        uVar55 = 3;
      }
      *(undefined4 *)(this + 0xfc) = uVar58;
      *(undefined4 *)(this + 0x100) = uVar49;
      uVar11 = DAT_000e29b0;
      uVar58 = 0xa8;
      uVar63 = (undefined4)DAT_000e29b8;
      uVar4 = (ulonglong)DAT_000e29b8 >> 0x20;
      uVar49 = 0x10;
      *(undefined4 *)(this + 0x104) = uVar55;
      if (uVar61 == 0) {
        uVar58 = 0x54;
      }
      *(undefined4 *)(this + 0x298) = uVar58;
      auVar7._8_4_ = uVar63;
      auVar7._0_8_ = uVar11;
      auVar7._12_4_ = (int)uVar4;
      auVar88 = VectorBitwiseSelect(auVar80,auVar7,auVar88);
      *(undefined4 *)(this + 0xdc) = uVar53;
      *(undefined4 *)(this + 0xe0) = uVar56;
      *(undefined4 *)(this + 0xe4) = uVar45;
      auVar89 = VectorBitwiseSelect(auVar80,auVar90,auVar89);
      *(undefined4 *)(this + 0xe8) = uVar56;
      *(int *)(this + 0x294) = auVar91._8_4_;
      auVar80 = VectorBitwiseSelect(auVar80,auVar81,auVar77);
      *(int *)(this + 0x29c) = auVar88._0_4_;
      uVar53 = 0x1e;
      uVar58 = 0x1c;
      *(longlong *)(this + 0xb0) = auVar80._0_8_;
      *(longlong *)(this + 0xb8) = auVar80._8_8_;
      *(longlong *)(this + 0xc0) = auVar89._0_8_;
      *(longlong *)(this + 200) = auVar89._8_8_;
      *(longlong *)(this + 0xec) = auVar91._0_8_;
      *(longlong *)(this + 0xf4) = auVar91._8_8_;
      *(longlong *)(this + 0x2a0) = auVar88._0_8_;
      *(longlong *)(this + 0x2a8) = auVar88._8_8_;
      *(longlong *)(this + 0x108) = auVar84._0_8_;
      *(longlong *)(this + 0x110) = auVar84._8_8_;
      uVar45 = 0x3c;
      if (uVar61 == 0) {
        uVar45 = 0x1e;
        uVar49 = 7;
      }
      *(undefined4 *)(this + 0x2b0) = uVar45;
      *(undefined4 *)(this + 0x2b4) = uVar50;
      *(undefined4 *)(this + 0x2b8) = uVar49;
      uVar45 = 0xe;
      if (uVar61 == 0) {
        uVar45 = 7;
      }
      uVar61 = (uint)*pbVar48;
      uVar56 = 2;
      if (uVar61 == 0) {
        uVar56 = 1;
        uVar58 = 0xf;
        uVar53 = 0xf;
      }
      *(undefined4 *)(this + 0x118) = uVar45;
      *(undefined4 *)(this + 0x11c) = uVar50;
      *(undefined4 *)(this + 0x120) = uVar58;
      *(undefined4 *)(this + 0x124) = uVar56;
      *(undefined4 *)(this + 0x128) = uVar56;
      *(undefined4 *)(this + 300) = uVar53;
      cVar62 = *pcVar57;
      if (cVar62 == '\0') {
        if (uVar61 == 0) {
          uVar45 = 0x6c;
          uVar50 = 0x38;
          if (*pcVar46 == '\0') {
            uVar45 = 0x50;
            uVar50 = 0x28;
          }
          *(undefined4 *)(this + 0x130) = uVar50;
          *(undefined4 *)(this + 0x134) = uVar45;
          uVar45 = 0x2f;
          if (*pcVar46 == '\0') {
            uVar45 = 0x23;
          }
        }
        else {
          *(undefined4 *)(this + 0x130) = 0x50;
          *(undefined4 *)(this + 0x134) = 0xa0;
          uVar45 = 0x46;
        }
      }
      else {
        *(undefined4 *)(this + 0x130) = 0x70;
        *(undefined4 *)(this + 0x134) = 0xd8;
        uVar45 = 0x5e;
      }
      auVar1._8_8_ = SUB128(SUB1612((undefined1  [16])0x0,4),4);
      auVar1._0_8_ = 0x1f0000001f;
      fVar70 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      uVar4 = (ulonglong)(uVar61 == 0) | (ulonglong)(uVar61 == 0) << 0x10;
      auVar80 = FloatVectorNeg(*(undefined1 (*) [16])(auVar1 << 0x40 | auVar1),1,4);
      bVar68 = true;
      bVar65 = 0;
      auVar89 = VectorCopyLong(uVar4 | uVar4 << 0x20,2,1);
      fVar69 = 8.0;
      auVar89 = VectorShiftLeft(auVar89,0x1f,0x20,0);
      auVar80 = VectorShiftLeft(auVar89,auVar80,4,0);
      auVar73._8_8_ = DAT_000e29c8;
      auVar73._0_8_ = DAT_000e29c0;
      auVar78._8_8_ = DAT_000e29d8;
      auVar78._0_8_ = DAT_000e29d0;
      uVar45 = 0x10e;
      auVar80 = VectorBitwiseSelect(auVar80,auVar78,auVar73);
      if (uVar61 == 0) {
        uVar45 = 0x87;
      }
      *(undefined4 *)(this + 0x14c) = uVar45;
      if (uVar61 == 0) {
        fVar69 = 4.0;
      }
      *(int *)(this + 0x138) = (int)fVar70;
      *(longlong *)(this + 0x13c) = auVar80._0_8_;
      *(longlong *)(this + 0x144) = auVar80._8_8_;
    }
    else {
      bVar68 = false;
      fVar69 = 6.0;
      bVar65 = 1;
      *(undefined4 *)(this + 0xa0) = 0x2d;
      *(undefined4 *)(this + 0xa4) = 9;
      *(undefined4 *)(this + 0xd0) = 0x16;
      *(undefined4 *)(this + 0xd4) = 0x24;
      *(undefined4 *)(this + 0xd8) = 0x37;
      *(undefined4 *)(this + 0xdc) = uVar45;
      *(undefined4 *)(this + 0xe0) = 0x3fc00000;
      *(undefined4 *)(this + 0xe4) = uVar50;
      *(undefined4 *)(this + 0xe8) = 0x3fc00000;
      *(undefined4 *)(this + 0xfc) = 0;
      *(undefined4 *)(this + 0x100) = 5;
      *(undefined4 *)(this + 0x104) = 4;
      *(undefined4 *)(this + 0x2b4) = 0x25;
      *(undefined4 *)(this + 0x2b8) = 0xe;
      *(undefined8 *)(this + 0xb0) = uVar11;
      *(undefined8 *)(this + 0xb8) = uVar12;
      *(undefined8 *)(this + 0xc0) = uVar13;
      *(undefined8 *)(this + 200) = uVar14;
      *(undefined8 *)(this + 0xec) = uVar15;
      *(undefined8 *)(this + 0xf4) = uVar16;
      *(undefined8 *)(this + 0x294) = uVar17;
      *(undefined8 *)(this + 0x29c) = uVar18;
      *(undefined8 *)(this + 0x2a4) = uVar19;
      *(undefined8 *)(this + 0x2ac) = uVar20;
      *(undefined8 *)(this + 0x118) = uVar21;
      *(undefined8 *)(this + 0x120) = uVar22;
      *(undefined8 *)(this + 0x108) = uVar23;
      *(undefined8 *)(this + 0x110) = uVar24;
      *(undefined8 *)(this + 0x128) = uVar25;
      *(undefined8 *)(this + 0x130) = uVar26;
      *(undefined8 *)(this + 0x140) = uVar27;
      *(undefined8 *)(this + 0x148) = uVar28;
      *(undefined4 *)(this + 0x138) = 0x34;
      *(undefined4 *)(this + 0x13c) = 0x19;
    }
    *(int *)(this + 0x150) = (int)fVar69;
    if (cVar62 == '\0') {
      cVar43 = *pcVar46;
      if (cVar43 == '\0') {
        if (bVar68) {
          uVar50 = 0x1e;
          if (uVar61 == 0) {
            uVar50 = 0xf;
          }
          uVar45 = 0x1c;
          *(undefined4 *)(this + 0x154) = uVar50;
          if (uVar61 == 0) {
            uVar45 = 0xe;
          }
          bVar42 = true;
        }
        else {
          uVar45 = 0xd;
          *(undefined4 *)(this + 0x154) = 0xe;
          bVar42 = false;
        }
      }
      else {
        uVar45 = 0x32;
        *(undefined4 *)(this + 0x154) = 0x32;
        bVar42 = bVar68;
      }
      uVar50 = 0x88;
      fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x158) = (int)fVar69;
      if (cVar43 != '\0') {
        uVar50 = 0x93;
      }
      bVar9 = (bool)(bVar42 & cVar43 != '\0');
      if ((cVar43 == '\0') && (bVar42)) {
        uVar50 = 0x9e;
        if (uVar61 == 0) {
          uVar50 = 0x4f;
        }
        bVar9 = true;
      }
      uVar45 = 0xd;
      fVar69 = (float)VectorSignedToFloat(uVar50,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x15c) = (int)fVar69;
      cVar43 = *pcVar46;
      if (cVar43 == '\0') {
        if (bVar9) {
          uVar45 = 0x1a;
          if (uVar61 == 0) {
            uVar45 = 0xd;
          }
          cVar43 = '\0';
        }
      }
      else {
        uVar45 = 0x31;
      }
    }
    else {
      *(undefined4 *)(this + 0x154) = 100;
      *(undefined4 *)(this + 0x158) = 100;
      *(undefined4 *)(this + 0x15c) = 0x126;
      uVar45 = 0x62;
      cVar43 = *pcVar46;
      bVar9 = bVar68;
    }
    fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 0x160) = (int)fVar69;
    fVar69 = DAT_000e1d88;
    if (cVar43 == '\0' && cVar62 == '\0') {
      if (bVar9) {
        uVar45 = 0x19;
        if (uVar61 == 0) {
          uVar45 = 0xd;
        }
        *(undefined4 *)(this + 0x164) = uVar45;
        goto LAB_000e1e38;
      }
      *(undefined4 *)(this + 0x164) = 0x17;
    }
    else {
      *(undefined4 *)(this + 0x164) = 0;
      fVar69 = DAT_000e1d88;
      if (bVar9) {
LAB_000e1e38:
        fVar69 = DAT_000e1d8c;
        if (uVar61 == 0) {
          fVar69 = 25.0;
        }
      }
    }
    *(int *)(this + 0x168) = (int)fVar69;
    uVar12 = DAT_000e29e8;
    uVar11 = DAT_000e29e0;
    if (cVar62 == '\0') {
      if (cVar43 == '\0') {
        if (bVar9) {
          uVar45 = 0x8a;
          if (uVar61 == 0) {
            uVar45 = 0x45;
          }
          bVar9 = true;
          *(undefined4 *)(this + 0x16c) = uVar45;
          uVar45 = 0x80;
          if (uVar61 == 0) {
            uVar45 = 0x40;
          }
        }
        else {
          bVar9 = false;
          *(undefined4 *)(this + 0x16c) = 0x73;
          uVar45 = 0x6a;
        }
      }
      else {
        *(undefined4 *)(this + 0x16c) = 0x83;
        uVar45 = 0x7c;
      }
      fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x170) = (int)fVar69;
      if (cVar43 == '\0') {
        bVar10 = 0;
        if (!bVar9) goto LAB_000e1f64;
        uVar45 = 0x3c;
        if (uVar61 == 0) {
          uVar45 = 0x1e;
        }
        *(undefined4 *)(this + 0x174) = uVar45;
LAB_000e1f4e:
        uVar45 = 0xaa;
        uVar50 = 0x2c;
        if (uVar61 == 0) {
          uVar45 = 0x55;
          uVar50 = 0x16;
        }
        *(undefined4 *)(this + 0x178) = uVar50;
        *(undefined4 *)(this + 0x17c) = uVar45;
LAB_000e1f84:
        uVar45 = 0x22;
        if (uVar61 == 0) {
          uVar45 = 0x11;
        }
        bVar42 = true;
      }
      else {
        bVar10 = 1;
LAB_000e1f64:
        uVar45 = 0x2d;
        if (cVar43 != '\0') {
          uVar45 = 0x2a;
        }
        *(undefined4 *)(this + 0x174) = uVar45;
        if (cVar43 == '\0') {
          if (!(bool)(bVar10 & bVar9)) {
            *(undefined4 *)(this + 0x178) = 0x1b;
            *(undefined4 *)(this + 0x17c) = 0x92;
            goto LAB_000e1f9e;
          }
          goto LAB_000e1f4e;
        }
        *(undefined4 *)(this + 0x178) = 0x3c;
        *(undefined4 *)(this + 0x17c) = 0x9b;
        if ((bool)(bVar10 & bVar9)) goto LAB_000e1f84;
LAB_000e1f9e:
        bVar42 = false;
        uVar45 = 0x1e;
      }
      *(undefined4 *)(this + 0x180) = uVar45;
      if (cVar43 == '\0') {
        if (bVar42) {
          uVar45 = 0xe8;
          if (uVar61 == 0) {
            uVar45 = 0x74;
          }
          *(undefined4 *)(this + 0x184) = uVar45;
          goto LAB_000e1fc6;
        }
        *(undefined4 *)(this + 0x184) = 0xd2;
LAB_000e1fda:
        bVar42 = false;
        uVar45 = 0x1e;
      }
      else {
        *(undefined4 *)(this + 0x184) = 0xad;
        if (!bVar42) goto LAB_000e1fda;
LAB_000e1fc6:
        uVar45 = 0x22;
        if (uVar61 == 0) {
          uVar45 = 0x11;
        }
        bVar42 = true;
      }
      *(undefined4 *)(this + 0x188) = uVar45;
      if (cVar43 == '\0') {
        if (bVar42) {
          uVar45 = 0x15c;
          uVar50 = 0xfffffffe;
          if (uVar61 == 0) {
            uVar45 = 0xae;
            uVar50 = 0xffffffff;
          }
          *(undefined4 *)(this + 0x18c) = uVar45;
          *(undefined4 *)(this + 400) = uVar50;
          goto LAB_000e2024;
        }
        *(undefined4 *)(this + 0x18c) = 0x140;
        *(undefined4 *)(this + 400) = 0xfffffffc;
LAB_000e203e:
        bVar42 = false;
        uVar45 = 10;
      }
      else {
        *(undefined4 *)(this + 0x18c) = 0xdf;
        *(undefined4 *)(this + 400) = 0xfffffffc;
        if (!bVar42) goto LAB_000e203e;
LAB_000e2024:
        uVar45 = 0xc;
        if (uVar61 == 0) {
          uVar45 = 6;
        }
        bVar42 = true;
      }
      *(undefined4 *)(this + 0x194) = uVar45;
      if (cVar43 == '\0') {
        if (!bVar42) {
          uVar45 = 10;
          uVar50 = 0x36;
          uVar58 = 9;
          goto LAB_000e1ef2;
        }
        uVar45 = 0x5a;
        uVar50 = 0xc;
        if (uVar61 == 0) {
          uVar45 = 0x2d;
          uVar50 = 6;
        }
        *(undefined4 *)(this + 0x198) = uVar50;
        *(undefined4 *)(this + 0x19c) = uVar45;
        uVar45 = 0xe;
        if (uVar61 == 0) {
          uVar45 = 7;
        }
        goto LAB_000e1ec0;
      }
      *(undefined4 *)(this + 0x198) = 0xc;
      *(undefined4 *)(this + 0x19c) = 0x40;
      *(undefined4 *)(this + 0x1a0) = 0xd;
      if (!bVar42) goto LAB_000e2086;
LAB_000e2064:
      uVar45 = 0x20;
      bVar42 = true;
      fVar69 = DAT_000e1d90;
      if (uVar61 == 0) {
        fVar69 = 16.0;
        uVar45 = 0x10;
      }
      *(undefined4 *)(this + 0x1a4) = uVar45;
    }
    else {
      *(undefined4 *)(this + 0x17c) = 0x136;
      *(undefined8 *)(this + 0x16c) = uVar11;
      *(undefined8 *)(this + 0x174) = uVar12;
      uVar12 = DAT_000e29f8;
      uVar11 = DAT_000e29f0;
      if (bVar9) {
        uVar45 = 0x22;
        if (uVar61 == 0) {
          uVar45 = 0x11;
        }
        uVar50 = 0xc;
        if (uVar61 == 0) {
          uVar50 = 6;
        }
        *(undefined4 *)(this + 0x180) = uVar45;
        *(undefined4 *)(this + 0x184) = 0x15a;
        *(undefined4 *)(this + 0x188) = uVar45;
        *(undefined4 *)(this + 0x18c) = 0x1be;
        *(undefined4 *)(this + 400) = 0xfffffff8;
        *(undefined4 *)(this + 0x194) = uVar50;
        *(undefined4 *)(this + 0x198) = 0x18;
        *(undefined4 *)(this + 0x19c) = 0x80;
        uVar45 = 0x1a;
LAB_000e1ec0:
        *(undefined4 *)(this + 0x1a0) = uVar45;
        goto LAB_000e2064;
      }
      uVar58 = 0x18;
      *(undefined4 *)(this + 400) = 0xfffffff8;
      *(undefined4 *)(this + 0x194) = 10;
      uVar50 = 0x80;
      *(undefined8 *)(this + 0x180) = uVar11;
      *(undefined8 *)(this + 0x188) = uVar12;
      uVar45 = 0x1a;
LAB_000e1ef2:
      *(undefined4 *)(this + 0x198) = uVar58;
      *(undefined4 *)(this + 0x19c) = uVar50;
      *(undefined4 *)(this + 0x1a0) = uVar45;
LAB_000e2086:
      fVar69 = 24.0;
      *(undefined4 *)(this + 0x1a4) = 0x1c;
      bVar42 = false;
    }
    *(int *)(this + 0x1a8) = (int)fVar69;
    if (cVar62 == '\0') {
      if (cVar43 != '\0') {
        uVar45 = 0xad;
        goto LAB_000e20ac;
      }
      if (!bVar42) {
        *(undefined4 *)(this + 0x1ac) = 0xd0;
        goto LAB_000e21a2;
      }
      uVar45 = 0xe8;
      if (uVar61 == 0) {
        uVar45 = 0x74;
      }
      *(undefined4 *)(this + 0x1ac) = uVar45;
    }
    else {
      uVar45 = 0x15a;
LAB_000e20ac:
      *(undefined4 *)(this + 0x1ac) = uVar45;
      if (!bVar42) {
LAB_000e21a2:
        uVar16 = DAT_000e2a28;
        uVar15 = DAT_000e2a20;
        uVar14 = DAT_000e2a18;
        uVar13 = DAT_000e2a10;
        uVar12 = DAT_000e2a08;
        uVar11 = DAT_000e2a00;
        fVar69 = 6.0;
        bVar42 = false;
        *(undefined4 *)(this + 0x1e0) = 0x40000000;
        *(undefined8 *)(this + 0x1c0) = uVar15;
        *(undefined8 *)(this + 0x1c8) = uVar16;
        *(undefined8 *)(this + 0x1b0) = uVar11;
        *(undefined8 *)(this + 0x1b8) = uVar12;
        *(undefined8 *)(this + 0x1d0) = uVar13;
        *(undefined8 *)(this + 0x1d8) = uVar14;
        goto LAB_000e21e2;
      }
    }
    auVar2._8_8_ = SUB128(SUB1612((undefined1  [16])0x0,4),4);
    auVar2._0_8_ = 0x1f0000001f;
    uVar45 = 0x40000000;
    uVar8 = CONCAT12(uVar61 == 0,(ushort)(uVar61 == 0));
    auVar80 = FloatVectorNeg(*(undefined1 (*) [16])(auVar2 << 0x40 | auVar2),1,4);
    bVar42 = true;
    auVar89 = VectorCopyLong((ulonglong)CONCAT34(uVar8,(uint)uVar8),2,1);
    auVar79._8_8_ = DAT_000e2a38;
    auVar79._0_8_ = DAT_000e2a30;
    fVar69 = 8.0;
    auVar82._8_8_ = DAT_000e2a48;
    auVar82._0_8_ = DAT_000e2a40;
    if (uVar61 == 0) {
      uVar45 = 0x3f800000;
    }
    auVar89 = VectorShiftLeft(auVar89,0x1f,0x20,0);
    auVar80 = VectorShiftLeft(auVar89,auVar80,4,0);
    auVar74._8_8_ = DAT_000e2a58;
    auVar74._0_8_ = DAT_000e2a50;
    auVar85._8_8_ = DAT_000e2a68;
    auVar85._0_8_ = DAT_000e2a60;
    if (uVar61 == 0) {
      fVar69 = 4.0;
    }
    *(undefined4 *)(this + 0x1e0) = uVar45;
    auVar89 = VectorBitwiseSelect(auVar80,auVar85,auVar74);
    auVar75._8_8_ = DAT_000e2a78;
    auVar75._0_8_ = DAT_000e2a70;
    auVar86._8_8_ = DAT_000e2a88;
    auVar86._0_8_ = DAT_000e2a80;
    auVar90 = VectorBitwiseSelect(auVar80,auVar86,auVar75);
    *(longlong *)(this + 0x1c0) = auVar89._0_8_;
    *(longlong *)(this + 0x1c8) = auVar89._8_8_;
    auVar80 = VectorBitwiseSelect(auVar80,auVar82,auVar79);
    *(longlong *)(this + 0x1b0) = auVar90._0_8_;
    *(longlong *)(this + 0x1b8) = auVar90._8_8_;
    *(longlong *)(this + 0x1d0) = auVar80._0_8_;
    *(longlong *)(this + 0x1d8) = auVar80._8_8_;
  }
  else {
    puVar47 = (undefined4 *)(DAT_000e1318 + 0xe105c);
    *(undefined8 *)(this + 0x14) = DAT_000e13a0;
    *(undefined8 *)(this + 0x1c) = uVar29;
    uVar29 = DAT_000e13b8;
    *(undefined8 *)(this + 4) = DAT_000e13b0;
    *(undefined8 *)(this + 0xc) = uVar29;
    uVar29 = DAT_000e13c8;
    *(undefined8 *)(this + 0x24) = DAT_000e13c0;
    *(undefined8 *)(this + 0x2c) = uVar29;
    uVar29 = DAT_000e13d8;
    *(undefined8 *)(this + 0x34) = DAT_000e13d0;
    *(undefined8 *)(this + 0x3c) = uVar29;
    uVar29 = DAT_000e13e8;
    *(undefined8 *)(this + 0x44) = DAT_000e13e0;
    *(undefined8 *)(this + 0x4c) = uVar29;
    uVar29 = DAT_000e13f8;
    *(undefined8 *)(this + 0x54) = DAT_000e13f0;
    *(undefined8 *)(this + 0x5c) = uVar29;
    uVar29 = DAT_000e1408;
    *(undefined8 *)(this + 100) = DAT_000e1400;
    *(undefined8 *)(this + 0x6c) = uVar29;
    uVar29 = DAT_000e1418;
    *(undefined8 *)(this + 0x2c8) = DAT_000e1410;
    *(undefined8 *)(this + 0x2d0) = uVar29;
    uVar29 = DAT_000e1428;
    *(undefined8 *)(this + 0x7c) = DAT_000e1420;
    *(undefined8 *)(this + 0x84) = uVar29;
    *(undefined4 *)(this + 0x74) = 0xa1;
    *(undefined4 *)(this + 0x78) = 0x171;
    uVar45 = DAT_000e1440;
    *(undefined4 *)(this + 0x2c0) = 0xfffffffe;
    *(undefined4 *)(this + 0x2c4) = 0x7e;
    *(undefined4 *)(this + 0x2d8) = 0x8d;
    pbVar48 = (byte *)*puVar47;
    *(undefined4 *)(this + 0xac) = 0x55;
    *(undefined4 *)(this + 0x8c) = 0x2d;
    *(undefined4 *)(this + 0x90) = uVar45;
    *(undefined4 *)(this + 0x94) = 0xe;
    *(undefined4 *)(this + 0x98) = 0x24e;
    *(undefined4 *)(this + 0x9c) = 0x46;
    *(undefined4 *)(this + 0xa0) = 0x23;
    *(undefined4 *)(this + 0xa4) = 7;
    *(undefined4 *)(this + 0xa8) = 0x55;
    uVar45 = DAT_000e1444;
    bVar65 = *pbVar48;
    *(undefined8 *)(this + 0xc0) = uVar11;
    *(undefined8 *)(this + 200) = uVar12;
    uVar11 = DAT_000e1450;
    *(undefined8 *)(this + 0xb0) = DAT_000e1448;
    *(undefined8 *)(this + 0xb8) = uVar11;
    uVar11 = DAT_000e1460;
    *(undefined8 *)(this + 0xec) = DAT_000e1458;
    *(undefined8 *)(this + 0xf4) = uVar11;
    uVar11 = DAT_000e1470;
    *(undefined8 *)(this + 0x294) = DAT_000e1468;
    *(undefined8 *)(this + 0x29c) = uVar11;
    uVar11 = DAT_000e1480;
    *(undefined8 *)(this + 0x2a4) = DAT_000e1478;
    *(undefined8 *)(this + 0x2ac) = uVar11;
    uVar11 = DAT_000e1490;
    *(undefined8 *)(this + 0x108) = DAT_000e1488;
    *(undefined8 *)(this + 0x110) = uVar11;
    uVar11 = DAT_000e14a0;
    *(undefined8 *)(this + 0x118) = DAT_000e1498;
    *(undefined8 *)(this + 0x120) = uVar11;
    iVar64 = DAT_000e14c0;
    uVar58 = DAT_000e14bc;
    uVar50 = DAT_000e14b8;
    uVar30 = DAT_000e14b0;
    uVar29 = DAT_000e14a8;
    uVar12 = DAT_000e1438;
    uVar11 = DAT_000e1430;
    *(undefined4 *)(this + 0xd0) = 0x15;
    *(undefined4 *)(this + 0xd4) = 0x1c;
    *(undefined4 *)(this + 0xd8) = 0x34;
    *(undefined4 *)(this + 0xdc) = uVar58;
    *(undefined4 *)(this + 0xe0) = uVar45;
    *(undefined4 *)(this + 0xe4) = uVar50;
    *(undefined4 *)(this + 0xe8) = uVar45;
    *(undefined4 *)(this + 0xfc) = 7;
    *(undefined4 *)(this + 0x100) = 2;
    *(undefined4 *)(this + 0x104) = 5;
    *(undefined4 *)(this + 0x2b4) = 0x23;
    *(undefined4 *)(this + 0x2b8) = 9;
    pbVar48 = *(byte **)(iVar64 + 0xe11ca);
    *(undefined8 *)(this + 0x128) = uVar13;
    *(undefined8 *)(this + 0x130) = uVar14;
    uVar14 = DAT_000e14d0;
    uVar13 = DAT_000e14c8;
    *(undefined8 *)(this + 0x140) = uVar15;
    *(undefined8 *)(this + 0x148) = uVar16;
    *(undefined8 *)(this + 0x150) = uVar17;
    *(undefined8 *)(this + 0x158) = uVar18;
    *(undefined8 *)(this + 0x160) = uVar19;
    *(undefined8 *)(this + 0x168) = uVar20;
    *(undefined8 *)(this + 0x170) = uVar21;
    *(undefined8 *)(this + 0x178) = uVar22;
    *(undefined8 *)(this + 0x180) = uVar23;
    *(undefined8 *)(this + 0x188) = uVar24;
    *(undefined8 *)(this + 400) = uVar25;
    *(undefined8 *)(this + 0x198) = uVar26;
    *(undefined8 *)(this + 0x1a0) = uVar27;
    *(undefined8 *)(this + 0x1a8) = uVar28;
    *(undefined8 *)(this + 0x1b0) = uVar11;
    *(undefined8 *)(this + 0x1b8) = uVar12;
    *(undefined8 *)(this + 0x1c0) = uVar29;
    *(undefined8 *)(this + 0x1c8) = uVar30;
    *(undefined8 *)(this + 0x1d0) = uVar13;
    *(undefined8 *)(this + 0x1d8) = uVar14;
    *(undefined4 *)(this + 0x138) = 0x42;
    *(undefined4 *)(this + 0x13c) = 0x17;
    *(undefined4 *)(this + 0x1e0) = uVar45;
    bVar68 = bVar65 == 0 && cVar5 == '\0';
    uVar61 = (uint)*pbVar48;
    bVar42 = bVar65 == 0;
    fVar69 = DAT_000e14d8;
  }
LAB_000e21e2:
  uVar44 = 1;
  if (bVar68) {
    uVar44 = uVar61;
  }
  *(uint *)(this + 0x1e8) = uVar44;
  *(int *)(this + 0x1e4) = (int)fVar69;
  uVar38 = DAT_000e2b68;
  uVar37 = DAT_000e2b60;
  uVar36 = DAT_000e2b58;
  uVar35 = DAT_000e2b50;
  uVar34 = DAT_000e2b48;
  uVar33 = DAT_000e2b40;
  uVar32 = DAT_000e2b38;
  uVar31 = DAT_000e2b30;
  uVar30 = DAT_000e2b28;
  uVar29 = DAT_000e2b20;
  uVar28 = DAT_000e2b18;
  uVar27 = DAT_000e2b10;
  uVar26 = DAT_000e2b08;
  uVar25 = DAT_000e2b00;
  uVar24 = DAT_000e2af8;
  uVar23 = DAT_000e2af0;
  uVar22 = DAT_000e2ae8;
  uVar21 = DAT_000e2ae0;
  uVar20 = DAT_000e2ad8;
  uVar19 = DAT_000e2ad0;
  uVar18 = DAT_000e2ac8;
  uVar17 = DAT_000e2ac0;
  uVar16 = DAT_000e2ab8;
  uVar15 = DAT_000e2ab0;
  uVar14 = DAT_000e2aa8;
  uVar13 = DAT_000e2aa0;
  uVar12 = DAT_000e2a98;
  uVar11 = DAT_000e2a90;
  if (cVar5 == '\0') {
    if (bVar42) {
      auVar3._8_8_ = SUB128(SUB1612((undefined1  [16])0x0,4),4);
      auVar3._0_8_ = 0x1f0000001f;
      auVar72._8_8_ = DAT_000e2b78;
      auVar72._0_8_ = DAT_000e2b70;
      uVar45 = 4;
      auVar76._8_8_ = DAT_000e2b88;
      auVar76._0_8_ = DAT_000e2b80;
      auVar80 = FloatVectorNeg(*(undefined1 (*) [16])(auVar3 << 0x40 | auVar3),1,4);
      if (uVar61 == 0) {
        uVar45 = 2;
      }
      auVar83._8_8_ = DAT_000e2b98;
      auVar83._0_8_ = DAT_000e2b90;
      uVar50 = 0x26;
      auVar87._8_8_ = DAT_000e2ba8;
      auVar87._0_8_ = DAT_000e2ba0;
      uVar53 = 0x28;
      uVar58 = 8;
      *(undefined4 *)(this + 0x1fc) = uVar45;
      uVar8 = CONCAT12(uVar61 == 0,(ushort)(uVar61 == 0));
      if (uVar61 == 0) {
        uVar50 = 0x13;
      }
      *(undefined4 *)(this + 0x1ec) = uVar50;
      if (uVar61 == 0) {
        uVar53 = 0x14;
      }
      auVar89 = VectorCopyLong((ulonglong)CONCAT34(uVar8,(uint)uVar8),2,1);
      uVar50 = 0x46;
      *(undefined4 *)(this + 0x224) = uVar53;
      if (uVar61 == 0) {
        uVar50 = 0x23;
      }
      *(undefined4 *)(this + 0x228) = uVar50;
      uVar50 = 2;
      auVar89 = VectorShiftLeft(auVar89,0x1f,0x20,0);
      *(undefined4 *)(this + 0x22c) = uVar45;
      if (uVar61 == 0) {
        uVar50 = 1;
      }
      uVar56 = 100;
      *(undefined4 *)(this + 0x230) = uVar50;
      auVar80 = VectorShiftLeft(auVar89,auVar80,4,0);
      *(undefined4 *)(this + 0x234) = uVar50;
      if (uVar61 == 0) {
        uVar56 = 0x32;
      }
      *(undefined4 *)(this + 0x238) = uVar56;
      uVar56 = 0;
      if (uVar61 != 0) {
        uVar56 = 0xfffffffe;
      }
      *(undefined4 *)(this + 0x24c) = uVar56;
      *(undefined4 *)(this + 0x250) = uVar50;
      auVar89 = VectorBitwiseSelect(auVar80,auVar87,auVar83);
      auVar80 = VectorBitwiseSelect(auVar80,auVar76,auVar72);
      uVar56 = auVar89._4_4_;
      *(undefined4 *)(this + 0x20c) = uVar56;
      *(undefined4 *)(this + 0x218) = uVar56;
      *(undefined4 *)(this + 0x220) = uVar56;
      uVar56 = 0x12;
      if (uVar61 == 0) {
        uVar56 = 9;
        uVar58 = 4;
      }
      *(undefined4 *)(this + 0x204) = uVar56;
      *(undefined4 *)(this + 0x208) = uVar58;
      uVar56 = 0xfffffffe;
      if (uVar61 == 0) {
        uVar56 = 1;
      }
      *(undefined4 *)(this + 0x210) = uVar58;
      *(undefined4 *)(this + 0x214) = uVar56;
      uVar56 = 0xc;
      if (uVar61 == 0) {
        uVar56 = 6;
      }
      *(undefined4 *)(this + 0x21c) = uVar56;
      *(longlong *)(this + 0x23c) = auVar89._0_8_;
      *(longlong *)(this + 0x244) = auVar89._8_8_;
      *(int *)(this + 0x278) = auVar80._0_4_;
      *(longlong *)(this + 0x264) = auVar80._0_8_;
      *(longlong *)(this + 0x26c) = auVar80._8_8_;
      uVar56 = 0x20;
      *(undefined4 *)(this + 0x254) = uVar58;
      *(undefined4 *)(this + 600) = uVar45;
      if (uVar61 == 0) {
        uVar56 = 0x10;
      }
      *(undefined4 *)(this + 0x25c) = uVar56;
      *(uint *)(this + 0x260) = uVar61 ^ 1;
      uVar45 = 0xc2;
      if (uVar61 == 0) {
        uVar45 = 0x61;
      }
      *(undefined4 *)(this + 0x274) = uVar45;
      *(undefined4 *)(this + 0x27c) = uVar53;
      uVar45 = 0;
      if (uVar61 != 0) {
        uVar45 = 5;
      }
    }
    else {
      uVar45 = 4;
      *(undefined4 *)(this + 0x1fc) = 4;
      *(undefined4 *)(this + 0x1ec) = 0x22;
      uVar50 = 2;
      *(undefined4 *)(this + 0x234) = 2;
      *(undefined4 *)(this + 0x238) = 0x5a;
      *(undefined4 *)(this + 0x27c) = 0x1e;
      *(undefined8 *)(this + 0x204) = uVar25;
      *(undefined8 *)(this + 0x20c) = uVar26;
      *(undefined8 *)(this + 0x214) = uVar27;
      *(undefined8 *)(this + 0x21c) = uVar28;
      *(undefined8 *)(this + 0x224) = uVar29;
      *(undefined8 *)(this + 0x22c) = uVar30;
      *(undefined8 *)(this + 0x23c) = uVar31;
      *(undefined8 *)(this + 0x244) = uVar32;
      *(undefined8 *)(this + 0x24c) = uVar33;
      *(undefined8 *)(this + 0x254) = uVar34;
      *(undefined8 *)(this + 0x25c) = uVar35;
      *(undefined8 *)(this + 0x264) = uVar36;
      *(undefined8 *)(this + 0x26c) = uVar37;
      *(undefined8 *)(this + 0x274) = uVar38;
    }
    fVar69 = (float)VectorSignedToFloat(uVar50,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(this + 0x280) = uVar45;
    uVar44 = uVar61;
    if (!bVar42) {
      uVar44 = 1;
    }
    *(uint *)(this + 0x3e4) = uVar44;
    *(int *)(this + 0x3e0) = (int)fVar69;
    if (!bVar42) {
      *(undefined4 *)(this + 1000) = 0x70;
      *(undefined4 *)(this + 0x3f4) = 0x39;
      *(undefined4 *)(this + 0x3f8) = 0x6f;
      goto LAB_000e24de;
    }
    if (**(char **)(DAT_000e2864 + 0xe24c2) == '\0') {
      if (**(char **)(DAT_000e2868 + 0xe24f0) == '\0') {
        uVar45 = 0x96;
        if (uVar61 == 0) {
          uVar45 = 0x4b;
        }
        *(undefined4 *)(this + 1000) = uVar45;
        iVar64 = 0x4c;
        if (uVar61 == 0) {
          iVar64 = 0x26;
        }
      }
      else {
        *(undefined4 *)(this + 1000) = 100;
        iVar64 = 0x37;
      }
    }
    else {
      *(undefined4 *)(this + 1000) = 200;
      iVar64 = 0x6e;
    }
    fVar69 = (float)VectorSignedToFloat(iVar64 + 0x11 << uVar61,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 0x3f4) = iVar64;
    *(int *)(this + 0x3f8) = (int)fVar69;
  }
  else {
    *(undefined4 *)(this + 0x1fc) = 2;
    *(undefined4 *)(this + 0x1ec) = 0x1a;
    *(undefined4 *)(this + 0x234) = 1;
    *(undefined4 *)(this + 0x238) = 0x46;
    *(undefined4 *)(this + 0x27c) = 0x1c;
    *(undefined4 *)(this + 0x280) = 0;
    *(undefined4 *)(this + 0x3e0) = 1;
    *(undefined4 *)(this + 0x3e4) = 0;
    *(undefined4 *)(this + 1000) = 0x8c;
    *(undefined4 *)(this + 0x3f4) = 0x4d;
    *(undefined4 *)(this + 0x3f8) = 0x84;
    *(undefined8 *)(this + 0x204) = uVar11;
    *(undefined8 *)(this + 0x20c) = uVar12;
    *(undefined8 *)(this + 0x214) = uVar13;
    *(undefined8 *)(this + 0x21c) = uVar14;
    *(undefined8 *)(this + 0x224) = uVar15;
    *(undefined8 *)(this + 0x22c) = uVar16;
    *(undefined8 *)(this + 0x23c) = uVar17;
    *(undefined8 *)(this + 0x244) = uVar18;
    *(undefined8 *)(this + 0x24c) = uVar19;
    *(undefined8 *)(this + 0x254) = uVar20;
    *(undefined8 *)(this + 0x25c) = uVar21;
    *(undefined8 *)(this + 0x264) = uVar22;
    *(undefined8 *)(this + 0x26c) = uVar23;
    *(undefined8 *)(this + 0x274) = uVar24;
    if (!bVar42) {
LAB_000e24de:
      *(undefined4 *)(this + 0x3ec) = 0x160;
      uVar45 = 0x4b;
      goto LAB_000e2548;
    }
  }
  uVar45 = 0x188;
  if (uVar61 == 0) {
    uVar45 = 0xc4;
  }
  *(undefined4 *)(this + 0x3ec) = uVar45;
  uVar45 = 100;
  if (uVar61 == 0) {
    uVar45 = 0x32;
  }
LAB_000e2548:
  *(undefined4 *)(this + 0x3f0) = uVar45;
  uVar45 = 2;
  if (cVar5 == '\0') {
    if (uVar61 != 0) {
      uVar45 = 4;
    }
    uVar50 = 0x10;
    uVar58 = 0xffffffe6;
    if (uVar61 != 0) {
      uVar50 = 0x20;
    }
    if (**(char **)(DAT_000e286c + 0xe2578) == '\0') {
      uVar50 = uVar45;
    }
    uVar53 = 6;
    uVar45 = 3;
    fVar69 = (float)VectorSignedToFloat(uVar50,(byte)(in_fpscr >> 0x16) & 3);
    iVar64 = (int)fVar69;
    if (uVar61 == 0) {
      uVar58 = 0xfffffff3;
    }
    if (!bVar42) {
      uVar58 = 0xffffffe6;
    }
    uVar50 = 0x26;
    if (uVar61 == 0) {
      uVar50 = 0x13;
    }
    if (!bVar42) {
      uVar50 = 0x22;
    }
    if (uVar61 == 0) {
      uVar53 = 3;
    }
    if (bVar42) {
      uVar45 = 2;
    }
    else {
      uVar53 = 5;
    }
    uVar56 = 4;
    if (**(char **)(DAT_000e286c + 0xe2578) == '\0') {
      uVar56 = 2;
    }
    if (uVar61 == 0) {
      uVar56 = 2;
    }
  }
  else {
    uVar58 = 0xffffffee;
    iVar64 = 0x16;
    uVar50 = 0x1a;
    uVar53 = 4;
    uVar56 = 2;
  }
  *(undefined4 *)(this + 700) = uVar45;
  *(undefined4 *)(this + 0x3fc) = uVar53;
  *(undefined4 *)(this + 0x200) = uVar58;
  uVar58 = 7;
  *(undefined4 *)(this + 0x1f0) = uVar50;
  *(int *)(this + 500) = iVar64;
  *(undefined4 *)(this + 0x1f8) = uVar56;
  this[0x284] = (Layout)(bVar65 ^ 1);
  this[0x285] = (Layout)0x1;
  uVar50 = 0xffffffed;
  this[0x286] = (Layout)(bVar65 ^ 1);
  uVar45 = 0x14;
  if (bVar42) {
    uVar45 = 0;
    uVar50 = 0;
  }
  if (uVar61 == 0) {
    uVar58 = 3;
  }
  uVar53 = 4;
  if (bVar42) {
    uVar53 = uVar58;
  }
  *(undefined4 *)(this + 0x288) = uVar45;
  *(undefined4 *)(this + 0x28c) = uVar50;
  *(undefined4 *)(this + 0x290) = uVar53;
  uVar45 = DAT_000e2874;
  if (cVar5 == '\0') {
    cVar43 = **(char **)(DAT_000e2870 + 0xe265a);
    if (cVar43 == '\0') {
      if (**(char **)(DAT_000e287c + 0xe2692) == '\0') {
        puVar47 = &DAT_000e2bb0;
        if (uVar61 == 0) {
          puVar47 = &DAT_000e2bb4;
        }
        uVar50 = 0x3c;
        uVar45 = DAT_000e2884;
        if (bVar42) {
          uVar45 = *puVar47;
        }
        if (uVar61 == 0) {
          uVar50 = 0x1e;
        }
        if (!bVar42) {
          uVar50 = 0x2d;
        }
      }
      else {
        uVar50 = 0x50;
        uVar45 = DAT_000e2880;
      }
    }
    else {
      uVar50 = 0xa0;
      uVar45 = DAT_000e2878;
    }
    *(undefined4 *)(this + 0x2f4) = uVar50;
    uVar50 = 0x48;
    uVar53 = 400;
    uVar56 = 700;
    if (cVar43 == '\0') {
      uVar50 = 0x24;
    }
    *(undefined4 *)(this + 0x2f8) = uVar45;
    if (cVar43 == '\0') {
      uVar53 = 200;
    }
    if (cVar43 == '\0') {
      uVar56 = 0x15e;
    }
    *(undefined4 *)(this + 0x2fc) = uVar50;
    *(undefined4 *)(this + 0x300) = uVar53;
    *(undefined4 *)(this + 0x304) = uVar56;
    uVar45 = 500;
    if (cVar43 == '\0') {
      uVar45 = 0xfa;
    }
  }
  else {
    pcVar46 = *(char **)(DAT_000e2870 + 0xe265a);
    *(undefined4 *)(this + 0x2f4) = 0x70;
    *(undefined4 *)(this + 0x2f8) = uVar45;
    uVar45 = 0x15f;
    *(undefined4 *)(this + 0x2fc) = 0x32;
    *(undefined4 *)(this + 0x300) = 0x119;
    *(undefined4 *)(this + 0x304) = 0x1ec;
    cVar43 = *pcVar46;
  }
  *(undefined4 *)(this + 0x308) = uVar45;
  if (cVar43 == '\0') {
    cVar59 = **(char **)(DAT_000e2888 + 0xe2728);
    if (cVar59 == '\0') {
      if (bVar42) {
        uVar45 = 0xfffffff7;
        if (uVar61 == 0) {
          uVar45 = 0xfffffffb;
        }
        *(undefined4 *)(this + 0x30c) = uVar58;
        *(undefined4 *)(this + 0x310) = uVar45;
        uVar45 = 0xfffffff8;
        if (uVar61 == 0) {
          uVar45 = 0xfffffffb;
        }
      }
      else {
        uVar45 = 0xfffffff8;
        *(undefined4 *)(this + 0x30c) = 4;
        *(undefined4 *)(this + 0x310) = 0xfffffff8;
      }
    }
    else {
      *(undefined4 *)(this + 0x30c) = 3;
      *(undefined4 *)(this + 0x310) = 0xfffffff4;
      uVar45 = 0xfffffffb;
    }
    *(undefined4 *)(this + 0x314) = uVar45;
    uVar45 = 0;
    if (cVar59 != '\0') {
      uVar45 = 4;
    }
  }
  else {
    *(undefined4 *)(this + 0x30c) = 6;
    *(undefined4 *)(this + 0x310) = 0xffffffe8;
    *(undefined4 *)(this + 0x314) = 0xfffffff6;
    uVar45 = 8;
  }
  *(undefined4 *)(this + 0x318) = uVar45;
  if (cVar5 == '\0') {
    if (cVar43 == '\0') {
      pfVar51 = (float *)&DAT_000e2bb8;
      pfVar54 = (float *)&DAT_000e2bc0;
      cVar5 = **(char **)(DAT_000e288c + 0xe27a8);
      if (cVar5 != '\0') {
        pfVar51 = (float *)&DAT_000e2bbc;
      }
      if (uVar61 == 0) {
        pfVar54 = (float *)&DAT_000e2bc4;
      }
      uVar45 = 0xec;
      fVar69 = *pfVar51;
      if (bVar42) {
        fVar69 = *pfVar54;
      }
      if (cVar5 != '\0') {
        fVar69 = *pfVar51;
      }
      if (cVar5 == '\0' && uVar61 == 0) {
        uVar45 = 0x76;
      }
      if (!bVar42) {
        uVar45 = 0xec;
      }
      iVar64 = (int)fVar69;
      *(undefined4 *)(this + 0x31c) = uVar45;
    }
    else {
      *(undefined4 *)(this + 0x31c) = 0x1d8;
      iVar64 = 0x120;
    }
  }
  else {
    *(undefined4 *)(this + 0x31c) = 0x14b;
    iVar64 = 0xca;
  }
  *(int *)(this + 800) = iVar64;
  return;
}

```

## target disasm
```
  000e1028: push {r4,r5,r6,r7,lr}
  000e102a: add r7,sp,#0xc
  000e102c: push {r6,r7,r8,r9,r10,r11}
  000e1030: ldr r1,[0x000e1300]
  000e1032: add r1,pc
  000e1034: ldr r1,[r1,#0x0]
  000e1036: ldrb.w r10,[r1,#0x0]
  000e103a: cmp.w r10,#0x0
  000e103e: beq.w 0x000e126c
  000e1042: adr r1,[0xe1308]
  000e1044: ldr r2,[0x000e1318]
  000e1046: vld1.64 {d0,d1},[r1]
  000e104a: adr r1,[0xe1320]
  000e104c: movs r6,#0xa1
  000e104e: movw r5,#0x171
  000e1052: vld1.64 {d30,d31},[r1]
  000e1056: adr r1,[0xe1330]
  000e1058: add r2,pc
  000e105a: mvn r3,#0x1
  000e105e: vld1.64 {d16,d17},[r1]
  000e1062: adr r1,[0xe1340]
  000e1064: movw r8,#0x24e
  000e1068: mov.w r12,#0x46
  000e106c: vld1.64 {d18,d19},[r1]
  000e1070: adr r1,[0xe1350]
  000e1072: mov.w lr,#0x23
  000e1076: mov.w r9,#0x7
  000e107a: vld1.64 {d20,d21},[r1]
  000e107e: adr r1,[0xe1360]
  000e1080: add.w r11,r0,#0xd0
  000e1084: vld1.64 {d22,d23},[r1]
  000e1088: adr r1,[0xe1370]
  000e108a: vld1.64 {d24,d25},[r1]
  000e108e: adr r1,[0xe1380]
  000e1090: vld1.64 {d26,d27},[r1]
  000e1094: adr r1,[0xe1390]
  000e1096: vld1.64 {d28,d29},[r1]
  000e109a: adr r1,[0xe13a0]
  000e109c: vld1.64 {d2,d3},[r1]
  000e10a0: add.w r1,r0,#0x14
  000e10a4: vst1.32 {d2,d3},[r1]
  000e10a8: adr r1,[0xe13b0]
  000e10aa: vld1.64 {d2,d3},[r1]
  000e10ae: adds r1,r0,#0x4
  000e10b0: vst1.32 {d2,d3},[r1]
  000e10b4: adr r1,[0xe13c0]
  000e10b6: vld1.64 {d2,d3},[r1]
  000e10ba: add.w r1,r0,#0x24
  000e10be: vst1.32 {d2,d3},[r1]
  000e10c2: adr r1,[0xe13d0]
  000e10c4: vld1.64 {d2,d3},[r1]
  000e10c8: add.w r1,r0,#0x34
  000e10cc: vst1.32 {d2,d3},[r1]
  000e10d0: adr r1,[0xe13e0]
  000e10d2: vld1.64 {d2,d3},[r1]
  000e10d6: add.w r1,r0,#0x44
  000e10da: vst1.32 {d2,d3},[r1]
  000e10de: adr r1,[0xe13f0]
  000e10e0: vld1.64 {d2,d3},[r1]
  000e10e4: add.w r1,r0,#0x54
  000e10e8: vst1.32 {d2,d3},[r1]
  000e10ec: adr r1,[0xe1400]
  000e10ee: vld1.64 {d2,d3},[r1]
  000e10f2: add.w r1,r0,#0x64
  000e10f6: vst1.32 {d2,d3},[r1]
  000e10fa: adr r1,[0xe1410]
  000e10fc: vld1.64 {d2,d3},[r1]
  000e1100: add.w r1,r0,#0x2c8
  000e1104: vst1.32 {d2,d3},[r1]
  000e1108: adr r1,[0xe1420]
  000e110a: vld1.64 {d2,d3},[r1]
  000e110e: add.w r1,r0,#0x7c
  000e1112: vst1.32 {d2,d3},[r1]
  000e1116: adr r1,[0xe1430]
  000e1118: strd r6,r5,[r0,#0x74]
  000e111c: movs r6,#0x7e
  000e111e: ldr r4,[0x000e1440]
  000e1120: movs r5,#0xe
  000e1122: strd r3,r6,[r0,#0x2c0]
  000e1126: movs r3,#0x8d
  000e1128: str.w r3,[r0,#0x2d8]
  000e112c: movs r3,#0x55
  000e112e: movs r6,#0x2d
  000e1130: ldr r2,[r2,#0x0]
  000e1132: str.w r3,[r0,#0xac]
  000e1136: strd r6,r4,[r0,#0x8c]
  000e113a: add.w r4,r0,#0x94
  000e113e: stm r4,{r5,r8,r12,lr}
  000e1142: movs r6,#0x1c
  000e1144: mov.w r8,#0x34
  000e1148: strd r9,r3,[r0,#0xa4]
  000e114c: movs r3,#0x15
  000e114e: ldrb.w r12,[r2,#0x0]
  000e1152: add.w r2,r0,#0xc0
  000e1156: ldr r4,[0x000e1444]
  000e1158: vst1.32 {d0,d1},[r2]
  000e115c: adr r2,[0xe1448]
  000e115e: vld1.64 {d0,d1},[r2]
  000e1162: add.w r2,r0,#0xb0
  000e1166: vst1.32 {d0,d1},[r2]
  000e116a: adr r2,[0xe1458]
  000e116c: vld1.64 {d0,d1},[r2]
  000e1170: add.w r2,r0,#0xec
  000e1174: vst1.32 {d0,d1},[r2]
  000e1178: adr r2,[0xe1468]
  000e117a: vld1.64 {d0,d1},[r2]
  000e117e: add.w r2,r0,#0x294
  000e1182: vst1.32 {d0,d1},[r2]
  000e1186: adr r2,[0xe1478]
  000e1188: vld1.64 {d0,d1},[r2]
  000e118c: add.w r2,r0,#0x2a4
  000e1190: vst1.32 {d0,d1},[r2]
  000e1194: adr r2,[0xe1488]
  000e1196: vld1.64 {d0,d1},[r2]
  000e119a: add.w r2,r0,#0x108
  000e119e: vst1.32 {d0,d1},[r2]
  000e11a2: adr r2,[0xe1498]
  000e11a4: vld1.64 {d0,d1},[r2]
  000e11a8: add.w r2,r0,#0x118
  000e11ac: vst1.32 {d0,d1},[r2]
  000e11b0: adr r2,[0xe14a8]
  000e11b2: vld1.64 {d2,d3},[r1]
  000e11b6: vld1.64 {d4,d5},[r2]
  000e11ba: ldr r1,[0x000e14b8]
  000e11bc: ldr r2,[0x000e14bc]
  000e11be: ldr r5,[0x000e14c0]
  000e11c0: stm r11,{r3,r6,r8}
  000e11c4: movs r3,#0x17
  000e11c6: add r5,pc
  000e11c8: strd r2,r4,[r0,#0xdc]
  000e11cc: movs r2,#0x5
  000e11ce: strd r1,r4,[r0,#0xe4]
  000e11d2: movs r1,#0x2
  000e11d4: strd r9,r1,[r0,#0xfc]
  000e11d8: movs r1,#0x9
  000e11da: str.w r2,[r0,#0x104]
  000e11de: strd lr,r1,[r0,#0x2b4]
  000e11e2: add.w r1,r0,#0x128
  000e11e6: ldr r2,[r5,#0x0]
  000e11e8: vst1.32 {d30,d31},[r1]
  000e11ec: adr r1,[0xe14c8]
  000e11ee: vld1.64 {d30,d31},[r1]
  000e11f2: add.w r1,r0,#0x140
  000e11f6: vst1.32 {d16,d17},[r1]
  000e11fa: add.w r1,r0,#0x150
  000e11fe: vst1.32 {d18,d19},[r1]
  000e1202: add.w r1,r0,#0x160
  000e1206: vst1.32 {d20,d21},[r1]
  000e120a: add.w r1,r0,#0x170
  000e120e: vst1.32 {d22,d23},[r1]
  000e1212: add.w r1,r0,#0x180
  000e1216: vst1.32 {d24,d25},[r1]
  000e121a: add.w r1,r0,#0x190
  000e121e: vst1.32 {d26,d27},[r1]
  000e1222: add.w r1,r0,#0x1a0
  000e1226: vst1.32 {d28,d29},[r1]
  000e122a: add.w r1,r0,#0x1b0
  000e122e: vst1.32 {d2,d3},[r1]
  000e1232: add.w r1,r0,#0x1c0
  000e1236: vst1.32 {d4,d5},[r1]
  000e123a: add.w r1,r0,#0x1d0
  000e123e: vst1.32 {d30,d31},[r1]
  000e1242: movs r1,#0x42
  000e1244: strd r1,r3,[r0,#0x138]
  000e1248: orr.w r1,r12,r10
  000e124c: clz r1,r1
  000e1250: str.w r4,[r0,#0x1e0]
  000e1254: vldr.32 s0,[pc,#0x280]
  000e1258: lsr.w lr,r1,#0x5
  000e125c: clz r1,r12
  000e1260: ldrb.w r8,[r2,#0x0]
  000e1264: lsr.w r9,r1,#0x5
  000e1268: b.w 0x000e21e2
  000e126c: ldr r1,[0x000e14dc]
  000e126e: add r1,pc
  000e1270: ldr r1,[r1,#0x0]
  000e1272: ldrb.w r11,[r1,#0x0]
  000e1276: cmp.w r11,#0x0
  000e127a: beq.w 0x000e1570
  000e127e: adr r1,[0xe14e0]
  000e1280: movs r2,#0xcf
  000e1282: vld1.64 {d16,d17},[r1]
  000e1286: adr r1,[0xe14f0]
  000e1288: movw r3,#0x1d9
  000e128c: movs r6,#0x6a
  000e128e: vld1.64 {d18,d19},[r1]
  000e1292: adr r1,[0xe1500]
  000e1294: add.w r12,r0,#0x2c0
  000e1298: vld1.64 {d20,d21},[r1]
  000e129c: adr r1,[0xe1510]
  000e129e: vld1.64 {d22,d23},[r1]
  000e12a2: adr r1,[0xe1520]
  000e12a4: vld1.64 {d24,d25},[r1]
  000e12a8: adr r1,[0xe1530]
  000e12aa: vld1.64 {d26,d27},[r1]
  000e12ae: adr r1,[0xe1540]
  000e12b0: strd r2,r3,[r0,#0x74]
  000e12b4: movs r2,#0x6c
  000e12b6: vld1.64 {d28,d29},[r1]
  000e12ba: mvn r1,#0x2
  000e12be: movs r3,#0x2d
  000e12c0: stm.w r12!,{r1,r2,r3,r6}
  000e12c4: add.w r1,r0,#0x14
  000e12c8: movs r2,#0x35
  000e12ca: vst1.32 {d16,d17},[r1]
  000e12ce: adds r1,r0,#0x4
  000e12d0: vst1.32 {d18,d19},[r1]
  000e12d4: add.w r1,r0,#0x24
  000e12d8: vst1.32 {d20,d21},[r1]
  000e12dc: add.w r1,r0,#0x34
  000e12e0: vst1.32 {d22,d23},[r1]
  000e12e4: add.w r1,r0,#0x44
  000e12e8: vst1.32 {d24,d25},[r1]
  000e12ec: add.w r1,r0,#0x54
  000e12f0: vst1.32 {d26,d27},[r1]
  000e12f4: add.w r1,r0,#0x64
  000e12f8: vst1.32 {d28,d29},[r1]
  000e12fc: b 0x000e1860
  000e1570: strd r11,r10,[sp,#0x0]
  000e1574: movs r4,#0x3c
  000e1576: ldr r2,[0x000e18d4]
  000e1578: movs r1,#0x1e
  000e157a: movs r6,#0x32
  000e157c: movs r5,#0x4a
  000e157e: add r2,pc
  000e1580: mov.w lr,#0x28
  000e1584: mov.w r9,#0x3c
  000e1588: mov.w r10,#0xe
  000e158c: ldr.w r8,[r2,#0x0]
  000e1590: movs r2,#0x3a
  000e1592: mov.w r11,#0xa
  000e1596: ldrb.w r3,[r8,#0x0]
  000e159a: cmp r3,#0x0
  000e159c: it eq
  000e159e: mov.eq r4,#0x1e
  000e15a0: it eq
  000e15a2: mov.eq r1,#0xf
  000e15a4: cmp r3,#0x0
  000e15a6: it eq
  000e15a8: mov.eq r2,#0x1d
  000e15aa: ldrb.w r3,[r8,#0x0]
  000e15ae: cmp r3,#0x0
  000e15b0: it eq
  000e15b2: mov.eq r6,#0x16
  000e15b4: it eq
  000e15b6: mov.eq r5,#0x25
  000e15b8: cmp r3,#0x0
  000e15ba: mov.w r3,#0xe
  000e15be: it eq
  000e15c0: mov.eq r3,#0x9
  000e15c2: it eq
  000e15c4: mov.eq.w lr,#0x14
  000e15c8: ldrb.w r12,[r8,#0x0]
  000e15cc: cmp.w r12,#0x0
  000e15d0: mov.w r12,#0xa
  000e15d4: it eq
  000e15d6: mov.eq.w r12,#0x5
  000e15da: strd r1,r4,[r0,#0x4]
  000e15de: add.w r1,r0,#0xc
  000e15e2: stmia r1!,{r2,r5,r6}
  000e15e4: movs r2,#0xc
  000e15e6: movw r6,#0x2b6
  000e15ea: strd r3,lr,[r0,#0x18]
  000e15ee: strd r12,r12,[r0,#0x20]
  000e15f2: str.w r12,[r0,#0x28]
  000e15f6: ldrb.w r1,[r8,#0x0]
  000e15fa: cmp r1,#0x0
  000e15fc: it eq
  000e15fe: mov.eq.w r9,#0x1e
  000e1602: mov.w r1,#0x6
  000e1606: str.w r9,[r0,#0x30]
  000e160a: it eq
  000e160c: mov.eq r1,#0x3
  000e160e: str r1,[r0,#0x2c]
  000e1610: ldrb.w r1,[r8,#0x0]
  000e1614: cmp r1,#0x0
  000e1616: it eq
  000e1618: mov.eq.w r10,#0x7
  000e161c: str.w r10,[r0,#0x38]
  000e1620: it eq
  000e1622: mov.eq r2,#0x6
  000e1624: str r2,[r0,#0x34]
  000e1626: movs r2,#0x76
  000e1628: cmp r1,#0x0
  000e162a: it eq
  000e162c: mov.eq r2,#0x3b
  000e162e: str r2,[r0,#0x3c]
  000e1630: ldrb.w r1,[r8,#0x0]
  000e1634: cmp r1,#0x0
  000e1636: mov.w r1,#0xa
  000e163a: it eq
  000e163c: mov.eq r1,#0x5
  000e163e: str r1,[r0,#0x44]
  000e1640: mov.w r1,#0xc8
  000e1644: it eq
  000e1646: mov.eq r1,#0x64
  000e1648: str r1,[r0,#0x40]
  000e164a: ldrb.w r1,[r8,#0x0]
  000e164e: ldr r2,[0x000e18d8]
  000e1650: cmp r1,#0x0
  000e1652: it eq
  000e1654: mov.eq.w r11,#0x5
  000e1658: mov.w r1,#0x16
  000e165c: str.w r11,[r0,#0x4c]
  000e1660: it eq
  000e1662: mov.eq r1,#0xb
  000e1664: add r2,pc
  000e1666: str r1,[r0,#0x48]
  000e1668: ldrb.w r1,[r8,#0x0]
  000e166c: ldr r3,[r2,#0x0]
  000e166e: movs r2,#0xb2
  000e1670: cmp r1,#0x0
  000e1672: it eq
  000e1674: movw.eq r6,#0x15b
  000e1678: str r6,[r0,#0x54]
  000e167a: it eq
  000e167c: mov.eq r2,#0x59
  000e167e: str r2,[r0,#0x50]
  000e1680: mov.w r2,#0x1e6
  000e1684: cmp r1,#0x0
  000e1686: it eq
  000e1688: mov.eq r2,#0xf3
  000e168a: str r2,[r0,#0x58]
  000e168c: ldrb r1,[r3,#0x0]
  000e168e: cbz r1,0x000e16aa
  000e1690: adr.w r1,0xe1550
  000e1694: mov.w r6,#0x110
  000e1698: vld1.64 {d16,d17},[r1]
  000e169c: add.w r1,r0,#0x5c
  000e16a0: vst1.32 {d16,d17},[r1]
  000e16a4: ldrd r11,r10,[sp,#0x0]
  000e16a8: b 0x000e1766
  000e16aa: ldrb.w r1,[r8,#0x0]
  000e16ae: cbz r1,0x000e16b8
  000e16b0: movs r1,#0x3c
  000e16b2: movs r6,#0x58
  000e16b4: str r1,[r0,#0x5c]
  000e16b6: b 0x000e16d2
  000e16b8: ldr r1,[0x000e18dc]
  000e16ba: movs r5,#0x2d
  000e16bc: ldrb r2,[r3,#0x0]
  000e16be: add r1,pc
  000e16c0: ldr r6,[r1,#0x0]
  000e16c2: ldrb r1,[r6,#0x0]
  000e16c4: cmp r1,#0x0
  000e16c6: it eq
  000e16c8: mov.eq r5,#0x1e
  000e16ca: cmp r2,#0x0
  000e16cc: str r5,[r0,#0x5c]
  000e16ce: beq 0x000e170c
  000e16d0: movs r6,#0x80
  000e16d2: ldrd r11,r10,[sp,#0x0]
  000e16d6: vmov s0,r6
  000e16da: vcvt.f32.s32 s0,s0
  000e16de: ldrb r1,[r3,#0x0]
  000e16e0: cmp r1,#0x0
  000e16e2: vcvt.s32.f32 s0,s0
  000e16e6: vstr.32 s0,[r0,#0x60]
  000e16ea: beq 0x000e16fc
  000e16ec: movw r1,#0x326
  000e16f0: movs r2,#0xa0
  000e16f2: strd r2,r1,[r0,#0x64]
  000e16f6: mov.w r6,#0x110
  000e16fa: b 0x000e1766
  000e16fc: ldrb.w r1,[r8,#0x0]
  000e1700: cbz r1,0x000e171e
  000e1702: movs r1,#0x78
  000e1704: movw r6,#0x25e
  000e1708: str r1,[r0,#0x64]
  000e170a: b 0x000e173a
  000e170c: ldrb.w r1,[r8,#0x0]
  000e1710: ldrd r11,r10,[sp,#0x0]
  000e1714: cmp r1,#0x0
  000e1716: beq.w 0x000e2844
  000e171a: movs r6,#0x58
  000e171c: b 0x000e16d6
  000e171e: ldr r1,[0x000e18e0]
  000e1720: movs r2,#0x50
  000e1722: movw r6,#0x193
  000e1726: add r1,pc
  000e1728: ldr r1,[r1,#0x0]
  000e172a: ldrb r1,[r1,#0x0]
  000e172c: cmp r1,#0x0
  000e172e: it eq
  000e1730: mov.eq r2,#0x3c
  000e1732: str r2,[r0,#0x64]
  000e1734: it eq
  000e1736: movw.eq r6,#0x12f
  000e173a: vmov s0,r6
  000e173e: vcvt.f32.s32 s0,s0
  000e1742: vcvt.s32.f32 s0,s0
  000e1746: vstr.32 s0,[r0,#0x68]
  000e174a: ldrb.w r1,[r8,#0x0]
  000e174e: cbz r1,0x000e1756
  000e1750: mov.w r6,#0x106
  000e1754: b 0x000e1766
  000e1756: ldr r1,[0x000e18e4]
  000e1758: movs r6,#0x88
  000e175a: add r1,pc
  000e175c: ldr r1,[r1,#0x0]
  000e175e: ldrb r1,[r1,#0x0]
  000e1760: cmp r1,#0x0
  000e1762: it eq
  000e1764: mov.eq r6,#0x83
  000e1766: vmov s0,r6
  000e176a: mvn r2,#0x3
  000e176e: vcvt.f32.s32 s0,s0
  000e1772: vcvt.s32.f32 s0,s0
  000e1776: vstr.32 s0,[r0,#0x6c]
  000e177a: ldrb.w r1,[r8,#0x0]
  000e177e: cmp r1,#0x0
  000e1780: it eq
  000e1782: mvn.eq r2,#0x1
  000e1786: str.w r2,[r0,#0x2c0]
  000e178a: ldrb r1,[r3,#0x0]
  000e178c: cbz r1,0x000e1798
  000e178e: movs r1,#0x5a
  000e1790: str r1,[r0,#0x70]
  000e1792: ldrb.w r6,[r8,#0x0]
  000e1796: b 0x000e17c2
  000e1798: ldrb.w r6,[r8,#0x0]
  000e179c: cbz r6,0x000e17a2
  000e179e: movs r5,#0x3c
  000e17a0: b 0x000e17b2
  000e17a2: ldr r1,[0x000e18e8]
  000e17a4: movs r5,#0x2d
  000e17a6: add r1,pc
  000e17a8: ldr r1,[r1,#0x0]
  000e17aa: ldrb r1,[r1,#0x0]
  000e17ac: cmp r1,#0x0
  000e17ae: it eq
  000e17b0: mov.eq r5,#0x1e
  000e17b2: vmov s0,r5
  000e17b6: vcvt.f32.s32 s0,s0
  000e17ba: vcvt.s32.f32 s0,s0
  000e17be: vstr.32 s0,[r0,#0x70]
  000e17c2: movw r1,#0x20e
  000e17c6: movs r2,#0xe6
  000e17c8: cmp r6,#0x0
  000e17ca: it eq
  000e17cc: movw.eq r1,#0x107
  000e17d0: it eq
  000e17d2: mov.eq r2,#0x73
  000e17d4: strd r2,r1,[r0,#0x74]
  000e17d8: ldrb r1,[r3,#0x0]
  000e17da: cbz r1,0x000e17e6
  000e17dc: movs r1,#0xb4
  000e17de: movs r6,#0x5a
  000e17e0: str.w r1,[r0,#0x2c4]
  000e17e4: b 0x000e1814
  000e17e6: ldrb.w r1,[r8,#0x0]
  000e17ea: cbz r1,0x000e17f4
  000e17ec: movs r1,#0x78
  000e17ee: str.w r1,[r0,#0x2c4]
  000e17f2: b 0x000e1812
  000e17f4: ldr r1,[0x000e18ec]
  000e17f6: movs r5,#0x5a
  000e17f8: ldrb.w r2,[r8,#0x0]
  000e17fc: add r1,pc
  000e17fe: ldr r6,[r1,#0x0]
  000e1800: ldrb r1,[r6,#0x0]
  000e1802: cmp r1,#0x0
  000e1804: it eq
  000e1806: mov.eq r5,#0x3c
  000e1808: cmp r2,#0x0
  000e180a: str.w r5,[r0,#0x2c4]
  000e180e: beq.w 0x000e2828
  000e1812: movs r6,#0x3c
  000e1814: vmov s0,r6
  000e1818: vcvt.f32.s32 s0,s0
  000e181c: vcvt.s32.f32 s0,s0
  000e1820: vstr.32 s0,[r0,#0x2c8]
  000e1824: ldrb r1,[r3,#0x0]
  000e1826: cbz r1,0x000e1832
  000e1828: movs r1,#0xb4
  000e182a: movs r2,#0x5a
  000e182c: str.w r1,[r0,#0x2cc]
  000e1830: b 0x000e1860
  000e1832: ldrb.w r1,[r8,#0x0]
  000e1836: cbz r1,0x000e1840
  000e1838: movs r1,#0x78
  000e183a: str.w r1,[r0,#0x2cc]
  000e183e: b 0x000e185e
  000e1840: ldr r1,[0x000e18f0]
  000e1842: movs r6,#0x5a
  000e1844: ldrb.w r2,[r8,#0x0]
  000e1848: add r1,pc
  000e184a: ldr r3,[r1,#0x0]
  000e184c: ldrb r1,[r3,#0x0]
  000e184e: cmp r1,#0x0
  000e1850: it eq
  000e1852: mov.eq r6,#0x3c
  000e1854: cmp r2,#0x0
  000e1856: str.w r6,[r0,#0x2cc]
  000e185a: beq.w 0x000e2836
  000e185e: movs r2,#0x3c
  000e1860: vmov s0,r2
  000e1864: movs r6,#0xc8
  000e1866: movs r4,#0xa0
  000e1868: vcvt.f32.s32 s0,s0
  000e186c: vcvt.s32.f32 s0,s0
  000e1870: ldr r1,[0x000e18f4]
  000e1872: ldr r2,[0x000e18f8]
  000e1874: add r1,pc
  000e1876: add r2,pc
  000e1878: ldr r1,[r1,#0x0]
  000e187a: ldr.w r12,[r2,#0x0]
  000e187e: movs r2,#0xa0
  000e1880: str r1,[sp,#0x4]
  000e1882: vstr.32 s0,[r0,#0x2d0]
  000e1886: ldrb r3,[r1,#0x0]
  000e1888: movs r1,#0x7d
  000e188a: ldrb.w r8,[r12,#0x0]
  000e188e: cmp r3,#0x0
  000e1890: it eq
  000e1892: mov.eq r1,#0x4b
  000e1894: it eq
  000e1896: mov.eq r6,#0x7d
  000e1898: orrs.w r5,r8,r11
  000e189c: it eq
  000e189e: mov.eq r6,r1
  000e18a0: movs r1,#0x64
  000e18a2: cmp r3,#0x0
  000e18a4: it eq
  000e18a6: mov.eq r1,#0x3c
  000e18a8: it eq
  000e18aa: mov.eq r4,#0x64
  000e18ac: cmp r5,#0x0
  000e18ae: it eq
  000e18b0: mov.eq r4,r1
  000e18b2: cmp.w r11,#0x0
  000e18b6: strd r4,r6,[r0,#0x2d4]
  000e18ba: beq 0x000e1904
  000e18bc: adr.w r1,0xe1560
  000e18c0: vldr.32 s0,[pc,#0x38]
  000e18c4: vld1.64 {d16,d17},[r1]
  000e18c8: add.w r1,r0,#0x7c
  000e18cc: vst1.32 {d16,d17},[r1]
  000e18d0: b 0x000e193c
  000e1904: movs r1,#0x14
  000e1906: cmp.w r8,#0x0
  000e190a: mov.w r6,#0x12
  000e190e: adr.w r5,0xe1d94
  000e1912: it eq
  000e1914: mov.eq r1,#0xa
  000e1916: it eq
  000e1918: mov.eq r6,#0x9
  000e191a: cmp.w r8,#0x0
  000e191e: it eq
  000e1920: mov.eq r2,#0x50
  000e1922: it eq
  000e1924: add.eq r5,#0x4
  000e1926: movs r4,#0x3c
  000e1928: vldr.32 s0,[r5]
  000e192c: cmp.w r8,#0x0
  000e1930: it eq
  000e1932: mov.eq r4,#0x1e
  000e1934: strd r6,r1,[r0,#0x7c]
  000e1938: strd r2,r4,[r0,#0x84]
  000e193c: vcvt.s32.f32 s0,s0
  000e1940: orr.w r2,r11,r3
  000e1944: orrs.w r1,r2,r8
  000e1948: adr.w r1,0xe2854
  000e194c: it eq
  000e194e: add.eq r1,#0x4
  000e1950: cmp.w r11,#0x0
  000e1954: vldr.32 s2,[r1]
  000e1958: vstr.32 s0,[r0,#0x8c]
  000e195c: vstr.32 s2,[r0,#0x90]
  000e1960: beq 0x000e1972
  000e1962: movs r3,#0xf
  000e1964: vldr.32 s0,[pc,#-0x68]
  000e1968: movw r1,#0x276
  000e196c: str.w r3,[r0,#0x94]
  000e1970: b 0x000e199a
  000e1972: mov.w r1,#0x348
  000e1976: adr.w r3,0xe285c
  000e197a: cmp.w r8,#0x0
  000e197e: it eq
  000e1980: mov.eq.w r1,#0x1a4
  000e1984: it eq
  000e1986: add.eq r3,#0x4
  000e1988: movs r6,#0x14
  000e198a: vldr.32 s0,[r3]
  000e198e: cmp.w r8,#0x0
  000e1992: it eq
  000e1994: mov.eq r6,#0xa
  000e1996: str.w r6,[r0,#0x94]
  000e199a: str.w r1,[r0,#0x98]
  000e199e: vcvt.s32.f32 s0,s0
  000e19a2: ldr r1,[0x000e1d7c]
  000e19a4: add r1,pc
  000e19a6: ldr r5,[r1,#0x0]
  000e19a8: vstr.32 s0,[r0,#0x9c]
  000e19ac: ldrb.w r9,[r5,#0x0]
  000e19b0: cmp.w r9,#0x0
  000e19b4: beq 0x000e19ba
  000e19b6: movs r2,#0x7a
  000e19b8: b 0x000e19ca
  000e19ba: cbz r2,0x000e19c0
  000e19bc: movs r2,#0x3d
  000e19be: b 0x000e19ca
  000e19c0: movs r2,#0x52
  000e19c2: cmp.w r8,#0x0
  000e19c6: it eq
  000e19c8: mov.eq r2,#0x29
  000e19ca: vmov s0,r2
  000e19ce: str.w r2,[r0,#0xac]
  000e19d2: cmp.w r11,#0x0
  000e19d6: vcvt.f32.s32 s0,s0
  000e19da: vcvt.s32.f32 s0,s0
  000e19de: vstr.32 s0,[r0,#0xa8]
  000e19e2: beq 0x000e1ac6
  000e19e4: adr.w r1,0xe2890
  000e19e8: ldr r2,[0x000e1d80]
  000e19ea: vld1.64 {d16,d17},[r1]
  000e19ee: adr.w r1,0xe28a0
  000e19f2: movs r6,#0x9
  000e19f4: movs r5,#0x2d
  000e19f6: vld1.64 {d18,d19},[r1]
  000e19fa: adr.w r1,0xe28b0
  000e19fe: movs r4,#0x37
  000e1a00: mov.w r3,#0x3fc00000
  000e1a04: vld1.64 {d20,d21},[r1]
  000e1a08: adr.w r1,0xe28c0
  000e1a0c: mov.w lr,#0x0
  000e1a10: vmov.f32 s0,0x40c00000
  000e1a14: vld1.64 {d22,d23},[r1]
  000e1a18: adr.w r1,0xe28d0
  000e1a1c: mov.w r12,#0x1
  000e1a20: vld1.64 {d24,d25},[r1]
  000e1a24: adr.w r1,0xe28e0
  000e1a28: vld1.64 {d26,d27},[r1]
  000e1a2c: adr.w r1,0xe28f0
  000e1a30: vld1.64 {d28,d29},[r1]
  000e1a34: adr.w r1,0xe2900
  000e1a38: vld1.64 {d30,d31},[r1]
  000e1a3c: adr.w r1,0xe2910
  000e1a40: vld1.64 {d2,d3},[r1]
  000e1a44: ldr r1,[0x000e1d84]
  000e1a46: strd r5,r6,[r0,#0xa0]
  000e1a4a: movs r6,#0x16
  000e1a4c: movs r5,#0x24
  000e1a4e: strd r6,r5,[r0,#0xd0]
  000e1a52: strd r4,r2,[r0,#0xd8]
  000e1a56: movs r2,#0x4
  000e1a58: strd r3,r1,[r0,#0xe0]
  000e1a5c: movs r1,#0x5
  000e1a5e: str.w r3,[r0,#0xe8]
  000e1a62: strd lr,r1,[r0,#0xfc]
  000e1a66: movs r1,#0x25
  000e1a68: str.w r2,[r0,#0x104]
  000e1a6c: movs r2,#0xe
  000e1a6e: strd r1,r2,[r0,#0x2b4]
  000e1a72: add.w r1,r0,#0xb0
  000e1a76: movs r2,#0x19
  000e1a78: vst1.32 {d16,d17},[r1]
  000e1a7c: add.w r1,r0,#0xc0
  000e1a80: vst1.32 {d18,d19},[r1]
  000e1a84: add.w r1,r0,#0xec
  000e1a88: vst1.32 {d20,d21},[r1]
  000e1a8c: add.w r1,r0,#0x294
  000e1a90: vst1.32 {d22,d23},[r1]
  000e1a94: add.w r1,r0,#0x2a4
  000e1a98: vst1.32 {d24,d25},[r1]
  000e1a9c: add.w r1,r0,#0x118
  000e1aa0: vst1.32 {d26,d27},[r1]
  000e1aa4: add.w r1,r0,#0x108
  000e1aa8: vst1.32 {d28,d29},[r1]
  000e1aac: add.w r1,r0,#0x128
  000e1ab0: vst1.32 {d30,d31},[r1]
  000e1ab4: add.w r1,r0,#0x140
  000e1ab8: vst1.32 {d2,d3},[r1]
  000e1abc: movs r1,#0x34
  000e1abe: strd r1,r2,[r0,#0x138]
  000e1ac2: ldr r6,[sp,#0x4]
  000e1ac4: b 0x000e1d2a
  000e1ac6: adr.w r1,0xe2920
  000e1aca: vmov.i32 q11,#0x1f
  000e1ace: vld1.64 {d16,d17},[r1]
  000e1ad2: adr.w r1,0xe2930
  000e1ad6: vmov.f32 s6,0x41200000
  000e1ada: adr.w r2,0xe2970
  000e1ade: vld1.64 {d18,d19},[r1]
  000e1ae2: clz r1,r8
  000e1ae6: vmov.f32 s2,0x41a00000
  000e1aea: cmp.w r8,#0x0
  000e1aee: lsr.w r1,r1,#0x5
  000e1af2: vmov.f32 s4,0x3f800000
  000e1af6: vdup.16 d24,r1
  000e1afa: adr.w r1,0xe2940
  000e1afe: vneg.s32 q13,q11
  000e1b02: vld1.64 {d20,d21},[r1]
  000e1b06: adr.w r1,0xe2950
  000e1b0a: mov.w r3,#0x1e
  000e1b0e: vmovl.u16 q12,d24
  000e1b12: vld1.64 {d22,d23},[r1]
  000e1b16: adr.w r1,0xe2960
  000e1b1a: vmov.f32 s12,s6
  000e1b1e: vld1.64 {d30,d31},[r1]
  000e1b22: mov.w r1,#0x28
  000e1b26: it eq
  000e1b28: mov.eq r1,#0x14
  000e1b2a: cmp.w r8,#0x0
  000e1b2e: vshl.I32 q12,q12,#0x1f
  000e1b32: vld1.64 {d4,d5},[r2]
  000e1b36: mov.w r2,#0x4a
  000e1b3a: vmov.f32 s0,0x40000000
  000e1b3e: str.w r1,[r0,#0xd4]
  000e1b42: it eq
  000e1b44: mov.eq r2,#0x25
  000e1b46: vshl.s32 q13,q12,q13
  000e1b4a: it eq
  000e1b4c: vmov.eq.f32 s12,s2
  000e1b50: cmp.w r8,#0x0
  000e1b54: itt eq
  000e1b56: vmov.eq.f32 s0,s4
  000e1b5a: vmov.eq.f32 s2,s6
  000e1b5e: cmp.w r8,#0x0
  000e1b62: it eq
  000e1b64: mov.eq r3,#0xf
  000e1b66: vmov q12,q13
  000e1b6a: adr.w r1,0xe2980
  000e1b6e: str.w r3,[r0,#0xd0]
  000e1b72: mov.w r3,#0x32
  000e1b76: mov.w r6,#0xa
  000e1b7a: vld1.64 {d28,d29},[r1]
  000e1b7e: it eq
  000e1b80: mov.eq r3,#0x19
  000e1b82: cmp.w r8,#0x0
  000e1b86: it eq
  000e1b88: mov.eq r6,#0x5
  000e1b8a: adr.w r1,0xe2990
  000e1b8e: vbsl q12,q2,q15
  000e1b92: vld1.64 {d4,d5},[r1]
  000e1b96: adr.w r1,0xe29a0
  000e1b9a: cmp.w r8,#0x0
  000e1b9e: strd r3,r6,[r0,#0xa0]
  000e1ba2: vmov q15,q13
  000e1ba6: str.w r2,[r0,#0xd8]
  000e1baa: mov.w r2,#0x4
  000e1bae: vld1.64 {d2,d3},[r1]
  000e1bb2: it eq
  000e1bb4: mov.eq r2,#0x2
  000e1bb6: mov.w r4,#0x5
  000e1bba: vbsl q15,q1,q2
  000e1bbe: it eq
  000e1bc0: mov.eq r4,#0x3
  000e1bc2: strd r6,r2,[r0,#0xfc]
  000e1bc6: adr.w r1,0xe29b0
  000e1bca: movs r2,#0xa8
  000e1bcc: vmov q2,q13
  000e1bd0: vld1.64 {d2,d3},[r1]
  000e1bd4: cmp.w r8,#0x0
  000e1bd8: mov.w r6,#0x10
  000e1bdc: str.w r4,[r0,#0x104]
  000e1be0: it eq
  000e1be2: mov.eq r2,#0x54
  000e1be4: str.w r2,[r0,#0x298]
  000e1be8: vbsl q2,q1,q14
  000e1bec: add.w r2,r0,#0x294
  000e1bf0: vstr.32 s2,[r0,#0xdc]
  000e1bf4: vmov q14,q13
  000e1bf8: vstr.32 s0,[r0,#0xe0]
  000e1bfc: vstr.32 s12,[r0,#0xe4]
  000e1c00: vbsl q13,q9,q8
  000e1c04: vstr.32 s0,[r0,#0xe8]
  000e1c08: vst1.32 d31[0x0],[r2:32]
  000e1c0c: add.w r2,r0,#0x29c
  000e1c10: vbsl q14,q11,q10
  000e1c14: cmp.w r8,#0x0
  000e1c18: add.w lr,r0,#0x2b0
  000e1c1c: vst1.32 d4[0x0],[r2:32]
  000e1c20: add.w r2,r0,#0xb0
  000e1c24: mov.w r1,#0x1e
  000e1c28: mov.w r4,#0x1c
  000e1c2c: vst1.32 {d28,d29},[r2]
  000e1c30: add.w r2,r0,#0xc0
  000e1c34: vst1.32 {d26,d27},[r2]
  000e1c38: add.w r2,r0,#0xec
  000e1c3c: vst1.32 {d30,d31},[r2]
  000e1c40: add.w r2,r0,#0x2a0
  000e1c44: vst1.32 {d4,d5},[r2]
  000e1c48: add.w r2,r0,#0x108
  000e1c4c: vst1.32 {d24,d25},[r2]
  000e1c50: mov.w r2,#0x3c
  000e1c54: it eq
  000e1c56: mov.eq r2,#0x1e
  000e1c58: it eq
  000e1c5a: mov.eq r6,#0x7
  000e1c5c: stm.w lr!,{r2,r3,r6}
  000e1c60: movs r2,#0xe
  000e1c62: cmp.w r8,#0x0
  000e1c66: it eq
  000e1c68: mov.eq r2,#0x7
  000e1c6a: ldrb.w r8,[r12,#0x0]
  000e1c6e: movs r6,#0x2
  000e1c70: add.w r12,r0,#0x118
  000e1c74: cmp.w r8,#0x0
  000e1c78: it eq
  000e1c7a: mov.eq r6,#0x1
  000e1c7c: it eq
  000e1c7e: mov.eq r4,#0xf
  000e1c80: cmp.w r8,#0x0
  000e1c84: it eq
  000e1c86: mov.eq r1,#0xf
  000e1c88: stm.w r12!,{r2,r3,r4,r6}
  000e1c8c: strd r6,r1,[r0,#0x128]
  000e1c90: ldrb.w r9,[r5,#0x0]
  000e1c94: cmp.w r9,#0x0
  000e1c98: beq 0x000e1ca6
  000e1c9a: movs r1,#0xd8
  000e1c9c: movs r2,#0x70
  000e1c9e: strd r2,r1,[r0,#0x130]
  000e1ca2: movs r1,#0x5e
  000e1ca4: b 0x000e1cb8
  000e1ca6: cmp.w r8,#0x0
  000e1caa: beq.w 0x000e2802
  000e1cae: movs r1,#0xa0
  000e1cb0: movs r2,#0x50
  000e1cb2: strd r2,r1,[r0,#0x130]
  000e1cb6: movs r1,#0x46
  000e1cb8: ldr r6,[sp,#0x4]
  000e1cba: clz r2,r8
  000e1cbe: vmov s0,r1
  000e1cc2: vmov.i32 q8,#0x1f
  000e1cc6: adr.w r1,0xe29c0
  000e1cca: lsrs r2,r2,#0x5
  000e1ccc: vcvt.f32.s32 s2,s0
  000e1cd0: vdup.16 d18,r2
  000e1cd4: vmov.f32 s4,0x40800000
  000e1cd8: vneg.s32 q8,q8
  000e1cdc: cmp.w r8,#0x0
  000e1ce0: mov.w lr,#0x1
  000e1ce4: mov.w r12,#0x0
  000e1ce8: vmovl.u16 q9,d18
  000e1cec: vmov.f32 s0,0x41000000
  000e1cf0: vcvt.s32.f32 s2,s2
  000e1cf4: vshl.I32 q9,q9,#0x1f
  000e1cf8: vshl.s32 q8,q9,q8
  000e1cfc: vld1.64 {d18,d19},[r1]
  000e1d00: adr.w r1,0xe29d0
  000e1d04: vld1.64 {d20,d21},[r1]
  000e1d08: mov.w r1,#0x10e
  000e1d0c: vbsl q8,q10,q9
  000e1d10: it eq
  000e1d12: mov.eq r1,#0x87
  000e1d14: str.w r1,[r0,#0x14c]
  000e1d18: add.w r1,r0,#0x13c
  000e1d1c: it eq
  000e1d1e: vmov.eq.f32 s0,s4
  000e1d22: vstr.32 s2,[r0,#0x138]
  000e1d26: vst1.32 {d16,d17},[r1]
  000e1d2a: vcvt.s32.f32 s0,s0
  000e1d2e: cmp.w r9,#0x0
  000e1d32: vstr.32 s0,[r0,#0x150]
  000e1d36: beq 0x000e1d4e
  000e1d38: movs r1,#0x64
  000e1d3a: mov.w r2,#0x126
  000e1d3e: strd r1,r1,[r0,#0x154]
  000e1d42: mov r5,lr
  000e1d44: str.w r2,[r0,#0x15c]
  000e1d48: movs r2,#0x62
  000e1d4a: ldrb r4,[r6,#0x0]
  000e1d4c: b 0x000e1e02
  000e1d4e: ldrb r2,[r6,#0x0]
  000e1d50: cbz r2,0x000e1d5c
  000e1d52: movs r4,#0x32
  000e1d54: mov r1,lr
  000e1d56: str.w r4,[r0,#0x154]
  000e1d5a: b 0x000e1da6
  000e1d5c: cmp.w lr,#0x0
  000e1d60: beq 0x000e1d9c
  000e1d62: movs r1,#0x1e
  000e1d64: cmp.w r8,#0x0
  000e1d68: it eq
  000e1d6a: mov.eq r1,#0xf
  000e1d6c: mov.w r4,#0x1c
  000e1d70: str.w r1,[r0,#0x154]
  000e1d74: it eq
  000e1d76: mov.eq r4,#0xe
  000e1d78: movs r1,#0x1
  000e1d7a: b 0x000e1da6
  000e1d9c: movs r1,#0xe
  000e1d9e: movs r4,#0xd
  000e1da0: str.w r1,[r0,#0x154]
  000e1da4: movs r1,#0x0
  000e1da6: vmov s0,r4
  000e1daa: movs r4,#0x88
  000e1dac: mov r3,r2
  000e1dae: cmp r2,#0x0
  000e1db0: vcvt.f32.s32 s0,s0
  000e1db4: vcvt.s32.f32 s0,s0
  000e1db8: vstr.32 s0,[r0,#0x158]
  000e1dbc: itt ne
  000e1dbe: mov.ne r4,#0x93
  000e1dc0: mov.ne r3,#0x1
  000e1dc2: cmp r2,#0x0
  000e1dc4: and.w r5,r1,r3
  000e1dc8: bne 0x000e1dd8
  000e1dca: cbz r1,0x000e1dd8
  000e1dcc: movs r4,#0x9e
  000e1dce: cmp.w r8,#0x0
  000e1dd2: it eq
  000e1dd4: mov.eq r4,#0x4f
  000e1dd6: movs r5,#0x1
  000e1dd8: vmov s0,r4
  000e1ddc: movs r2,#0xd
  000e1dde: vcvt.f32.s32 s0,s0
  000e1de2: vcvt.s32.f32 s0,s0
  000e1de6: vstr.32 s0,[r0,#0x15c]
  000e1dea: ldrb r4,[r6,#0x0]
  000e1dec: cmp r4,#0x0
  000e1dee: it ne
  000e1df0: mov.ne r2,#0x31
  000e1df2: bne 0x000e1e02
  000e1df4: cbz r5,0x000e1e02
  000e1df6: movs r2,#0x1a
  000e1df8: cmp.w r8,#0x0
  000e1dfc: it eq
  000e1dfe: mov.eq r2,#0xd
  000e1e00: movs r4,#0x0
  000e1e02: vmov s0,r2
  000e1e06: orrs.w r1,r4,r9
  000e1e0a: vcvt.f32.s32 s0,s0
  000e1e0e: vcvt.s32.f32 s0,s0
  000e1e12: vstr.32 s0,[r0,#0x160]
  000e1e16: beq 0x000e1e28
  000e1e18: movs r1,#0x0
  000e1e1a: cmp r5,#0x0
  000e1e1c: str.w r1,[r0,#0x164]
  000e1e20: bne 0x000e1e38
  000e1e22: vldr.32 s0,[pc,#-0x9c]
  000e1e26: b 0x000e1e56
  000e1e28: cbz r5,0x000e1e4c
  000e1e2a: movs r1,#0x19
  000e1e2c: cmp.w r8,#0x0
  000e1e30: it eq
  000e1e32: mov.eq r1,#0xd
  000e1e34: str.w r1,[r0,#0x164]
  000e1e38: vmov.f32 s2,0x41c80000
  000e1e3c: vldr.32 s0,[pc,#-0xb4]
  000e1e40: cmp.w r8,#0x0
  000e1e44: it eq
  000e1e46: vmov.eq.f32 s0,s2
  000e1e4a: b 0x000e1e56
  000e1e4c: movs r1,#0x17
  000e1e4e: vldr.32 s0,[pc,#-0xc8]
  000e1e52: str.w r1,[r0,#0x164]
  000e1e56: vcvt.s32.f32 s0,s0
  000e1e5a: cmp.w r9,#0x0
  000e1e5e: vstr.32 s0,[r0,#0x168]
  000e1e62: beq 0x000e1ec6
  000e1e64: adr.w r1,0xe29e0
  000e1e68: cmp r5,#0x0
  000e1e6a: vld1.64 {d16,d17},[r1]
  000e1e6e: mov.w r1,#0x136
  000e1e72: str.w r1,[r0,#0x17c]
  000e1e76: add.w r1,r0,#0x16c
  000e1e7a: vst1.32 {d16,d17},[r1]
  000e1e7e: beq 0x000e1ed2
  000e1e80: movs r2,#0x22
  000e1e82: cmp.w r8,#0x0
  000e1e86: it eq
  000e1e88: mov.eq r2,#0x11
  000e1e8a: mov r6,r9
  000e1e8c: movs r1,#0xc
  000e1e8e: mov.w r9,#0x15a
  000e1e92: cmp.w r8,#0x0
  000e1e96: it eq
  000e1e98: mov.eq r1,#0x6
  000e1e9a: str.w r2,[r0,#0x180]
  000e1e9e: mvn r11,#0x7
  000e1ea2: str.w r9,[r0,#0x184]
  000e1ea6: movs r3,#0x18
  000e1ea8: str.w r2,[r0,#0x188]
  000e1eac: mov.w r2,#0x1be
  000e1eb0: strd r2,r11,[r0,#0x18c]
  000e1eb4: movs r5,#0x80
  000e1eb6: add.w r2,r0,#0x194
  000e1eba: mov r9,r6
  000e1ebc: stmia r2!,{r1,r3,r5}
  000e1ebe: movs r1,#0x1a
  000e1ec0: str.w r1,[r0,#0x1a0]
  000e1ec4: b 0x000e2064
  000e1ec6: cbz r4,0x000e1efc
  000e1ec8: movs r1,#0x83
  000e1eca: str.w r1,[r0,#0x16c]
  000e1ece: movs r1,#0x7c
  000e1ed0: b 0x000e1f24
  000e1ed2: adr.w r1,0xe29f0
  000e1ed6: movs r2,#0xa
  000e1ed8: vld1.64 {d16,d17},[r1]
  000e1edc: mvn r1,#0x7
  000e1ee0: movs r5,#0x18
  000e1ee2: strd r1,r2,[r0,#0x190]
  000e1ee6: add.w r1,r0,#0x180
  000e1eea: movs r2,#0x80
  000e1eec: vst1.32 {d16,d17},[r1]
  000e1ef0: movs r1,#0x1a
  000e1ef2: strd r5,r2,[r0,#0x198]
  000e1ef6: str.w r1,[r0,#0x1a0]
  000e1efa: b 0x000e2086
  000e1efc: cbz r5,0x000e1f1a
  000e1efe: movs r1,#0x8a
  000e1f00: cmp.w r8,#0x0
  000e1f04: it eq
  000e1f06: mov.eq r1,#0x45
  000e1f08: mov.w r5,#0x1
  000e1f0c: str.w r1,[r0,#0x16c]
  000e1f10: mov.w r1,#0x80
  000e1f14: it eq
  000e1f16: mov.eq r1,#0x40
  000e1f18: b 0x000e1f24
  000e1f1a: movs r1,#0x73
  000e1f1c: movs r5,#0x0
  000e1f1e: str.w r1,[r0,#0x16c]
  000e1f22: movs r1,#0x6a
  000e1f24: vmov s0,r1
  000e1f28: mov r1,r4
  000e1f2a: cmp r4,#0x0
  000e1f2c: vcvt.f32.s32 s0,s0
  000e1f30: vcvt.s32.f32 s0,s0
  000e1f34: vstr.32 s0,[r0,#0x170]
  000e1f38: it ne
  000e1f3a: mov.ne r1,#0x1
  000e1f3c: bne 0x000e1f64
  000e1f3e: cbz r5,0x000e1f64
  000e1f40: movs r1,#0x3c
  000e1f42: cmp.w r8,#0x0
  000e1f46: it eq
  000e1f48: mov.eq r1,#0x1e
  000e1f4a: str.w r1,[r0,#0x174]
  000e1f4e: movs r1,#0xaa
  000e1f50: movs r2,#0x2c
  000e1f52: cmp.w r8,#0x0
  000e1f56: it eq
  000e1f58: mov.eq r1,#0x55
  000e1f5a: it eq
  000e1f5c: mov.eq r2,#0x16
  000e1f5e: strd r2,r1,[r0,#0x178]
  000e1f62: b 0x000e1f84
  000e1f64: ands r1,r5
  000e1f66: movs r2,#0x2d
  000e1f68: cmp r4,#0x0
  000e1f6a: it ne
  000e1f6c: mov.ne r2,#0x2a
  000e1f6e: str.w r2,[r0,#0x174]
  000e1f72: beq 0x000e1f92
  000e1f74: cmp r1,#0x0
  000e1f76: mov.w r2,#0x9b
  000e1f7a: mov.w r3,#0x3c
  000e1f7e: strd r3,r2,[r0,#0x178]
  000e1f82: beq 0x000e1f9e
  000e1f84: movs r2,#0x22
  000e1f86: cmp.w r8,#0x0
  000e1f8a: it eq
  000e1f8c: mov.eq r2,#0x11
  000e1f8e: movs r1,#0x1
  000e1f90: b 0x000e1fa2
  000e1f92: cmp r1,#0x0
  000e1f94: bne 0x000e1f4e
  000e1f96: movs r1,#0x92
  000e1f98: movs r2,#0x1b
  000e1f9a: strd r2,r1,[r0,#0x178]
  000e1f9e: movs r1,#0x0
  000e1fa0: movs r2,#0x1e
  000e1fa2: cmp r4,#0x0
  000e1fa4: str.w r2,[r0,#0x180]
  000e1fa8: beq 0x000e1fb6
  000e1faa: movs r2,#0xad
  000e1fac: cmp r1,#0x0
  000e1fae: str.w r2,[r0,#0x184]
  000e1fb2: bne 0x000e1fc6
  000e1fb4: b 0x000e1fda
  000e1fb6: cbz r1,0x000e1fd4
  000e1fb8: movs r1,#0xe8
  000e1fba: cmp.w r8,#0x0
  000e1fbe: it eq
  000e1fc0: mov.eq r1,#0x74
  000e1fc2: str.w r1,[r0,#0x184]
  000e1fc6: movs r1,#0x22
  000e1fc8: cmp.w r8,#0x0
  000e1fcc: it eq
  000e1fce: mov.eq r1,#0x11
  000e1fd0: movs r2,#0x1
  000e1fd2: b 0x000e1fde
  000e1fd4: movs r1,#0xd2
  000e1fd6: str.w r1,[r0,#0x184]
  000e1fda: movs r2,#0x0
  000e1fdc: movs r1,#0x1e
  000e1fde: cmp r4,#0x0
  000e1fe0: str.w r1,[r0,#0x188]
  000e1fe4: beq 0x000e1ff8
  000e1fe6: cmp r2,#0x0
  000e1fe8: mvn r1,#0x3
  000e1fec: mov.w r3,#0xdf
  000e1ff0: strd r3,r1,[r0,#0x18c]
  000e1ff4: bne 0x000e2024
  000e1ff6: b 0x000e203e
  000e1ff8: cbz r2,0x000e2032
  000e1ffa: mov.w r1,#0x15c
  000e1ffe: mvn r3,#0x1
  000e2002: cmp.w r8,#0x0
  000e2006: it eq
  000e2008: mov.eq r1,#0xae
  000e200a: it eq
  000e200c: mov.eq.w r3,#0xffffffff
  000e2010: mvn r5,#0x3
  000e2014: cmp r4,#0x0
  000e2016: it ne
  000e2018: mov.ne r3,r5
  000e201a: cmp r2,#0x0
  000e201c: it eq
  000e201e: mov.eq r3,r5
  000e2020: strd r1,r3,[r0,#0x18c]
  000e2024: movs r1,#0xc
  000e2026: cmp.w r8,#0x0
  000e202a: it eq
  000e202c: mov.eq r1,#0x6
  000e202e: movs r2,#0x1
  000e2030: b 0x000e2042
  000e2032: mvn r1,#0x3
  000e2036: mov.w r2,#0x140
  000e203a: strd r2,r1,[r0,#0x18c]
  000e203e: movs r2,#0x0
  000e2040: movs r1,#0xa
  000e2042: cmp r4,#0x0
  000e2044: str.w r1,[r0,#0x194]
  000e2048: beq.w 0x000e215e
  000e204c: cmp r2,#0x0
  000e204e: mov.w r1,#0x40
  000e2052: mov.w r3,#0xc
  000e2056: mov.w r5,#0xd
  000e205a: strd r3,r1,[r0,#0x198]
  000e205e: str.w r5,[r0,#0x1a0]
  000e2062: beq 0x000e2086
  000e2064: vmov.f32 s2,0x41800000
  000e2068: vldr.32 s0,[pc,#-0x2dc]
  000e206c: movs r1,#0x20
  000e206e: cmp.w r8,#0x0
  000e2072: mov.w r2,#0x1
  000e2076: it eq
  000e2078: vmov.eq.f32 s0,s2
  000e207c: it eq
  000e207e: mov.eq r1,#0x10
  000e2080: str.w r1,[r0,#0x1a4]
  000e2084: b 0x000e2092
  000e2086: movs r1,#0x1c
  000e2088: vmov.f32 s0,0x41c00000
  000e208c: str.w r1,[r0,#0x1a4]
  000e2090: movs r2,#0x0
  000e2092: vcvt.s32.f32 s0,s0
  000e2096: cmp.w r9,#0x0
  000e209a: vstr.32 s0,[r0,#0x1a8]
  000e209e: beq 0x000e20a6
  000e20a0: mov.w r1,#0x15a
  000e20a4: b 0x000e20ac
  000e20a6: cmp r4,#0x0
  000e20a8: beq 0x000e2182
  000e20aa: movs r1,#0xad
  000e20ac: cmp r2,#0x0
  000e20ae: str.w r1,[r0,#0x1ac]
  000e20b2: beq 0x000e21a2
  000e20b4: clz r1,r8
  000e20b8: vmov.i32 q8,#0x1f
  000e20bc: vmov.f32 s4,0x3f800000
  000e20c0: cmp.w r8,#0x0
  000e20c4: lsr.w r1,r1,#0x5
  000e20c8: vmov.f32 s2,0x40000000
  000e20cc: vdup.16 d18,r1
  000e20d0: vneg.s32 q8,q8
  000e20d4: adr.w r1,0xe2a30
  000e20d8: mov.w r9,#0x1
  000e20dc: vmovl.u16 q9,d18
  000e20e0: vld1.64 {d20,d21},[r1]
  000e20e4: adr.w r1,0xe2a40
  000e20e8: vmov.f32 s0,0x41000000
  000e20ec: vld1.64 {d22,d23},[r1]
  000e20f0: adr.w r1,0xe2a50
  000e20f4: it eq
  000e20f6: vmov.eq.f32 s2,s4
  000e20fa: vmov.f32 s4,0x40800000
  000e20fe: vshl.I32 q9,q9,#0x1f
  000e2102: cmp.w r8,#0x0
  000e2106: vshl.s32 q8,q9,q8
  000e210a: vld1.64 {d18,d19},[r1]
  000e210e: adr.w r1,0xe2a60
  000e2112: vld1.64 {d24,d25},[r1]
  000e2116: adr.w r1,0xe2a70
  000e211a: vmov q13,q8
  000e211e: it eq
  000e2120: vmov.eq.f32 s0,s4
  000e2124: vmov q14,q8
  000e2128: vstr.32 s2,[r0,#0x1e0]
  000e212c: vbsl q13,q12,q9
  000e2130: vld1.64 {d18,d19},[r1]
  000e2134: adr.w r1,0xe2a80
  000e2138: vld1.64 {d24,d25},[r1]
  000e213c: add.w r1,r0,#0x1c0
  000e2140: vbsl q14,q12,q9
  000e2144: vst1.32 {d26,d27},[r1]
  000e2148: add.w r1,r0,#0x1b0
  000e214c: vbsl q8,q11,q10
  000e2150: vst1.32 {d28,d29},[r1]
  000e2154: add.w r1,r0,#0x1d0
  000e2158: vst1.32 {d16,d17},[r1]
  000e215c: b 0x000e21e2
  000e215e: cbz r2,0x000e2194
  000e2160: movs r1,#0x5a
  000e2162: movs r2,#0xc
  000e2164: cmp.w r8,#0x0
  000e2168: it eq
  000e216a: mov.eq r1,#0x2d
  000e216c: it eq
  000e216e: mov.eq r2,#0x6
  000e2170: cmp.w r8,#0x0
  000e2174: strd r2,r1,[r0,#0x198]
  000e2178: mov.w r1,#0xe
  000e217c: it eq
  000e217e: mov.eq r1,#0x7
  000e2180: b 0x000e1ec0
  000e2182: cbz r2,0x000e219c
  000e2184: movs r1,#0xe8
  000e2186: cmp.w r8,#0x0
  000e218a: it eq
  000e218c: mov.eq r1,#0x74
  000e218e: str.w r1,[r0,#0x1ac]
  000e2192: b 0x000e20b4
  000e2194: movs r1,#0xa
  000e2196: movs r2,#0x36
  000e2198: movs r5,#0x9
  000e219a: b 0x000e1ef2
  000e219c: movs r1,#0xd0
  000e219e: str.w r1,[r0,#0x1ac]
  000e21a2: adr.w r1,0xe2a00
  000e21a6: vmov.f32 s0,0x40c00000
  000e21aa: vld1.64 {d16,d17},[r1]
  000e21ae: adr.w r1,0xe2a10
  000e21b2: mov.w r9,#0x0
  000e21b6: vld1.64 {d18,d19},[r1]
  000e21ba: adr.w r1,0xe2a20
  000e21be: vld1.64 {d20,d21},[r1]
  000e21c2: mov.w r1,#0x40000000
  000e21c6: str.w r1,[r0,#0x1e0]
  000e21ca: add.w r1,r0,#0x1c0
  000e21ce: vst1.32 {d20,d21},[r1]
  000e21d2: add.w r1,r0,#0x1b0
  000e21d6: vst1.32 {d16,d17},[r1]
  000e21da: add.w r1,r0,#0x1d0
  000e21de: vst1.32 {d18,d19},[r1]
  000e21e2: vcvt.s32.f32 s0,s0
  000e21e6: cmp.w lr,#0x0
  000e21ea: mov.w r1,#0x1
  000e21ee: mov.w r2,#0x1
  000e21f2: it ne
  000e21f4: mov.ne r1,r8
  000e21f6: cmp.w r10,#0x0
  000e21fa: str.w r1,[r0,#0x1e8]
  000e21fe: vstr.32 s0,[r0,#0x1e4]
  000e2202: beq 0x000e22b8
  000e2204: adr.w r1,0xe2a90
  000e2208: movs r3,#0x0
  000e220a: vld1.64 {d16,d17},[r1]
  000e220e: adr.w r1,0xe2aa0
  000e2212: cmp.w r9,#0x0
  000e2216: vld1.64 {d18,d19},[r1]
  000e221a: adr.w r1,0xe2ab0
  000e221e: vld1.64 {d20,d21},[r1]
  000e2222: adr.w r1,0xe2ac0
  000e2226: vld1.64 {d22,d23},[r1]
  000e222a: adr.w r1,0xe2ad0
  000e222e: vld1.64 {d24,d25},[r1]
  000e2232: adr.w r1,0xe2ae0
  000e2236: vld1.64 {d26,d27},[r1]
  000e223a: adr.w r1,0xe2af0
  000e223e: vld1.64 {d28,d29},[r1]
  000e2242: mov.w r1,#0x2
  000e2246: str.w r1,[r0,#0x1fc]
  000e224a: mov.w r1,#0x1a
  000e224e: str.w r1,[r0,#0x1ec]
  000e2252: mov.w r1,#0x46
  000e2256: strd r2,r1,[r0,#0x234]
  000e225a: mov.w r1,#0x1c
  000e225e: strd r1,r3,[r0,#0x27c]
  000e2262: mov.w r1,#0x8c
  000e2266: strd r2,r3,[r0,#0x3e0]
  000e226a: mov.w r2,#0x84
  000e226e: str.w r1,[r0,#0x3e8]
  000e2272: mov.w r1,#0x4d
  000e2276: strd r1,r2,[r0,#0x3f4]
  000e227a: add.w r1,r0,#0x204
  000e227e: vst1.32 {d16,d17},[r1]
  000e2282: add.w r1,r0,#0x214
  000e2286: vst1.32 {d18,d19},[r1]
  000e228a: add.w r1,r0,#0x224
  000e228e: vst1.32 {d20,d21},[r1]
  000e2292: add.w r1,r0,#0x23c
  000e2296: vst1.32 {d22,d23},[r1]
  000e229a: add.w r1,r0,#0x24c
  000e229e: vst1.32 {d24,d25},[r1]
  000e22a2: add.w r1,r0,#0x25c
  000e22a6: vst1.32 {d26,d27},[r1]
  000e22aa: add.w r1,r0,#0x26c
  000e22ae: vst1.32 {d28,d29},[r1]
  000e22b2: beq.w 0x000e24de
  000e22b6: b 0x000e2530
  000e22b8: cmp.w r9,#0x0
  000e22bc: beq.w 0x000e240e
  000e22c0: adr.w r1,0xe2b70
  000e22c4: vmov.i32 q10,#0x1f
  000e22c8: vld1.64 {d16,d17},[r1]
  000e22cc: adr.w r1,0xe2b80
  000e22d0: movs r4,#0x4
  000e22d2: cmp.w r8,#0x0
  000e22d6: vld1.64 {d18,d19},[r1]
  000e22da: adr.w r1,0xe2b90
  000e22de: vneg.s32 q10,q10
  000e22e2: it eq
  000e22e4: mov.eq r4,#0x2
  000e22e6: vld1.64 {d22,d23},[r1]
  000e22ea: adr.w r1,0xe2ba0
  000e22ee: movs r2,#0x26
  000e22f0: cmp.w r8,#0x0
  000e22f4: vld1.64 {d24,d25},[r1]
  000e22f8: clz r1,r8
  000e22fc: mov.w r5,#0x28
  000e2300: mov.w r3,#0x8
  000e2304: lsr.w r1,r1,#0x5
  000e2308: str.w r4,[r0,#0x1fc]
  000e230c: vdup.16 d26,r1
  000e2310: it eq
  000e2312: mov.eq r2,#0x13
  000e2314: str.w r2,[r0,#0x1ec]
  000e2318: it eq
  000e231a: mov.eq r5,#0x14
  000e231c: vmovl.u16 q13,d26
  000e2320: movs r1,#0x46
  000e2322: str.w r5,[r0,#0x224]
  000e2326: cmp.w r8,#0x0
  000e232a: it eq
  000e232c: mov.eq r1,#0x23
  000e232e: cmp.w r8,#0x0
  000e2332: str.w r1,[r0,#0x228]
  000e2336: mov.w r1,#0x2
  000e233a: vshl.I32 q13,q13,#0x1f
  000e233e: str.w r4,[r0,#0x22c]
  000e2342: it eq
  000e2344: mov.eq r1,#0x1
  000e2346: movs r2,#0x64
  000e2348: str.w r1,[r0,#0x230]
  000e234c: cmp.w r8,#0x0
  000e2350: vshl.s32 q10,q13,q10
  000e2354: str.w r1,[r0,#0x234]
  000e2358: it eq
  000e235a: mov.eq r2,#0x32
  000e235c: str.w r2,[r0,#0x238]
  000e2360: mov r2,r8
  000e2362: vmov q13,q10
  000e2366: it ne
  000e2368: mvn.ne r2,#0x1
  000e236c: strd r2,r1,[r0,#0x24c]
  000e2370: add.w r2,r0,#0x20c
  000e2374: vbsl q13,q12,q11
  000e2378: cmp.w r8,#0x0
  000e237c: vbsl q10,q9,q8
  000e2380: vst1.32 d26[0x1],[r2:32]
  000e2384: add.w r2,r0,#0x218
  000e2388: vst1.32 d26[0x1],[r2:32]
  000e238c: add.w r2,r0,#0x220
  000e2390: vst1.32 d26[0x1],[r2:32]
  000e2394: mov.w r2,#0x12
  000e2398: it eq
  000e239a: mov.eq r2,#0x9
  000e239c: it eq
  000e239e: mov.eq r3,#0x4
  000e23a0: cmp.w r8,#0x0
  000e23a4: strd r2,r3,[r0,#0x204]
  000e23a8: mvn r2,#0x1
  000e23ac: it eq
  000e23ae: mov.eq r2,#0x1
  000e23b0: strd r3,r2,[r0,#0x210]
  000e23b4: mov.w r2,#0xc
  000e23b8: it eq
  000e23ba: mov.eq r2,#0x6
  000e23bc: cmp.w r8,#0x0
  000e23c0: str.w r2,[r0,#0x21c]
  000e23c4: add.w r2,r0,#0x23c
  000e23c8: vst1.32 {d26,d27},[r2]
  000e23cc: add.w r2,r0,#0x278
  000e23d0: vst1.32 d20[0x0],[r2:32]
  000e23d4: add.w r2,r0,#0x264
  000e23d8: vst1.32 {d20,d21},[r2]
  000e23dc: mov.w r2,#0x20
  000e23e0: strd r3,r4,[r0,#0x254]
  000e23e4: it eq
  000e23e6: mov.eq r2,#0x10
  000e23e8: str.w r2,[r0,#0x25c]
  000e23ec: eor r2,r8,#0x1
  000e23f0: str.w r2,[r0,#0x260]
  000e23f4: movs r2,#0xc2
  000e23f6: cmp.w r8,#0x0
  000e23fa: it eq
  000e23fc: mov.eq r2,#0x61
  000e23fe: str.w r2,[r0,#0x274]
  000e2402: mov r2,r8
  000e2404: str.w r5,[r0,#0x27c]
  000e2408: it ne
  000e240a: mov.ne r2,#0x5
  000e240c: b 0x000e2498
  000e240e: adr.w r1,0xe2b00
  000e2412: movs r2,#0x4
  000e2414: vld1.64 {d16,d17},[r1]
  000e2418: adr.w r1,0xe2b10
  000e241c: movs r3,#0x5a
  000e241e: vld1.64 {d18,d19},[r1]
  000e2422: adr.w r1,0xe2b20
  000e2426: vld1.64 {d20,d21},[r1]
  000e242a: adr.w r1,0xe2b30
  000e242e: vld1.64 {d22,d23},[r1]
  000e2432: adr.w r1,0xe2b40
  000e2436: vld1.64 {d24,d25},[r1]
  000e243a: adr.w r1,0xe2b50
  000e243e: vld1.64 {d26,d27},[r1]
  000e2442: adr.w r1,0xe2b60
  000e2446: vld1.64 {d28,d29},[r1]
  000e244a: movs r1,#0x22
  000e244c: str.w r2,[r0,#0x1fc]
  000e2450: str.w r1,[r0,#0x1ec]
  000e2454: movs r1,#0x2
  000e2456: strd r1,r3,[r0,#0x234]
  000e245a: movs r3,#0x1e
  000e245c: str.w r3,[r0,#0x27c]
  000e2460: add.w r3,r0,#0x204
  000e2464: vst1.32 {d16,d17},[r3]
  000e2468: add.w r3,r0,#0x214
  000e246c: vst1.32 {d18,d19},[r3]
  000e2470: add.w r3,r0,#0x224
  000e2474: vst1.32 {d20,d21},[r3]
  000e2478: add.w r3,r0,#0x23c
  000e247c: vst1.32 {d22,d23},[r3]
  000e2480: add.w r3,r0,#0x24c
  000e2484: vst1.32 {d24,d25},[r3]
  000e2488: add.w r3,r0,#0x25c
  000e248c: vst1.32 {d26,d27},[r3]
  000e2490: add.w r3,r0,#0x26c
  000e2494: vst1.32 {d28,d29},[r3]
  000e2498: vmov s0,r1
  000e249c: mov r1,r8
  000e249e: cmp.w r9,#0x0
  000e24a2: vcvt.f32.s32 s0,s0
  000e24a6: vcvt.s32.f32 s0,s0
  000e24aa: str.w r2,[r0,#0x280]
  000e24ae: it eq
  000e24b0: mov.eq r1,#0x1
  000e24b2: str.w r1,[r0,#0x3e4]
  000e24b6: vstr.32 s0,[r0,#0x3e0]
  000e24ba: beq 0x000e24d0
  000e24bc: ldr r1,[0x000e2864]
  000e24be: add r1,pc
  000e24c0: ldr r1,[r1,#0x0]
  000e24c2: ldrb r1,[r1,#0x0]
  000e24c4: cbz r1,0x000e24ea
  000e24c6: movs r1,#0xc8
  000e24c8: str.w r1,[r0,#0x3e8]
  000e24cc: movs r1,#0x6e
  000e24ce: b 0x000e2514
  000e24d0: movs r2,#0x70
  000e24d2: movs r1,#0x39
  000e24d4: str.w r2,[r0,#0x3e8]
  000e24d8: movs r2,#0x6f
  000e24da: strd r1,r2,[r0,#0x3f4]
  000e24de: mov.w r1,#0x160
  000e24e2: str.w r1,[r0,#0x3ec]
  000e24e6: movs r1,#0x4b
  000e24e8: b 0x000e2548
  000e24ea: ldr r1,[0x000e2868]
  000e24ec: add r1,pc
  000e24ee: ldr r1,[r1,#0x0]
  000e24f0: ldrb r1,[r1,#0x0]
  000e24f2: cbz r1,0x000e24fe
  000e24f4: movs r1,#0x64
  000e24f6: str.w r1,[r0,#0x3e8]
  000e24fa: movs r1,#0x37
  000e24fc: b 0x000e2514
  000e24fe: movs r1,#0x96
  000e2500: cmp.w r8,#0x0
  000e2504: it eq
  000e2506: mov.eq r1,#0x4b
  000e2508: str.w r1,[r0,#0x3e8]
  000e250c: mov.w r1,#0x4c
  000e2510: it eq
  000e2512: mov.eq r1,#0x26
  000e2514: add.w r2,r1,#0x11
  000e2518: lsl.w r2,r2,r8
  000e251c: vmov s0,r2
  000e2520: vcvt.f32.s32 s0,s0
  000e2524: vcvt.s32.f32 s0,s0
  000e2528: str.w r1,[r0,#0x3f4]
  000e252c: vstr.32 s0,[r0,#0x3f8]
  000e2530: mov.w r1,#0x188
  000e2534: cmp.w r8,#0x0
  000e2538: it eq
  000e253a: mov.eq r1,#0xc4
  000e253c: str.w r1,[r0,#0x3ec]
  000e2540: mov.w r1,#0x64
  000e2544: it eq
  000e2546: mov.eq r1,#0x32
  000e2548: str.w r1,[r0,#0x3f0]
  000e254c: movs r6,#0x2
  000e254e: movs r1,#0x4
  000e2550: cmp.w r10,#0x0
  000e2554: beq 0x000e2566
  000e2556: mvn r5,#0x11
  000e255a: mov.w r11,#0x16
  000e255e: movs r2,#0x1a
  000e2560: movs r3,#0x4
  000e2562: movs r4,#0x2
  000e2564: b 0x000e25f8
  000e2566: ldr r2,[0x000e286c]
  000e2568: cmp.w r8,#0x0
  000e256c: it ne
  000e256e: mov.ne r6,#0x4
  000e2570: mov.w r3,#0x10
  000e2574: add r2,pc
  000e2576: mov r4,r10
  000e2578: mvn r5,#0x19
  000e257c: ldr r2,[r2,#0x0]
  000e257e: ldrb.w r10,[r2,#0x0]
  000e2582: it ne
  000e2584: mov.ne r3,#0x20
  000e2586: mvn r2,#0x19
  000e258a: cmp.w r10,#0x0
  000e258e: it eq
  000e2590: mov.eq r3,r6
  000e2592: vmov s0,r3
  000e2596: cmp.w r8,#0x0
  000e259a: mov.w r3,#0x6
  000e259e: mov.w r6,#0x3
  000e25a2: vcvt.f32.s32 s0,s0
  000e25a6: vcvt.s32.f32 s0,s0
  000e25aa: it eq
  000e25ac: mvn.eq r5,#0xc
  000e25b0: cmp.w r9,#0x0
  000e25b4: it eq
  000e25b6: mov.eq r5,r2
  000e25b8: movs r2,#0x26
  000e25ba: cmp.w r8,#0x0
  000e25be: it eq
  000e25c0: mov.eq r2,#0x13
  000e25c2: cmp.w r9,#0x0
  000e25c6: it eq
  000e25c8: mov.eq r2,#0x22
  000e25ca: cmp.w r8,#0x0
  000e25ce: it eq
  000e25d0: mov.eq r3,#0x3
  000e25d2: cmp.w r9,#0x0
  000e25d6: it eq
  000e25d8: mov.eq r3,#0x5
  000e25da: it ne
  000e25dc: mov.ne r6,#0x2
  000e25de: vmov r11,s0
  000e25e2: cmp.w r10,#0x0
  000e25e6: mov r10,r4
  000e25e8: mov.w r4,#0x4
  000e25ec: it eq
  000e25ee: mov.eq r4,#0x2
  000e25f0: cmp.w r8,#0x0
  000e25f4: it eq
  000e25f6: mov.eq r4,#0x2
  000e25f8: str.w r6,[r0,#0x2bc]
  000e25fc: cmp.w r9,#0x0
  000e2600: str.w r3,[r0,#0x3fc]
  000e2604: mov.w r3,#0x1
  000e2608: str.w r5,[r0,#0x200]
  000e260c: mov.w r6,#0x7
  000e2610: strd r2,r11,[r0,#0x1f0]
  000e2614: eor r2,r12,#0x1
  000e2618: str.w r4,[r0,#0x1f8]
  000e261c: strb.w r2,[r0,#0x284]
  000e2620: strb.w r3,[r0,#0x285]
  000e2624: mvn r3,#0x12
  000e2628: strb.w r2,[r0,#0x286]
  000e262c: mov.w r2,#0x14
  000e2630: it ne
  000e2632: mov.ne r2,#0x0
  000e2634: it ne
  000e2636: mov.ne r3,#0x0
  000e2638: cmp.w r8,#0x0
  000e263c: it eq
  000e263e: mov.eq r6,#0x3
  000e2640: cmp.w r9,#0x0
  000e2644: it ne
  000e2646: mov.ne r1,r6
  000e2648: strd r2,r3,[r0,#0x288]
  000e264c: cmp.w r10,#0x0
  000e2650: str.w r1,[r0,#0x290]
  000e2654: ldr r1,[0x000e2870]
  000e2656: add r1,pc
  000e2658: beq 0x000e267e
  000e265a: ldr r2,[0x000e2874]
  000e265c: movs r3,#0x70
  000e265e: ldr r1,[r1,#0x0]
  000e2660: movs r5,#0x32
  000e2662: movw r12,#0x119
  000e2666: mov.w r4,#0x1ec
  000e266a: strd r3,r2,[r0,#0x2f4]
  000e266e: movw r2,#0x15f
  000e2672: strd r5,r12,[r0,#0x2fc]
  000e2676: str.w r4,[r0,#0x304]
  000e267a: ldrb r1,[r1,#0x0]
  000e267c: b 0x000e2704
  000e267e: ldr r1,[r1,#0x0]
  000e2680: ldrb r1,[r1,#0x0]
  000e2682: cbz r1,0x000e268c
  000e2684: movs r2,#0xa0
  000e2686: vldr.32 s0,[pc,#0x1f0]
  000e268a: b 0x000e26d0
  000e268c: ldr r2,[0x000e287c]
  000e268e: add r2,pc
  000e2690: ldr r2,[r2,#0x0]
  000e2692: ldrb r2,[r2,#0x0]
  000e2694: cbz r2,0x000e269e
  000e2696: movs r2,#0x50
  000e2698: vldr.32 s0,[pc,#0x1e4]
  000e269c: b 0x000e26d0
  000e269e: adr.w r2,0xe2bb0
  000e26a2: cmp.w r8,#0x0
  000e26a6: it eq
  000e26a8: add.eq r2,#0x4
  000e26aa: vldr.32 s0,[pc,#0x1d8]
  000e26ae: vldr.32 s2,[r2]
  000e26b2: cmp.w r9,#0x0
  000e26b6: mov.w r2,#0x3c
  000e26ba: it ne
  000e26bc: vmov.ne.f32 s0,s2
  000e26c0: cmp.w r8,#0x0
  000e26c4: it eq
  000e26c6: mov.eq r2,#0x1e
  000e26c8: cmp.w r9,#0x0
  000e26cc: it eq
  000e26ce: mov.eq r2,#0x2d
  000e26d0: str.w r2,[r0,#0x2f4]
  000e26d4: movs r2,#0x48
  000e26d6: cmp r1,#0x0
  000e26d8: mov.w r3,#0x190
  000e26dc: mov.w r5,#0x2bc
  000e26e0: it eq
  000e26e2: mov.eq r2,#0x24
  000e26e4: vstr.32 s0,[r0,#0x2f8]
  000e26e8: it eq
  000e26ea: mov.eq r3,#0xc8
  000e26ec: cmp r1,#0x0
  000e26ee: it eq
  000e26f0: mov.eq.w r5,#0x15e
  000e26f4: add.w r12,r0,#0x2fc
  000e26f8: stm.w r12!,{r2,r3,r5}
  000e26fc: mov.w r2,#0x1f4
  000e2700: it eq
  000e2702: mov.eq r2,#0xfa
  000e2704: cmp r1,#0x0
  000e2706: str.w r2,[r0,#0x308]
  000e270a: beq 0x000e2722
  000e270c: mvn r2,#0x17
  000e2710: movs r3,#0x6
  000e2712: mvn r6,#0x9
  000e2716: strd r3,r2,[r0,#0x30c]
  000e271a: str.w r6,[r0,#0x314]
  000e271e: movs r2,#0x8
  000e2720: b 0x000e2774
  000e2722: ldr r2,[0x000e2888]
  000e2724: add r2,pc
  000e2726: ldr r2,[r2,#0x0]
  000e2728: ldrb r2,[r2,#0x0]
  000e272a: cbz r2,0x000e273c
  000e272c: mvn r3,#0xb
  000e2730: movs r6,#0x3
  000e2732: strd r6,r3,[r0,#0x30c]
  000e2736: mvn r3,#0x4
  000e273a: b 0x000e276a
  000e273c: cmp.w r9,#0x0
  000e2740: beq 0x000e2760
  000e2742: mvn r3,#0x8
  000e2746: cmp.w r8,#0x0
  000e274a: it eq
  000e274c: mvn.eq r3,#0x4
  000e2750: strd r6,r3,[r0,#0x30c]
  000e2754: mvn r3,#0x7
  000e2758: it eq
  000e275a: mvn.eq r3,#0x4
  000e275e: b 0x000e276a
  000e2760: mvn r3,#0x7
  000e2764: movs r6,#0x4
  000e2766: strd r6,r3,[r0,#0x30c]
  000e276a: str.w r3,[r0,#0x314]
  000e276e: cmp r2,#0x0
  000e2770: it ne
  000e2772: mov.ne r2,#0x4
  000e2774: cmp.w r10,#0x0
  000e2778: str.w r2,[r0,#0x318]
  000e277c: beq 0x000e278a
  000e277e: movw r1,#0x14b
  000e2782: str.w r1,[r0,#0x31c]
  000e2786: movs r1,#0xca
  000e2788: b 0x000e27f8
  000e278a: cbz r1,0x000e279a
  000e278c: mov.w r1,#0x1d8
  000e2790: str.w r1,[r0,#0x31c]
  000e2794: mov.w r1,#0x120
  000e2798: b 0x000e27f8
  000e279a: ldr r1,[0x000e288c]
  000e279c: adr.w r2,0xe2bb8
  000e27a0: adr.w r3,0xe2bc0
  000e27a4: add r1,pc
  000e27a6: ldr r1,[r1,#0x0]
  000e27a8: ldrb r1,[r1,#0x0]
  000e27aa: cmp r1,#0x0
  000e27ac: it ne
  000e27ae: add.ne r2,#0x4
  000e27b0: vldr.32 s0,[r2]
  000e27b4: cmp.w r8,#0x0
  000e27b8: it eq
  000e27ba: add.eq r3,#0x4
  000e27bc: cmp.w r9,#0x0
  000e27c0: vldr.32 s2,[r3]
  000e27c4: vmov.f32 s4,s0
  000e27c8: mov.w r2,#0xec
  000e27cc: it ne
  000e27ce: vmov.ne.f32 s4,s2
  000e27d2: cmp r1,#0x0
  000e27d4: it ne
  000e27d6: vmov.ne.f32 s4,s0
  000e27da: orrs.w r1,r1,r8
  000e27de: it eq
  000e27e0: mov.eq r2,#0x76
  000e27e2: movs r1,#0xec
  000e27e4: cmp.w r9,#0x0
  000e27e8: it eq
  000e27ea: mov.eq r2,r1
  000e27ec: vcvt.s32.f32 s0,s4
  000e27f0: str.w r2,[r0,#0x31c]
  000e27f4: vmov r1,s0
  000e27f8: str.w r1,[r0,#0x320]
  000e27fc: pop.w {r2,r3,r8,r9,r10,r11}
  000e2800: pop {r4,r5,r6,r7,pc}
  000e2802: ldr r6,[sp,#0x4]
  000e2804: movs r2,#0x6c
  000e2806: ldrb r1,[r6,#0x0]
  000e2808: cmp r1,#0x0
  000e280a: mov.w r1,#0x38
  000e280e: it eq
  000e2810: mov.eq r2,#0x50
  000e2812: it eq
  000e2814: mov.eq r1,#0x28
  000e2816: strd r1,r2,[r0,#0x130]
  000e281a: movs r1,#0x2f
  000e281c: ldrb r2,[r6,#0x0]
  000e281e: cmp r2,#0x0
  000e2820: it eq
  000e2822: mov.eq r1,#0x23
  000e2824: b.w 0x000e1cba
  000e2828: ldrb r1,[r6,#0x0]
  000e282a: movs r6,#0x2d
  000e282c: cmp r1,#0x0
  000e282e: it eq
  000e2830: mov.eq r6,#0x1e
  000e2832: b.w 0x000e1814
  000e2836: ldrb r1,[r3,#0x0]
  000e2838: movs r2,#0x2d
  000e283a: cmp r1,#0x0
  000e283c: it eq
  000e283e: mov.eq r2,#0x1e
  000e2840: b.w 0x000e1860
  000e2844: ldrb r1,[r6,#0x0]
  000e2846: movs r6,#0x40
  000e2848: cmp r1,#0x0
  000e284a: it eq
  000e284c: mov.eq r6,#0x2c
  000e284e: b.w 0x000e16d6
```
