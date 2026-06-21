// Array.cpp -- the single translation unit that materialises the engine's
// out-of-line Array<T> container helpers. The template definitions live in
// Array.h behind GOF2_ARRAY_INSTANTIATIONS (so ordinary TUs are unaffected);
// defining the macro here pulls them in, and the explicit instantiations below
// emit exactly the set of helper symbols the original library exports.
//
// Every helper is a value/pointer container operation -- value-typed element
// types (AEMath::Matrix / AEMath::Vector and the ParticleSet selector) need
// their full definition for sizeof / operator=, and the ArrayReleaseClasses
// instantiations that `delete` their pointees need the pointee's full type.
// Pure pointer element types only need a declaration, so they are forward
// declared to keep this TU free of the rest of the engine.

#define GOF2_ARRAY_INSTANTIATIONS
#include "engine/core/Array.h"

#include "mathtypes.h"                       // AbyssEngine::AEMath::Vector / Matrix (value types)
#include "AEString.h"                        // AbyssEngine::String (deleted by ReleaseClasses)
#include "engine/core/KeyFrame.h"            // AbyssEngine::KeyFrame (deleted by ReleaseClasses)
#include "engine/render/ShaderBaseStruct.h"  // AbyssEngine::ShaderBaseStruct (deleted by ReleaseClasses)
#include "engine/render/ParticleSettings.h"  // ParticleSettings::ParticleSet (value selector)

namespace AbyssEngine {
// Pointer-only element types: the container helpers never dereference these, so
// a forward declaration is sufficient and avoids dragging in their headers.
class Mesh;
class Transform;
class IApplicationModule;
class SpriteSystem;
class AESoundInterface;
class Camera;
class Image2D;
class KeyCode;
class Material;
class CheatCode;
class ImageFont;
class TokenStruct;
class AELoadedTexture;
class Resource;
} // namespace AbyssEngine

// Global (non-namespaced) pointer-only element types.
class GameRecord;
class AbstractGun;
class TouchSlider;
class RadioMessage;
class AEGeometry;
class RepairBeam;
class SpacePoint;
class SolarSystem;
class ImagePart;
class TouchButton;
class AELowLevelFile;
class BoundingVolume;
class PendingProduct;
class Gun;
class Item;
class Node;
class Ship;
class Agent;
class Player;
class Wanted;
class Mission;
class Station;
class KIPlayer;
class ListItem;
class NewsItem;
class Waypoint;
class BluePrint;
class Explosion;

namespace ae = AbyssEngine;

// --- ArrayRemove<T> -------------------------------------------------------
template void ArrayRemove<ae::Mesh*>(ae::Mesh*, Array<ae::Mesh*>&);
template void ArrayRemove<ae::String*>(ae::String*, Array<ae::String*>&);
template void ArrayRemove<ae::Transform*>(ae::Transform*, Array<ae::Transform*>&);

// --- ArrayRelease<T> ------------------------------------------------------
template void ArrayRelease<unsigned int>(Array<unsigned int>&);
template void ArrayRelease<ae::AEMath::Matrix>(Array<ae::AEMath::Matrix>&);
template void ArrayRelease<ae::AEMath::Vector>(Array<ae::AEMath::Vector>&);
template void ArrayRelease<ae::IApplicationModule*>(Array<ae::IApplicationModule*>&);
template void ArrayRelease<ae::Mesh*>(Array<ae::Mesh*>&);

// --- ArrayAddCached<T> ----------------------------------------------------
template void ArrayAddCached<unsigned int>(unsigned int, Array<unsigned int>&);
template void ArrayAddCached<ae::AEMath::Matrix>(ae::AEMath::Matrix, Array<ae::AEMath::Matrix>&);
template void ArrayAddCached<ae::Mesh*>(ae::Mesh*, Array<ae::Mesh*>&);

// --- ArrayRemoveAll<T> ----------------------------------------------------
template void ArrayRemoveAll<ae::AEMath::Matrix>(Array<ae::AEMath::Matrix>&);
template void ArrayRemoveAll<ae::SpriteSystem*>(Array<ae::SpriteSystem*>&);
template void ArrayRemoveAll<ae::AESoundInterface*>(Array<ae::AESoundInterface*>&);
template void ArrayRemoveAll<ae::Mesh*>(Array<ae::Mesh*>&);
template void ArrayRemoveAll<ae::Camera*>(Array<ae::Camera*>&);
template void ArrayRemoveAll<ae::String*>(Array<ae::String*>&);
template void ArrayRemoveAll<ae::Image2D*>(Array<ae::Image2D*>&);
template void ArrayRemoveAll<ae::KeyCode*>(Array<ae::KeyCode*>&);
template void ArrayRemoveAll<ae::Material*>(Array<ae::Material*>&);
template void ArrayRemoveAll<ae::CheatCode*>(Array<ae::CheatCode*>&);
template void ArrayRemoveAll<ae::ImageFont*>(Array<ae::ImageFont*>&);
template void ArrayRemoveAll<ae::Transform*>(Array<ae::Transform*>&);

// --- ArraySetLength<T> ----------------------------------------------------
template void ArraySetLength<ae::AEMath::Matrix>(unsigned int, Array<ae::AEMath::Matrix>&);
template void ArraySetLength<ae::AEMath::Vector>(unsigned int, Array<ae::AEMath::Vector>&);
template void ArraySetLength<Array<ae::AEMath::Vector*>*>(unsigned int, Array<Array<ae::AEMath::Vector*>*>&);
template void ArraySetLength<Array<ae::String*>*>(unsigned int, Array<Array<ae::String*>*>&);
template void ArraySetLength<ae::AESoundInterface*>(unsigned int, Array<ae::AESoundInterface*>&);
template void ArraySetLength<ae::Mesh*>(unsigned int, Array<ae::Mesh*>&);
template void ArraySetLength<ae::String*>(unsigned int, Array<ae::String*>&);

// --- ArrayReleaseClasses<T> -----------------------------------------------
template void ArrayReleaseClasses<ae::AEMath::Vector*>(Array<ae::AEMath::Vector*>&);
template void ArrayReleaseClasses<Array<ae::AEMath::Vector*>*>(Array<Array<ae::AEMath::Vector*>*>&);
template void ArrayReleaseClasses<Array<ae::String*>*>(Array<Array<ae::String*>*>&);
template void ArrayReleaseClasses<ae::ShaderBaseStruct*>(Array<ae::ShaderBaseStruct*>&);
template void ArrayReleaseClasses<ae::String*>(Array<ae::String*>&);
template void ArrayReleaseClasses<ae::KeyFrame*>(Array<ae::KeyFrame*>&);

// --- ArrayAdd<T> ----------------------------------------------------------
template void ArrayAdd<long long>(long long, Array<long long>&);
template void ArrayAdd<ae::AEMath::Matrix>(ae::AEMath::Matrix, Array<ae::AEMath::Matrix>&);
template void ArrayAdd<ParticleSettings::ParticleSet>(ParticleSettings::ParticleSet, Array<ParticleSettings::ParticleSet>&);
template void ArrayAdd<ae::TokenStruct*>(ae::TokenStruct*, Array<ae::TokenStruct*>&);
template void ArrayAdd<ae::AELoadedTexture*>(ae::AELoadedTexture*, Array<ae::AELoadedTexture*>&);
template void ArrayAdd<ae::ShaderBaseStruct*>(ae::ShaderBaseStruct*, Array<ae::ShaderBaseStruct*>&);
template void ArrayAdd<ae::IApplicationModule*>(ae::IApplicationModule*, Array<ae::IApplicationModule*>&);
template void ArrayAdd<ae::Camera*>(ae::Camera*, Array<ae::Camera*>&);
template void ArrayAdd<ae::String*>(ae::String*, Array<ae::String*>&);
template void ArrayAdd<ae::Image2D*>(ae::Image2D*, Array<ae::Image2D*>&);
template void ArrayAdd<ae::KeyCode*>(ae::KeyCode*, Array<ae::KeyCode*>&);
template void ArrayAdd<ae::KeyFrame*>(ae::KeyFrame*, Array<ae::KeyFrame*>&);
template void ArrayAdd<ae::Material*>(ae::Material*, Array<ae::Material*>&);

template void ArrayAdd<ae::Resource*>(ae::Resource*, Array<ae::Resource*>&);

// --- ArrayAdd<T>(const T*, count, Array<T>&) ------------------------------
// Bulk-append (block copy) overload.
template void ArrayAdd<ae::Resource*>(ae::Resource* const*, unsigned int, Array<ae::Resource*>&);

// --- ArraySet<T> ----------------------------------------------------------
template void ArraySet<ParticleSettings::ParticleSet>(const ParticleSettings::ParticleSet*, unsigned int, Array<ParticleSettings::ParticleSet>&);
template void ArraySet<ParticleSettings::ParticleSet>(const Array<ParticleSettings::ParticleSet>&, Array<ParticleSettings::ParticleSet>&);
template void ArraySet<ae::KeyFrame*>(ae::KeyFrame* const*, unsigned int, Array<ae::KeyFrame*>&);
template void ArraySet<ae::KeyFrame*>(const Array<ae::KeyFrame*>&, Array<ae::KeyFrame*>&);

// --- Array<T>::Array() / ~Array() (ctor & dtor) ---------------------------

// --- ArrayAdd<T> (single-item) for ImageFont ------------------------------
template void ArrayAdd<ae::ImageFont*>(ae::ImageFont*, Array<ae::ImageFont*>&);

// --- Array<T> member methods ----------------------------------------------

// Game-object pointer containers.
