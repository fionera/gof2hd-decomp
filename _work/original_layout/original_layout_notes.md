# Original GoF2 source-layout notes (from "source file list dump")

Companion to `original_layout.json` (same directory). All observations come from
`source file lists dump.txt` plus the three per-TU symbol dumps
(`gof2hd_1.1.19_ios.cpp`, `gof2sd_1.0.1_ios.cpp`, `gof3d_1.1.4_ios.cpp`).

## Dump inventory / provenance

| dump | TUs | symbols | build env |
|---|---|---|---|
| gof2hd_1.1.19_ios | 125 (124 app + arclite.o) | 3627 | Xcode DerivedData, arm64, iPhoneOS 16.4 SDK, Release re-archive (`/Users/n.tran/projects/gof2/`); object names carry content hashes |
| gof2sd_1.0.1_ios | 158 (118 app + 40 engine `.a` members) | 3591 | armv6, iPhoneOS 4.0/4.1 SDK, `Distribute-iphoneos` (`/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/`) |
| gof3d_1.1.4_ios | 111 | 2152 | armv6, GoF1 "3D" (`/Users/marchehmeyer/Documents/Dev/GalaxyOnFire/`) |

## Folder taxonomy per platform

### gof2 hd iOS 1.1.19 (project root `/Users/n.tran/projects/gof2/`)
- `AE2/Classes/` — the Abyss Engine 2. Only **headers** surface in this dump
  (`Array.h, Vector.h, Matrix.h, Transform.h, AEString.h, PaintCanvas.h, Mesh.h,
  EaseInOutMatrix.h, IApplicationModule.h`); engine .cpp TUs are absent from the
  app link listing → AE2 was a separate library target, headers included from `AE2/Classes/`.
- `Classes/` — iOS app glue (ObjC/ObjC++): `AppController.mm, AppView.mm,
  CloudHandler.mm, Gof2SaveDocument.m, GamePadHandler.mm, main.m`.
- `Classes/Game/` — the entire game layer, FLAT (~100 .cpp): world, ships, guns,
  menus, UI windows all in one folder. Only two subfolders:
  - `Classes/Game/ParticleSystems/` (ParticleSettings, ParticleSystemManager,
    IParticleSystem, ParticleSystemSprite, ParticleSystemMesh)
  - `Classes/Game/MeshMerger/` (`AMeshMerger.cpp`, `LodMeshMerger.cpp`)
- `Classes/AECommunityTools/` (GameCenter), `Classes/Facebook/`,
  `Classes/Facebook/JSON/` (SBJson).
- FMOD headers referenced from `FMOD/v44442/inc/` (SD used `FMOD/v432`).
- Quirk: `AutoPilotList.cpp` appears as a TU but is missing from the txt file
  list and has no self-contributor line (resolved via carried dir context to
  `Classes/Game/AutoPilotList.cpp`).

### gof2 sd 1.0.1 iOS (root `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/`)
- Same `Classes/` + flat `Classes/Game/` layout as HD, plus `Classes/zlib/`
  (zlib compiled in-tree — like the Android .so later does).
- Engine delivered as **prebuilt static lib**
  `/Volumes/realtime/projects/1605_Abyss_Engine/iPhone/lib_float_iOS4/libAbyssEngineDevice.a`,
  headers from `.../include_float_iOS4/`. The 40 archive members expose the
  engine source layout at `/Users/marchehmeyer/Documents/Dev/Svn_iPhone_AE2/Classes/`:
  flat engine folder (AEFile, AEString.mm, ApplicationManager, Camera, CheatCode,
  configreader, EaseInOut, EaseInOutMatrix, Engine, Engine_iPhone.mm,
  FileInterfaceIPhone.mm, FLStat.m, FLStat_cpp.mm, GameText, GameText_iPhone.mm,
  Image, Image2D, ImageFont, IApplicationModule, Material, Math16_iPhone, Matrix,
  Mesh, PaintCanvas, Quaternion, Random, SpriteSystem, Transform, Vector,
  AESoundRessource, AlertView.mm, AccelerometerFilter.mm) with ONE subfolder
  `Classes/Shader/` (BaseShader, SimpleShader, NoTexShader, NoTexVtxColorShader,
  TextureShader, TextureAlphaTestShader, TextureLightShader, TextureVtxColorShader).

### gof3d 1.1.4 / 1.1.5 (GoF1; roots `.../GalaxyOnFire/`, `.../GOF/`)
- No engine/game project split at all: engine files live directly in `Classes/`
  (AEFile, AESound.mm, AEString, ApplicationManager.mm, Camera, CheatCode,
  configreader, Engine.mm, Image, Image2D, ImageFont, Material, Math16, Matrix,
  Mesh, PaintCanvas, Random, SpriteSystem, Transform, Vector …) and game files in
  `Classes/Game/`. Some game-ish files sit in `Classes/` (iExplosion,
  ExplosionHandler, SpaceStation, SelectionList, SellCargoWindow).
  `Classes/zlib/` in-tree. GoF1 menu naming: `MBriefing, MHangar, MMainMenu,
  MMap, MStation` vs GoF2's `MGame, MTitle, ModMainMenu, ModStation`.
- 1.1.5 adds AECommunityTools + zlib compress/deflate/trees vs 1.1.4
  (annotated "# missing in 1.1.4").

### gof3d zeebo (BREW/Zeebo port)
- Windows-style paths, different top folders: `AbyssEngine\` (mesh, image,
  engine, aesound, fileinterfacebrew), `game\` (MMainMenu), `gof\` (gof.cpp,
  GamepadMgr.cpp). Engine folder literally named "AbyssEngine".

### windows
- Flat `Classes\` with `Engine_DirectX.cpp`, `FBOContainer.cpp`, `PaintCanvas.cpp`,
  `Mesh.cpp`, `Image.cpp`, `ImageFont.cpp`, and `Classes\Shader\BaseShader.cpp`.
  FBOContainer exists in our repo (`src/engine/render/FBOContainer.*`) → the
  Android/desktop-era engine kept this file.

### mac fhd (root `.../GalaxyOnFire2_merged/`)
- "merged" tree: `Classes/Game/...` for game code plus an app dir literally named
  `Galaxy on Fire 2/` containing `OptionsMenu/KeyBinder.m` and
  `Validation/asn1/*` (Mac App Store receipt validation).
- Key rename vs iOS: `Classes/Game/MeshMerger.cpp` (no "A" prefix, single file)
  alongside `Classes/Game/MeshMerger/LodMeshMerger.cpp`.

### Asserts gof2 sd MeeGo 1.2.7
- Paths `../../../src/Classes/Game/...` → MeeGo build nested the tree under
  `src/`. Reveals `MeshMerger/SimpleMeshMerger.cpp` (matches our
  `src/engine/render/SimpleMeshMerger.cpp`!) AND a combined
  `Classes/Game/MeshMerger.cpp` defining `MeshMerger::*` with both ctor
  overloads — i.e. post-1.1.x the AMeshMerger/LodMeshMerger pair was refactored
  into MeshMerger + SimpleMeshMerger + LodMeshMerger. Types: `AEint, AEbyte,
  AEushort, AEubyte, AEbool`, namespace `AbyssEngine::AEMath::Matrix` — exactly
  the type vocabulary in the Android binary.

## Multi-class / header-inline TUs (from symbol attribution)

- `BeamGun.cpp` defines **AbstractGun** too (AbstractGun.h is header-implemented;
  contributors: `Classes/Game/AbstractGun.h`, `BeamGun.h`). Our repo splits
  AbstractGun.cpp out — original had no AbstractGun.cpp.
- `BombGun.cpp` carries inlined **RocketGun** methods (RocketGun.h contributor)
  even though RocketGun.cpp is its own TU.
- `ParticleSettings.cpp` also defines **ParticleSettingsRef** (statics: `cur`,
  `init`, `assertInit`, DEBUG_particleCount*).
- `ParticleSystemSprite.cpp` emits `IParticleSystem::~IParticleSystem()`.
- `SystemPathFinder.cpp` defines helper class **Node**.
- `PlayerEgo.cpp` (HD) pulls in **EaseInOutMatrix** code from AE2 header;
  SD's `SpaceLounge.cpp` does the same.
- `MTitle.cpp` (HD) / `MGame.cpp` (SD) emit **IApplicationModule** code
  (IApplicationModule.h inline).
- SD `PlayerAsteroid.cpp` emits **KIPlayer** methods (KIPlayer.h inline);
  gof3d does the same in PlayerFixedObject/PlayerJunk/Waypoint.
- SD `GameInit.cpp` has `GameData.h` contributor → GameData is header-defined
  (our repo's engine/core/GameData.*).
- Engine (SD .a): `AEFile.cpp` defines AEFile/AEPakFile/AENormalFile/AELowLevelFile;
  `CheatCode.cpp` defines CheatCode + CheatHandler; `Image.cpp` also emits
  ImageFont + AELoadedTexture bits; `PaintCanvas.cpp` is a kitchen-sink TU
  (PaintCanvas, Camera, Material, Image2D, ResourceMesh/Texture/Transform,
  AELoadedTexture); `configreader.cpp` defines ConfigReader + TokenStruct;
  `Transform.cpp` also KeyFrame; `Engine.cpp` also ShaderBaseStruct.
- HD headers seen inline (headers_seen union): AE2/Classes/{Array, Vector,
  Matrix, Transform, AEString, PaintCanvas, Mesh, EaseInOutMatrix,
  IApplicationModule}.h; Classes/Game/{AbstractGun, BeamGun, BombGun, RocketGun,
  MineGun, KIPlayer, GameData, MGame, MTitle, ModMainMenu, ModStation}.h;
  ParticleSystems/{ParticleSettings, IParticleSystem, ParticleSystemSprite}.h.

## Notable names
- `AMeshMerger.cpp` (iOS 1.x abstract base) → `MeshMerger.cpp` +
  `SimpleMeshMerger.cpp` (mac fhd / MeeGo 1.2.7 / our Android tree).
- `BoundingSphere.cpp`, `BoundingAAB.cpp`, `BoundingVolume.cpp` — game-side
  bounding volumes in `Classes/Game/` (HD & SD; distinct from engine BSphere).
- `AbstractGun.h` — header-only gun base in every original tree.
- `FileRead.cpp` — bulk data readers, own TU in HD/SD.
- HD-only gameplay TUs vs SD 1.0.1: AutoPilotList, DialogueWindow*, MineGun,
  PlayerTurret, SentryGun, HackingGame, PlayerGasCloud, RepairBeam, SpacePoint,
  Wanted, WantedWindow, BoundingSphere, LODManager*, GamePadHandler, CloudHandler,
  Gof2SaveDocument (* = present in both; MineGun/Wanted/etc. are the Valkyrie /
  Most-Wanted-era additions). SD-only: `Classes/zlib/*` (engine lib had no zlib).
- SD FLStat.m / FLStat_cpp.mm — Fishlabs stats module inside the engine lib.
- `?0B8l` (x2) and `"NSNotification"8l` in the HD dump are garbled ObjC block
  descriptor lines; retained verbatim in the symbol arrays.

## Kinship to Android HD 2.0.16 (our target)

**Closest: `gof2hd_1.1.19_ios`, by a wide margin.**
- 103/123 app TU stems exist in our `src/` (SD: 93/117, gof3d: 63/110). Every
  one of the 20 misses is iOS-only glue (AppController/AppView, Facebook+SBJson,
  AECommunityTools, CloudHandler, Gof2SaveDocument, GamePadHandler) or
  GameInit/GameResource (merged/renamed in our tree) — exactly the layer Android
  replaces with `src/platform/android` (JniBridge, NdkHooks, ApkLoader,
  FileInterfaceAndroid).
- All HD-era content classes in the Android binary are present in 1.1.19
  (Wanted/WantedWindow, SentryGun, HackingGame, PlayerGasCloud, RepairBeam,
  SpacePoint, MineGun, AutoPilotList, SpaceLounge, MiningGame). SD 1.0.1 lacks
  them; gof3d is a different game.
- Android 2.0.16 additions NOT in the 1.1.19 dump: PlayerCreature, SpriteGun,
  the GLES2 shader zoo (BumpShader*, GlowShader, …), Supernova/Kaamo content —
  i.e. Android = 1.1.19 layout + 2.0.x content + engine compiled in.
- Engine linkage differs: iOS keeps AE2 as a separate lib (`AE2/` in 1.1.19,
  `libAbyssEngineDevice.a` in 1.0.1); Android statically compiles engine + zlib +
  libzip into one .so (cf. MEMORY: zlib 1.2.3, libzip, NDK r18b). The SD dump is
  the best witness for the ENGINE file inventory (40 engine TUs incl. `Shader/`
  subfolder ≙ our `src/engine/render/shaders`), while the MeeGo 1.2.7 asserts
  (`src/Classes/...`, SimpleMeshMerger, AE-typedefs) are the closest witness for
  the post-1.2 engine refactor state the Android build grew from.
- Original layout is `Classes/Game` FLAT; our repo's `src/game/{core,menu,
  mission,ship,ui,weapons,world}` split is our own invention (fine, but no
  original precedent), and our `src/engine/*` matches AE2/Svn_iPhone_AE2
  content-wise with an added folder taxonomy.
