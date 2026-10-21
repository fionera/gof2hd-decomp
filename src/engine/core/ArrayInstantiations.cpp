

#include "engine/core/Array.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "engine/core/Node.h"
#include "game/ship/Agent.h"
#include "game/mission/PendingProduct.h"
#include "game/mission/BluePrint.h"
#include "engine/file/AELowLevelFile.h"
#include "game/ui/TouchButton.h"
#include "game/ui/TouchSlider.h"
#include "engine/render/ImagePart.h"
#include "game/ui/ListItem.h"
#include "game/ship/Player.h"
#include "engine/render/AEGeometry.h"
#include "game/weapons/AbstractGun.h"
#include "engine/math/Vector.h"
#include "engine/core/KeyFrame.h"
#include "engine/render/ShaderBaseStruct.h"
#include "engine/core/AEString.h"
#include "game/weapons/Gun.h"
#include "engine/math/BoundingVolume.h"
#include "game/mission/Explosion.h"
#include "game/mission/Item.h"
#include "game/ship/KIPlayer.h"
#include "game/world/NewsItem.h"
#include "game/mission/Objective.h"
#include "engine/render/ParticleSystemMesh.h"
#include "engine/render/ParticleSystemSprite.h"
#include "game/core/RadioMessage.h"
#include "game/weapons/RepairBeam.h"
#include "game/ship/Ship.h"
#include "game/world/SolarSystem.h"
#include "game/world/SpacePoint.h"
#include "game/world/Station.h"
#include "game/world/Wanted.h"
#include "game/world/Waypoint.h"

template void ArrayReleaseClasses<AbstractGun*>(Array<AbstractGun*> &);

template void ArrayReleaseClasses<Agent*>(Array<Agent*> &);

template void ArrayReleaseClasses<BoundingVolume*>(Array<BoundingVolume*> &);

template void ArrayReleaseClasses<ImagePart*>(Array<ImagePart*> &);

template void ArrayReleaseClasses<Item*>(Array<Item*> &);

template void ArrayReleaseClasses<ListItem*>(Array<ListItem*> &);

template void ArrayReleaseClasses<NewsItem*>(Array<NewsItem*> &);

template void ArrayReleaseClasses<ParticleSystemMesh*>(Array<ParticleSystemMesh*> &);

template void ArrayReleaseClasses<ParticleSystemSprite*>(Array<ParticleSystemSprite*> &);

template void ArrayReleaseClasses<RadioMessage*>(Array<RadioMessage*> &);

template void ArrayReleaseClasses<RepairBeam*>(Array<RepairBeam*> &);

template void ArrayReleaseClasses<Ship*>(Array<Ship*> &);

template void ArrayReleaseClasses<SolarSystem*>(Array<SolarSystem*> &);

template void ArrayReleaseClasses<SpacePoint*>(Array<SpacePoint*> &);

template void ArrayReleaseClasses<Station*>(Array<Station*> &);

template void ArrayReleaseClasses<TouchButton*>(Array<TouchButton*> &);

template void ArrayReleaseClasses<TouchSlider*>(Array<TouchSlider*> &);

template void ArrayReleaseClasses<Wanted*>(Array<Wanted*> &);

template void ArrayReleaseClasses<Waypoint*>(Array<Waypoint*> &);


template void ArrayReleaseArrays<signed char *>(Array<signed char *> &);

template void ArrayRemove<Node *>(Node *, Array<Node *> &);

template void ArrayRemove<Agent *>(Agent *, Array<Agent *> &);

template void ArrayRelease<Node *>(Array<Node *> &);

template void ArrayRelease<bool>(Array<bool> &);

template void ArrayRelease<int>(Array<int> &);

// emitted by PaintCanvas in the original (RemoveAll* helpers); kept exported here until that body lands
template void ArrayRemoveAll<unsigned int>(Array<unsigned int> &);

template void ArraySetLength<PendingProduct *>(unsigned int, Array<PendingProduct *> &);

template void ArraySetLength<BluePrint *>(unsigned int, Array<BluePrint *> &);

template void ArraySetLength<signed char>(unsigned int, Array<signed char> &);

template void ArraySetLength<Array<ImagePart *> *>(unsigned int, Array<Array<ImagePart *> *> &);

template void ArrayReleaseClasses<AELowLevelFile *>(Array<AELowLevelFile *> &);

template void ArrayReleaseClasses<PendingProduct *>(Array<PendingProduct *> &);


template void ArrayReleaseClasses<BluePrint *>(Array<BluePrint *> &);

template void ArrayReleaseClasses<Array<ListItem *> *>(Array<Array<ListItem *> *> &);

template void ArrayReleaseClasses<Array<ImagePart *> *>(Array<Array<ImagePart *> *> &);

template void ArrayAdd<TouchButton *>(TouchButton *, Array<TouchButton *> &);

template void ArrayAdd<TouchSlider *>(TouchSlider *, Array<TouchSlider *> &);

template void ArrayAdd<bool>(bool, Array<bool> &);

template void ArrayAdd<float>(float, Array<float> &);

template void ArrayAdd<unsigned char>(const unsigned char *, unsigned int, Array<unsigned char> &);

template void ArrayAdd<Player *>(Player * const *, unsigned int, Array<Player *> &);

template void ArrayAdd<Player *>(const Array<Player *> &, Array<Player *> &);

class GameRecord;

namespace AbyssEngine {
    class KeyCode;
    class String;
}

template Array<GameRecord *>::~Array();

template Array<TouchSlider *>::Array();

template Array<TouchSlider *>::~Array();

template Array<PendingProduct *>::~Array();

template Array<BluePrint *>::Array();

template Array<BluePrint *>::~Array();

template Array<AbyssEngine::KeyCode *>::Array();

template Array<AbyssEngine::KeyCode *>::~Array();

template Array<Array<ImagePart *> *>::Array();

template Array<Array<AbyssEngine::String *> *>::Array();

template Array<Array<AbyssEngine::String *> *>::~Array();

template Array<bool>::~Array();