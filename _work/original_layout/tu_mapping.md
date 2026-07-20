# TU mapping: our src/ vs original GoF2 layout (primary witness gof2hd_1.1.19_ios; engine witness gof2sd_1.0.1 libAbyssEngineDevice.a; secondary mac fhd / MeeGo / windows)

Stats: {'exact_hd': 102, 'exact_sd_engine': 20, 'rename': 7, 'split': 14, 'merge': 1, 'partial': 4, 'witnessed_other': 4, 'platform_analog': 4, 'no_witness': 40, 'invented': 6}, total ours=204

| our TU | category | original TU | note |
|---|---|---|---|
| engine/audio/AESound.cpp | witnessed(gof3d) | gof3d Classes/AESound.mm | no gof2 witness |
| engine/audio/AESoundRessource.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/AESoundRessource.cpp |  |
| engine/audio/FModSound.cpp | rename(.mm) | Classes/Game/FModSound.mm | game-side TU originally |
| engine/core/AERandom.cpp | rename | Svn_iPhone_AE2/Classes/Random.cpp | AERandom class == engine Random.cpp |
| engine/core/AbyssEngine.cpp | invented-merge | Image.cpp+Image2D.cpp+ImageFont.cpp+SpriteSystem.cpp+Mesh.cpp+Camera.cpp+Material.cpp+... (engine free-function TUs) | 2958-line kitchen sink; holds ImageCreateFromFile/Image2DRelease/ImageFontDrawString/SpriteSystemDraw/MaterialDraw etc. that each had their own original TU |
| engine/core/ApplicationManager.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/ApplicationManager.cpp |  |
| engine/core/Array.cpp | invented | (Array.h header-only in all dumps) | template impl+instantiations |
| engine/core/ArrayInstantiations.cpp | invented | (none) | explicit template instantiations; orig emitted per-TU weak copies |
| engine/core/EngineFlags.cpp | invented | (none) |  |
| engine/core/GameData.cpp | split | Classes/Game/GameInit.cpp | GameData.h header-inline, emitted in GameInit.cpp; GameInit also had OnCreateApplication/OnDestroyApplication/loadingScreen/cheatFunction (ours: JniBridge.cpp; cheatFunction undefined) |
| engine/core/GameText.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/GameText.cpp |  |
| engine/core/IApplicationModule.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/IApplicationModule.cpp |  |
| engine/core/JNIEnvCalls.cpp | invented(android) | (none) |  |
| engine/core/KeyCode.cpp | invented | (none) |  |
| engine/core/KeyFrame.cpp | split | Svn_iPhone_AE2/Classes/Transform.cpp | KeyFrame defined in Transform.cpp |
| engine/core/NFC.cpp | no-witness(2.0.x) | (none) | Android-era feature |
| engine/core/Node.cpp | split+moved | Classes/Game/SystemPathFinder.cpp | Node was a local helper class of SystemPathFinder.cpp |
| engine/file/AEFile.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/AEFile.cpp |  |
| engine/file/AENormalFile.cpp | split | Svn_iPhone_AE2/Classes/AEFile.cpp | AEFile.cpp = AEFile+AEPakFile+AENormalFile+AELowLevelFile |
| engine/file/AEPakFile.cpp | split | Svn_iPhone_AE2/Classes/AEFile.cpp | same |
| engine/file/BlowfishSubkeyFile.cpp | no-witness | (none) | blowfish C code, likely separate .c originally |
| engine/file/ConfigReader.cpp | rename(case) | Svn_iPhone_AE2/Classes/configreader.cpp | also orig defines TokenStruct here |
| engine/file/FileInterfaceAndroid.cpp | platform-analog | Svn_iPhone_AE2/Classes/FileInterfaceIPhone.mm | Android replacement |
| engine/file/FileRead.cpp | exact(HD) | Classes/Game/FileRead.cpp | folder drift: orig game-side, ours engine/ |
| engine/input/VirtualInput.cpp | no-witness(2.0.x) | (none) | Android/desktop input layer |
| engine/math/AEMath.cpp | rename+merge | Svn_iPhone_AE2/Classes/Math16_iPhone.cpp (+parts of Vector.cpp/Matrix.cpp) | AEMath::Sqrtf/Sinf/... = Math16; also holds Vector/Matrix operator defs that were in engine Vector.cpp/Matrix.cpp |
| engine/math/BSphere.cpp | split | Svn_iPhone_AE2/Classes/Mesh.cpp | AEMath::BSphere sym emitted from engine Mesh.cpp |
| engine/math/BoundingAAB.cpp | exact(HD) | Classes/Game/BoundingAAB.cpp | folder drift: orig game-side, ours engine/ |
| engine/math/BoundingSphere.cpp | exact(HD) | Classes/Game/BoundingSphere.cpp | folder drift: orig game-side, ours engine/ |
| engine/math/BoundingVolume.cpp | exact(HD) | Classes/Game/BoundingVolume.cpp | folder drift: orig game-side, ours engine/ |
| engine/math/EaseInOut.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/EaseInOut.cpp |  |
| engine/math/EaseInOutMatrix.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/EaseInOutMatrix.cpp |  |
| engine/math/Matrix.cpp | partial | Svn_iPhone_AE2/Classes/Matrix.cpp | shares Matrix free fns with our AEMath.cpp |
| engine/math/Quaternion.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Quaternion.cpp |  |
| engine/math/Transform.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Transform.cpp |  |
| engine/math/Vector.cpp | partial | Svn_iPhone_AE2/Classes/Vector.cpp | shares Vector free fns with our AEMath.cpp |
| engine/render/AEGeometry.cpp | exact(HD) | Classes/Game/AEGeometry.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/AMeshMerger.cpp | exact(HD) | Classes/Game/MeshMerger/AMeshMerger.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/BuildResourceList.cpp | rename+split | Classes/Game/GameResource.cpp | orig GameResource.cpp = BuildResourceList + loadPortraits + loadLowTexturesAndMaterials + ResourceTexture ctor |
| engine/render/Camera.cpp | partial | Svn_iPhone_AE2/Classes/Camera.cpp | orig TU = 2 free fns only; Camera class code lived in PaintCanvas.cpp |
| engine/render/Engine.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Engine.cpp |  |
| engine/render/FBOContainer.cpp | witnessed(windows) | Classes\FBOContainer.cpp |  |
| engine/render/GLES1Compat.cpp | invented | (none) | GLES1 stub shims |
| engine/render/IParticleSystem.cpp | exact(HD) | Classes/Game/ParticleSystems/IParticleSystem.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/ImageFactory.cpp | exact(HD) | Classes/Game/ImageFactory.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/ImagePart.cpp | exact(HD) | Classes/Game/ImagePart.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/LODManager.cpp | exact(HD) | Classes/Game/LODManager.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/LensFlare.cpp | exact(HD) | Classes/Game/LensFlare.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/LodMeshMerger.cpp | exact(HD) | Classes/Game/MeshMerger/LodMeshMerger.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/LowResourceTable.cpp | rename+split | Classes/Game/GameResource.cpp | loadLowTexturesAndMaterials |
| engine/render/MarqueeImage.cpp | exact(HD) | Classes/Game/MarqueeImage.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/Material.cpp | partial | Svn_iPhone_AE2/Classes/Material.cpp | orig TU = MaterialDraw + Array helpers; Material class ctor in PaintCanvas.cpp |
| engine/render/Mesh.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Mesh.cpp |  |
| engine/render/MeshMerger.cpp | witnessed(mac/MeeGo) | GalaxyOnFire2_merged Classes/Game/MeshMerger.cpp | post-1.2 refactor of AMeshMerger |
| engine/render/MovingStars.cpp | exact(HD) | Classes/Game/MovingStars.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/PaintCanvas.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/PaintCanvas.cpp |  |
| engine/render/ParticleSettings.cpp | exact(HD) | Classes/Game/ParticleSystems/ParticleSettings.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/ParticleSettingsRef.cpp | split | Classes/Game/ParticleSystems/ParticleSettings.cpp | ParticleSettingsRef + statics lived in ParticleSettings.cpp |
| engine/render/ParticleSystemManager.cpp | exact(HD) | Classes/Game/ParticleSystems/ParticleSystemManager.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/ParticleSystemMesh.cpp | exact(HD) | Classes/Game/ParticleSystems/ParticleSystemMesh.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/ParticleSystemSprite.cpp | exact(HD) | Classes/Game/ParticleSystems/ParticleSystemSprite.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/ResourceMaterial.cpp | split | Svn_iPhone_AE2/Classes/PaintCanvas.cpp (class) / Classes/Game/GameResource.cpp (loadPortraits) | no ResourceMaterial witness; holds loadPortraits from GameResource.cpp |
| engine/render/ResourceMesh.cpp | split | Svn_iPhone_AE2/Classes/PaintCanvas.cpp | Resource* classes lived in PaintCanvas.cpp kitchen-sink TU |
| engine/render/ResourceTexture.cpp | split | Svn_iPhone_AE2/Classes/PaintCanvas.cpp | + HD GameResource.cpp emitted ResourceTexture ctor |
| engine/render/ResourceTransform.cpp | split | Svn_iPhone_AE2/Classes/PaintCanvas.cpp |  |
| engine/render/ShaderBaseStruct.cpp | split | Svn_iPhone_AE2/Classes/Engine.cpp | ShaderBaseStruct defined in Engine.cpp |
| engine/render/SimpleMeshMerger.cpp | witnessed(MeeGo) | src/Classes/Game/MeshMerger/SimpleMeshMerger.cpp |  |
| engine/render/Sparks.cpp | exact(HD) | Classes/Game/Sparks.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/Sprite.cpp | exact(HD) | Classes/Game/Sprite.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/Trail.cpp | exact(HD) | Classes/Game/Trail.cpp | folder drift: orig game-side, ours engine/ |
| engine/render/shaders/BloomShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/BlurShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/BumpMapping.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/BumpRimCubeShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/BumpShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/ColorMixAdd.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/CubeMapping.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/CubeNormalMapping.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/DNSShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/DrawFBOShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/EnergyShield.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/GenericShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/GenericShader1.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/GenericShader2.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/GlowPPShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/GlowShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/GreenShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/MaskShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/NoTexShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/NoTexShader.cpp |  |
| engine/render/shaders/NoTexVtxColorShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/NoTexVtxColorShader.cpp |  |
| engine/render/shaders/PostBWShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/PulseShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/SandboxShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/SimpleRefractionShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/SimpleShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/SimpleShader.cpp |  |
| engine/render/shaders/SpecCubeAlphaMapping.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/SpecCubeMapping.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/TexOnlyShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/TextureAlphaTestShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/TextureAlphaTestShader.cpp |  |
| engine/render/shaders/TextureLightShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/TextureLightShader.cpp |  |
| engine/render/shaders/TextureShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/TextureShader.cpp |  |
| engine/render/shaders/TextureVtxColorShader.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/Shader/TextureVtxColorShader.cpp |  |
| engine/render/shaders/VertexColorAlphaTextureShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| engine/render/shaders/VertexColorShader.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/AutoPilotList.cpp | exact(HD) | Classes/Game/AutoPilotList.cpp |  |
| game/core/BumpRimCubeShader_new.cpp | no-witness | (none) |  |
| game/core/BumpShaderCloak.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/BumpShaderParticle.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/BumpShaderRefract.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/BumpShaderV2.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/BumpShaderV3.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/BumpShaderV4.cpp | no-witness(2.0.x shader) | (none) | GLES2 shader zoo, absent from all dumps |
| game/core/CheatCode.cpp | exact(SD engine) | Svn_iPhone_AE2/Classes/CheatCode.cpp |  |
| game/core/CheatHandler.cpp | split | Svn_iPhone_AE2/Classes/CheatCode.cpp | orig CheatCode.cpp = CheatCode + CheatHandler |
| game/core/CutScene.cpp | exact(HD) | Classes/Game/CutScene.cpp |  |
| game/core/Globals.cpp | exact(HD) | Classes/Game/Globals.cpp |  |
| game/core/HangarList.cpp | exact(HD) | Classes/Game/HangarList.cpp |  |
| game/core/Matrix.cpp | wrong-TU dup | Svn_iPhone_AE2/Classes/Matrix.cpp | AEMath::Matrix/Quaternion operators in game/core; orig one engine Matrix.cpp TU |
| game/core/Radio.cpp | exact(HD) | Classes/Game/Radio.cpp |  |
| game/core/RadioMessage.cpp | exact(HD) | Classes/Game/RadioMessage.cpp |  |
| game/core/String.cpp | rename+moved | Svn_iPhone_AE2/Classes/AEString.mm | AbyssEngine::String; engine TU, ours sits in game/core |
| game/core/TextureConference.cpp | no-witness(2.0.x) | (none) | shader-ish class in game/core |
| game/core/Vector.cpp | wrong-TU dup | Svn_iPhone_AE2/Classes/Vector.cpp | AEMath::Vector operators in game/core |
| game/menu/HackingGame.cpp | exact(HD) | Classes/Game/HackingGame.cpp |  |
| game/menu/MGame.cpp | exact(HD) | Classes/Game/MGame.cpp |  |
| game/menu/MTitle.cpp | exact(HD) | Classes/Game/MTitle.cpp |  |
| game/menu/MiningGame.cpp | exact(HD) | Classes/Game/MiningGame.cpp |  |
| game/menu/ModMainMenu.cpp | exact(HD) | Classes/Game/ModMainMenu.cpp |  |
| game/menu/ModStation.cpp | exact(HD) | Classes/Game/ModStation.cpp |  |
| game/menu/SpaceLounge.cpp | exact(HD) | Classes/Game/SpaceLounge.cpp |  |
| game/mission/Achievements.cpp | exact(HD) | Classes/Game/Achievements.cpp |  |
| game/mission/BluePrint.cpp | exact(HD) | Classes/Game/BluePrint.cpp |  |
| game/mission/Explosion.cpp | exact(HD) | Classes/Game/Explosion.cpp |  |
| game/mission/GameRecord.cpp | exact(HD) | Classes/Game/GameRecord.cpp |  |
| game/mission/Generator.cpp | exact(HD) | Classes/Game/Generator.cpp |  |
| game/mission/Item.cpp | exact(HD) | Classes/Game/Item.cpp |  |
| game/mission/Mission.cpp | exact(HD) | Classes/Game/Mission.cpp |  |
| game/mission/Objective.cpp | exact(HD) | Classes/Game/Objective.cpp |  |
| game/mission/PendingProduct.cpp | exact(HD) | Classes/Game/PendingProduct.cpp |  |
| game/mission/RecordHandler.cpp | exact(HD) | Classes/Game/RecordHandler.cpp |  |
| game/mission/Status.cpp | exact(HD) | Classes/Game/Status.cpp |  |
| game/ship/Agent.cpp | exact(HD) | Classes/Game/Agent.cpp |  |
| game/ship/KIPlayer.cpp | exact(HD) | Classes/Game/KIPlayer.cpp |  |
| game/ship/Player.cpp | exact(HD) | Classes/Game/Player.cpp |  |
| game/ship/PlayerAsteroid.cpp | exact(HD) | Classes/Game/PlayerAsteroid.cpp |  |
| game/ship/PlayerCreature.cpp | no-witness(2.0.x) | (none) | 2.0.x content |
| game/ship/PlayerEgo.cpp | exact(HD) | Classes/Game/PlayerEgo.cpp |  |
| game/ship/PlayerFighter.cpp | exact(HD) | Classes/Game/PlayerFighter.cpp |  |
| game/ship/PlayerFixedObject.cpp | exact(HD) | Classes/Game/PlayerFixedObject.cpp |  |
| game/ship/PlayerGasCloud.cpp | exact(HD) | Classes/Game/PlayerGasCloud.cpp |  |
| game/ship/PlayerJumpgate.cpp | exact(HD) | Classes/Game/PlayerJumpgate.cpp |  |
| game/ship/PlayerJunk.cpp | exact(HD) | Classes/Game/PlayerJunk.cpp |  |
| game/ship/PlayerStatic.cpp | exact(HD) | Classes/Game/PlayerStatic.cpp |  |
| game/ship/PlayerStaticFar.cpp | exact(HD) | Classes/Game/PlayerStaticFar.cpp |  |
| game/ship/PlayerStation.cpp | exact(HD) | Classes/Game/PlayerStation.cpp |  |
| game/ship/PlayerTurret.cpp | exact(HD) | Classes/Game/PlayerTurret.cpp |  |
| game/ship/PlayerWormHole.cpp | exact(HD) | Classes/Game/PlayerWormHole.cpp |  |
| game/ship/Ship.cpp | exact(HD) | Classes/Game/Ship.cpp |  |
| game/ship/TargetFollowCamera.cpp | exact(HD) | Classes/Game/TargetFollowCamera.cpp |  |
| game/ui/ChoiceWindow.cpp | exact(HD) | Classes/Game/ChoiceWindow.cpp |  |
| game/ui/DialogueWindow.cpp | exact(HD) | Classes/Game/DialogueWindow.cpp |  |
| game/ui/HangarWindow.cpp | exact(HD) | Classes/Game/HangarWindow.cpp |  |
| game/ui/Hud.cpp | exact(HD) | Classes/Game/Hud.cpp |  |
| game/ui/Layout.cpp | exact(HD) | Classes/Game/Layout.cpp |  |
| game/ui/ListItem.cpp | exact(HD) | Classes/Game/ListItem.cpp |  |
| game/ui/ListItemWindow.cpp | exact(HD) | Classes/Game/ListItemWindow.cpp |  |
| game/ui/MenuTouchWindow.cpp | exact(HD) | Classes/Game/MenuTouchWindow.cpp |  |
| game/ui/MissionsWindow.cpp | exact(HD) | Classes/Game/MissionsWindow.cpp |  |
| game/ui/ScrollTouchBox.cpp | exact(HD) | Classes/Game/ScrollTouchBox.cpp |  |
| game/ui/ScrollTouchWindow.cpp | exact(HD) | Classes/Game/ScrollTouchWindow.cpp |  |
| game/ui/StatusWindow.cpp | exact(HD) | Classes/Game/StatusWindow.cpp |  |
| game/ui/TouchButton.cpp | exact(HD) | Classes/Game/TouchButton.cpp |  |
| game/ui/TouchSlider.cpp | exact(HD) | Classes/Game/TouchSlider.cpp |  |
| game/ui/WantedWindow.cpp | exact(HD) | Classes/Game/WantedWindow.cpp |  |
| game/weapons/AbstractGun.cpp | split | Classes/Game/BeamGun.cpp | AbstractGun.h header-only; code emitted in BeamGun.cpp (no original AbstractGun.cpp ever) |
| game/weapons/BeamGun.cpp | exact(HD) | Classes/Game/BeamGun.cpp |  |
| game/weapons/BombGun.cpp | exact(HD) | Classes/Game/BombGun.cpp |  |
| game/weapons/Gun.cpp | exact(HD) | Classes/Game/Gun.cpp |  |
| game/weapons/MineGun.cpp | exact(HD) | Classes/Game/MineGun.cpp |  |
| game/weapons/ObjectGun.cpp | exact(HD) | Classes/Game/ObjectGun.cpp |  |
| game/weapons/Radar.cpp | exact(HD) | Classes/Game/Radar.cpp |  |
| game/weapons/RepairBeam.cpp | exact(HD) | Classes/Game/RepairBeam.cpp |  |
| game/weapons/RocketGun.cpp | exact(HD) | Classes/Game/RocketGun.cpp |  |
| game/weapons/SentryGun.cpp | exact(HD) | Classes/Game/SentryGun.cpp |  |
| game/weapons/SpriteGun.cpp | no-witness(2.0.x) | (none) | 2.0.x content |
| game/weapons/TractorBeam.cpp | exact(HD) | Classes/Game/TractorBeam.cpp |  |
| game/world/Galaxy.cpp | exact(HD) | Classes/Game/Galaxy.cpp |  |
| game/world/Level.cpp | exact(HD) | Classes/Game/Level.cpp |  |
| game/world/LevelScript.cpp | exact(HD) | Classes/Game/LevelScript.cpp |  |
| game/world/NewsItem.cpp | exact(HD) | Classes/Game/NewsItem.cpp |  |
| game/world/NewsTicker.cpp | exact(HD) | Classes/Game/NewsTicker.cpp |  |
| game/world/Route.cpp | exact(HD) | Classes/Game/Route.cpp |  |
| game/world/SolarSystem.cpp | exact(HD) | Classes/Game/SolarSystem.cpp |  |
| game/world/SpacePoint.cpp | exact(HD) | Classes/Game/SpacePoint.cpp |  |
| game/world/Standing.cpp | exact(HD) | Classes/Game/Standing.cpp |  |
| game/world/StarMap.cpp | exact(HD) | Classes/Game/StarMap.cpp |  |
| game/world/StarSystem.cpp | exact(HD) | Classes/Game/StarSystem.cpp |  |
| game/world/Station.cpp | exact(HD) | Classes/Game/Station.cpp |  |
| game/world/SystemPathFinder.cpp | exact(HD) | Classes/Game/SystemPathFinder.cpp |  |
| game/world/Wanted.cpp | exact(HD) | Classes/Game/Wanted.cpp |  |
| game/world/Waypoint.cpp | exact(HD) | Classes/Game/Waypoint.cpp |  |
| platform/android/ApkLoader.cpp | platform-analog | (none) | APK asset access |
| platform/android/JniBridge.cpp | platform-analog | Classes/AppController.mm + Classes/Game/GameInit.cpp | holds OnCreateApplication/loadingScreen from GameInit.cpp |
| platform/android/NdkHooks.cpp | platform-analog | (iOS glue) |  |

## Original TUs with NO our-side file (Android-relevant only)

- Engine (SD .a witnesses; code exists in Android binary — exported Image2DRelease/ImageFontDrawString/SpriteSystemDraw confirm — but our tree folds it into engine/core/AbyssEngine.cpp or leaves it unimplemented):
  - Svn_iPhone_AE2/Classes/Image.cpp (ImageCreate*/TextureCreateFromFile + Image/ImageFont/AELoadedTexture ctors)
  - Svn_iPhone_AE2/Classes/Image2D.cpp (Image2DRelease)
  - Svn_iPhone_AE2/Classes/ImageFont.cpp (ImageFontGetWidth/DrawString/...)
  - Svn_iPhone_AE2/Classes/SpriteSystem.cpp (SpriteSystemCreate/Draw/SetUv/...)
  - Svn_iPhone_AE2/Classes/Shader/BaseShader.cpp (no BaseShader anywhere in our src; possibly refactored away by 2.0.x)
- Game:
  - Classes/Game/GameInit.cpp (OnCreateApplication/OnDestroyApplication/loadingScreen/cheatFunction + GameData ctors; ours scatters these into JniBridge.cpp/GameData.cpp; cheatFunction has no definition in our tree)
  - Classes/Game/GameResource.cpp (renamed/split into BuildResourceList.cpp + LowResourceTable.cpp + ResourceMaterial.cpp:loadPortraits)
- Engine iOS-only, replaced on Android: AEString.mm(.mm form), Engine_iPhone.mm, FileInterfaceIPhone.mm, GameText_iPhone.mm, AlertView.mm, AccelerometerFilter.mm, FLStat.m/FLStat_cpp.mm (FLStat absent from Android dynsym), Math16_iPhone.cpp (ours AEMath.cpp)
- iOS app glue, N/A on Android: AppController.mm, AppView.mm, CloudHandler.mm, GamePadHandler.mm, Gof2SaveDocument.m, AECommunityTools*, Facebook/*, SBJson*, main.m, arclite
