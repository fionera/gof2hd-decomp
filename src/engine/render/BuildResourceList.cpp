

#include "engine/render/ResourceTexture.h"
#include "engine/render/ResourceMaterial.h"
#include "engine/render/ResourceMesh.h"
#ifndef GOF2_ENUM_BlendMode
#define GOF2_ENUM_BlendMode
#endif
#include "engine/render/Engine.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/PaintCanvas.h"

namespace AbyssEngine {
    struct Resource {
        unsigned short id;
        int kind;
        int unused;
        void *payload;
    };

    struct ResourceImage {
        unsigned short lo;
        unsigned short hi;
    };
}

void loadPortraits(AbyssEngine::Engine * engine);
void loadLowTexturesAndMaterials(AbyssEngine::Engine * engine);

namespace {
    inline AbyssEngine::Resource *makeRes(unsigned short id, int kind, void *payload) {
        AbyssEngine::Resource *r = new AbyssEngine::Resource;
        r->id = id;
        r->kind = kind;
        r->unused = -1;
        r->payload = payload;
        return r;
    }

    inline AbyssEngine::ResourceImage *newImage(unsigned short lo, unsigned short hi) {
        AbyssEngine::ResourceImage *p = new AbyssEngine::ResourceImage;
        p->lo = lo;
        p->hi = hi;
        return p;
    }
}

void BuildResourceList(AbyssEngine::Engine *engine) {
    using namespace AbyssEngine;
    PaintCanvas *canvas = engine->appManager->paintCanvas;
    canvas->TextureCreateGlobal(String("data/assets/main/3d/textures/low/etc/fx/cloak_map.aei"), 6);

    static Resource *resources[2488] = {};
    resources[0] = makeRes(
        11742, 2, new AbyssEngine::ResourceTexture("data/assets/supernova/3d/textures/low/etc/suns/sn_sun_011.aei",
                                                   0.0f));
    resources[1] = makeRes(27335, 2, new AbyssEngine::ResourceTexture(
                               "data/assets/supernova/3d/textures/low/etc/misc/sn_fireworks_rocket_diffuse.aei", 0.0f));
    resources[2] = makeRes(27336, 2, new AbyssEngine::ResourceTexture(
                               "data/assets/supernova/3d/textures/low/etc/misc/sn_fireworks_rocket_normal_specular.aei",
                               0.0f));
    resources[3] = makeRes(27337, 6, new AbyssEngine::ResourceMaterial(27335, 27336, static_cast<BlendMode>(28)));
    resources[4] = makeRes(
        27338, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_fireworks_rocket.aem", 27337,
                                                false));
    resources[5] = makeRes(
        27309, 2, new AbyssEngine::ResourceTexture("data/assets/supernova/3d/textures/low/etc/fx/sn_fireworks.aei",
                                                   0.0f));
    resources[6] = makeRes(27308, 6, new AbyssEngine::ResourceMaterial(27309, BlendMode_2));
    resources[7] = makeRes(
        16809, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_fireworks_lookat_anim_add.aem",
                                                27308, false));
    resources[8] = makeRes(27320, 2, new AbyssEngine::ResourceTexture(
                               "data/assets/supernova/3d/textures/low/etc/fx/sn_sprite_fireworks_rocket_sparks.aei",
                               0.0f));
    resources[9] = makeRes(27321, 6, new AbyssEngine::ResourceMaterial(27320, BlendMode_2));
    resources[10] = makeRes(
        29100, 2, new AbyssEngine::ResourceTexture("data/assets/supernova/3d/textures/low/etc/fx/sn_projectiles.aei",
                                                   0.0f));
    resources[11] = makeRes(29101, 6, new AbyssEngine::ResourceMaterial(29100, BlendMode_2));
    resources[12] = makeRes(29102, 6, new AbyssEngine::ResourceMaterial(29100, BlendMode_1));
    resources[13] = makeRes(
        19090, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_projectile_228_anim_add.aem",
                                                29101, false));
    resources[14] = makeRes(
        19091, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_projectile_229_anim_alpha.aem",
                                                29102, false));
    resources[15] = makeRes(
        19094, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_projectile_231_anim_add.aem",
                                                29101, false));
    resources[16] = makeRes(
        19092, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_projectile_207_anim_add.aem",
                                                29101, false));
    resources[17] = makeRes(
        19093, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_projectile_222_anim_add.aem",
                                                29101, false));
    resources[18] = makeRes(
        29001, 2, new AbyssEngine::ResourceTexture("data/assets/supernova/3d/textures/low/etc/fx/sn_plasma_stream.aei",
                                                   0.0f));
    resources[19] = makeRes(
        29002, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_plasma_stream_normal.aei", 0.0f));
    resources[20] = makeRes(29003, 6, new AbyssEngine::ResourceMaterial(29001, static_cast<BlendMode>(3)));
    resources[21] = makeRes(
        19071, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_plasma_stream_anim_add.aem",
                                                29003, false));
    resources[22] = makeRes(
        29004, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_plasma_explosion.aei", 0.0f));
    resources[23] = makeRes(29005, 6, new AbyssEngine::ResourceMaterial(29004, static_cast<BlendMode>(3)));
    resources[24] = makeRes(19070, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/fx/sn_plasma_explosion_anim_lookat_add.aem", 29005,
                                false));
    resources[25] = makeRes(
        29006, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_shock_blast_sphere.aei", 0.0f));
    resources[26] = makeRes(
        29008, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_shock_blast_glow.aei", 0.0f));
    resources[27] = makeRes(29007, 6, new AbyssEngine::ResourceMaterial(29006, static_cast<BlendMode>(3)));
    resources[28] = makeRes(29009, 6, new AbyssEngine::ResourceMaterial(29008, static_cast<BlendMode>(3)));
    resources[29] = makeRes(
        18995, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_shock_blast_sphere_anim_add.aem",
                                                29007, false));
    resources[30] = makeRes(18996, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/fx/sn_shock_blast_glow_anim_lookat_add.aem", 29009,
                                false));
    resources[31] = makeRes(
        29046, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/misc/sn_sentry_gun_001_diffuse.aei", 0.0f));
    resources[32] = makeRes(29047, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/misc/sn_sentry_gun_001_normal_specular.aei",
                                0.0f));
    resources[33] = makeRes(29048, 6, new AbyssEngine::ResourceMaterial(29046, 29047, static_cast<BlendMode>(28)));
    resources[34] = makeRes(29049, 6, new AbyssEngine::ResourceMaterial(29046, BlendMode_2));
    resources[35] = makeRes(
        18880, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_001.aem", 29048,
                                                false));
    resources[36] = makeRes(
        18883, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_001_anim.aem",
                                                29048, false));
    resources[37] = makeRes(
        18886, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_001_add.aem", 29049,
                                                false));
    resources[38] = makeRes(
        18889, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_001_anim_add.aem",
                                                29049, false));
    resources[39] = makeRes(
        29050, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/misc/sn_sentry_gun_002_diffuse.aei", 0.0f));
    resources[40] = makeRes(29051, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/misc/sn_sentry_gun_002_normal_specular.aei",
                                0.0f));
    resources[41] = makeRes(29052, 6, new AbyssEngine::ResourceMaterial(29050, 29051, static_cast<BlendMode>(28)));
    resources[42] = makeRes(29053, 6, new AbyssEngine::ResourceMaterial(29050, BlendMode_2));
    resources[43] = makeRes(
        18881, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_002.aem", 29052,
                                                false));
    resources[44] = makeRes(
        18884, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_002_anim.aem",
                                                29052, false));
    resources[45] = makeRes(
        18887, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_002_add.aem", 29053,
                                                false));
    resources[46] = makeRes(
        18890, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_002_anim_add.aem",
                                                29053, false));
    resources[47] = makeRes(
        29054, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/misc/sn_sentry_gun_003_diffuse.aei", 0.0f));
    resources[48] = makeRes(29055, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/misc/sn_sentry_gun_003_normal_specular.aei",
                                0.0f));
    resources[49] = makeRes(29056, 6, new AbyssEngine::ResourceMaterial(29054, 29055, static_cast<BlendMode>(28)));
    resources[50] = makeRes(29057, 6, new AbyssEngine::ResourceMaterial(29054, BlendMode_2));
    resources[51] = makeRes(
        18882, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_003.aem", 29056,
                                                false));
    resources[52] = makeRes(
        18885, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_003_anim.aem",
                                                29056, false));
    resources[53] = makeRes(
        18888, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_003_add.aem", 29057,
                                                false));
    resources[54] = makeRes(
        18891, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_sentry_gun_003_anim_add.aem",
                                                29057, false));
    resources[55] = makeRes(18847, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/skyboxes/sn_skybox_planet_ring_alpha.aem", 65535,
                                false));
    resources[56] = makeRes(
        29018, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/skyboxes/sn_skybox_planet_ring.aei", 0.0f));
    resources[57] = makeRes(
        18848, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/skyboxes/sn_skybox_storms_anim_add.aem", 65535, false));
    resources[58] = makeRes(
        29019, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/skyboxes/sn_skybox_storms.aei", 0.0f));
    resources[59] = makeRes(11700, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/stations/sn_station_mining_plant_part1_diffuse.aei",
                                0.0f));
    resources[60] = makeRes(11702, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/stations/sn_station_mining_plant_part2_diffuse.aei",
                                0.0f));
    resources[61] = makeRes(11701, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/stations/sn_station_mining_plant_part1_normal_specular.aei",
                                0.0f));
    resources[62] = makeRes(11703, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/stations/sn_station_mining_plant_part2_normal_specular.aei",
                                0.0f));
    resources[63] = makeRes(11704, 6, new AbyssEngine::ResourceMaterial(11700, 11701, static_cast<BlendMode>(28)));
    resources[64] = makeRes(11708, 6, new AbyssEngine::ResourceMaterial(11702, 11703, static_cast<BlendMode>(28)));
    resources[65] = makeRes(11705, 6, new AbyssEngine::ResourceMaterial(11700, BlendMode_2));
    resources[66] = makeRes(11706, 6, new AbyssEngine::ResourceMaterial(11700, BlendMode_1));
    resources[67] = makeRes(11707, 6, new AbyssEngine::ResourceMaterial(11700, BlendMode_dummy));
    resources[68] = makeRes(11709, 6, new AbyssEngine::ResourceMaterial(11702, BlendMode_2));
    resources[69] = makeRes(11710, 6, new AbyssEngine::ResourceMaterial(11702, BlendMode_1));
    resources[70] = makeRes(11711, 6, new AbyssEngine::ResourceMaterial(11702, BlendMode_dummy));
    resources[71] = makeRes(19080, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part1.aem", 11704,
                                false));
    resources[72] = makeRes(19081, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part1_add.aem", 11705,
                                false));
    resources[73] = makeRes(19084, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part1_alpha.aem",
                                11706, false));
    resources[74] = makeRes(19085, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part1_emissive.aem",
                                11707, false));
    resources[75] = makeRes(19082, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part1_glow_anim_add.aem",
                                11705, false));
    resources[76] = makeRes(19083, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part1_fire_anim_add.aem",
                                11705, false));
    resources[77] = makeRes(19086, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part2.aem", 11708,
                                false));
    resources[78] = makeRes(19087, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part2_add.aem", 11709,
                                false));
    resources[79] = makeRes(19088, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/stations/sn_station_mining_plant_part2_emissive.aem",
                                11711, false));
    resources[80] = makeRes(11712, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/misc/sn_plasma_array_midorian_diffuse.aei",
                                0.0f));
    resources[81] = makeRes(11713, 2, new AbyssEngine::ResourceTexture(
                                "data/assets/supernova/3d/textures/low/etc/misc/sn_plasma_array_midorian_normal_specular.aei",
                                0.0f));
    resources[82] = makeRes(11714, 6, new AbyssEngine::ResourceMaterial(11712, 11713, static_cast<BlendMode>(28)));
    resources[83] = makeRes(11715, 6, new AbyssEngine::ResourceMaterial(11712, BlendMode_2));
    resources[84] = makeRes(11716, 6, new AbyssEngine::ResourceMaterial(11712, BlendMode_1));
    resources[85] = makeRes(18750, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_001.aem", 11714,
                                false));
    resources[86] = makeRes(18751, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_001_anim_add.aem",
                                11715, false));
    resources[87] = makeRes(18752, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_001_alpha.aem",
                                11716, false));
    resources[88] = makeRes(18753, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_002.aem", 11714,
                                false));
    resources[89] = makeRes(18754, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_002_anim_add.aem",
                                11715, false));
    resources[90] = makeRes(18755, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_002_alpha.aem",
                                11716, false));
    resources[91] = makeRes(18756, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_003.aem", 11714,
                                false));
    resources[92] = makeRes(18757, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_003_anim_add.aem",
                                11715, false));
    resources[93] = makeRes(18758, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_003_alpha.aem",
                                11716, false));
    resources[94] = makeRes(18759, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_004.aem", 11714,
                                false));
    resources[95] = makeRes(18760, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_004_anim_add.aem",
                                11715, false));
    resources[96] = makeRes(18761, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_004_alpha.aem",
                                11716, false));
    resources[97] = makeRes(18762, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_005.aem", 11714,
                                false));
    resources[98] = makeRes(18763, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_005_lights_anim_add.aem",
                                11715, false));
    resources[99] = makeRes(18765, 4, new AbyssEngine::ResourceMesh(
                                "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_005_glow_anim_add.aem",
                                11715, false));
    resources[100] = makeRes(18764, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_stage_005_alpha.aem",
                                 11716, false));
    resources[101] = makeRes(19050, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_plasma_array_midorian_explosion_anim.aem",
                                 11714, false));
    resources[102] = makeRes(11725, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/misc/sn_plasma_gun_valkyrie_diffuse.aei",
                                 0.0f));
    resources[103] = makeRes(11726, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/misc/sn_plasma_gun_valkyrie_normal_specular.aei",
                                 0.0f));
    resources[104] = makeRes(11727, 6, new AbyssEngine::ResourceMaterial(11725, 11726, static_cast<BlendMode>(28)));
    resources[105] = makeRes(11728, 6, new AbyssEngine::ResourceMaterial(11725, BlendMode_2));
    resources[106] = makeRes(11729, 6, new AbyssEngine::ResourceMaterial(11725, static_cast<BlendMode>(10)));
    resources[107] = makeRes(
        18768, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_plasma_gun_valkyrie.aem",
                                                11727, false));
    resources[108] = makeRes(18769, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_plasma_gun_valkyrie_lights_add.aem", 11728,
                                 false));
    resources[109] = makeRes(
        18770, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_plasma_gun_valkyrie_alpha.aem",
                                                11729, false));
    resources[110] = makeRes(
        11850, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_plasma_gun_fx_valkyrie.aei", 0.0f));
    resources[111] = makeRes(11851, 6, new AbyssEngine::ResourceMaterial(11850, static_cast<BlendMode>(3)));
    resources[112] = makeRes(19061, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/fx/sn_plasma_gun_fx_valkyrie_beam_anim_add.aem",
                                 11851, false));
    resources[113] = makeRes(19062, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/fx/sn_plasma_gun_fx_valkyrie_cone_anim_add.aem",
                                 11851, false));
    resources[114] = makeRes(19063, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/fx/sn_plasma_gun_fx_valkyrie_star_add.aem", 11851,
                                 false));
    resources[115] = makeRes(29093, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_cargo_midorian_wrecked_diffuse.aei",
                                 0.0f));
    resources[116] = makeRes(29094, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_cargo_midorian_wrecked_normal_specular.aei",
                                 0.0f));
    resources[117] = makeRes(29090, 6, new AbyssEngine::ResourceMaterial(29093, 29094, static_cast<BlendMode>(28)));
    resources[118] = makeRes(29091, 6, new AbyssEngine::ResourceMaterial(29093, BlendMode_2));
    resources[119] = makeRes(29092, 6, new AbyssEngine::ResourceMaterial(29093, BlendMode_dummy));
    resources[120] = makeRes(18766, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_cargo_001_midorian_wrecked.aem", 29090,
                                 false));
    resources[121] = makeRes(19072, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_cargo_001_midorian_wrecked_anim.aem", 29090,
                                 false));
    resources[122] = makeRes(19073, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_cargo_001_midorian_wrecked_glow_add.aem",
                                 29091, false));
    resources[123] = makeRes(19074, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_cargo_001_midorian_wrecked_glow_anim_add.aem",
                                 29091, false));
    resources[124] = makeRes(19075, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_cargo_001_midorian_wrecked_lights_add.aem",
                                 0, false));
    resources[125] = makeRes(19076, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_cargo_001_midorian_wrecked_lights_emissive.aem",
                                 29092, false));
    resources[126] = makeRes(11730, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/stations/sn_stations_midorian_wrecked_diffuse.aei",
                                 0.0f));
    resources[127] = makeRes(11731, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/stations/sn_stations_midorian_wrecked_normal_specular.aei",
                                 0.0f));
    resources[128] = makeRes(11732, 6, new AbyssEngine::ResourceMaterial(11730, 11731, static_cast<BlendMode>(28)));
    resources[129] = makeRes(11733, 6, new AbyssEngine::ResourceMaterial(11730, static_cast<BlendMode>(10)));
    resources[130] = makeRes(18771, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_station_midorian_wrecked.aem", 11732,
                                 false));
    resources[131] = makeRes(18772, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_station_midorian_wrecked_alpha_emissive.aem",
                                 11733, false));
    resources[132] = makeRes(11747, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/stations/sn_burning_station_fire.aei",
                                 0.0f));
    resources[133] = makeRes(11750, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/stations/sn_burning_station_fire_normal.aei",
                                 0.0f));
    resources[134] = makeRes(11748, 6, new AbyssEngine::ResourceMaterial(11747, static_cast<BlendMode>(3)));
    resources[135] = makeRes(11749, 6, new AbyssEngine::ResourceMaterial(11747, BlendMode_1));
    resources[136] = makeRes(
        18781, 6, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_burning_station.aem",
                                                20007, false));
    resources[137] = makeRes(18782, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_station_emissive.aem", 20008,
                                 false));
    resources[138] = makeRes(18783, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_station_fire_anim_add.aem", 11748,
                                 false));
    resources[139] = makeRes(18784, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_station_fire_anim_alpha.aem",
                                 11749, false));
    resources[140] = makeRes(18832, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_station_fire_intro_anim_add.aem",
                                 11748, false));
    resources[141] = makeRes(18833, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_station_fire_intro_anim_alpha.aem",
                                 11749, false));
    resources[142] = makeRes(19095, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_valkyrie_stage_1_fire_anim_add.aem",
                                 11748, false));
    resources[143] = makeRes(19096, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_valkyrie_stage_1_smoke_anim_alpha.aem",
                                 11749, false));
    resources[144] = makeRes(19097, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_valkyrie_stage_2_fire_anim_add.aem",
                                 11748, false));
    resources[145] = makeRes(19098, 6, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/stations/sn_burning_valkyrie_stage_2_smoke_anim_alpha.aem",
                                 11749, false));
    resources[146] = makeRes(
        18785, 6, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_secure_container_nivelian.aem",
                                                0, false));
    resources[147] = makeRes(
        18831, 6, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/misc/sn_secure_container_nivelian_add.aem", 0, false));
    resources[148] = makeRes(
        29095, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/misc/sn_junk_field_diffuse.aei", 0.0f));
    resources[149] = makeRes(29096, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/misc/sn_junk_field_normal_specular.aei",
                                 0.0f));
    resources[150] = makeRes(29097, 6, new AbyssEngine::ResourceMaterial(29095, 29096, static_cast<BlendMode>(28)));
    resources[151] = makeRes(
        18786, 6, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_junk_field.aem", 29097,
                                                false));
    resources[152] = makeRes(
        11755, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_ship_blaze_flames.aei", 0.0f));
    resources[153] = makeRes(
        11759, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_ship_blaze_flames_normal.aei", 0.0f));
    resources[154] = makeRes(11756, 6, new AbyssEngine::ResourceMaterial(11755, static_cast<BlendMode>(3)));
    resources[155] = makeRes(
        11757, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_ship_blaze_glow.aei", 0.0f));
    resources[156] = makeRes(11758, 6, new AbyssEngine::ResourceMaterial(11757, static_cast<BlendMode>(3)));
    resources[157] = makeRes(
        18803, 6, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_ship_blaze_flames_anim_add.aem",
                                                11756, false));
    resources[158] = makeRes(
        18802, 6, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/fx/sn_ship_blaze_glow_anim_add.aem",
                                                11758, false));
    resources[159] = makeRes(29021, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_plasma_collector_001_diffuse.aei",
                                 0.0f));
    resources[160] = makeRes(29022, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_plasma_collector_001_normal_specular.aei",
                                 0.0f));
    resources[161] = makeRes(29023, 6, new AbyssEngine::ResourceMaterial(29021, 29022, static_cast<BlendMode>(28)));
    resources[162] = makeRes(29024, 6, new AbyssEngine::ResourceMaterial(29021, BlendMode_2));
    resources[163] = makeRes(29025, 6, new AbyssEngine::ResourceMaterial(29021, BlendMode_8));
    resources[164] = makeRes(
        18787, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_001.aem",
                                                29023, false));
    resources[165] = makeRes(18788, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_001_add.aem", 29024,
                                 false));
    resources[166] = makeRes(18791, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_001_gun_anim.aem", 29023,
                                 false));
    resources[167] = makeRes(18790, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_001_gun_anim_add.aem",
                                 29024, false));
    resources[168] = makeRes(29027, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_plasma_collector_002_diffuse.aei",
                                 0.0f));
    resources[169] = makeRes(29028, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_plasma_collector_002_normal_specular.aei",
                                 0.0f));
    resources[170] = makeRes(29029, 6, new AbyssEngine::ResourceMaterial(29027, 29028, static_cast<BlendMode>(28)));
    resources[171] = makeRes(29030, 6, new AbyssEngine::ResourceMaterial(29027, BlendMode_2));
    resources[172] = makeRes(29031, 6, new AbyssEngine::ResourceMaterial(29027, BlendMode_8));
    resources[173] = makeRes(
        18792, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_002.aem",
                                                29029, false));
    resources[174] = makeRes(18793, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_002_add.aem", 29030,
                                 false));
    resources[175] = makeRes(18795, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_002_gun_anim.aem", 29029,
                                 false));
    resources[176] = makeRes(18794, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_002_gun_anim_add.aem",
                                 29030, false));
    resources[177] = makeRes(29033, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_plasma_collector_003_diffuse.aei",
                                 0.0f));
    resources[178] = makeRes(29034, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_plasma_collector_003_normal_specular.aei",
                                 0.0f));
    resources[179] = makeRes(29035, 6, new AbyssEngine::ResourceMaterial(29033, 29034, static_cast<BlendMode>(28)));
    resources[180] = makeRes(29036, 6, new AbyssEngine::ResourceMaterial(29033, BlendMode_2));
    resources[181] = makeRes(29037, 6, new AbyssEngine::ResourceMaterial(29033, BlendMode_8));
    resources[182] = makeRes(29038, 6, new AbyssEngine::ResourceMaterial(29033, BlendMode_dummy));
    resources[183] = makeRes(
        18796, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_003.aem",
                                                29035, false));
    resources[184] = makeRes(18797, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_003_add.aem", 29036,
                                 false));
    resources[185] = makeRes(18799, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_003_gun_anim.aem", 29035,
                                 false));
    resources[186] = makeRes(18800, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_003_gun_emissive.aem",
                                 29038, false));
    resources[187] = makeRes(18798, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/turrets/sn_plasma_collector_003_gun_anim_add.aem",
                                 29036, false));
    resources[188] = makeRes(11770, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_carrier_terran_1_diffuse.aei",
                                 0.0f));
    resources[189] = makeRes(11771, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_carrier_terran_2_diffuse.aei",
                                 0.0f));
    resources[190] = makeRes(11772, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_carrier_terran_3_diffuse.aei",
                                 0.0f));
    resources[191] = makeRes(11773, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_carrier_terran_1_normal_specular.aei",
                                 0.0f));
    resources[192] = makeRes(11774, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_carrier_terran_2_normal_specular.aei",
                                 0.0f));
    resources[193] = makeRes(11775, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_carrier_terran_3_normal_specular.aei",
                                 0.0f));
    resources[194] = makeRes(11776, 6, new AbyssEngine::ResourceMaterial(11770, 11773, static_cast<BlendMode>(28)));
    resources[195] = makeRes(11777, 6, new AbyssEngine::ResourceMaterial(11771, 11774, static_cast<BlendMode>(28)));
    resources[196] = makeRes(11778, 6, new AbyssEngine::ResourceMaterial(11772, 11775, static_cast<BlendMode>(28)));
    resources[197] = makeRes(11779, 6, new AbyssEngine::ResourceMaterial(11770, BlendMode_dummy));
    resources[198] = makeRes(11780, 6, new AbyssEngine::ResourceMaterial(11771, BlendMode_dummy));
    resources[199] = makeRes(11781, 6, new AbyssEngine::ResourceMaterial(11772, BlendMode_dummy));
    resources[200] = makeRes(11782, 6, new AbyssEngine::ResourceMaterial(11772, BlendMode_2));
    resources[201] = makeRes(
        18804, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_carrier_terran_1.aem", 11776,
                                                false));
    resources[202] = makeRes(
        18805, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_carrier_terran_2.aem", 11777,
                                                false));
    resources[203] = makeRes(
        18807, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_carrier_terran_3.aem", 11778,
                                                false));
    resources[204] = makeRes(
        18808, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_carrier_terran_1_emissive.aem", 11779, false));
    resources[205] = makeRes(
        18809, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_carrier_terran_2_emissive.aem", 11780, false));
    resources[206] = makeRes(
        18810, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_carrier_terran_3_emissive.aem", 11781, false));
    resources[207] = makeRes(
        18806, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_carrier_terran_lights_add.aem", 11782, false));
    resources[208] = makeRes(29039, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_battleship_vossk_diffuse.aei",
                                 0.0f));
    resources[209] = makeRes(29040, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_battleship_vossk_normal_specular.aei",
                                 0.0f));
    resources[210] = makeRes(29041, 6, new AbyssEngine::ResourceMaterial(29039, 29040, static_cast<BlendMode>(28)));
    resources[211] = makeRes(29042, 6, new AbyssEngine::ResourceMaterial(29039, BlendMode_2));
    resources[212] = makeRes(
        19052, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_battleship_vossk_add.aem",
                                                29042, false));
    resources[213] = makeRes(19053, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_battleship_vossk_lights_add.aem", 29042,
                                 false));
    resources[214] = makeRes(
        19051, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_battleship_vossk.aem", 29041,
                                                false));
    resources[215] = makeRes(
        17251, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_044_elite_nivelian.aem",
                                                0, false));
    resources[216] = makeRes(18779, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_044_elite_nivelian_lights.aem", 32544,
                                 false));
    resources[217] = makeRes(18044, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_044_elite_nivelian_engine_add.aem",
                                 32544, false));
    resources[218] = makeRes(
        17252, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_ship_044_elite_nivelian_lod_1.aem", 0, false));
    resources[219] = makeRes(
        17253, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_ship_044_elite_nivelian_lod_2.aem", 0, false));
    resources[220] = makeRes(
        17257, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_045_most_wanted.aem", 0,
                                                false));
    resources[221] = makeRes(18045, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_045_most_wanted_engine_add.aem", 32545,
                                 false));
    resources[222] = makeRes(18811, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_045_most_wanted_lights.aem", 32545,
                                 false));
    resources[223] = makeRes(
        17263, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_046_most_wanted.aem", 0,
                                                false));
    resources[224] = makeRes(18046, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_046_most_wanted_engine_add.aem", 32546,
                                 false));
    resources[225] = makeRes(18812, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_046_most_wanted_lights_add.aem", 32546,
                                 false));
    resources[226] = makeRes(
        17269, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_047_most_wanted.aem", 0,
                                                false));
    resources[227] = makeRes(18813, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_047_most_wanted_lights_add.aem", 32547,
                                 false));
    resources[228] = makeRes(18047, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_047_most_wanted_engine_add.aem", 32547,
                                 false));
    resources[229] = makeRes(
        17275, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_048_most_wanted.aem", 0,
                                                false));
    resources[230] = makeRes(18780, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_048_most_wanted_lights.aem", 32548,
                                 false));
    resources[231] = makeRes(18048, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_048_most_wanted_engine_add.aem", 32548,
                                 false));
    resources[232] = makeRes(
        17281, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_049_boss_nivelian.aem",
                                                0, false));
    resources[233] = makeRes(18049, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_049_boss_nivelian_engine_add.aem",
                                 32549, false));
    resources[234] = makeRes(17286, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_049_boss_nivelian_lights_add.aem",
                                 32549, false));
    resources[235] = makeRes(
        17293, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_051_dropship_terran.aem",
                                                0, false));
    resources[236] = makeRes(18051, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_051_dropship_terran_engine_add.aem",
                                 32551, false));
    resources[237] = makeRes(18814, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_051_dropship_terran_lights.aem", 32551,
                                 false));
    resources[238] = makeRes(
        17299, 4,
        new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_052_retro.aem", 0, false));
    resources[239] = makeRes(
        18052, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_ship_052_retro_engine_add.aem", 32552, false));
    resources[240] = makeRes(
        18815, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_ship_052_retro_lights_add.aem", 32552, false));
    resources[241] = makeRes(
        17311, 4,
        new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_054_vossk.aem", 0, false));
    resources[242] = makeRes(
        18054, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_ship_054_vossk_engine_add.aem", 32554, false));
    resources[243] = makeRes(
        17316, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_054_vossk_emissive.aem",
                                                0, false));
    resources[244] = makeRes(
        17317, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_055_modified.aem", 0,
                                                false));
    resources[245] = makeRes(18818, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_055_modified_lights_add.aem", 32555,
                                 false));
    resources[246] = makeRes(
        17323, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_056_modified.aem", 0,
                                                false));
    resources[247] = makeRes(18819, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_056_modified_lights_add.aem", 32556,
                                 false));
    resources[248] = makeRes(
        17329, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_057_modified.aem", 0,
                                                false));
    resources[249] = makeRes(18820, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_057_modified_lights_add.aem", 32557,
                                 false));
    resources[250] = makeRes(
        17335, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_058_modified.aem", 0,
                                                false));
    resources[251] = makeRes(18821, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_058_modified_lights_add.aem", 32558,
                                 false));
    resources[252] = makeRes(
        17341, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_059_modified.aem", 0,
                                                false));
    resources[253] = makeRes(18822, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/ships/sn_ship_059_modified_lights_add.aem", 32559,
                                 false));
    resources[254] = makeRes(
        17347, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/ships/sn_ship_060_modified.aem", 0,
                                                false));
    resources[255] = makeRes(
        18823, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/ships/sn_ship_060_modified_lights_add.aem", 0, false));
    resources[256] = makeRes(
        17353, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_061_elite_nivelian_prototype.aem", 0, false));
    resources[257] = makeRes(18824, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/main/3d/meshes/ships/ship_061_elite_nivelian_prototype_lights_add.aem",
                                 32561, false));
    resources[258] = makeRes(
        17359, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_062_prototype.aem", 0, false));
    resources[259] = makeRes(
        18825, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_062_prototype_lights_add.aem",
                                                32562, false));
    resources[260] = makeRes(
        17365, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_063_vossk_prototype.aem", 0,
                                                false));
    resources[261] = makeRes(18826, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/main/3d/meshes/ships/ship_063_vossk_prototype_lights_add.aem", 32563,
                                 false));
    resources[262] = makeRes(17370, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/main/3d/meshes/ships/ship_063_vossk_prototype_lights_emissive.aem", 0,
                                 false));
    resources[263] = makeRes(
        11741, 2, new AbyssEngine::ResourceTexture("data/assets/supernova/3d/textures/low/etc/fx/sn_supernova.aei",
                                                   0.0f));
    resources[264] = makeRes(
        11761, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/meshes/fx/sn_sun_explosion_ring_anim_add.aem", 0.0f));
    resources[265] = makeRes(
        11762, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/meshes/fx/sn_sun_explosion_core_anim_add.aem", 0.0f));
    resources[266] = makeRes(
        11764, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_sun_explosion_ring.aei", 0.0f));
    resources[267] = makeRes(
        11763, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/fx/sn_sun_explosion_core.aei", 0.0f));
    resources[268] = makeRes(
        11870, 2, new AbyssEngine::ResourceTexture("data/assets/supernova/3d/textures/low/etc/misc/sn_gas_plasma.aei",
                                                   0.0f));
    resources[269] = makeRes(
        11871, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/misc/sn_gas_plasma_normal.aei", 0.0f));
    resources[270] = makeRes(29045, 6, new AbyssEngine::ResourceMaterial(11870, BlendMode_2));
    resources[271] = makeRes(18997, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_gas_cloud_green_anim_lookat_add.aem", 29045,
                                 false));
    resources[272] = makeRes(18998, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_gas_cloud_blue_anim_lookat_add.aem", 29045,
                                 false));
    resources[273] = makeRes(18999, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_gas_cloud_violet_anim_lookat_add.aem", 29045,
                                 false));
    resources[274] = makeRes(19000, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_gas_cloud_red_anim_lookat_add.aem", 29045,
                                 false));
    resources[275] = makeRes(19001, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_plasma_green_anim_lookat_add.aem", 29045,
                                 false));
    resources[276] = makeRes(19002, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_plasma_blue_anim_lookat_add.aem", 29045,
                                 false));
    resources[277] = makeRes(19003, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/misc/sn_plasma_violet_anim_lookat_add.aem", 29045,
                                 false));
    resources[278] = makeRes(
        19004, 4, new AbyssEngine::ResourceMesh(
            "data/assets/supernova/3d/meshes/misc/sn_plasma_red_anim_lookat_add.aem", 29045, false));
    resources[279] = makeRes(11753, 6, new AbyssEngine::ResourceMaterial(10080, BlendMode_dummy));
    resources[280] = makeRes(11754, 6, new AbyssEngine::ResourceMaterial(10084, BlendMode_dummy));
    resources[281] = makeRes(11783, 6, new AbyssEngine::ResourceMaterial(10081, BlendMode_dummy));
    resources[282] = makeRes(29060, 6, new AbyssEngine::ResourceMaterial(10082, BlendMode_dummy));
    resources[283] = makeRes(29061, 6, new AbyssEngine::ResourceMaterial(10083, BlendMode_dummy));
    resources[284] = makeRes(
        17815, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/skyboxes/sn_skybox_015.aem", 11753,
                                                false));
    resources[285] = makeRes(17824, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/skyboxes/sn_skybox_015_flares_1_anim.aem", 65535,
                                 false));
    resources[286] = makeRes(17825, 4, new AbyssEngine::ResourceMesh(
                                 "data/assets/supernova/3d/meshes/skyboxes/sn_skybox_015_flares_2_anim.aem", 65535,
                                 false));
    resources[287] = makeRes(
        17816, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/skyboxes/sn_skybox_016.aem", 11783,
                                                false));
    resources[288] = makeRes(
        17817, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/skyboxes/sn_skybox_017.aem", 29060,
                                                false));
    resources[289] = makeRes(
        17818, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/skyboxes/sn_skybox_018.aem", 29061,
                                                false));
    resources[290] = makeRes(10150, 4, new AbyssEngine::ResourceMesh("data/textures/lens_flare_0.aei", 65535, false));
    resources[291] = makeRes(10151, 4, new AbyssEngine::ResourceMesh("data/textures/lens_flare_1.aei", 65535, false));
    resources[292] = makeRes(10152, 4, new AbyssEngine::ResourceMesh("data/textures/lens_flare_2.aei", 65535, false));
    resources[293] = makeRes(
        34500, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/glow_midorian.aei", 0.0f));
    resources[294] = makeRes(
        34501, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/glow_nivelian.aei", 0.0f));
    resources[295] = makeRes(
        34502, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/glow_pirates.aei", 0.0f));
    resources[296] = makeRes(
        34503, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/glow_terran.aei", 0.0f));
    resources[297] = makeRes(
        34504, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/glow_vossk.aei", 0.0f));
    resources[298] = makeRes(
        34505, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/glow_void.aei", 0.0f));
    resources[299] = makeRes(
        10031, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_000.aei", 0.0f));
    resources[300] = makeRes(
        10032, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_001.aei", 0.0f));
    resources[301] = makeRes(
        10033, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_002.aei", 0.0f));
    resources[302] = makeRes(
        10034, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_003.aei", 0.0f));
    resources[303] = makeRes(
        10035, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_004.aei", 0.0f));
    resources[304] = makeRes(
        10036, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_005.aei", 0.0f));
    resources[305] = makeRes(
        10037, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_006.aei", 0.0f));
    resources[306] = makeRes(
        10038, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_007.aei", 0.0f));
    resources[307] = makeRes(
        10039, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_008.aei", 0.0f));
    resources[308] = makeRes(
        10040, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_009.aei", 0.0f));
    resources[309] = makeRes(
        10041, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/suns/sun_010.aei", 0.0f));
    resources[310] = makeRes(6801, 4, newImage(0x0u, 0x0u));
    resources[311] = makeRes(33011, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_terran_normal_specular.aei",
                                 0.0f));
    resources[312] = makeRes(
        33008, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/stations_pirates_diffuse.aei", 0.0f));
    resources[313] = makeRes(33009, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_pirates_normal_specular.aei",
                                 0.0f));
    resources[314] = makeRes(
        33006, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/stations_nivelian_diffuse.aei", 0.0f));
    resources[315] = makeRes(33007, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_nivelian_normal_specular.aei",
                                 0.0f));
    resources[316] = makeRes(
        33004, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/stations_midorian_diffuse.aei", 0.0f));
    resources[317] = makeRes(33005, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_midorian_normal_specular.aei",
                                 0.0f));
    resources[318] = makeRes(
        33014, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/stations_vossk_diffuse.aei", 0.0f));
    resources[319] = makeRes(33015, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_vossk_normal_specular.aei",
                                 0.0f));
    resources[320] = makeRes(
        33012, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/stations_void_diffuse.aei", 0.0f));
    resources[321] = makeRes(33013, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_void_normal_specular.aei",
                                 0.0f));
    resources[322] = makeRes(
        33016, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/stations_void_glow.aei", 0.0f));
    resources[323] = makeRes(20048, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[324] = makeRes(
        34000, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_000_midorian_diffuse.aei", 0.0f));
    resources[325] = makeRes(
        34001, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_001_terran_diffuse.aei", 0.0f));
    resources[326] = makeRes(
        34002, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_002_pirates_diffuse.aei", 0.0f));
    resources[327] = makeRes(
        34003, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_003_midorian_diffuse.aei", 0.0f));
    resources[328] = makeRes(
        34004, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_004_nivelian_diffuse.aei", 0.0f));
    resources[329] = makeRes(
        34005, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_005_terran_diffuse.aei", 0.0f));
    resources[330] = makeRes(
        34006, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_006_midorian_diffuse.aei", 0.0f));
    resources[331] = makeRes(
        34007, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_007_terran_diffuse.aei", 0.0f));
    resources[332] = makeRes(
        34008, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_008_void_diffuse.aei", 0.0f));
    resources[333] = makeRes(
        34009, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_009_vossk_diffuse.aei", 0.0f));
    resources[334] = makeRes(
        34010, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_010_terran_diffuse.aei", 0.0f));
    resources[335] = makeRes(
        34011, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_011_pirates_diffuse.aei", 0.0f));
    resources[336] = makeRes(
        34012, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_012_nivelian_diffuse.aei", 0.0f));
    resources[337] = makeRes(
        34016, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_016_nivelian_diffuse.aei", 0.0f));
    resources[338] = makeRes(
        34017, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_017_terran_diffuse.aei", 0.0f));
    resources[339] = makeRes(
        34018, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_018_nivelian_diffuse.aei", 0.0f));
    resources[340] = makeRes(
        34019, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_019_midorian_diffuse.aei", 0.0f));
    resources[341] = makeRes(
        34020, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_020_midorian_diffuse.aei", 0.0f));
    resources[342] = makeRes(
        34021, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_021_nivelian_diffuse.aei", 0.0f));
    resources[343] = makeRes(
        34022, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_022_terran_diffuse.aei", 0.0f));
    resources[344] = makeRes(
        34023, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_023_pirates_diffuse.aei", 0.0f));
    resources[345] = makeRes(
        34024, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_024_pirates_diffuse.aei", 0.0f));
    resources[346] = makeRes(
        34025, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_025_pirates_diffuse.aei", 0.0f));
    resources[347] = makeRes(
        34026, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_026_terran_diffuse.aei", 0.0f));
    resources[348] = makeRes(
        34027, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_027_terran_diffuse.aei", 0.0f));
    resources[349] = makeRes(
        34028, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_028_terran_diffuse.aei", 0.0f));
    resources[350] = makeRes(
        34029, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_029_pirates_diffuse.aei", 0.0f));
    resources[351] = makeRes(
        34030, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_030_midorian_diffuse.aei", 0.0f));
    resources[352] = makeRes(
        34031, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_031_nivelian_diffuse.aei", 0.0f));
    resources[353] = makeRes(
        34032, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_032_pirates_diffuse.aei", 0.0f));
    resources[354] = makeRes(
        34033, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_033_terran_diffuse.aei", 0.0f));
    resources[355] = makeRes(
        34034, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_034_terran_diffuse.aei", 0.0f));
    resources[356] = makeRes(
        34035, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_035_nivelian_diffuse.aei", 0.0f));
    resources[357] = makeRes(
        34036, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_036_terran_diffuse.aei", 0.0f));
    resources[358] = makeRes(34037, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_037_deep_science_diffuse.aei",
                                 0.0f));
    resources[359] = makeRes(34038, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_038_deep_science_diffuse.aei",
                                 0.0f));
    resources[360] = makeRes(
        34039, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_039_vossk_diffuse.aei", 0.0f));
    resources[361] = makeRes(34040, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_040_deep_science_diffuse.aei",
                                 0.0f));
    resources[362] = makeRes(
        34041, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_041_vossk_diffuse.aei", 0.0f));
    resources[363] = makeRes(
        18083, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27284, false));
    resources[364] = makeRes(
        34043, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_043_retro_diffuse.aei", 0.0f));
    resources[365] = makeRes(34044, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_044_elite_nivelian_diffuse.aei",
                                 0.0f));
    resources[366] = makeRes(34045, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_045_most_wanted_diffuse.aei",
                                 0.0f));
    resources[367] = makeRes(34046, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_046_most_wanted_diffuse.aei",
                                 0.0f));
    resources[368] = makeRes(34047, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_047_most_wanted_diffuse.aei",
                                 0.0f));
    resources[369] = makeRes(34048, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_048_most_wanted_diffuse.aei",
                                 0.0f));
    resources[370] = makeRes(34049, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_049_boss_nivelian_diffuse.aei",
                                 0.0f));
    resources[371] = makeRes(34051, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_051_dropship_terran_diffuse.aei",
                                 0.0f));
    resources[372] = makeRes(
        34052, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_052_retro_diffuse.aei", 0.0f));
    resources[373] = makeRes(
        34054, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_054_vossk_diffuse.aei", 0.0f));
    resources[374] = makeRes(34055, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_055_modified_diffuse.aei",
                                 0.0f));
    resources[375] = makeRes(34056, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_056_modified_diffuse.aei",
                                 0.0f));
    resources[376] = makeRes(34057, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_057_modified_diffuse.aei",
                                 0.0f));
    resources[377] = makeRes(34058, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_058_modified_diffuse.aei",
                                 0.0f));
    resources[378] = makeRes(34059, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_059_modified_diffuse.aei",
                                 0.0f));
    resources[379] = makeRes(34060, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_060_modified_diffuse.aei",
                                 0.0f));
    resources[380] = makeRes(34061, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_061_elite_nivelian_prototype_diffuse.aei",
                                 0.0f));
    resources[381] = makeRes(
        34062, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_062_prototype_diffuse.aei", 0.0f));
    resources[382] = makeRes(
        18086, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27287, false));
    resources[383] = makeRes(34100, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_000_midorian_normal_specular.aei",
                                 0.0f));
    resources[384] = makeRes(34101, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_001_terran_normal_specular.aei",
                                 0.0f));
    resources[385] = makeRes(34102, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_002_pirates_normal_specular.aei",
                                 0.0f));
    resources[386] = makeRes(34103, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_003_midorian_normal_specular.aei",
                                 0.0f));
    resources[387] = makeRes(34104, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_004_nivelian_normal_specular.aei",
                                 0.0f));
    resources[388] = makeRes(34105, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_005_terran_normal_specular.aei",
                                 0.0f));
    resources[389] = makeRes(34106, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_006_midorian_normal_specular.aei",
                                 0.0f));
    resources[390] = makeRes(34107, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_007_terran_normal_specular.aei",
                                 0.0f));
    resources[391] = makeRes(
        34108, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_008_void_normal_specular.aei", 0.0f));
    resources[392] = makeRes(
        34109, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_009_vossk_normal_specular.aei", 0.0f));
    resources[393] = makeRes(34110, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_010_terran_normal_specular.aei",
                                 0.0f));
    resources[394] = makeRes(34111, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_011_pirates_normal_specular.aei",
                                 0.0f));
    resources[395] = makeRes(34112, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_012_nivelian_normal_specular.aei",
                                 0.0f));
    resources[396] = makeRes(34116, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_016_nivelian_normal_specular.aei",
                                 0.0f));
    resources[397] = makeRes(34117, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_017_terran_normal_specular.aei",
                                 0.0f));
    resources[398] = makeRes(34118, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_018_nivelian_normal_specular.aei",
                                 0.0f));
    resources[399] = makeRes(34119, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_019_midorian_normal_specular.aei",
                                 0.0f));
    resources[400] = makeRes(34120, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_020_midorian_normal_specular.aei",
                                 0.0f));
    resources[401] = makeRes(34121, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_021_nivelian_normal_specular.aei",
                                 0.0f));
    resources[402] = makeRes(34122, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_022_terran_normal_specular.aei",
                                 0.0f));
    resources[403] = makeRes(34123, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_023_pirates_normal_specular.aei",
                                 0.0f));
    resources[404] = makeRes(34124, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_024_pirates_normal_specular.aei",
                                 0.0f));
    resources[405] = makeRes(34125, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_025_pirates_normal_specular.aei",
                                 0.0f));
    resources[406] = makeRes(34126, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_026_terran_normal_specular.aei",
                                 0.0f));
    resources[407] = makeRes(34127, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_027_terran_normal_specular.aei",
                                 0.0f));
    resources[408] = makeRes(34128, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_028_terran_normal_specular.aei",
                                 0.0f));
    resources[409] = makeRes(34129, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_029_pirates_normal_specular.aei",
                                 0.0f));
    resources[410] = makeRes(34130, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_030_midorian_normal_specular.aei",
                                 0.0f));
    resources[411] = makeRes(34131, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_031_nivelian_normal_specular.aei",
                                 0.0f));
    resources[412] = makeRes(34132, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_032_pirates_normal_specular.aei",
                                 0.0f));
    resources[413] = makeRes(34133, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_033_terran_normal_specular.aei",
                                 0.0f));
    resources[414] = makeRes(34134, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_034_terran_normal_specular.aei",
                                 0.0f));
    resources[415] = makeRes(34135, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_035_nivelian_normal_specular.aei",
                                 0.0f));
    resources[416] = makeRes(34136, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_036_terran_normal_specular.aei",
                                 0.0f));
    resources[417] = makeRes(34137, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_037_deep_science_normal_specular.aei",
                                 0.0f));
    resources[418] = makeRes(34138, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_038_deep_science_normal_specular.aei",
                                 0.0f));
    resources[419] = makeRes(34139, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_039_vossk_normal_specular.aei",
                                 0.0f));
    resources[420] = makeRes(34140, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_040_deep_science_normal_specular.aei",
                                 0.0f));
    resources[421] = makeRes(34141, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/ships/v_ship_041_vossk_normal_specular.aei",
                                 0.0f));
    resources[422] = makeRes(
        34142, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_042_retro_normal_specular.aei", 0.0f));
    resources[423] = makeRes(
        34143, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_043_retro_normal_specular.aei", 0.0f));
    resources[424] = makeRes(34144, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_044_elite_nivelian_normal_specular.aei",
                                 0.0f));
    resources[425] = makeRes(34145, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_045_most_wanted_normal_specular.aei",
                                 0.0f));
    resources[426] = makeRes(34146, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_046_most_wanted_normal_specular.aei",
                                 0.0f));
    resources[427] = makeRes(34147, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_047_most_wanted_normal_specular.aei",
                                 0.0f));
    resources[428] = makeRes(34148, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_048_most_wanted_normal_specular.aei",
                                 0.0f));
    resources[429] = makeRes(34149, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_049_boss_nivelian_normal_specular.aei",
                                 0.0f));
    resources[430] = makeRes(34151, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_051_dropship_terran_normal_specular.aei",
                                 0.0f));
    resources[431] = makeRes(34152, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_052_retro_normal_specular.aei",
                                 0.0f));
    resources[432] = makeRes(34154, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_054_vossk_normal_specular.aei",
                                 0.0f));
    resources[433] = makeRes(34155, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_055_modified_normal_specular.aei",
                                 0.0f));
    resources[434] = makeRes(34156, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_056_modified_normal_specular.aei",
                                 0.0f));
    resources[435] = makeRes(34157, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_057_modified_normal_specular.aei",
                                 0.0f));
    resources[436] = makeRes(34158, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_058_modified_normal_specular.aei",
                                 0.0f));
    resources[437] = makeRes(34159, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_059_modified_normal_specular.aei",
                                 0.0f));
    resources[438] = makeRes(34160, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/ships/sn_ship_060_modified_normal_specular.aei",
                                 0.0f));
    resources[439] = makeRes(34161, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_061_elite_nivelian_prototype_normal_specular.aei",
                                 0.0f));
    resources[440] = makeRes(34162, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/ships/ship_062_prototype_normal_specular.aei",
                                 0.0f));
    resources[441] = makeRes(
        34163, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/ships/ship_063_vossk_normal_specular.aei", 0.0f));
    resources[442] = makeRes(
        10107, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/misc/asteroid_explosion.aei",
                                                   0.0f));
    resources[443] = makeRes(
        10155, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_asteroid_ice_diffuse.aei", 0.0f));
    resources[444] = makeRes(10156, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/misc/v_asteroid_ice_normal_specular.aei",
                                 0.0f));
    resources[445] = makeRes(27314, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/misc/sn_asteroid_magma_explosion.aei",
                                 0.0f));
    resources[446] = makeRes(
        28999, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/misc/sn_asteroid_magma_diffuse.aei", 0.0f));
    resources[447] = makeRes(29000, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/misc/sn_asteroid_magma_normal_specular.aei",
                                 0.0f));
    resources[448] = makeRes(
        10104, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/misc/wormhole.aei", 0.0f));
    resources[449] = makeRes(
        10121, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/fog.aei", 0.0f));
    resources[450] = makeRes(
        10132, 2, new AbyssEngine::ResourceTexture("data/assets/valkyrie/3d/textures/low/etc/fx/v_fog_ice.aei", 0.0f));
    resources[451] = makeRes(
        10100, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/galaxymap/galaxymap_bg.aei",
                                                   0.0f));
    resources[452] = makeRes(
        10101, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/galaxymap/galaxymap_fog_layer_0.aei", 0.0f));
    resources[453] = makeRes(
        10102, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/galaxymap/galaxymap_fog_layer_1.aei", 0.0f));
    resources[454] = makeRes(
        10145, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/galaxymap/galaxymap_planets_diffuse.aei", 0.0f));
    resources[455] = makeRes(
        10144, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/galaxymap/galaxymap_orbit.aei",
                                                   0.0f));
    resources[456] = makeRes(
        10159, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/galaxymap/v_galaxymap_planets.aei", 0.0f));
    resources[457] = makeRes(
        18180, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_000.aem", 20027, false));
    resources[458] = makeRes(
        0, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/bar_visitor_glow_vossk.aei",
                                               0.0f));
    resources[459] = makeRes(
        18186, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_006.aem", 20027, false));
    resources[460] = makeRes(
        33403, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/bars/bar_visitor_glow_midorian.aei", 0.0f));
    resources[461] = makeRes(
        33404, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/bar_visitor_shadow.aei",
                                                   0.0f));
    resources[462] = makeRes(
        10161, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/galaxymap/sn_galaxymap_planets.aei", 0.0f));
    resources[463] = makeRes(
        11606, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_bobolan.aei",
                                                   0.0f));
    resources[464] = makeRes(
        11607, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_grey.aei", 0.0f));
    resources[465] = makeRes(
        11608, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_multipod.aei",
                                                   0.0f));
    resources[466] = makeRes(
        11609, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_nivelian.aei",
                                                   0.0f));
    resources[467] = makeRes(
        11610, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_terran_f.aei",
                                                   0.0f));
    resources[468] = makeRes(
        11611, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_terran_m.aei",
                                                   0.0f));
    resources[469] = makeRes(
        11612, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/bars/visitor_vossk.aei",
                                                   0.0f));
    resources[470] = makeRes(10157, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/hangars/v_hangar_battlestation_diffuse.aei",
                                 0.0f));
    resources[471] = makeRes(10158, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/hangars/v_hangar_battlestation_normal_specular.aei",
                                 0.0f));
    resources[472] = makeRes(10134, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/hangars/v_hangar_deep_science_diffuse.aei",
                                 0.0f));
    resources[473] = makeRes(10135, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/hangars/v_hangar_deep_science_normal_specular.aei",
                                 0.0f));
    resources[474] = makeRes(11600, 2, new AbyssEngine::ResourceTexture("data/textures/col_test.aei", 0.0f));
    resources[475] = makeRes(
        12000, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/misc/space_junk_diffuse.aei",
                                                   0.0f));
    resources[476] = makeRes(
        12001, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/space_junk_normal_specular.aei", 0.0f));
    resources[477] = makeRes(
        11601, 2,
        new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/sprite_explosion.aei", 0.0f));
    resources[478] = makeRes(
        11602, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/sprite_smoke.aei", 0.0f));
    resources[479] = makeRes(
        11599, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/sprite_fire.aei", 0.0f));
    resources[480] = makeRes(
        11598, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/explosion.aei", 0.0f));
    resources[481] = makeRes(
        12002, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/turrets/turret_001_diffuse.aei", 0.0f));
    resources[482] = makeRes(
        12003, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/turrets/turret_001_normal_specular.aei", 0.0f));
    resources[483] = makeRes(
        12004, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/turrets/turret_002_diffuse.aei", 0.0f));
    resources[484] = makeRes(
        12005, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/turrets/turret_002_normal_specular.aei", 0.0f));
    resources[485] = makeRes(
        12006, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/turrets/turret_003_diffuse.aei", 0.0f));
    resources[486] = makeRes(
        12007, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/turrets/turret_003_normal_specular.aei", 0.0f));
    resources[487] = makeRes(
        11860, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/turrets/sn_turret_004_diffuse.aei", 0.0f));
    resources[488] = makeRes(11861, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/supernova/3d/textures/low/etc/turrets/sn_turret_004_normal_specular.aei",
                                 0.0f));
    resources[489] = makeRes(
        12050, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/turrets/v_autoturret_001_diffuse.aei", 0.0f));
    resources[490] = makeRes(12051, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/turrets/v_autoturret_001_normal_specular.aei",
                                 0.0f));
    resources[491] = makeRes(
        12052, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/turrets/v_autoturret_002_diffuse.aei", 0.0f));
    resources[492] = makeRes(12053, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/turrets/v_autoturret_002_normal_specular.aei",
                                 0.0f));
    resources[493] = makeRes(
        12054, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/turrets/v_autoturret_003_diffuse.aei", 0.0f));
    resources[494] = makeRes(12055, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/turrets/v_autoturret_003_normal_specular.aei",
                                 0.0f));
    resources[495] = makeRes(
        12008, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/scanner_probe_diffuse.aei", 0.0f));
    resources[496] = makeRes(
        12009, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/scanner_probe_normal_specular.aei", 0.0f));
    resources[497] = makeRes(
        11594, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/misc/beer_and_bra_diffuse.aei",
                                                   0.0f));
    resources[498] = makeRes(
        11595, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/beer_and_bra_normal_specular.aei", 0.0f));
    resources[499] = makeRes(
        10148, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_guided_missile_diffuse.aei", 0.0f));
    resources[500] = makeRes(10149, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/misc/v_guided_missile_normal_specular.aei",
                                 0.0f));
    resources[501] = makeRes(
        10126, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/fx/v_scattergun_000_explosion.aei", 0.0f));
    resources[502] = makeRes(
        10127, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/fx/v_scattergun_001_explosion.aei", 0.0f));
    resources[503] = makeRes(
        10128, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/fx/v_scattergun_002_explosion.aei", 0.0f));
    resources[504] = makeRes(
        24210, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_mine_001_diffuse.aei", 0.0f));
    resources[505] = makeRes(
        24211, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_mine_001_normal_specular.aei", 0.0f));
    resources[506] = makeRes(
        24212, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_mine_002_diffuse.aei", 0.0f));
    resources[507] = makeRes(
        24213, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_mine_002_normal_specular.aei", 0.0f));
    resources[508] = makeRes(
        24214, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_mine_003_diffuse.aei", 0.0f));
    resources[509] = makeRes(
        24215, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/misc/v_mine_003_normal_specular.aei", 0.0f));
    resources[510] = makeRes(11617, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/stations/v_station_battlestation_diffuse.aei",
                                 0.0f));
    resources[511] = makeRes(11618, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/stations/v_station_battlestation_normal_specular.aei",
                                 0.0f));
    resources[512] = makeRes(11615, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/stations/v_station_deep_science_diffuse.aei",
                                 0.0f));
    resources[513] = makeRes(11616, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/stations/v_station_deep_science_normal_specular.aei",
                                 0.0f));
    resources[514] = makeRes(11620, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/stations/v_station_deep_science_glow.aei",
                                 0.0f));
    resources[515] = makeRes(34816, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/valkyrie/3d/textures/low/etc/stations/v_station_deep_science_emitters.aei",
                                 0.0f));
    resources[516] = makeRes(
        11628, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/stations/tex_player_station_fx.aei", 0.0f));
    resources[517] = makeRes(
        11633, 2, new AbyssEngine::ResourceTexture("data/assets/valkyrie/3d/textures/low/etc/fx/v_shield_normal.aei",
                                                   0.0f));
    resources[518] = makeRes(
        11634, 2, new AbyssEngine::ResourceTexture("data/assets/valkyrie/3d/textures/low/etc/fx/v_shield_noise.aei",
                                                   0.0f));
    resources[519] = makeRes(
        33300, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/khador_jump.aei", 0.0f));
    resources[520] = makeRes(
        24203, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/hyper_drive.aei", 0.0f));
    resources[521] = makeRes(11647, 2, new AbyssEngine::ResourceTexture("data/textures/gas_cloud.aei", 0.0f));
    resources[523] = makeRes(33411, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/skyboxes/skybox_asteroid_belt_normal_specular.aei",
                                 0.0f));
    resources[524] = makeRes(27160, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[526] = makeRes(33241, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/container_003_terran_normal_specular.aei",
                                 0.0f));
    resources[527] = makeRes(
        33242, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/container_004_vossk_diffuse.aei", 0.0f));
    resources[528] = makeRes(33243, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/container_004_vossk_normal_specular.aei",
                                 0.0f));
    resources[529] = makeRes(
        33244, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/container_002_nivelian_diffuse.aei", 0.0f));
    resources[530] = makeRes(33245, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/container_002_nivelian_normal_specular.aei",
                                 0.0f));
    resources[531] = makeRes(
        33246, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/container_001_midorian_diffuse.aei", 0.0f));
    resources[532] = makeRes(33247, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/container_001_midorian_normal_specular.aei",
                                 0.0f));
    resources[533] = makeRes(
        24202, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/particles.aei", 0.0f));
    resources[534] = makeRes(
        24208, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/space_particle_diffuse.aei", 0.0f));
    resources[535] = makeRes(
        24209, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/space_particle_normal_specular.aei", 0.0f));
    resources[536] = makeRes(20092, 6, new AbyssEngine::ResourceMaterial(24208, 24209, static_cast<BlendMode>(36)));
    resources[537] = makeRes(20090, 6, new AbyssEngine::ResourceMaterial(24202, static_cast<BlendMode>(3)));
    resources[539] = makeRes(27300, 6, new AbyssEngine::ResourceMaterial(33301, BlendMode_2));
    resources[541] = makeRes(33341, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/stations/stations_pirates_dmg_normal_specular.aei",
                                 0.0f));
    resources[542] = makeRes(33357, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[543] = makeRes(33352, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[544] = makeRes(33353, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[545] = makeRes(33354, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[546] = makeRes(33355, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[547] = makeRes(33356, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[548] = makeRes(
        12020, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_hangar_terran.aei", 0.0f));
    resources[549] = makeRes(
        12021, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_hangar_vossk.aei", 0.0f));
    resources[550] = makeRes(
        12022, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_hangar_nivelian.aei", 0.0f));
    resources[551] = makeRes(
        12023, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_hangar_midorian.aei", 0.0f));
    resources[552] = makeRes(
        12030, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_000.aei", 0.0f));
    resources[553] = makeRes(
        12031, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_001.aei", 0.0f));
    resources[554] = makeRes(
        12032, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_002.aei", 0.0f));
    resources[555] = makeRes(
        12033, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_003.aei", 0.0f));
    resources[556] = makeRes(
        12034, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_004.aei", 0.0f));
    resources[557] = makeRes(
        12035, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_005.aei", 0.0f));
    resources[558] = makeRes(
        12036, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_006.aei", 0.0f));
    resources[559] = makeRes(
        12037, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_007.aei", 0.0f));
    resources[560] = makeRes(
        12038, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_008.aei", 0.0f));
    resources[561] = makeRes(
        12039, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_009.aei", 0.0f));
    resources[562] = makeRes(
        12040, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/cubemaps/cubemap_skybox_010.aei", 0.0f));
    resources[563] = makeRes(
        12041, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/cubemaps/v_cubemap_skybox_011.aei", 0.0f));
    resources[564] = makeRes(
        12042, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/cubemaps/v_cubemap_skybox_012.aei", 0.0f));
    resources[565] = makeRes(
        12043, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/cubemaps/v_cubemap_skybox_013.aei", 0.0f));
    resources[566] = makeRes(
        12044, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/cubemaps/v_cubemap_skybox_014.aei", 0.0f));
    resources[567] = makeRes(
        12045, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/cubemaps/sn_cubemap_skybox_015.aei", 0.0f));
    resources[568] = makeRes(
        12046, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/cubemaps/sn_cubemap_skybox_016.aei", 0.0f));
    resources[569] = makeRes(
        12047, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/cubemaps/sn_cubemap_skybox_017.aei", 0.0f));
    resources[570] = makeRes(
        12048, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/cubemaps/sn_cubemap_skybox_018.aei", 0.0f));
    resources[572] = makeRes(
        33304, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/bomb_explosive_a_diffuse.aei", 0.0f));
    resources[573] = makeRes(33305, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/bomb_explosive_a_normal_specular.aei",
                                 0.0f));
    resources[574] = makeRes(
        33307, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/bomb_explosive_b_diffuse.aei", 0.0f));
    resources[575] = makeRes(33308, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/bomb_explosive_b_normal_specular.aei",
                                 0.0f));
    resources[576] = makeRes(
        33310, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/misc/bomb_emp_a_diffuse.aei",
                                                   0.0f));
    resources[578] = makeRes(
        24204, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/rocket_explosive_diffuse.aei", 0.0f));
    resources[579] = makeRes(24205, 2, new AbyssEngine::ResourceTexture(
                                 "data/assets/main/3d/textures/low/etc/misc/rocket_explosive_normal_specular.aei",
                                 0.0f));
    resources[580] = makeRes(
        24206, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/misc/rocket_emp_diffuse.aei",
                                                   0.0f));
    resources[581] = makeRes(
        24207, 2, new AbyssEngine::ResourceTexture(
            "data/assets/main/3d/textures/low/etc/misc/rocket_emp_normal_specular.aei", 0.0f));
    resources[583] = makeRes(
        34812, 2,
        new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/fx/ship_engine_glow.aei", 0.0f));
    resources[584] = makeRes(34813, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[585] = makeRes(
        34814, 2, new AbyssEngine::ResourceTexture("data/assets/valkyrie/3d/textures/low/etc/fx/v_ship_engine_glow.aei",
                                                   0.0f));
    resources[586] = makeRes(34815, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[587] = makeRes(27301, 6, new AbyssEngine::ResourceMaterial(0, static_cast<BlendMode>(3)));
    resources[588] = makeRes(27302, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_1));
    resources[590] = makeRes(33306, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[591] = makeRes(33309, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[593] = makeRes(20004, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[594] = makeRes(20003, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[595] = makeRes(20005, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[596] = makeRes(20006, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[597] = makeRes(20104, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[598] = makeRes(20105, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[599] = makeRes(20007, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[600] = makeRes(20008, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[601] = makeRes(20046, 6, new AbyssEngine::ResourceMaterial(10094, BlendMode_dummy));
    resources[602] = makeRes(20045, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[603] = makeRes(20047, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[604] = makeRes(20082, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_1));
    resources[605] = makeRes(20032, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[606] = makeRes(20031, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[607] = makeRes(20033, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[610] = makeRes(34702, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_8));
    resources[612] = makeRes(0, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[613] = makeRes(34501, 6, new AbyssEngine::ResourceMaterial(34501, BlendMode_2));
    resources[614] = makeRes(34706, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_8));
    resources[615] = makeRes(34707, 6, new AbyssEngine::ResourceMaterial(34501, BlendMode_dummy));
    resources[616] = makeRes(34708, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[617] = makeRes(34709, 6, new AbyssEngine::ResourceMaterial(34700, BlendMode_2));
    resources[618] = makeRes(34710, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_8));
    resources[619] = makeRes(34711, 6, new AbyssEngine::ResourceMaterial(34501, BlendMode_dummy));
    resources[620] = makeRes(34712, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[621] = makeRes(34713, 6, new AbyssEngine::ResourceMaterial(34703, BlendMode_2));
    resources[622] = makeRes(34714, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_8));
    resources[623] = makeRes(34715, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[624] = makeRes(34716, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[625] = makeRes(34717, 6, new AbyssEngine::ResourceMaterial(34700, BlendMode_2));
    resources[626] = makeRes(34718, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_8));
    resources[627] = makeRes(34719, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[628] = makeRes(34600, 6, new AbyssEngine::ResourceMaterial(33301, BlendMode_2));
    resources[630] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[631] = makeRes(34603, 6, new AbyssEngine::ResourceMaterial(34700, BlendMode_2));
    resources[633] = makeRes(34605, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[635] = makeRes(34201, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[636] = makeRes(34202, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[641] = makeRes(34207, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[642] = makeRes(34208, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[644] = makeRes(34210, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[649] = makeRes(34218, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[654] = makeRes(34223, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[659] = makeRes(34228, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[664] = makeRes(34233, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[666] = makeRes(34235, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[668] = makeRes(34237, 6, new AbyssEngine::ResourceMaterial(34234, 0, static_cast<BlendMode>(28)));
    resources[670] = makeRes(34239, 6, new AbyssEngine::ResourceMaterial(34237, 0, static_cast<BlendMode>(28)));
    resources[671] = makeRes(34240, 6, new AbyssEngine::ResourceMaterial(34237, 0, static_cast<BlendMode>(28)));
    resources[672] = makeRes(34241, 6, new AbyssEngine::ResourceMaterial(34232, 0, static_cast<BlendMode>(28)));
    resources[674] = makeRes(34243, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[675] = makeRes(34236, 6, new AbyssEngine::ResourceMaterial(34236, 0, static_cast<BlendMode>(28)));
    resources[676] = makeRes(34234, 6, new AbyssEngine::ResourceMaterial(34234, 0, static_cast<BlendMode>(28)));
    resources[677] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[678] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[679] = makeRes(34048, 6, new AbyssEngine::ResourceMaterial(34048, 0, static_cast<BlendMode>(28)));
    resources[680] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[682] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[683] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[684] = makeRes(34253, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[685] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[686] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[687] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[688] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[689] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[690] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[691] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[692] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[693] = makeRes(65535, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[694] = makeRes(0, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[695] = makeRes(34400, 6, new AbyssEngine::ResourceMaterial(34042, BlendMode_8));
    resources[696] = makeRes(34401, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[697] = makeRes(34402, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[698] = makeRes(34403, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[699] = makeRes(34404, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[700] = makeRes(34405, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[701] = makeRes(34406, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[702] = makeRes(34407, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[703] = makeRes(34408, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[704] = makeRes(34409, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[705] = makeRes(34410, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[706] = makeRes(34411, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[707] = makeRes(34412, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[708] = makeRes(34416, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[709] = makeRes(34417, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[710] = makeRes(34418, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[711] = makeRes(34419, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[712] = makeRes(34420, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[713] = makeRes(34421, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[714] = makeRes(34422, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[715] = makeRes(34423, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[716] = makeRes(34424, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[717] = makeRes(34425, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[718] = makeRes(34426, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[719] = makeRes(34427, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[720] = makeRes(34428, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[721] = makeRes(34429, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[722] = makeRes(34430, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[723] = makeRes(34431, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[724] = makeRes(34432, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[725] = makeRes(34433, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[726] = makeRes(34434, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[727] = makeRes(34435, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[728] = makeRes(34436, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[729] = makeRes(34442, 6, new AbyssEngine::ResourceMaterial(34042, BlendMode_8));
    resources[730] = makeRes(34443, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[731] = makeRes(34444, 6, new AbyssEngine::ResourceMaterial(34236, BlendMode_8));
    resources[732] = makeRes(34445, 6, new AbyssEngine::ResourceMaterial(34234, BlendMode_8));
    resources[733] = makeRes(34446, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[734] = makeRes(34447, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[735] = makeRes(34448, 6, new AbyssEngine::ResourceMaterial(34048, BlendMode_8));
    resources[736] = makeRes(34449, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[737] = makeRes(34450, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[738] = makeRes(34451, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[739] = makeRes(34452, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[740] = makeRes(34453, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[741] = makeRes(34454, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[742] = makeRes(34455, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[743] = makeRes(34456, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[744] = makeRes(34457, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[745] = makeRes(34458, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[746] = makeRes(34459, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[747] = makeRes(34460, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[748] = makeRes(34461, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[749] = makeRes(34462, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_8));
    resources[750] = makeRes(34463, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_8));
    resources[751] = makeRes(34300, 6, new AbyssEngine::ResourceMaterial(34042, BlendMode_dummy));
    resources[752] = makeRes(34301, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[753] = makeRes(34302, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[754] = makeRes(34303, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[755] = makeRes(34304, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[756] = makeRes(34305, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[757] = makeRes(34306, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[758] = makeRes(34307, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[759] = makeRes(34308, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[760] = makeRes(34309, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[761] = makeRes(34310, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[762] = makeRes(34311, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[763] = makeRes(34312, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[764] = makeRes(34313, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[765] = makeRes(34314, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[766] = makeRes(34315, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[767] = makeRes(34316, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[768] = makeRes(34317, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[769] = makeRes(34318, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[770] = makeRes(34319, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[771] = makeRes(34320, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[772] = makeRes(34321, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[773] = makeRes(34322, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[774] = makeRes(34323, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[775] = makeRes(34324, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[776] = makeRes(34325, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[777] = makeRes(34326, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[778] = makeRes(34327, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[779] = makeRes(34328, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[780] = makeRes(34329, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[781] = makeRes(34330, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[782] = makeRes(34331, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[783] = makeRes(34332, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[784] = makeRes(34333, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[785] = makeRes(34334, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[786] = makeRes(34335, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[787] = makeRes(34336, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[788] = makeRes(32537, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[789] = makeRes(32538, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[790] = makeRes(32539, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[791] = makeRes(32540, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[792] = makeRes(32541, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[793] = makeRes(34342, 6, new AbyssEngine::ResourceMaterial(34042, BlendMode_dummy));
    resources[794] = makeRes(32542, 6, new AbyssEngine::ResourceMaterial(34042, BlendMode_2));
    resources[795] = makeRes(34343, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[796] = makeRes(32543, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[797] = makeRes(34344, 6, new AbyssEngine::ResourceMaterial(34236, BlendMode_dummy));
    resources[798] = makeRes(32544, 6, new AbyssEngine::ResourceMaterial(34236, BlendMode_2));
    resources[799] = makeRes(34345, 6, new AbyssEngine::ResourceMaterial(34234, BlendMode_dummy));
    resources[800] = makeRes(32545, 6, new AbyssEngine::ResourceMaterial(34234, BlendMode_2));
    resources[801] = makeRes(34346, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[802] = makeRes(32546, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[803] = makeRes(34347, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[804] = makeRes(32547, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[805] = makeRes(34348, 6, new AbyssEngine::ResourceMaterial(34048, BlendMode_dummy));
    resources[806] = makeRes(32548, 6, new AbyssEngine::ResourceMaterial(34048, BlendMode_2));
    resources[807] = makeRes(34349, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[808] = makeRes(32549, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[809] = makeRes(34350, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[810] = makeRes(32550, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[811] = makeRes(34351, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[812] = makeRes(32551, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[813] = makeRes(34352, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[814] = makeRes(32552, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[815] = makeRes(34353, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[816] = makeRes(32553, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[817] = makeRes(34354, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[818] = makeRes(32554, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[819] = makeRes(34355, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[820] = makeRes(32555, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[821] = makeRes(34356, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[822] = makeRes(32556, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[823] = makeRes(34357, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[824] = makeRes(32557, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[825] = makeRes(34358, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[826] = makeRes(32558, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[827] = makeRes(34359, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[828] = makeRes(32559, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[829] = makeRes(34360, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[830] = makeRes(32560, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[831] = makeRes(34361, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[832] = makeRes(32561, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[833] = makeRes(34362, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_dummy));
    resources[834] = makeRes(32562, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[835] = makeRes(0, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_dummy));
    resources[836] = makeRes(32563, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[837] = makeRes(20128, 6, new AbyssEngine::ResourceMaterial(10129, static_cast<BlendMode>(6)));
    resources[838] = makeRes(20129, 6, new AbyssEngine::ResourceMaterial(10129, BlendMode_2));
    resources[839] = makeRes(20001, 6, new AbyssEngine::ResourceMaterial(10031, BlendMode_2));
    resources[840] = makeRes(27280, 6, new AbyssEngine::ResourceMaterial(10031, BlendMode_2));
    resources[841] = makeRes(27281, 6, new AbyssEngine::ResourceMaterial(10032, BlendMode_2));
    resources[842] = makeRes(27282, 6, new AbyssEngine::ResourceMaterial(10033, BlendMode_2));
    resources[843] = makeRes(27283, 6, new AbyssEngine::ResourceMaterial(10034, BlendMode_2));
    resources[844] = makeRes(27284, 6, new AbyssEngine::ResourceMaterial(10035, BlendMode_2));
    resources[845] = makeRes(27285, 6, new AbyssEngine::ResourceMaterial(10036, BlendMode_2));
    resources[846] = makeRes(27286, 6, new AbyssEngine::ResourceMaterial(10037, BlendMode_2));
    resources[847] = makeRes(27287, 6, new AbyssEngine::ResourceMaterial(10038, BlendMode_2));
    resources[848] = makeRes(27288, 6, new AbyssEngine::ResourceMaterial(10039, BlendMode_2));
    resources[849] = makeRes(27289, 6, new AbyssEngine::ResourceMaterial(10040, BlendMode_2));
    resources[850] = makeRes(27290, 6, new AbyssEngine::ResourceMaterial(10041, BlendMode_2));
    resources[851] = makeRes(20093, 6, new AbyssEngine::ResourceMaterial(10104, BlendMode_2));
    resources[852] = makeRes(20094, 6, new AbyssEngine::ResourceMaterial(12000, 12001, static_cast<BlendMode>(28)));
    resources[853] = makeRes(20095, 6, new AbyssEngine::ResourceMaterial(10121, BlendMode_2));
    resources[854] = makeRes(20137, 6, new AbyssEngine::ResourceMaterial(10132, BlendMode_2));
    resources[855] = makeRes(20009, 6, new AbyssEngine::ResourceMaterial(10065, BlendMode_dummy));
    resources[856] = makeRes(20010, 6, new AbyssEngine::ResourceMaterial(10066, BlendMode_dummy));
    resources[857] = makeRes(20011, 6, new AbyssEngine::ResourceMaterial(10067, BlendMode_dummy));
    resources[858] = makeRes(20012, 6, new AbyssEngine::ResourceMaterial(10068, BlendMode_dummy));
    resources[859] = makeRes(20013, 6, new AbyssEngine::ResourceMaterial(10069, BlendMode_dummy));
    resources[860] = makeRes(20014, 6, new AbyssEngine::ResourceMaterial(10070, BlendMode_dummy));
    resources[861] = makeRes(20015, 6, new AbyssEngine::ResourceMaterial(10071, BlendMode_dummy));
    resources[862] = makeRes(20016, 6, new AbyssEngine::ResourceMaterial(10072, BlendMode_dummy));
    resources[863] = makeRes(20017, 6, new AbyssEngine::ResourceMaterial(10073, BlendMode_dummy));
    resources[864] = makeRes(20018, 6, new AbyssEngine::ResourceMaterial(10074, BlendMode_dummy));
    resources[865] = makeRes(20019, 6, new AbyssEngine::ResourceMaterial(10075, BlendMode_dummy));
    resources[866] = makeRes(20130, 6, new AbyssEngine::ResourceMaterial(10076, BlendMode_dummy));
    resources[867] = makeRes(20131, 6, new AbyssEngine::ResourceMaterial(10077, BlendMode_dummy));
    resources[868] = makeRes(20132, 6, new AbyssEngine::ResourceMaterial(10078, BlendMode_dummy));
    resources[869] = makeRes(27149, 6, new AbyssEngine::ResourceMaterial(10079, BlendMode_dummy));
    resources[870] = makeRes(27200, 6, new AbyssEngine::ResourceMaterial(10086, BlendMode_dummy));
    resources[871] = makeRes(27201, 6, new AbyssEngine::ResourceMaterial(10087, BlendMode_dummy));
    resources[872] = makeRes(27202, 6, new AbyssEngine::ResourceMaterial(10088, BlendMode_dummy));
    resources[873] = makeRes(20021, 6, new AbyssEngine::ResourceMaterial(10001, BlendMode_1));
    resources[874] = makeRes(20022, 6, new AbyssEngine::ResourceMaterial(10001, BlendMode_2));
    resources[875] = makeRes(20052, 6, new AbyssEngine::ResourceMaterial(10001, static_cast<BlendMode>(3)));
    resources[876] = makeRes(20100, 6, new AbyssEngine::ResourceMaterial(10001, static_cast<BlendMode>(6)));
    resources[877] = makeRes(20108, 6, new AbyssEngine::ResourceMaterial(10001, BlendMode_dummy));
    resources[878] = makeRes(20124, 6, new AbyssEngine::ResourceMaterial(10001, static_cast<BlendMode>(4)));
    resources[879] = makeRes(20023, 6, new AbyssEngine::ResourceMaterial(33000, 0, static_cast<BlendMode>(28)));
    resources[880] = makeRes(20049, 6, new AbyssEngine::ResourceMaterial(33000, 0, static_cast<BlendMode>(28)));
    resources[881] = makeRes(20050, 6, new AbyssEngine::ResourceMaterial(10107, BlendMode_2));
    resources[882] = makeRes(20051, 6, new AbyssEngine::ResourceMaterial(10107, BlendMode_1));
    resources[883] = makeRes(20134, 6, new AbyssEngine::ResourceMaterial(10155, 10156, static_cast<BlendMode>(28)));
    resources[884] = makeRes(20133, 6, new AbyssEngine::ResourceMaterial(10155, BlendMode_1));
    resources[885] = makeRes(27312, 6, new AbyssEngine::ResourceMaterial(28999, 29000, static_cast<BlendMode>(28)));
    resources[886] = makeRes(27313, 6, new AbyssEngine::ResourceMaterial(27314, BlendMode_1));
    resources[887] = makeRes(20024, 6, new AbyssEngine::ResourceMaterial(10100, BlendMode_dummy));
    resources[888] = makeRes(20025, 6, new AbyssEngine::ResourceMaterial(10101, BlendMode_2));
    resources[889] = makeRes(20026, 6, new AbyssEngine::ResourceMaterial(10102, BlendMode_2));
    resources[890] = makeRes(20027, 6, new AbyssEngine::ResourceMaterial(10145, static_cast<BlendMode>(6)));
    resources[891] = makeRes(27305, 6, new AbyssEngine::ResourceMaterial(10159, static_cast<BlendMode>(6)));
    resources[892] = makeRes(20029, 6, new AbyssEngine::ResourceMaterial(10144, BlendMode_2));
    resources[893] = makeRes(20030, 6, new AbyssEngine::ResourceMaterial(10145, BlendMode_1));
    resources[894] = makeRes(27306, 6, new AbyssEngine::ResourceMaterial(10161, static_cast<BlendMode>(6)));
    resources[895] = makeRes(27307, 6, new AbyssEngine::ResourceMaterial(10161, BlendMode_2));
    resources[898] = makeRes(34802, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[901] = makeRes(20114, 6, new AbyssEngine::ResourceMaterial(11606, static_cast<BlendMode>(10)));
    resources[902] = makeRes(20115, 6, new AbyssEngine::ResourceMaterial(11607, static_cast<BlendMode>(10)));
    resources[903] = makeRes(20116, 6, new AbyssEngine::ResourceMaterial(11608, static_cast<BlendMode>(10)));
    resources[904] = makeRes(20117, 6, new AbyssEngine::ResourceMaterial(11609, static_cast<BlendMode>(10)));
    resources[905] = makeRes(20118, 6, new AbyssEngine::ResourceMaterial(11610, static_cast<BlendMode>(10)));
    resources[906] = makeRes(20119, 6, new AbyssEngine::ResourceMaterial(11611, static_cast<BlendMode>(10)));
    resources[907] = makeRes(20120, 6, new AbyssEngine::ResourceMaterial(11612, static_cast<BlendMode>(10)));
    resources[908] = makeRes(20056, 6, new AbyssEngine::ResourceMaterial(10136, 10140, static_cast<BlendMode>(28)));
    resources[909] = makeRes(20057, 6, new AbyssEngine::ResourceMaterial(10136, BlendMode_2));
    resources[910] = makeRes(20058, 6, new AbyssEngine::ResourceMaterial(10136, BlendMode_1));
    resources[911] = makeRes(20059, 6, new AbyssEngine::ResourceMaterial(10137, 10141, static_cast<BlendMode>(28)));
    resources[912] = makeRes(20060, 6, new AbyssEngine::ResourceMaterial(10137, BlendMode_2));
    resources[913] = makeRes(20061, 6, new AbyssEngine::ResourceMaterial(10137, static_cast<BlendMode>(24)));
    resources[914] = makeRes(20062, 6, new AbyssEngine::ResourceMaterial(10138, 10142, static_cast<BlendMode>(28)));
    resources[915] = makeRes(20063, 6, new AbyssEngine::ResourceMaterial(10138, BlendMode_2));
    resources[916] = makeRes(20064, 6, new AbyssEngine::ResourceMaterial(10138, BlendMode_1));
    resources[917] = makeRes(20065, 6, new AbyssEngine::ResourceMaterial(10139, 10143, static_cast<BlendMode>(28)));
    resources[918] = makeRes(20066, 6, new AbyssEngine::ResourceMaterial(10139, BlendMode_2));
    resources[919] = makeRes(20067, 6, new AbyssEngine::ResourceMaterial(10139, BlendMode_1));
    resources[920] = makeRes(24070, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[921] = makeRes(20077, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_1));
    resources[922] = makeRes(20078, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[923] = makeRes(24074, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[924] = makeRes(20071, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[925] = makeRes(24073, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[926] = makeRes(20074, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_1));
    resources[927] = makeRes(20075, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[928] = makeRes(24071, 6, new AbyssEngine::ResourceMaterial(0, 0, static_cast<BlendMode>(28)));
    resources[929] = makeRes(20106, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_1));
    resources[930] = makeRes(20069, 6, new AbyssEngine::ResourceMaterial(0, BlendMode_2));
    resources[931] = makeRes(20157, 6, new AbyssEngine::ResourceMaterial(10157, 10158, static_cast<BlendMode>(28)));
    resources[932] = makeRes(27138, 6, new AbyssEngine::ResourceMaterial(10157, BlendMode_dummy));
    resources[933] = makeRes(27139, 6, new AbyssEngine::ResourceMaterial(10157, BlendMode_1));
    resources[934] = makeRes(27140, 6, new AbyssEngine::ResourceMaterial(10157, BlendMode_2));
    resources[935] = makeRes(27141, 6, new AbyssEngine::ResourceMaterial(10134, BlendMode_dummy));
    resources[936] = makeRes(27142, 6, new AbyssEngine::ResourceMaterial(10134, BlendMode_1));
    resources[937] = makeRes(27143, 6, new AbyssEngine::ResourceMaterial(10134, BlendMode_2));
    resources[938] = makeRes(27144, 6, new AbyssEngine::ResourceMaterial(10134, 10135, static_cast<BlendMode>(28)));
    resources[939] = makeRes(20087, 6, new AbyssEngine::ResourceMaterial(11600, BlendMode_2));
    resources[940] = makeRes(27252, 6, new AbyssEngine::ResourceMaterial(11598, BlendMode_2));
    resources[941] = makeRes(27253, 6, new AbyssEngine::ResourceMaterial(11598, static_cast<BlendMode>(18)));
    resources[942] = makeRes(27250, 6, new AbyssEngine::ResourceMaterial(11599, BlendMode_2));
    resources[943] = makeRes(20099, 6, new AbyssEngine::ResourceMaterial(11601, BlendMode_2));
    resources[944] = makeRes(20101, 6, new AbyssEngine::ResourceMaterial(11602, BlendMode_1));
    resources[945] = makeRes(24075, 6, new AbyssEngine::ResourceMaterial(12002, 12003, static_cast<BlendMode>(28)));
    resources[946] = makeRes(24076, 6, new AbyssEngine::ResourceMaterial(12004, 12005, static_cast<BlendMode>(28)));
    resources[947] = makeRes(24077, 6, new AbyssEngine::ResourceMaterial(12006, 12007, static_cast<BlendMode>(28)));
    resources[948] = makeRes(24078, 6, new AbyssEngine::ResourceMaterial(12002, BlendMode_8));
    resources[949] = makeRes(24079, 6, new AbyssEngine::ResourceMaterial(12004, BlendMode_8));
    resources[950] = makeRes(24080, 6, new AbyssEngine::ResourceMaterial(12006, BlendMode_8));
    resources[951] = makeRes(24085, 6, new AbyssEngine::ResourceMaterial(11860, 11861, static_cast<BlendMode>(28)));
    resources[952] = makeRes(24086, 6, new AbyssEngine::ResourceMaterial(11860, BlendMode_2));
    resources[953] = makeRes(24087, 6, new AbyssEngine::ResourceMaterial(11860, BlendMode_8));
    resources[954] = makeRes(24081, 6, new AbyssEngine::ResourceMaterial(12050, 12051, static_cast<BlendMode>(28)));
    resources[955] = makeRes(24082, 6, new AbyssEngine::ResourceMaterial(12052, 12053, static_cast<BlendMode>(28)));
    resources[956] = makeRes(24083, 6, new AbyssEngine::ResourceMaterial(12054, 12055, static_cast<BlendMode>(28)));
    resources[957] = makeRes(20103, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[958] = makeRes(20111, 6, new AbyssEngine::ResourceMaterial(11594, 11595, static_cast<BlendMode>(28)));
    resources[959] = makeRes(20113, 6, new AbyssEngine::ResourceMaterial(12008, 12009, static_cast<BlendMode>(28)));
    resources[960] = makeRes(20121, 6, new AbyssEngine::ResourceMaterial(24210, 24211, static_cast<BlendMode>(28)));
    resources[961] = makeRes(20122, 6, new AbyssEngine::ResourceMaterial(24212, 24213, static_cast<BlendMode>(28)));
    resources[962] = makeRes(20123, 6, new AbyssEngine::ResourceMaterial(24214, 24215, static_cast<BlendMode>(28)));
    resources[963] = makeRes(20154, 6, new AbyssEngine::ResourceMaterial(24210, BlendMode_2));
    resources[964] = makeRes(20155, 6, new AbyssEngine::ResourceMaterial(24212, BlendMode_2));
    resources[965] = makeRes(20156, 6, new AbyssEngine::ResourceMaterial(24214, BlendMode_2));
    resources[966] = makeRes(20125, 6, new AbyssEngine::ResourceMaterial(10148, 10149, static_cast<BlendMode>(28)));
    resources[967] = makeRes(20126, 6, new AbyssEngine::ResourceMaterial(10148, BlendMode_2));
    resources[968] = makeRes(20151, 6, new AbyssEngine::ResourceMaterial(10126, BlendMode_2));
    resources[969] = makeRes(20152, 6, new AbyssEngine::ResourceMaterial(10127, BlendMode_2));
    resources[970] = makeRes(20153, 6, new AbyssEngine::ResourceMaterial(10128, BlendMode_2));
    resources[971] = makeRes(20135, 6, new AbyssEngine::ResourceMaterial(11617, 11618, static_cast<BlendMode>(28)));
    resources[972] = makeRes(20136, 6, new AbyssEngine::ResourceMaterial(11617, BlendMode_dummy));
    resources[973] = makeRes(20158, 6, new AbyssEngine::ResourceMaterial(11617, BlendMode_2));
    resources[974] = makeRes(27145, 6, new AbyssEngine::ResourceMaterial(11615, 11616, static_cast<BlendMode>(28)));
    resources[975] = makeRes(27146, 6, new AbyssEngine::ResourceMaterial(11615, BlendMode_dummy));
    resources[976] = makeRes(27147, 6, new AbyssEngine::ResourceMaterial(11620, BlendMode_2));
    resources[978] = makeRes(27162, 6, new AbyssEngine::ResourceMaterial(11628, static_cast<BlendMode>(6)));
    resources[979] = makeRes(27163, 6, new AbyssEngine::ResourceMaterial(11628, BlendMode_dummy));
    resources[980] = makeRes(27164, 6, new AbyssEngine::ResourceMaterial(11628, BlendMode_2));
    resources[981] = makeRes(27165, 6, new AbyssEngine::ResourceMaterial(11628, BlendMode_1));
    resources[982] = makeRes(27148, 6, new AbyssEngine::ResourceMaterial(10129, BlendMode_8));
    resources[983] = makeRes(27150, 6, new AbyssEngine::ResourceMaterial(11634, 11633, static_cast<BlendMode>(34)));
    resources[984] = makeRes(27154, 6, new AbyssEngine::ResourceMaterial(11629, static_cast<BlendMode>(6)));
    resources[985] = makeRes(27155, 6, new AbyssEngine::ResourceMaterial(11629, BlendMode_dummy));
    resources[986] = makeRes(27156, 6, new AbyssEngine::ResourceMaterial(11629, BlendMode_8));
    resources[987] = makeRes(27157, 6, new AbyssEngine::ResourceMaterial(11631, static_cast<BlendMode>(6)));
    resources[988] = makeRes(27158, 6, new AbyssEngine::ResourceMaterial(11631, BlendMode_dummy));
    resources[989] = makeRes(27159, 6, new AbyssEngine::ResourceMaterial(11631, BlendMode_8));
    resources[990] = makeRes(27311, 6, new AbyssEngine::ResourceMaterial(11647, BlendMode_2));
    resources[991] = makeRes(27254, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[992] = makeRes(27255, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[993] = makeRes(27256, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[994] = makeRes(27257, 6, new AbyssEngine::ResourceMaterial(65535, 0, static_cast<BlendMode>(28)));
    resources[995] = makeRes(27260, 6, new AbyssEngine::ResourceMaterial(65535, BlendMode_2));
    resources[996] = makeRes(27262, 6, new AbyssEngine::ResourceMaterial(24203, BlendMode_2));
    resources[999] = makeRes(11636, 1, newImage(0x2d73u, 0x0u));
    resources[1000] = makeRes(11638, 1, newImage(0x2d75u, 0x0u));
    resources[1001] = makeRes(11640, 1, newImage(0x2d77u, 0x0u));
    resources[1002] = makeRes(11642, 1, newImage(0x2d79u, 0x0u));
    resources[1003] = makeRes(11644, 1, newImage(0x2d7bu, 0x0u));
    resources[1004] = makeRes(11646, 1, newImage(0x2d7du, 0x0u));
    resources[1005] = makeRes(1102, 3, newImage(0x274eu, 0x2u));
    resources[1006] = makeRes(1106, 3, newImage(0x274eu, 0x6u));
    resources[1007] = makeRes(1108, 3, newImage(0x274eu, 0x8u));
    resources[1008] = makeRes(1109, 3, newImage(0x274eu, 0x9u));
    resources[1009] = makeRes(1110, 3, newImage(0x274eu, 0xau));
    resources[1010] = makeRes(1111, 1, newImage(0x274eu, 0x0u));
    resources[1011] = makeRes(1112, 3, newImage(0x274eu, 0xbu));
    resources[1012] = makeRes(1113, 3, newImage(0x274eu, 0xcu));
    resources[1013] = makeRes(1114, 3, newImage(0x274eu, 0xdu));
    resources[1014] = makeRes(1115, 3, newImage(0x274eu, 0xeu));
    resources[1015] = makeRes(1116, 3, newImage(0x274eu, 0xfu));
    resources[1016] = makeRes(1117, 3, newImage(0x274eu, 0x10u));
    resources[1017] = makeRes(1118, 3, newImage(0x274eu, 0x11u));
    resources[1018] = makeRes(1119, 3, newImage(0x274eu, 0x12u));
    resources[1019] = makeRes(1120, 3, newImage(0x274eu, 0x13u));
    resources[1020] = makeRes(1121, 3, newImage(0x274eu, 0x14u));
    resources[1021] = makeRes(1122, 3, newImage(0x274eu, 0x15u));
    resources[1022] = makeRes(1123, 3, newImage(0x274eu, 0x16u));
    resources[1023] = makeRes(1124, 3, newImage(0x274eu, 0x17u));
    resources[1024] = makeRes(1125, 3, newImage(0x274eu, 0x18u));
    resources[1025] = makeRes(1126, 3, newImage(0x274eu, 0x19u));
    resources[1026] = makeRes(1127, 3, newImage(0x274eu, 0x1au));
    resources[1027] = makeRes(1128, 3, newImage(0x274eu, 0x1bu));
    resources[1028] = makeRes(1129, 3, newImage(0x274eu, 0x1cu));
    resources[1029] = makeRes(1130, 3, newImage(0x274eu, 0x1du));
    resources[1030] = makeRes(1131, 3, newImage(0x274eu, 0x1eu));
    resources[1031] = makeRes(1132, 3, newImage(0x274eu, 0x1fu));
    resources[1032] = makeRes(1133, 3, newImage(0x274eu, 0x20u));
    resources[1033] = makeRes(1134, 3, newImage(0x274eu, 0x21u));
    resources[1034] = makeRes(1135, 3, newImage(0x274eu, 0x22u));
    resources[1035] = makeRes(1136, 3, newImage(0x274eu, 0x23u));
    resources[1036] = makeRes(1137, 3, newImage(0x274eu, 0x24u));
    resources[1037] = makeRes(1138, 3, newImage(0x274eu, 0x25u));
    resources[1038] = makeRes(1139, 3, newImage(0x274eu, 0x26u));
    resources[1039] = makeRes(1140, 3, newImage(0x274eu, 0x27u));
    resources[1040] = makeRes(1141, 3, newImage(0x274eu, 0x28u));
    resources[1041] = makeRes(1142, 3, newImage(0x274eu, 0x29u));
    resources[1042] = makeRes(1143, 3, newImage(0x274eu, 0x2au));
    resources[1043] = makeRes(1144, 3, newImage(0x274eu, 0x2bu));
    resources[1044] = makeRes(1145, 3, newImage(0x274eu, 0x2cu));
    resources[1045] = makeRes(1146, 3, newImage(0x274eu, 0x2du));
    resources[1046] = makeRes(1147, 3, newImage(0x274eu, 0x2eu));
    resources[1047] = makeRes(1148, 3, newImage(0x274eu, 0x2fu));
    resources[1048] = makeRes(1149, 3, newImage(0x274eu, 0x30u));
    resources[1049] = makeRes(1150, 3, newImage(0x274eu, 0x31u));
    resources[1050] = makeRes(1151, 3, newImage(0x274eu, 0x32u));
    resources[1051] = makeRes(1152, 3, newImage(0x274eu, 0x33u));
    resources[1052] = makeRes(1153, 3, newImage(0x274eu, 0x34u));
    resources[1053] = makeRes(1154, 3, newImage(0x274eu, 0x35u));
    resources[1054] = makeRes(1155, 3, newImage(0x274eu, 0x36u));
    resources[1055] = makeRes(1156, 3, newImage(0x274eu, 0x37u));
    resources[1056] = makeRes(1158, 3, newImage(0x274eu, 0x39u));
    resources[1057] = makeRes(1159, 3, newImage(0x274eu, 0x3au));
    resources[1058] = makeRes(1160, 3, newImage(0x274eu, 0x3bu));
    resources[1059] = makeRes(1161, 3, newImage(0x274eu, 0x3cu));
    resources[1060] = makeRes(1162, 3, newImage(0x274eu, 0x3du));
    resources[1061] = makeRes(1163, 3, newImage(0x274eu, 0x3eu));
    resources[1062] = makeRes(1164, 3, newImage(0x274eu, 0x3fu));
    resources[1063] = makeRes(1165, 3, newImage(0x274eu, 0x40u));
    resources[1064] = makeRes(1166, 3, newImage(0x274eu, 0x41u));
    resources[1065] = makeRes(1167, 3, newImage(0x274eu, 0x42u));
    resources[1066] = makeRes(1168, 3, newImage(0x274eu, 0x43u));
    resources[1067] = makeRes(1169, 3, newImage(0x274eu, 0x44u));
    resources[1068] = makeRes(1170, 3, newImage(0x274eu, 0x45u));
    resources[1069] = makeRes(1171, 3, newImage(0x274eu, 0x46u));
    resources[1070] = makeRes(1172, 3, newImage(0x274eu, 0x47u));
    resources[1071] = makeRes(1173, 3, newImage(0x274eu, 0x48u));
    resources[1072] = makeRes(1174, 3, newImage(0x274eu, 0x49u));
    resources[1073] = makeRes(1175, 3, newImage(0x274eu, 0x4au));
    resources[1074] = makeRes(1176, 3, newImage(0x274eu, 0x4bu));
    resources[1075] = makeRes(1177, 3, newImage(0x274eu, 0x4cu));
    resources[1076] = makeRes(1178, 3, newImage(0x274eu, 0x4du));
    resources[1077] = makeRes(1179, 3, newImage(0x274eu, 0x4eu));
    resources[1078] = makeRes(1180, 3, newImage(0x274eu, 0x4fu));
    resources[1079] = makeRes(1181, 3, newImage(0x274eu, 0x50u));
    resources[1080] = makeRes(1182, 3, newImage(0x274eu, 0x51u));
    resources[1081] = makeRes(1183, 3, newImage(0x274eu, 0x52u));
    resources[1082] = makeRes(1184, 3, newImage(0x274eu, 0x53u));
    resources[1083] = makeRes(1185, 3, newImage(0x274eu, 0x54u));
    resources[1084] = makeRes(1186, 3, newImage(0x274eu, 0x55u));
    resources[1085] = makeRes(1187, 3, newImage(0x274eu, 0x56u));
    resources[1086] = makeRes(1188, 3, newImage(0x274eu, 0x57u));
    resources[1087] = makeRes(1189, 3, newImage(0x274eu, 0x58u));
    resources[1088] = makeRes(1190, 3, newImage(0x274eu, 0x59u));
    resources[1089] = makeRes(1194, 3, newImage(0x274eu, 0x5du));
    resources[1090] = makeRes(1195, 3, newImage(0x274eu, 0x5eu));
    resources[1091] = makeRes(1196, 3, newImage(0x274eu, 0x5fu));
    resources[1092] = makeRes(1197, 3, newImage(0x274eu, 0x60u));
    resources[1093] = makeRes(1216, 3, newImage(0x274eu, 0x73u));
    resources[1094] = makeRes(1218, 3, newImage(0x274eu, 0x75u));
    resources[1095] = makeRes(1219, 3, newImage(0x274eu, 0x76u));
    resources[1096] = makeRes(1220, 3, newImage(0x274eu, 0x77u));
    resources[1097] = makeRes(1221, 3, newImage(0x274eu, 0x78u));
    resources[1098] = makeRes(1230, 3, newImage(0x274eu, 0x81u));
    resources[1099] = makeRes(1231, 3, newImage(0x274eu, 0x82u));
    resources[1100] = makeRes(1232, 3, newImage(0x274eu, 0x83u));
    resources[1101] = makeRes(1233, 3, newImage(0x274eu, 0x84u));
    resources[1102] = makeRes(1245, 3, newImage(0x274eu, 0x90u));
    resources[1103] = makeRes(1246, 3, newImage(0x274eu, 0x91u));
    resources[1104] = makeRes(1247, 3, newImage(0x274eu, 0x92u));
    resources[1105] = makeRes(1248, 3, newImage(0x274eu, 0x93u));
    resources[1106] = makeRes(1249, 3, newImage(0x274eu, 0x94u));
    resources[1107] = makeRes(1250, 3, newImage(0x274eu, 0x95u));
    resources[1108] = makeRes(1251, 3, newImage(0x274eu, 0x96u));
    resources[1109] = makeRes(1252, 3, newImage(0x274eu, 0x97u));
    resources[1110] = makeRes(1253, 3, newImage(0x274eu, 0x98u));
    resources[1111] = makeRes(1254, 3, newImage(0x274eu, 0x99u));
    resources[1112] = makeRes(1255, 3, newImage(0x274eu, 0x9au));
    resources[1113] = makeRes(1256, 3, newImage(0x274eu, 0x9bu));
    resources[1114] = makeRes(1259, 3, newImage(0x274eu, 0x9eu));
    resources[1115] = makeRes(1260, 3, newImage(0x274eu, 0x9fu));
    resources[1116] = makeRes(1261, 3, newImage(0x274eu, 0xa0u));
    resources[1117] = makeRes(1267, 3, newImage(0x274eu, 0xa6u));
    resources[1118] = makeRes(1268, 3, newImage(0x274eu, 0xa7u));
    resources[1119] = makeRes(1269, 3, newImage(0x274eu, 0xa8u));
    resources[1120] = makeRes(1270, 3, newImage(0x274eu, 0xa9u));
    resources[1121] = makeRes(1277, 3, newImage(0x274eu, 0xb0u));
    resources[1122] = makeRes(1278, 3, newImage(0x274eu, 0xb1u));
    resources[1123] = makeRes(1279, 3, newImage(0x274eu, 0xb2u));
    resources[1124] = makeRes(1280, 3, newImage(0x274eu, 0xb3u));
    resources[1125] = makeRes(1281, 3, newImage(0x274eu, 0xb4u));
    resources[1126] = makeRes(1282, 3, newImage(0x274eu, 0xb5u));
    resources[1127] = makeRes(1283, 3, newImage(0x274eu, 0xb6u));
    resources[1128] = makeRes(1284, 3, newImage(0x274eu, 0xb7u));
    resources[1129] = makeRes(1285, 3, newImage(0x274eu, 0xb8u));
    resources[1130] = makeRes(1286, 3, newImage(0x274eu, 0xb9u));
    resources[1131] = makeRes(1287, 3, newImage(0x274eu, 0xbau));
    resources[1132] = makeRes(1288, 3, newImage(0x274eu, 0xbbu));
    resources[1133] = makeRes(1289, 3, newImage(0x274eu, 0xbcu));
    resources[1134] = makeRes(1290, 3, newImage(0x274eu, 0xbdu));
    resources[1135] = makeRes(1291, 3, newImage(0x274eu, 0xbeu));
    resources[1136] = makeRes(1292, 3, newImage(0x274eu, 0xbfu));
    resources[1137] = makeRes(1293, 3, newImage(0x274eu, 0xc0u));
    resources[1138] = makeRes(1294, 3, newImage(0x274eu, 0xc1u));
    resources[1139] = makeRes(1295, 3, newImage(0x274eu, 0xc2u));
    resources[1140] = makeRes(1296, 3, newImage(0x274eu, 0xc3u));
    resources[1141] = makeRes(1298, 3, newImage(0x274eu, 0xc5u));
    resources[1142] = makeRes(1299, 3, newImage(0x274eu, 0xc6u));
    resources[1143] = makeRes(1300, 3, newImage(0x274eu, 0xc7u));
    resources[1144] = makeRes(1301, 3, newImage(0x274eu, 0xc8u));
    resources[1145] = makeRes(1302, 3, newImage(0x274eu, 0xc9u));
    resources[1146] = makeRes(1303, 3, newImage(0x274eu, 0xcau));
    resources[1147] = makeRes(1304, 3, newImage(0x274eu, 0xcbu));
    resources[1148] = makeRes(1305, 3, newImage(0x274eu, 0xccu));
    resources[1149] = makeRes(1306, 3, newImage(0x274eu, 0xcdu));
    resources[1150] = makeRes(1307, 3, newImage(0x274eu, 0xceu));
    resources[1151] = makeRes(1308, 3, newImage(0x274eu, 0xcfu));
    resources[1152] = makeRes(1309, 3, newImage(0x274eu, 0xd0u));
    resources[1153] = makeRes(1310, 1, newImage(0x274eu, 0x1u));
    resources[1154] = makeRes(1311, 3, newImage(0x274eu, 0xd1u));
    resources[1155] = makeRes(1313, 3, newImage(0x274eu, 0xd3u));
    resources[1156] = makeRes(1314, 3, newImage(0x274eu, 0xd4u));
    resources[1157] = makeRes(1315, 3, newImage(0x274eu, 0xd5u));
    resources[1158] = makeRes(1335, 3, newImage(0x274eu, 0xe9u));
    resources[1159] = makeRes(1336, 3, newImage(0x274eu, 0xeau));
    resources[1160] = makeRes(1337, 3, newImage(0x274eu, 0xebu));
    resources[1161] = makeRes(1338, 3, newImage(0x274eu, 0xecu));
    resources[1162] = makeRes(1339, 3, newImage(0x274eu, 0xedu));
    resources[1163] = makeRes(1340, 3, newImage(0x274eu, 0xeeu));
    resources[1164] = makeRes(1341, 3, newImage(0x274eu, 0xefu));
    resources[1165] = makeRes(1342, 3, newImage(0x274eu, 0xf0u));
    resources[1166] = makeRes(1349, 3, newImage(0x274eu, 0xf7u));
    resources[1167] = makeRes(1352, 3, newImage(0x274eu, 0xfau));
    resources[1168] = makeRes(8000, 3, newImage(0x2769u, 0x0u));
    resources[1169] = makeRes(8001, 3, newImage(0x2769u, 0x1u));
    resources[1170] = makeRes(8002, 3, newImage(0x2769u, 0x2u));
    resources[1171] = makeRes(8003, 3, newImage(0x2769u, 0x3u));
    resources[1172] = makeRes(8004, 3, newImage(0x2769u, 0x4u));
    resources[1173] = makeRes(8005, 3, newImage(0x2769u, 0x5u));
    resources[1174] = makeRes(8006, 3, newImage(0x2769u, 0x6u));
    resources[1175] = makeRes(8007, 3, newImage(0x2769u, 0x7u));
    resources[1176] = makeRes(8008, 3, newImage(0x2769u, 0x8u));
    resources[1177] = makeRes(8009, 3, newImage(0x2769u, 0x9u));
    resources[1178] = makeRes(8010, 3, newImage(0x2769u, 0xau));
    resources[1179] = makeRes(8011, 3, newImage(0x2769u, 0xbu));
    resources[1180] = makeRes(8012, 3, newImage(0x2769u, 0xcu));
    resources[1181] = makeRes(8013, 3, newImage(0x2769u, 0xdu));
    resources[1182] = makeRes(8014, 3, newImage(0x2769u, 0xeu));
    resources[1183] = makeRes(8015, 3, newImage(0x2769u, 0xfu));
    resources[1184] = makeRes(8016, 3, newImage(0x2769u, 0x10u));
    resources[1185] = makeRes(8017, 3, newImage(0x2769u, 0x11u));
    resources[1186] = makeRes(8018, 3, newImage(0x2769u, 0x12u));
    resources[1187] = makeRes(8019, 3, newImage(0x2769u, 0x13u));
    resources[1188] = makeRes(8020, 3, newImage(0x2769u, 0x14u));
    resources[1189] = makeRes(8021, 3, newImage(0x2769u, 0x15u));
    resources[1190] = makeRes(8022, 3, newImage(0x2769u, 0x16u));
    resources[1191] = makeRes(8023, 3, newImage(0x2769u, 0x17u));
    resources[1192] = makeRes(8024, 3, newImage(0x2769u, 0x18u));
    resources[1193] = makeRes(2200, 3, newImage(0x2750u, 0x0u));
    resources[1194] = makeRes(2201, 3, newImage(0x2750u, 0x1u));
    resources[1195] = makeRes(2202, 3, newImage(0x2750u, 0x2u));
    resources[1196] = makeRes(2203, 3, newImage(0x2750u, 0x3u));
    resources[1197] = makeRes(2204, 3, newImage(0x2750u, 0x4u));
    resources[1198] = makeRes(2205, 3, newImage(0x2750u, 0x5u));
    resources[1199] = makeRes(2206, 3, newImage(0x2750u, 0x6u));
    resources[1200] = makeRes(2207, 3, newImage(0x2750u, 0x7u));
    resources[1201] = makeRes(2208, 3, newImage(0x2750u, 0x8u));
    resources[1202] = makeRes(2209, 3, newImage(0x2750u, 0x9u));
    resources[1203] = makeRes(2210, 3, newImage(0x2750u, 0xau));
    resources[1204] = makeRes(2211, 3, newImage(0x2750u, 0xbu));
    resources[1205] = makeRes(2212, 3, newImage(0x2750u, 0xcu));
    resources[1206] = makeRes(2213, 3, newImage(0x2750u, 0xdu));
    resources[1207] = makeRes(2214, 3, newImage(0x2750u, 0xeu));
    resources[1208] = makeRes(2215, 3, newImage(0x2750u, 0xfu));
    resources[1209] = makeRes(2216, 3, newImage(0x2750u, 0x10u));
    resources[1210] = makeRes(2217, 3, newImage(0x2750u, 0x11u));
    resources[1211] = makeRes(2218, 3, newImage(0x2750u, 0x12u));
    resources[1212] = makeRes(2219, 3, newImage(0x2750u, 0x13u));
    resources[1213] = makeRes(2220, 3, newImage(0x2750u, 0x14u));
    resources[1214] = makeRes(2221, 3, newImage(0x2750u, 0x15u));
    resources[1215] = makeRes(2222, 3, newImage(0x2750u, 0x16u));
    resources[1216] = makeRes(2223, 3, newImage(0x2750u, 0x17u));
    resources[1217] = makeRes(2224, 3, newImage(0x2750u, 0x18u));
    resources[1218] = makeRes(2225, 3, newImage(0x2750u, 0x19u));
    resources[1219] = makeRes(2226, 3, newImage(0x2750u, 0x1au));
    resources[1220] = makeRes(2227, 3, newImage(0x2750u, 0x1bu));
    resources[1221] = makeRes(2228, 3, newImage(0x2750u, 0x1cu));
    resources[1222] = makeRes(2229, 3, newImage(0x2750u, 0x1du));
    resources[1223] = makeRes(2230, 3, newImage(0x2750u, 0x1eu));
    resources[1224] = makeRes(2231, 3, newImage(0x2750u, 0x1fu));
    resources[1225] = makeRes(2232, 3, newImage(0x2750u, 0x20u));
    resources[1226] = makeRes(2233, 3, newImage(0x2750u, 0x21u));
    resources[1227] = makeRes(2234, 3, newImage(0x2750u, 0x22u));
    resources[1228] = makeRes(2235, 3, newImage(0x2750u, 0x23u));
    resources[1229] = makeRes(2236, 3, newImage(0x2750u, 0x24u));
    resources[1230] = makeRes(2237, 3, newImage(0x2750u, 0x25u));
    resources[1231] = makeRes(2238, 3, newImage(0x2750u, 0x26u));
    resources[1232] = makeRes(2239, 3, newImage(0x2750u, 0x27u));
    resources[1233] = makeRes(2240, 3, newImage(0x2750u, 0x28u));
    resources[1234] = makeRes(2241, 3, newImage(0x2750u, 0x29u));
    resources[1235] = makeRes(2242, 3, newImage(0x2750u, 0x2au));
    resources[1236] = makeRes(2243, 3, newImage(0x2750u, 0x2bu));
    resources[1237] = makeRes(2244, 3, newImage(0x2750u, 0x2cu));
    resources[1238] = makeRes(2245, 3, newImage(0x2750u, 0x2du));
    resources[1239] = makeRes(2246, 3, newImage(0x2750u, 0x2eu));
    resources[1240] = makeRes(2247, 3, newImage(0x2750u, 0x2fu));
    resources[1241] = makeRes(2248, 3, newImage(0x2750u, 0x30u));
    resources[1242] = makeRes(2249, 3, newImage(0x2750u, 0x31u));
    resources[1243] = makeRes(2250, 3, newImage(0x2750u, 0x32u));
    resources[1244] = makeRes(2251, 3, newImage(0x2750u, 0x33u));
    resources[1245] = makeRes(2252, 3, newImage(0x2750u, 0x34u));
    resources[1246] = makeRes(2253, 3, newImage(0x2750u, 0x35u));
    resources[1247] = makeRes(2254, 3, newImage(0x2750u, 0x36u));
    resources[1248] = makeRes(2255, 3, newImage(0x2750u, 0x37u));
    resources[1249] = makeRes(2256, 3, newImage(0x2750u, 0x38u));
    resources[1250] = makeRes(2257, 3, newImage(0x2750u, 0x39u));
    resources[1251] = makeRes(2258, 3, newImage(0x2750u, 0x3au));
    resources[1252] = makeRes(2259, 3, newImage(0x2750u, 0x3bu));
    resources[1253] = makeRes(2260, 3, newImage(0x2750u, 0x3cu));
    resources[1254] = makeRes(2261, 3, newImage(0x2750u, 0x3du));
    resources[1255] = makeRes(2262, 3, newImage(0x2750u, 0x3eu));
    resources[1256] = makeRes(2263, 3, newImage(0x2750u, 0x3fu));
    resources[1257] = makeRes(2264, 3, newImage(0x2750u, 0x40u));
    resources[1258] = makeRes(2265, 3, newImage(0x2750u, 0x41u));
    resources[1259] = makeRes(2266, 3, newImage(0x2750u, 0x42u));
    resources[1260] = makeRes(2267, 3, newImage(0x2750u, 0x43u));
    resources[1261] = makeRes(2268, 3, newImage(0x2750u, 0x44u));
    resources[1262] = makeRes(2269, 3, newImage(0x2750u, 0x45u));
    resources[1263] = makeRes(2270, 3, newImage(0x2750u, 0x46u));
    resources[1264] = makeRes(2271, 3, newImage(0x2750u, 0x47u));
    resources[1265] = makeRes(2272, 3, newImage(0x2750u, 0x48u));
    resources[1266] = makeRes(2273, 3, newImage(0x2750u, 0x49u));
    resources[1267] = makeRes(2274, 3, newImage(0x2750u, 0x4au));
    resources[1268] = makeRes(2275, 3, newImage(0x2750u, 0x4bu));
    resources[1269] = makeRes(2276, 3, newImage(0x2750u, 0x4cu));
    resources[1270] = makeRes(2277, 3, newImage(0x2750u, 0x4du));
    resources[1271] = makeRes(2278, 3, newImage(0x2750u, 0x4eu));
    resources[1272] = makeRes(2279, 3, newImage(0x2750u, 0x4fu));
    resources[1273] = makeRes(2280, 3, newImage(0x2750u, 0x50u));
    resources[1274] = makeRes(2281, 3, newImage(0x2750u, 0x51u));
    resources[1275] = makeRes(2282, 3, newImage(0x2750u, 0x52u));
    resources[1276] = makeRes(2283, 3, newImage(0x2750u, 0x53u));
    resources[1277] = makeRes(2284, 3, newImage(0x2750u, 0x54u));
    resources[1278] = makeRes(2285, 3, newImage(0x2750u, 0x55u));
    resources[1279] = makeRes(2286, 3, newImage(0x2750u, 0x56u));
    resources[1280] = makeRes(2287, 3, newImage(0x2750u, 0x57u));
    resources[1281] = makeRes(2288, 3, newImage(0x2750u, 0x58u));
    resources[1282] = makeRes(2289, 3, newImage(0x2750u, 0x59u));
    resources[1283] = makeRes(2290, 3, newImage(0x2750u, 0x5au));
    resources[1284] = makeRes(2291, 3, newImage(0x2750u, 0x5bu));
    resources[1285] = makeRes(2292, 3, newImage(0x2750u, 0x5cu));
    resources[1286] = makeRes(2293, 3, newImage(0x2750u, 0x5du));
    resources[1287] = makeRes(2294, 3, newImage(0x2750u, 0x5eu));
    resources[1288] = makeRes(2295, 3, newImage(0x2750u, 0x5fu));
    resources[1289] = makeRes(2296, 3, newImage(0x2750u, 0x60u));
    resources[1290] = makeRes(2297, 3, newImage(0x2750u, 0x61u));
    resources[1291] = makeRes(2298, 3, newImage(0x2750u, 0x62u));
    resources[1292] = makeRes(2299, 3, newImage(0x2750u, 0x63u));
    resources[1293] = makeRes(2300, 3, newImage(0x2750u, 0x64u));
    resources[1294] = makeRes(2301, 3, newImage(0x2750u, 0x65u));
    resources[1295] = makeRes(2302, 3, newImage(0x2750u, 0x66u));
    resources[1296] = makeRes(2303, 3, newImage(0x2750u, 0x67u));
    resources[1297] = makeRes(2304, 3, newImage(0x2750u, 0x68u));
    resources[1298] = makeRes(2305, 3, newImage(0x2750u, 0x69u));
    resources[1299] = makeRes(2306, 3, newImage(0x2750u, 0x6au));
    resources[1300] = makeRes(2307, 3, newImage(0x2750u, 0x6bu));
    resources[1301] = makeRes(2308, 3, newImage(0x2750u, 0x6cu));
    resources[1302] = makeRes(2309, 3, newImage(0x2750u, 0x6du));
    resources[1303] = makeRes(2310, 3, newImage(0x2750u, 0x6eu));
    resources[1304] = makeRes(2311, 3, newImage(0x2750u, 0x6fu));
    resources[1305] = makeRes(2312, 3, newImage(0x2750u, 0x70u));
    resources[1306] = makeRes(2313, 3, newImage(0x2750u, 0x71u));
    resources[1307] = makeRes(2314, 3, newImage(0x2750u, 0x72u));
    resources[1308] = makeRes(2315, 3, newImage(0x2750u, 0x73u));
    resources[1309] = makeRes(2316, 3, newImage(0x2750u, 0x74u));
    resources[1310] = makeRes(2317, 3, newImage(0x2750u, 0x75u));
    resources[1311] = makeRes(2318, 3, newImage(0x2750u, 0x76u));
    resources[1312] = makeRes(2319, 3, newImage(0x2750u, 0x77u));
    resources[1313] = makeRes(2320, 3, newImage(0x2750u, 0x78u));
    resources[1314] = makeRes(2321, 3, newImage(0x2750u, 0x79u));
    resources[1315] = makeRes(2322, 3, newImage(0x2750u, 0x7au));
    resources[1316] = makeRes(2323, 3, newImage(0x2750u, 0x7bu));
    resources[1317] = makeRes(2324, 3, newImage(0x2750u, 0x7cu));
    resources[1318] = makeRes(2325, 3, newImage(0x2750u, 0x7du));
    resources[1319] = makeRes(2326, 3, newImage(0x2750u, 0x7eu));
    resources[1320] = makeRes(2327, 3, newImage(0x2750u, 0x7fu));
    resources[1321] = makeRes(2328, 3, newImage(0x2750u, 0x80u));
    resources[1322] = makeRes(2329, 3, newImage(0x2750u, 0x81u));
    resources[1323] = makeRes(2330, 3, newImage(0x2750u, 0x82u));
    resources[1324] = makeRes(2331, 3, newImage(0x2750u, 0x83u));
    resources[1325] = makeRes(2332, 3, newImage(0x2750u, 0x84u));
    resources[1326] = makeRes(2333, 3, newImage(0x2750u, 0x85u));
    resources[1327] = makeRes(2334, 3, newImage(0x2750u, 0x86u));
    resources[1328] = makeRes(2335, 3, newImage(0x2750u, 0x87u));
    resources[1329] = makeRes(2336, 3, newImage(0x2750u, 0x88u));
    resources[1330] = makeRes(2337, 3, newImage(0x2750u, 0x89u));
    resources[1331] = makeRes(2338, 3, newImage(0x2750u, 0x8au));
    resources[1332] = makeRes(2339, 3, newImage(0x2750u, 0x8bu));
    resources[1333] = makeRes(2340, 3, newImage(0x2750u, 0x8cu));
    resources[1334] = makeRes(2341, 3, newImage(0x2750u, 0x8du));
    resources[1335] = makeRes(2342, 3, newImage(0x2750u, 0x8eu));
    resources[1336] = makeRes(2343, 3, newImage(0x2750u, 0x8fu));
    resources[1337] = makeRes(2344, 3, newImage(0x2750u, 0x90u));
    resources[1338] = makeRes(2345, 3, newImage(0x2750u, 0x91u));
    resources[1339] = makeRes(2346, 3, newImage(0x2750u, 0x92u));
    resources[1340] = makeRes(2347, 3, newImage(0x2750u, 0x93u));
    resources[1341] = makeRes(2348, 3, newImage(0x2750u, 0x94u));
    resources[1342] = makeRes(2349, 3, newImage(0x2750u, 0x95u));
    resources[1343] = makeRes(2350, 3, newImage(0x2750u, 0x96u));
    resources[1344] = makeRes(2351, 3, newImage(0x2750u, 0x97u));
    resources[1345] = makeRes(2352, 3, newImage(0x2750u, 0x98u));
    resources[1346] = makeRes(2353, 3, newImage(0x2750u, 0x99u));
    resources[1347] = makeRes(2354, 3, newImage(0x2750u, 0x9au));
    resources[1348] = makeRes(2355, 3, newImage(0x2750u, 0x9bu));
    resources[1349] = makeRes(2356, 3, newImage(0x2750u, 0x9cu));
    resources[1350] = makeRes(2357, 3, newImage(0x2750u, 0x9du));
    resources[1351] = makeRes(2358, 3, newImage(0x2750u, 0x9eu));
    resources[1352] = makeRes(2359, 3, newImage(0x2750u, 0x9fu));
    resources[1353] = makeRes(2360, 3, newImage(0x2750u, 0xa0u));
    resources[1354] = makeRes(2361, 3, newImage(0x2750u, 0xa1u));
    resources[1355] = makeRes(2362, 3, newImage(0x2750u, 0xa2u));
    resources[1356] = makeRes(2363, 3, newImage(0x2750u, 0xa3u));
    resources[1357] = makeRes(2364, 3, newImage(0x2750u, 0xa4u));
    resources[1358] = makeRes(2365, 3, newImage(0x2750u, 0xa5u));
    resources[1359] = makeRes(2366, 3, newImage(0x2750u, 0xa6u));
    resources[1360] = makeRes(2367, 3, newImage(0x2750u, 0xa7u));
    resources[1361] = makeRes(2368, 3, newImage(0x2750u, 0xa8u));
    resources[1362] = makeRes(2369, 3, newImage(0x2750u, 0xa9u));
    resources[1363] = makeRes(2370, 3, newImage(0x2750u, 0xaau));
    resources[1364] = makeRes(2371, 3, newImage(0x2750u, 0xabu));
    resources[1365] = makeRes(2372, 3, newImage(0x2750u, 0xacu));
    resources[1366] = makeRes(2373, 3, newImage(0x2750u, 0xadu));
    resources[1367] = makeRes(2374, 3, newImage(0x2750u, 0xaeu));
    resources[1368] = makeRes(2375, 3, newImage(0x2750u, 0xafu));
    resources[1369] = makeRes(2376, 3, newImage(0x2750u, 0xb0u));
    resources[1370] = makeRes(2377, 3, newImage(0x2750u, 0xb1u));
    resources[1371] = makeRes(2378, 3, newImage(0x2750u, 0xb2u));
    resources[1372] = makeRes(2379, 3, newImage(0x2750u, 0xb3u));
    resources[1373] = makeRes(2380, 3, newImage(0x2750u, 0xb4u));
    resources[1374] = makeRes(2381, 3, newImage(0x2750u, 0xb5u));
    resources[1375] = makeRes(2382, 3, newImage(0x2750u, 0xb6u));
    resources[1376] = makeRes(2383, 3, newImage(0x2750u, 0xb7u));
    resources[1377] = makeRes(2384, 3, newImage(0x2750u, 0xb8u));
    resources[1378] = makeRes(2385, 3, newImage(0x2750u, 0xb9u));
    resources[1379] = makeRes(2386, 3, newImage(0x2750u, 0xbau));
    resources[1380] = makeRes(2387, 3, newImage(0x2750u, 0xbbu));
    resources[1381] = makeRes(2388, 3, newImage(0x2750u, 0xbcu));
    resources[1382] = makeRes(2389, 3, newImage(0x2750u, 0xbdu));
    resources[1383] = makeRes(2390, 3, newImage(0x2750u, 0xbeu));
    resources[1384] = makeRes(2391, 3, newImage(0x2750u, 0xbfu));
    resources[1385] = makeRes(2392, 3, newImage(0x2750u, 0xc0u));
    resources[1386] = makeRes(2393, 3, newImage(0x2750u, 0xc1u));
    resources[1387] = makeRes(2394, 3, newImage(0x2750u, 0xc2u));
    resources[1388] = makeRes(2395, 3, newImage(0x2750u, 0xc3u));
    resources[1389] = makeRes(2396, 3, newImage(0x2750u, 0xc4u));
    resources[1390] = makeRes(2397, 3, newImage(0x2750u, 0xc5u));
    resources[1391] = makeRes(2398, 3, newImage(0x2750u, 0xc6u));
    resources[1392] = makeRes(2399, 3, newImage(0x2750u, 0xc7u));
    resources[1393] = makeRes(2400, 3, newImage(0x2750u, 0xc8u));
    resources[1394] = makeRes(2401, 3, newImage(0x2750u, 0xc9u));
    resources[1395] = makeRes(2402, 3, newImage(0x2750u, 0xcau));
    resources[1396] = makeRes(2403, 3, newImage(0x2750u, 0xcbu));
    resources[1397] = makeRes(2404, 3, newImage(0x2750u, 0xccu));
    resources[1398] = makeRes(2405, 3, newImage(0x2750u, 0xcdu));
    resources[1399] = makeRes(2406, 3, newImage(0x2750u, 0xceu));
    resources[1400] = makeRes(2407, 3, newImage(0x2750u, 0xcfu));
    resources[1401] = makeRes(2408, 3, newImage(0x2750u, 0xd0u));
    resources[1402] = makeRes(2409, 3, newImage(0x2750u, 0xd1u));
    resources[1403] = makeRes(2410, 3, newImage(0x2750u, 0xd2u));
    resources[1404] = makeRes(2411, 3, newImage(0x2750u, 0xd3u));
    resources[1405] = makeRes(2412, 3, newImage(0x2750u, 0xd4u));
    resources[1406] = makeRes(2413, 3, newImage(0x2750u, 0xd5u));
    resources[1407] = makeRes(2414, 3, newImage(0x2750u, 0xd6u));
    resources[1408] = makeRes(2415, 3, newImage(0x2750u, 0xd7u));
    resources[1409] = makeRes(2416, 3, newImage(0x2750u, 0xd8u));
    resources[1410] = makeRes(2417, 3, newImage(0x2750u, 0xd9u));
    resources[1411] = makeRes(2418, 3, newImage(0x2750u, 0xdau));
    resources[1412] = makeRes(2419, 3, newImage(0x2750u, 0xdbu));
    resources[1413] = makeRes(2420, 3, newImage(0x2750u, 0xdcu));
    resources[1414] = makeRes(2421, 3, newImage(0x2750u, 0xddu));
    resources[1415] = makeRes(2422, 3, newImage(0x2750u, 0xdeu));
    resources[1416] = makeRes(2423, 3, newImage(0x2750u, 0xdfu));
    resources[1417] = makeRes(2424, 3, newImage(0x2750u, 0xe0u));
    resources[1418] = makeRes(2425, 3, newImage(0x2750u, 0xe1u));
    resources[1419] = makeRes(2426, 3, newImage(0x2750u, 0xe2u));
    resources[1420] = makeRes(2427, 3, newImage(0x2750u, 0xe3u));
    resources[1421] = makeRes(2428, 3, newImage(0x2750u, 0xe4u));
    resources[1422] = makeRes(2429, 3, newImage(0x2750u, 0xe5u));
    resources[1423] = makeRes(2430, 3, newImage(0x2750u, 0xe6u));
    resources[1424] = makeRes(2431, 3, newImage(0x2750u, 0xe7u));
    resources[1425] = makeRes(2432, 3, newImage(0x2750u, 0xe8u));
    resources[1426] = makeRes(2433, 3, newImage(0x2750u, 0xe9u));
    resources[1427] = makeRes(2434, 3, newImage(0x2750u, 0xeau));
    resources[1428] = makeRes(2435, 3, newImage(0x2750u, 0xebu));
    resources[1429] = makeRes(2436, 3, newImage(0x2750u, 0xecu));
    resources[1430] = makeRes(2437, 3, newImage(0x2750u, 0xedu));
    resources[1431] = makeRes(2438, 3, newImage(0x2750u, 0xeeu));
    resources[1432] = makeRes(2439, 3, newImage(0x2750u, 0xefu));
    resources[1433] = makeRes(2440, 3, newImage(0x2750u, 0xf0u));
    resources[1434] = makeRes(2441, 3, newImage(0x2750u, 0xf1u));
    resources[1435] = makeRes(2442, 3, newImage(0x2750u, 0xf2u));
    resources[1436] = makeRes(2443, 3, newImage(0x2750u, 0xf3u));
    resources[1437] = makeRes(2444, 3, newImage(0x2750u, 0xf4u));
    resources[1438] = makeRes(2445, 3, newImage(0x2750u, 0xf5u));
    resources[1439] = makeRes(2446, 3, newImage(0x2750u, 0xf6u));
    resources[1440] = makeRes(2447, 3, newImage(0x2750u, 0xf7u));
    resources[1441] = makeRes(2448, 3, newImage(0x2750u, 0xf8u));
    resources[1442] = makeRes(2449, 3, newImage(0x2750u, 0xf9u));
    resources[1443] = makeRes(2450, 3, newImage(0x2750u, 0xfau));
    resources[1444] = makeRes(2451, 3, newImage(0x2750u, 0xfbu));
    resources[1445] = makeRes(2452, 3, newImage(0x2750u, 0xfcu));
    resources[1446] = makeRes(2453, 3, newImage(0x2750u, 0xfdu));
    resources[1447] = makeRes(2454, 3, newImage(0x2750u, 0xfeu));
    resources[1448] = makeRes(2455, 3, newImage(0x2750u, 0xffu));
    resources[1449] = makeRes(2456, 3, newImage(0x2750u, 0x100u));
    resources[1450] = makeRes(2457, 3, newImage(0x2750u, 0x101u));
    resources[1451] = makeRes(2458, 3, newImage(0x2750u, 0x102u));
    resources[1452] = makeRes(2459, 3, newImage(0x2750u, 0x103u));
    resources[1453] = makeRes(2460, 3, newImage(0x2750u, 0x104u));
    resources[1454] = makeRes(2461, 3, newImage(0x2750u, 0x105u));
    resources[1455] = makeRes(2462, 3, newImage(0x2750u, 0x106u));
    resources[1456] = makeRes(2463, 3, newImage(0x2750u, 0x107u));
    resources[1457] = makeRes(2464, 3, newImage(0x2750u, 0x108u));
    resources[1458] = makeRes(2465, 3, newImage(0x2750u, 0x109u));
    resources[1459] = makeRes(2466, 3, newImage(0x2750u, 0x10au));
    resources[1460] = makeRes(2467, 3, newImage(0x2750u, 0x10bu));
    resources[1461] = makeRes(2468, 3, newImage(0x2750u, 0x10cu));
    resources[1462] = makeRes(2469, 3, newImage(0x2750u, 0x10du));
    resources[1463] = makeRes(2470, 3, newImage(0x2750u, 0x10eu));
    resources[1464] = makeRes(2471, 3, newImage(0x2750u, 0x10fu));
    resources[1465] = makeRes(4000, 3, newImage(0x2d70u, 0x0u));
    resources[1466] = makeRes(4001, 3, newImage(0x2d70u, 0x1u));
    resources[1467] = makeRes(4002, 3, newImage(0x2d70u, 0x2u));
    resources[1468] = makeRes(4003, 3, newImage(0x2d70u, 0x3u));
    resources[1469] = makeRes(4004, 3, newImage(0x2d70u, 0x4u));
    resources[1470] = makeRes(4005, 3, newImage(0x2d70u, 0x5u));
    resources[1471] = makeRes(4006, 3, newImage(0x2d70u, 0x6u));
    resources[1472] = makeRes(4007, 3, newImage(0x2d70u, 0x7u));
    resources[1473] = makeRes(4008, 3, newImage(0x2d70u, 0x8u));
    resources[1474] = makeRes(4009, 3, newImage(0x2d70u, 0x9u));
    resources[1475] = makeRes(4010, 3, newImage(0x2d70u, 0xau));
    resources[1476] = makeRes(4011, 3, newImage(0x2d70u, 0xbu));
    resources[1477] = makeRes(4012, 3, newImage(0x2d70u, 0xcu));
    resources[1478] = makeRes(4013, 3, newImage(0x2d70u, 0xdu));
    resources[1479] = makeRes(4014, 3, newImage(0x2d70u, 0xeu));
    resources[1480] = makeRes(4015, 3, newImage(0x2d70u, 0xfu));
    resources[1481] = makeRes(4016, 3, newImage(0x2d70u, 0x10u));
    resources[1482] = makeRes(4017, 3, newImage(0x2d70u, 0x11u));
    resources[1483] = makeRes(4018, 3, newImage(0x2d70u, 0x12u));
    resources[1484] = makeRes(4019, 3, newImage(0x2d70u, 0x13u));
    resources[1485] = makeRes(4020, 3, newImage(0x2d70u, 0x14u));
    resources[1486] = makeRes(4021, 3, newImage(0x2d70u, 0x15u));
    resources[1487] = makeRes(4022, 3, newImage(0x2d70u, 0x16u));
    resources[1488] = makeRes(4023, 3, newImage(0x2d70u, 0x17u));
    resources[1489] = makeRes(4024, 3, newImage(0x2d70u, 0x18u));
    resources[1490] = makeRes(4025, 3, newImage(0x2d70u, 0x19u));
    resources[1491] = makeRes(4026, 3, newImage(0x2d70u, 0x1au));
    resources[1492] = makeRes(4027, 3, newImage(0x2d70u, 0x1bu));
    resources[1493] = makeRes(4028, 3, newImage(0x2d70u, 0x1cu));
    resources[1494] = makeRes(4029, 3, newImage(0x2d70u, 0x1du));
    resources[1495] = makeRes(4030, 3, newImage(0x2d70u, 0x1eu));
    resources[1496] = makeRes(4031, 3, newImage(0x2d70u, 0x1fu));
    resources[1497] = makeRes(4032, 3, newImage(0x2d70u, 0x20u));
    resources[1498] = makeRes(4033, 3, newImage(0x2d70u, 0x21u));
    resources[1499] = makeRes(4034, 3, newImage(0x2d70u, 0x22u));
    resources[1500] = makeRes(4035, 3, newImage(0x2d70u, 0x23u));
    resources[1501] = makeRes(4036, 3, newImage(0x2d70u, 0x24u));
    resources[1502] = makeRes(4037, 3, newImage(0x2d70u, 0x25u));
    resources[1503] = makeRes(4038, 3, newImage(0x2d70u, 0x26u));
    resources[1504] = makeRes(4039, 3, newImage(0x2d70u, 0x27u));
    resources[1505] = makeRes(4040, 3, newImage(0x2d70u, 0x28u));
    resources[1506] = makeRes(4041, 3, newImage(0x2d70u, 0x29u));
    resources[1507] = makeRes(4042, 3, newImage(0x2d70u, 0x2au));
    resources[1508] = makeRes(4043, 3, newImage(0x2d70u, 0x2bu));
    resources[1509] = makeRes(4044, 3, newImage(0x2d70u, 0x2cu));
    resources[1510] = makeRes(4045, 3, newImage(0x2d70u, 0x2du));
    resources[1511] = makeRes(4046, 3, newImage(0x2d70u, 0x2eu));
    resources[1512] = makeRes(4047, 3, newImage(0x2d70u, 0x2fu));
    resources[1513] = makeRes(4048, 3, newImage(0x2d70u, 0x30u));
    resources[1514] = makeRes(4049, 3, newImage(0x2d70u, 0x31u));
    resources[1515] = makeRes(4050, 3, newImage(0x2d70u, 0x32u));
    resources[1516] = makeRes(4051, 3, newImage(0x2d70u, 0x33u));
    resources[1517] = makeRes(4052, 3, newImage(0x2d70u, 0x34u));
    resources[1518] = makeRes(4053, 3, newImage(0x2d70u, 0x35u));
    resources[1519] = makeRes(4054, 3, newImage(0x2d70u, 0x36u));
    resources[1520] = makeRes(4055, 3, newImage(0x2d70u, 0x37u));
    resources[1521] = makeRes(4056, 3, newImage(0x2d70u, 0x38u));
    resources[1522] = makeRes(2472, 3, newImage(0x2d70u, 0x39u));
    resources[1523] = makeRes(2473, 3, newImage(0x2d70u, 0x3au));
    resources[1524] = makeRes(2474, 3, newImage(0x2d70u, 0x3bu));
    resources[1525] = makeRes(2475, 3, newImage(0x2d70u, 0x3cu));
    resources[1526] = makeRes(2476, 3, newImage(0x2d70u, 0x3du));
    resources[1527] = makeRes(2477, 3, newImage(0x2d70u, 0x3eu));
    resources[1528] = makeRes(2478, 3, newImage(0x2d70u, 0x3fu));
    resources[1529] = makeRes(2479, 3, newImage(0x2d70u, 0x40u));
    resources[1530] = makeRes(2480, 3, newImage(0x2d70u, 0x41u));
    resources[1531] = makeRes(5500, 3, newImage(0x2750u, 0x0u));
    resources[1532] = makeRes(5501, 3, newImage(0x2750u, 0x1u));
    resources[1533] = makeRes(5502, 3, newImage(0x2750u, 0x2u));
    resources[1534] = makeRes(7000, 3, newImage(0x2712u, 0x0u));
    resources[1535] = makeRes(7001, 3, newImage(0x2712u, 0x1u));
    resources[1536] = makeRes(7002, 3, newImage(0x2712u, 0x2u));
    resources[1537] = makeRes(7003, 3, newImage(0x2712u, 0x3u));
    resources[1538] = makeRes(7004, 3, newImage(0x2712u, 0x4u));
    resources[1539] = makeRes(6000, 3, newImage(0x6acdu, 0x0u));
    resources[1540] = makeRes(6001, 3, newImage(0x6acdu, 0x1u));
    resources[1541] = makeRes(6002, 3, newImage(0x6acdu, 0x2u));
    resources[1542] = makeRes(6003, 3, newImage(0x6acdu, 0x3u));
    resources[1543] = makeRes(6004, 3, newImage(0x6acdu, 0x4u));
    resources[1544] = makeRes(6005, 3, newImage(0x6acdu, 0x5u));
    resources[1545] = makeRes(6006, 3, newImage(0x6acdu, 0x6u));
    resources[1546] = makeRes(6007, 3, newImage(0x6acdu, 0x7u));
    resources[1547] = makeRes(6008, 3, newImage(0x6acdu, 0x8u));
    resources[1548] = makeRes(6009, 3, newImage(0x6acdu, 0x9u));
    resources[1549] = makeRes(6010, 3, newImage(0x6acdu, 0xau));
    resources[1550] = makeRes(6600, 3, newImage(0x733cu, 0x0u));
    resources[1551] = makeRes(6601, 3, newImage(0x733cu, 0x1u));
    resources[1552] = makeRes(6602, 3, newImage(0x733cu, 0x2u));
    resources[1553] = makeRes(6603, 3, newImage(0x733cu, 0x3u));
    resources[1554] = makeRes(6604, 3, newImage(0x733cu, 0x4u));
    resources[1555] = makeRes(6605, 3, newImage(0x733cu, 0x5u));
    resources[1556] = makeRes(6606, 3, newImage(0x733cu, 0x6u));
    resources[1557] = makeRes(6607, 3, newImage(0x733cu, 0x7u));
    resources[1558] = makeRes(6650, 3, newImage(0x733du, 0x0u));
    resources[1559] = makeRes(
        14379, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian.aem", 24071,
                                                false));
    resources[1560] = makeRes(
        14310, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_add.aem", 20069,
                                                false));
    resources[1561] = makeRes(
        14264, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_alpha.aem", 20106,
                                                false));
    resources[1562] = makeRes(
        14377, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_vossk.aem", 24074, false));
    resources[1563] = makeRes(
        14304, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_vossk_anim_add.aem", 20071,
                                                false));
    resources[1564] = makeRes(
        14378, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian.aem", 24073,
                                                false));
    resources[1565] = makeRes(
        14307, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_alpha.aem", 20074,
                                                false));
    resources[1566] = makeRes(
        14308, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_add.aem", 20075,
                                                false));
    resources[1567] = makeRes(
        14376, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran.aem", 24070, false));
    resources[1568] = makeRes(
        14301, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_alpha.aem", 20077,
                                                false));
    resources[1569] = makeRes(
        14302, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_add.aem", 20078,
                                                false));
    resources[1570] = makeRes(
        14354, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/hangars/v_hangar_battlestation.aem",
                                                20157, false));
    resources[1571] = makeRes(14355, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/hangars/v_hangar_battlestation_emissive.aem", 27138,
                                  false));
    resources[1572] = makeRes(14356, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/hangars/v_hangar_battlestation_alpha.aem", 27139,
                                  false));
    resources[1573] = makeRes(
        14357, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/hangars/v_hangar_battlestation_add.aem",
                                                27140, false));
    resources[1574] = makeRes(14358, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/hangars/v_hangar_deep_science_emissive.aem", 27141,
                                  false));
    resources[1575] = makeRes(
        14359, 4, new AbyssEngine::ResourceMesh(
            "data/assets/valkyrie/3d/meshes/hangars/v_hangar_deep_science_alpha.aem", 27142, false));
    resources[1576] = makeRes(14360, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/hangars/v_hangar_deep_science_anim_add.aem", 27143,
                                  false));
    resources[1577] = makeRes(
        14362, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/hangars/v_hangar_deep_science.aem",
                                                27144, false));
    resources[1578] = makeRes(
        14265, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_a_anim.aem",
                                                24070, false));
    resources[1579] = makeRes(
        14266, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_b_anim.aem",
                                                24070, false));
    resources[1580] = makeRes(
        14267, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_c_anim.aem",
                                                24070, false));
    resources[1581] = makeRes(
        14268, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_d_anim.aem",
                                                24070, false));
    resources[1582] = makeRes(
        14269, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_a_anim_add.aem",
                                                20078, false));
    resources[1583] = makeRes(
        14270, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_b_anim_add.aem",
                                                20078, false));
    resources[1584] = makeRes(
        14271, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_c_anim_add.aem",
                                                20078, false));
    resources[1585] = makeRes(
        14272, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_bot_d_anim_add.aem",
                                                20078, false));
    resources[1586] = makeRes(
        14273, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_bot_a_anim.aem",
                                                24073, false));
    resources[1587] = makeRes(
        14274, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_bot_b_anim.aem",
                                                24073, false));
    resources[1588] = makeRes(
        14275, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_bot_c_anim.aem",
                                                24073, false));
    resources[1589] = makeRes(
        14276, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_bot_a_anim_add.aem",
                                                20075, false));
    resources[1590] = makeRes(
        14277, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_bot_b_anim_add.aem",
                                                20075, false));
    resources[1591] = makeRes(
        14278, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_bot_c_anim_add.aem",
                                                20075, false));
    resources[1592] = makeRes(
        14380, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x1.aem", 24070, false));
    resources[1593] = makeRes(
        14381, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x1_shadow_alpha.aem",
                                                20077, false));
    resources[1594] = makeRes(
        14382, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x2.aem", 24070, false));
    resources[1595] = makeRes(
        14383, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x2_shadow_alpha.aem",
                                                20077, false));
    resources[1596] = makeRes(
        14384, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x3.aem", 24070, false));
    resources[1597] = makeRes(
        14385, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x3_shadow_alpha.aem",
                                                20077, false));
    resources[1598] = makeRes(
        14386, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x4.aem", 24070, false));
    resources[1599] = makeRes(
        14387, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x4_shadow_alpha.aem",
                                                20077, false));
    resources[1600] = makeRes(
        14388, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x5.aem", 24070, false));
    resources[1601] = makeRes(
        14389, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x5_shadow_alpha.aem",
                                                20077, false));
    resources[1602] = makeRes(
        14390, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_terran_x6.aem", 24070, false));
    resources[1603] = makeRes(
        14391, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_x1.aem", 24071,
                                                false));
    resources[1604] = makeRes(
        14392, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_midorian_x1_shadow_alpha.aem", 20106, false));
    resources[1605] = makeRes(
        14393, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_x2.aem", 24071,
                                                false));
    resources[1606] = makeRes(
        14394, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_midorian_x2_shadow_alpha.aem", 20106, false));
    resources[1607] = makeRes(
        14395, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_x3.aem", 24071,
                                                false));
    resources[1608] = makeRes(
        14396, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_midorian_x3_shadow_alpha.aem", 20106, false));
    resources[1609] = makeRes(
        14397, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_x4.aem", 24071,
                                                false));
    resources[1610] = makeRes(
        14398, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_midorian_x4_shadow_alpha.aem", 20106, false));
    resources[1611] = makeRes(
        14399, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_x5.aem", 24071,
                                                false));
    resources[1612] = makeRes(
        14400, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_midorian_x5_shadow_alpha.aem", 20106, false));
    resources[1613] = makeRes(
        14401, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_midorian_x5.aem", 24071,
                                                false));
    resources[1614] = makeRes(
        14402, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_midorian_x5_shadow_alpha.aem", 20106, false));
    resources[1615] = makeRes(
        14403, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_x1.aem", 24073,
                                                false));
    resources[1616] = makeRes(
        14404, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_nivelian_x1_shadow_alpha.aem", 20074, false));
    resources[1617] = makeRes(
        14405, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_x2.aem", 24073,
                                                false));
    resources[1618] = makeRes(
        14406, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_nivelian_x2_shadow_alpha.aem", 20074, false));
    resources[1619] = makeRes(
        14407, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_x3.aem", 24073,
                                                false));
    resources[1620] = makeRes(
        14408, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_nivelian_x3_shadow_alpha.aem", 20074, false));
    resources[1621] = makeRes(
        14409, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_x4.aem", 24073,
                                                false));
    resources[1622] = makeRes(
        14410, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_nivelian_x4_shadow_alpha.aem", 20074, false));
    resources[1623] = makeRes(
        14411, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/hangars/hangar_nivelian_x5.aem", 24073,
                                                false));
    resources[1624] = makeRes(
        14412, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/hangars/hangar_nivelian_x5_shadow_alpha.aem", 20074, false));
    resources[1625] = makeRes(
        14025, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_terran.aem", 20056, false));
    resources[1626] = makeRes(
        14026, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_terran_add.aem", 20057, false));
    resources[1627] = makeRes(
        14027, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_terran_alpha.aem", 20058, false));
    resources[1628] = makeRes(
        14280, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_terran_bot_anim.aem", 20056, false));
    resources[1629] = makeRes(
        14279, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_terran_bot_anim_add.aem", 20057,
                                                false));
    resources[1630] = makeRes(
        14028, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_vossk.aem", 20059, false));
    resources[1631] = makeRes(
        14029, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_vossk_anim_add.aem", 20060,
                                                false));
    resources[1632] = makeRes(
        14031, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_nivelian.aem", 20062, false));
    resources[1633] = makeRes(
        14032, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_nivelian_anim_add.aem", 20063,
                                                false));
    resources[1634] = makeRes(
        14033, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_nivelian_alpha.aem", 20064,
                                                false));
    resources[1635] = makeRes(
        14034, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_midorian_anim.aem", 20065, false));
    resources[1636] = makeRes(
        14035, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_midorian_add.aem", 20066, false));
    resources[1637] = makeRes(
        14036, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_midorian_alpha.aem", 20067,
                                                false));
    resources[1638] = makeRes(
        14038, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_midorian_alpha_anim.aem", 20067,
                                                false));
    resources[1639] = makeRes(
        14725, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_terran_m.aem", 20119,
                                                false));
    resources[1640] = makeRes(
        14724, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_terran_f.aem", 20118,
                                                false));
    resources[1641] = makeRes(
        14726, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_vossk.aem", 20120, false));
    resources[1642] = makeRes(
        14723, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_nivelian.aem", 20117,
                                                false));
    resources[1643] = makeRes(
        14722, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_multipod.aem", 20116,
                                                false));
    resources[1644] = makeRes(
        14720, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_bobolan.aem", 20114, false));
    resources[1645] = makeRes(
        14721, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_grey.aem", 20115, false));
    resources[1646] = makeRes(
        14281, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_glow.aem", 65535, false));
    resources[1647] = makeRes(
        14282, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_glow.aem", 65535, false));
    resources[1648] = makeRes(
        14283, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_glow.aem", 65535, false));
    resources[1649] = makeRes(
        14284, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_glow.aem", 65535, false));
    resources[1650] = makeRes(
        14348, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/bars/bar_visitor_shadow.aem", 65535,
                                                false));
    resources[1651] = makeRes(
        17800, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_000.aem", 20009, false));
    resources[1652] = makeRes(
        17801, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_001.aem", 20010, false));
    resources[1653] = makeRes(
        17802, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_002.aem", 20011, false));
    resources[1654] = makeRes(
        17803, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_003.aem", 20012, false));
    resources[1655] = makeRes(
        17804, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_004.aem", 20013, false));
    resources[1656] = makeRes(
        17805, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_005.aem", 20014, false));
    resources[1657] = makeRes(
        17806, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_006.aem", 20015, false));
    resources[1658] = makeRes(
        17807, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_007.aem", 20016, false));
    resources[1659] = makeRes(
        17808, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_008.aem", 20017, false));
    resources[1660] = makeRes(
        17809, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_009.aem", 20018, false));
    resources[1661] = makeRes(
        17810, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_010.aem", 20019, false));
    resources[1662] = makeRes(
        17811, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/skyboxes/v_skybox_011.aem", 20130,
                                                false));
    resources[1663] = makeRes(
        17812, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/skyboxes/v_skybox_012.aem", 20131,
                                                false));
    resources[1664] = makeRes(
        17813, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/skyboxes/v_skybox_013.aem", 20132,
                                                false));
    resources[1665] = makeRes(
        17814, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/skyboxes/v_skybox_013.aem", 27149,
                                                false));
    resources[1666] = makeRes(
        17850, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_stars.aem", 20009, false));
    resources[1667] = makeRes(
        17851, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_stars.aem", 20010, false));
    resources[1668] = makeRes(
        17852, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_stars.aem", 20011, false));
    resources[1669] = makeRes(
        6768, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 65535, false));
    resources[1670] = makeRes(
        16916, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/container_005_void.aem", 65535,
                                                false));
    resources[1671] = makeRes(
        16805, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_emp_anim_lookat_add.aem",
                                                27300, false));
    resources[1672] = makeRes(
        16820, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_anim_lookat_add.aem", 27252,
                                                false));
    resources[1673] = makeRes(
        16821, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_anim_lookat_alpha.aem", 27253,
                                                false));
    resources[1674] = makeRes(16806, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/fx/v_scattergun_000_explosion_lookat_anim_add.aem",
                                  20151, false));
    resources[1675] = makeRes(16807, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/fx/v_scattergun_000_explosion_lookat_anim_add.aem",
                                  20152, false));
    resources[1676] = makeRes(16808, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/fx/v_scattergun_000_explosion_lookat_anim_add.aem",
                                  20153, false));
    resources[1677] = makeRes(
        6754, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_000_anim_add.aem", 24201,
                                               false));
    resources[1678] = makeRes(
        6755, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_001_anim_add.aem", 24201,
                                               false));
    resources[1679] = makeRes(
        6756, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_002_anim_add.aem", 24201,
                                               false));
    resources[1680] = makeRes(
        6760, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_003_anim_add.aem", 24201,
                                               false));
    resources[1681] = makeRes(
        6761, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_004_anim_add.aem", 24201,
                                               false));
    resources[1682] = makeRes(
        6762, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_005_anim_add.aem", 24201,
                                               false));
    resources[1683] = makeRes(
        6763, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_006_anim_add.aem", 24201,
                                               false));
    resources[1684] = makeRes(
        6764, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_007_anim_add.aem", 24201,
                                               false));
    resources[1685] = makeRes(
        6765, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_008_anim_add.aem", 24201,
                                               false));
    resources[1686] = makeRes(
        14297, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_183_anim_add.aem",
                                                24096, false));
    resources[1687] = makeRes(
        6788, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_012_anim_add.aem", 24201,
                                               false));
    resources[1688] = makeRes(
        6789, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_013_anim_add.aem", 24201,
                                               false));
    resources[1689] = makeRes(
        6790, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_014_anim_add.aem", 24201,
                                               false));
    resources[1690] = makeRes(
        6791, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_015_anim_add.aem", 24201,
                                               false));
    resources[1691] = makeRes(
        6792, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_016_anim_add.aem", 24201,
                                               false));
    resources[1692] = makeRes(
        6793, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_017_anim_add.aem", 24201,
                                               false));
    resources[1693] = makeRes(
        6794, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_018_anim_add.aem", 24201,
                                               false));
    resources[1694] = makeRes(
        6795, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_019_anim_add.aem", 24201,
                                               false));
    resources[1695] = makeRes(
        6796, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_020_anim_add.aem", 24201,
                                               false));
    resources[1696] = makeRes(
        6797, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_021_anim_add.aem", 24201,
                                               false));
    resources[1697] = makeRes(
        6798, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_022_anim_add.aem", 24201,
                                               false));
    resources[1698] = makeRes(
        6799, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_023_anim_add.aem", 24201,
                                               false));
    resources[1699] = makeRes(
        6800, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_024_anim_add.aem", 24201,
                                               false));
    resources[1701] = makeRes(
        6802, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_026_anim_add.aem", 24201,
                                               false));
    resources[1702] = makeRes(
        6803, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_027_anim_add.aem", 24201,
                                               false));
    resources[1703] = makeRes(
        14232, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_068_anim_add.aem", 24201,
                                                false));
    resources[1704] = makeRes(
        14233, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_069_anim_add.aem", 24201,
                                                false));
    resources[1705] = makeRes(
        14234, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_070_anim_add.aem", 24201,
                                                false));
    resources[1706] = makeRes(
        14229, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_009_anim_add.aem", 24201,
                                                false));
    resources[1707] = makeRes(
        14230, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_010_anim_add.aem", 24201,
                                                false));
    resources[1708] = makeRes(
        14231, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_011_anim_add.aem", 24201,
                                                false));
    resources[1709] = makeRes(
        14236, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_028_anim_add.aem", 24201,
                                                false));
    resources[1710] = makeRes(
        14237, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_029_anim_add.aem", 24201,
                                                false));
    resources[1711] = makeRes(
        14238, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/projectile_030_anim_add.aem", 24201,
                                                false));
    resources[1712] = makeRes(
        6900, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_176_anim_add.aem", 24096,
                                               false));
    resources[1713] = makeRes(
        6901, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_177_anim_add.aem", 24096,
                                               false));
    resources[1714] = makeRes(
        6902, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_178_anim_add.aem", 24096,
                                               false));
    resources[1715] = makeRes(
        6905, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_180_anim_add.aem", 24096,
                                               false));
    resources[1716] = makeRes(
        6906, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_181_anim_add.aem", 24096,
                                               false));
    resources[1717] = makeRes(
        14239, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_193_anim_add.aem",
                                                24096, false));
    resources[1718] = makeRes(
        14235, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_projectile_194_anim_add.aem",
                                                24096, false));
    resources[1719] = makeRes(
        14247, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/rocket_explosive.aem", 65535, false));
    resources[1720] = makeRes(
        14248, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/rocket_explosive_add.aem", 0, false));
    resources[1721] = makeRes(
        14249, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/rocket_emp.aem", 65535, false));
    resources[1722] = makeRes(
        14250, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/rocket_emp_add.aem", 0, false));
    resources[1723] = makeRes(
        14680, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bomb_explosive_a.aem", 65535, false));
    resources[1724] = makeRes(
        14681, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bomb_explosive_a_add.aem", 0, false));
    resources[1725] = makeRes(
        14682, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bomb_explosive_b.aem", 65535, false));
    resources[1726] = makeRes(
        14683, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bomb_explosive_b_add.aem", 0, false));
    resources[1727] = makeRes(
        14684, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bomb_emp_a.aem", 65535, false));
    resources[1728] = makeRes(
        14685, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bomb_emp_a_add.aem", 0, false));
    resources[1729] = makeRes(
        14293, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_guided_missile_anim.aem", 20125,
                                                false));
    resources[1730] = makeRes(
        14294, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_guided_missile_add.aem", 20126,
                                                false));
    resources[1731] = makeRes(
        15000, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_terran.aem", 20004,
                                                false));
    resources[1732] = makeRes(
        15016, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_terran_lod_1.aem", 20004,
                                                false));
    resources[1733] = makeRes(
        15020, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_terran_lod_2.aem", 20004,
                                                false));
    resources[1734] = makeRes(
        15002, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_terran_emissive.aem",
                                                20003, false));
    resources[1735] = makeRes(
        15001, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_terran_anim_add.aem", 3,
                                                false));
    resources[1736] = makeRes(
        15003, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/jumpgates/jumpgate_terran_jump_anim_add.aem", 3, false));
    resources[1737] = makeRes(
        15004, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_anim.aem", 20032,
                                                false));
    resources[1738] = makeRes(
        15017, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_lod_1_anim.aem",
                                                20032, false));
    resources[1739] = makeRes(
        15021, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_lod_2_anim.aem",
                                                20032, false));
    resources[1740] = makeRes(
        15006, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_emissive.aem",
                                                20031, false));
    resources[1741] = makeRes(
        15024, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_lod_1_emissive.aem", 20031, false));
    resources[1742] = makeRes(
        15025, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_lod_2_emissive.aem", 20031, false));
    resources[1743] = makeRes(
        15005, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_anim_add.aem",
                                                20033, false));
    resources[1744] = makeRes(
        15007, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_vossk_jump_anim_add.aem",
                                                20033, false));
    resources[1745] = makeRes(
        15008, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_nivelian.aem", 20005,
                                                false));
    resources[1746] = makeRes(
        15018, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_nivelian_lod_1.aem",
                                                20005, false));
    resources[1747] = makeRes(
        15022, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_nivelian_lod_2.aem",
                                                20005, false));
    resources[1748] = makeRes(
        15010, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_nivelian_emissive.aem",
                                                20006, false));
    resources[1749] = makeRes(
        15009, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_nivelian_anim_add.aem",
                                                34810, false));
    resources[1750] = makeRes(15011, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/jumpgates/jumpgate_nivelian_jump_anim_add.aem", 34810,
                                  false));
    resources[1751] = makeRes(
        15012, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_midorian.aem", 20007,
                                                false));
    resources[1752] = makeRes(
        15019, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_midorian_lod_1.aem",
                                                20007, false));
    resources[1753] = makeRes(
        15023, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_midorian_lod_2.aem",
                                                20007, false));
    resources[1754] = makeRes(
        15014, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_midorian_emissive.aem",
                                                20008, false));
    resources[1755] = makeRes(
        15013, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/jumpgates/jumpgate_midorian_anim_add.aem",
                                                34600, false));
    resources[1756] = makeRes(15015, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/jumpgates/jumpgate_midorian_jump_anim_add.aem", 34600,
                                  false));
    resources[1757] = makeRes(
        15026, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/khador_jump.aem", 27260, false));
    resources[1758] = makeRes(
        15027, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/hyper_drive.aem", 27262, false));
    resources[1759] = makeRes(
        16917, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/space_junk_001.aem", 20094, false));
    resources[1760] = makeRes(
        16918, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/space_junk_002.aem", 20094, false));
    resources[1761] = makeRes(
        16919, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/space_junk_003.aem", 20094, false));
    resources[1762] = makeRes(
        16920, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/space_junk_004.aem", 20094, false));
    resources[1763] = makeRes(
        16926, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_01_junk.aem", 20023, false));
    resources[1764] = makeRes(
        16927, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_void_junk.aem", 20103,
                                                false));
    resources[1765] = makeRes(
        16992, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/container_003_terran.aem", 27254,
                                                false));
    resources[1766] = makeRes(
        16990, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/container_001_midorian.aem", 27257,
                                                false));
    resources[1767] = makeRes(
        16991, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/container_004_vossk.aem", 27255, false));
    resources[1768] = makeRes(
        16993, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/container_002_nivelian.aem", 27256,
                                                false));
    resources[1769] = makeRes(
        16994, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/wormhole_anim_add.aem", 20093, false));
    resources[1770] = makeRes(
        16921, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_void.aem", 20103, false));
    resources[1771] = makeRes(
        16922, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_void_lod_1.aem", 20103, false));
    resources[1772] = makeRes(
        16923, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_void_lod_2.aem", 20103, false));
    resources[1773] = makeRes(
        16924, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_void_lod_3.aem", 20103, false));
    resources[1774] = makeRes(
        16925, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_void_explosion_anim.aem",
                                                20103, false));
    resources[1775] = makeRes(
        14311, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/battleship_terran.aem", 0, false));
    resources[1776] = makeRes(
        14312, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/battleship_terran_lod_1.aem", 0,
                                                false));
    resources[1777] = makeRes(
        14313, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/battleship_terran_lod_2.aem", 0,
                                                false));
    resources[1778] = makeRes(
        14315, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/battleship_terran_lights_add.aem",
                                                65535, false));
    resources[1779] = makeRes(
        14316, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/battleship_terran_lights_emissive.aem", 65535, false));
    resources[1780] = makeRes(
        18304, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/battleship_terran_explosion_anim.aem",
                                                0, false));
    resources[1781] = makeRes(
        17060, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_002_nivelian.aem", 34811, false));
    resources[1782] = makeRes(
        17062, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_002_nivelian_lod_1.aem", 34811,
                                                false));
    resources[1783] = makeRes(
        17063, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_002_nivelian_lod_2.aem", 34811,
                                                false));
    resources[1784] = makeRes(
        17064, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_002_nivelian_engine_add.aem",
                                                34810, false));
    resources[1785] = makeRes(
        17061, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_002_nivelian_lights_add.aem",
                                                65535, false));
    resources[1786] = makeRes(
        18301, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/cargo_002_nivelian_explosion_anim.aem", 34811, false));
    resources[1787] = makeRes(
        17065, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran.aem", 34811, false));
    resources[1788] = makeRes(
        17066, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran_lod_1.aem", 34811,
                                                false));
    resources[1789] = makeRes(
        17067, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran_lod_2.aem", 34811,
                                                false));
    resources[1790] = makeRes(
        17069, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran_engine_add.aem", 3,
                                                false));
    resources[1791] = makeRes(
        17074, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran_lights_add.aem", 0,
                                                false));
    resources[1792] = makeRes(
        17070, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran_lights_emissive.aem",
                                                0, false));
    resources[1793] = makeRes(
        18302, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_003_terran_explosion_anim.aem",
                                                34811, false));
    resources[1794] = makeRes(
        17049, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_001_midorian.aem", 0, false));
    resources[1795] = makeRes(
        17050, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_001_midorian_lod_1.aem", 0,
                                                false));
    resources[1796] = makeRes(
        17051, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_001_midorian_lod_2.aem", 0,
                                                false));
    resources[1797] = makeRes(
        17052, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_001_midorian_container.aem", 0,
                                                false));
    resources[1798] = makeRes(
        17053, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/cargo_001_midorian_container_lod_1.aem", 0, false));
    resources[1799] = makeRes(
        17054, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_001_midorian_engine_add.aem",
                                                65535, false));
    resources[1800] = makeRes(17055, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/ships/cargo_001_midorian_lights_emissive.aem", 65535,
                                  false));
    resources[1801] = makeRes(
        18300, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/cargo_001_midorian_explosion_anim.aem", 0, false));
    resources[1802] = makeRes(
        17038, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_004_vossk_engine_add.aem",
                                                65535, false));
    resources[1803] = makeRes(
        18303, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_004_vossk_explosion_anim.aem",
                                                0, false));
    resources[1804] = makeRes(
        17139, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_004_vossk_lod_1.aem", 0,
                                                false));
    resources[1805] = makeRes(
        17140, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_004_vossk_lod_2.aem", 0,
                                                false));
    resources[1806] = makeRes(
        18713, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_004_vossk_lights_emissive.aem",
                                                65535, false));
    resources[1807] = makeRes(
        17013, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/cargo_004_vossk.aem", 0, false));
    resources[1808] = makeRes(14363, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_turret.aem", 20135,
                                  false));
    resources[1809] = makeRes(14364, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_turret_gun.aem",
                                  20135, false));
    resources[1810] = makeRes(14365, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_shield.aem", 20135,
                                  false));
    resources[1811] = makeRes(14366, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_laser_anim_add.aem",
                                  20158, false));
    resources[1812] = makeRes(
        6770, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/turrets/turret_001.aem", 24075, false));
    resources[1813] = makeRes(
        6771, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/turrets/turret_001_gun.aem", 24075, false));
    resources[1814] = makeRes(
        6772, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/turrets/turret_002.aem", 24076, false));
    resources[1815] = makeRes(
        6773, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/turrets/turret_002_gun.aem", 24076, false));
    resources[1816] = makeRes(
        6774, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/turrets/turret_003.aem", 24077, false));
    resources[1817] = makeRes(
        6775, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/turrets/turret_003_gun.aem", 24077, false));
    resources[1818] = makeRes(
        18842, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_turret_004.aem", 24085,
                                                false));
    resources[1819] = makeRes(
        18843, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_turret_004_gun.aem", 24085,
                                                false));
    resources[1820] = makeRes(
        18844, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_turret_004_add.aem", 24086,
                                                false));
    resources[1821] = makeRes(
        18845, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/turrets/sn_turret_004_gun_add.aem",
                                                24086, false));
    resources[1822] = makeRes(
        6805, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/turrets/v_autoturret_001_anim.aem",
                                               24081, false));
    resources[1823] = makeRes(
        6806, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/turrets/v_autoturret_001_gun_anim.aem",
                                               24081, false));
    resources[1824] = makeRes(
        6807, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/turrets/v_autoturret_002.aem", 24082,
                                               false));
    resources[1825] = makeRes(
        6808, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/turrets/v_autoturret_002_gun.aem", 24082,
                                               false));
    resources[1826] = makeRes(
        6809, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/turrets/v_autoturret_003.aem", 24083,
                                               false));
    resources[1827] = makeRes(
        6810, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/turrets/v_autoturret_003_gun.aem", 24083,
                                               false));
    resources[1828] = makeRes(
        14050, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_mine_001.aem", 20121, false));
    resources[1829] = makeRes(
        14051, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_mine_001_add.aem", 20154,
                                                false));
    resources[1830] = makeRes(
        14054, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_mine_002.aem", 20122, false));
    resources[1831] = makeRes(
        14055, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_mine_002_add.aem", 20155,
                                                false));
    resources[1832] = makeRes(
        14052, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_mine_003.aem", 20123, false));
    resources[1833] = makeRes(
        14053, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_mine_003_add.aem", 20156,
                                                false));
    resources[1834] = makeRes(
        6779, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_orbit.aem", 20029, false));
    resources[1835] = makeRes(
        14288, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/beer.aem", 20111, false));
    resources[1836] = makeRes(
        14289, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/bra.aem", 20111, false));
    resources[1837] = makeRes(
        14290, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/scanner_probe.aem", 20113, false));
    resources[1838] = makeRes(
        14291, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_debris_anim_alpha.aem", 27253,
                                                false));
    resources[1839] = makeRes(
        14292, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_debris_anim_add.aem", 27252,
                                                false));
    resources[1840] = makeRes(
        14600, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_000_lookat_anim_add.aem", 24201,
                                                false));
    resources[1841] = makeRes(
        14601, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_001_lookat_anim_add.aem", 24201,
                                                false));
    resources[1842] = makeRes(
        14602, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_002_lookat_anim_add.aem", 24201,
                                                false));
    resources[1843] = makeRes(
        14603, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_003_lookat_anim_add.aem", 24201,
                                                false));
    resources[1844] = makeRes(
        14604, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_004_lookat_anim_add.aem", 24201,
                                                false));
    resources[1845] = makeRes(
        14605, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_005_lookat_anim_add.aem", 24201,
                                                false));
    resources[1846] = makeRes(
        14606, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/impact_006_lookat_anim_add.aem", 24201,
                                                false));
    resources[1847] = makeRes(
        14500, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_000_anim_add.aem", 24201,
                                                false));
    resources[1848] = makeRes(
        14501, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_001_anim_add.aem", 24201,
                                                false));
    resources[1849] = makeRes(
        14502, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_002_anim_add.aem", 24201,
                                                false));
    resources[1850] = makeRes(
        14503, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_003_anim_add.aem", 24201,
                                                false));
    resources[1851] = makeRes(
        14504, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_004_anim_add.aem", 24201,
                                                false));
    resources[1852] = makeRes(
        14505, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_005_anim_add.aem", 24201,
                                                false));
    resources[1853] = makeRes(
        14506, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_006_anim_add.aem", 24201,
                                                false));
    resources[1854] = makeRes(
        14507, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_007_anim_add.aem", 24201,
                                                false));
    resources[1855] = makeRes(
        14508, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/muzzle_flash_008_anim_add.aem", 24201,
                                                false));
    resources[1856] = makeRes(
        14509, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_muzzle_flash_009_anim_add.aem",
                                                24096, false));
    resources[1857] = makeRes(
        14510, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_muzzle_flash_010_anim_add.aem",
                                                24096, false));
    resources[1858] = makeRes(
        18070, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27280, false));
    resources[1859] = makeRes(
        18071, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27281, false));
    resources[1860] = makeRes(
        18072, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27282, false));
    resources[1861] = makeRes(
        18073, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27283, false));
    resources[1862] = makeRes(
        18074, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27284, false));
    resources[1863] = makeRes(
        18075, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27285, false));
    resources[1864] = makeRes(
        18076, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27286, false));
    resources[1865] = makeRes(
        18077, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27287, false));
    resources[1866] = makeRes(
        18078, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27288, false));
    resources[1867] = makeRes(
        18079, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27289, false));
    resources[1868] = makeRes(
        18080, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27290, false));
    resources[1869] = makeRes(
        27281, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27281, false));
    resources[1870] = makeRes(
        18082, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27288, false));
    resources[1872] = makeRes(
        18084, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27280, false));
    resources[1873] = makeRes(
        18085, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27285, false));
    resources[1875] = makeRes(
        18087, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27282, false));
    resources[1876] = makeRes(
        27286, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/plane.aem", 27286, false));
    resources[1878] = makeRes(
        18181, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_001.aem", 20027, false));
    resources[1879] = makeRes(
        18182, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_002.aem", 20027, false));
    resources[1880] = makeRes(
        20027, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_003.aem", 20027, false));
    resources[1881] = makeRes(
        18184, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_004.aem", 20027, false));
    resources[1882] = makeRes(
        20027, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_005.aem", 20027, false));
    resources[1884] = makeRes(
        18187, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_007.aem", 20027, false));
    resources[1885] = makeRes(
        20027, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_008.aem", 20027, false));
    resources[1886] = makeRes(
        18189, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_009.aem", 20027, false));
    resources[1888] = makeRes(
        18191, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_011.aem", 20027, false));
    resources[1889] = makeRes(
        18192, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_012.aem", 20027, false));
    resources[1891] = makeRes(
        18194, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_014.aem", 20027, false));
    resources[1892] = makeRes(
        18195, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_015.aem", 20027, false));
    resources[1893] = makeRes(
        18196, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_016.aem", 20027, false));
    resources[1894] = makeRes(
        18197, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_017.aem", 20027, false));
    resources[1895] = makeRes(
        18198, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_018.aem", 20027, false));
    resources[1896] = makeRes(
        18199, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_019.aem", 20027, false));
    resources[1897] = makeRes(
        18200, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/galaxymap/v_map_planet_020.aem", 27305,
                                                false));
    resources[1898] = makeRes(
        18201, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/galaxymap/v_map_planet_021.aem", 27305,
                                                false));
    resources[1899] = makeRes(
        18202, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/galaxymap/v_map_planet_022.aem", 27305,
                                                false));
    resources[1900] = makeRes(
        18204, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/galaxymap/sn_map_planet_024.aem",
                                                27306, false));
    resources[1901] = makeRes(
        18205, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/galaxymap/sn_map_planet_025.aem",
                                                27306, false));
    resources[1902] = makeRes(
        18206, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/galaxymap/sn_map_planet_026.aem",
                                                27306, false));
    resources[1903] = makeRes(
        17900, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_000_midorian_engine_glow_add.aem", 34811, false));
    resources[1904] = makeRes(
        17901, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_001_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1905] = makeRes(
        17902, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_002_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1906] = makeRes(
        17903, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_003_midorian_engine_glow_add.aem", 34811, false));
    resources[1907] = makeRes(
        17904, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_004_nivelian_engine_glow_add.aem", 34811, false));
    resources[1908] = makeRes(
        17905, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_005_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1909] = makeRes(
        17906, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_006_midorian_engine_glow_add.aem", 34811, false));
    resources[1910] = makeRes(
        17907, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_007_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1911] = makeRes(
        17908, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_008_void_engine_glow_add.aem",
                                                34811, false));
    resources[1912] = makeRes(
        17909, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_009_vossk_engine_glow_add.aem",
                                                34811, false));
    resources[1913] = makeRes(
        17910, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_010_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1914] = makeRes(
        17911, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_011_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1915] = makeRes(
        17912, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_012_nivelian_engine_glow_add.aem", 34811, false));
    resources[1916] = makeRes(
        17916, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_016_nivelian_engine_glow_add.aem", 34811, false));
    resources[1917] = makeRes(
        17917, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_017_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1918] = makeRes(
        17918, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_018_nivelian_engine_glow_add.aem", 34811, false));
    resources[1919] = makeRes(
        17919, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_019_midorian_engine_glow_add.aem", 34811, false));
    resources[1920] = makeRes(
        17920, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_020_midorian_engine_glow_add.aem", 34811, false));
    resources[1921] = makeRes(
        17921, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_021_nivelian_engine_glow_add.aem", 34811, false));
    resources[1922] = makeRes(
        17922, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_022_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1923] = makeRes(
        17923, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_023_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1924] = makeRes(
        17924, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_024_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1925] = makeRes(
        17925, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_025_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1926] = makeRes(
        17926, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_026_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1927] = makeRes(
        17927, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_027_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1928] = makeRes(
        17928, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_028_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1929] = makeRes(
        17929, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_029_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1930] = makeRes(
        17930, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_030_midorian_engine_glow_add.aem", 34811, false));
    resources[1931] = makeRes(
        17931, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_031_nivelian_engine_glow_add.aem", 34811, false));
    resources[1932] = makeRes(
        17932, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_032_pirates_engine_glow_add.aem",
                                                34811, false));
    resources[1933] = makeRes(
        17933, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_033_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1934] = makeRes(
        17934, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_034_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1935] = makeRes(
        17935, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/ships/ship_035_nivelian_engine_glow_add.aem", 34811, false));
    resources[1936] = makeRes(
        17936, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_036_terran_engine_glow_add.aem",
                                                34811, false));
    resources[1937] = makeRes(17937, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_037_deep_science_engine_glow_add.aem", 0,
                                  false));
    resources[1938] = makeRes(17938, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_038_deep_science_engine_glow_add.aem", 0,
                                  false));
    resources[1939] = makeRes(17940, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_040_deep_science_engine_glow_add.aem", 0,
                                  false));
    resources[1940] = makeRes(
        17942, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_042_retro_engine_glow_add.aem",
                                                34811, false));
    resources[1941] = makeRes(
        17943, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_043_retro_engine_glow_add.aem",
                                                34811, false));
    resources[1942] = makeRes(17944, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_044_elite_nivelian_engine_glow_add.aem",
                                  32544, false));
    resources[1943] = makeRes(17945, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_045_most_wanted_engine_glow_add.aem",
                                  32545, false));
    resources[1944] = makeRes(17946, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_046_most_wanted_engine_glow_add.aem",
                                  32546, false));
    resources[1945] = makeRes(17947, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_047_most_wanted_engine_glow_add.aem",
                                  32547, false));
    resources[1946] = makeRes(17948, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_048_most_wanted_engine_glow_add.aem",
                                  32548, false));
    resources[1947] = makeRes(17949, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_049_boss_nivelian_engine_glow_add.aem",
                                  32549, false));
    resources[1948] = makeRes(17951, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_051_dropship_terran_engine_glow_add.aem",
                                  32551, false));
    resources[1949] = makeRes(17952, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_052_retro_engine_glow_add.aem", 32552,
                                  false));
    resources[1950] = makeRes(17954, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_054_vossk_engine_glow_add.aem", 32554,
                                  false));
    resources[1951] = makeRes(17955, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_055_modified_engine_glow_add.aem",
                                  34811, false));
    resources[1952] = makeRes(17956, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_056_modified_engine_glow_add.aem",
                                  34811, false));
    resources[1953] = makeRes(17957, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_057_modified_engine_glow_add.aem",
                                  34811, false));
    resources[1954] = makeRes(17958, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_058_modified_engine_glow_add.aem",
                                  34811, false));
    resources[1955] = makeRes(17959, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_059_modified_engine_glow_add.aem",
                                  34811, false));
    resources[1956] = makeRes(17960, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/ships/sn_ship_060_modified_engine_glow_add.aem",
                                  34811, false));
    resources[1957] = makeRes(17961, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/ships/ship_061_elite_nivelian_prototype_engine_glow_add.aem",
                                  32561, false));
    resources[1958] = makeRes(17962, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/ships/ship_062_prototype_engine_glow_add.aem", 32562,
                                  false));
    resources[1959] = makeRes(17963, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/ships/ship_063_vossk_prototype_engine_glow_add.aem",
                                  32563, false));
    resources[1960] = makeRes(
        17000, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_000_midorian.aem", 0, false));
    resources[1961] = makeRes(
        17100, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_000_midorian_lod_1.aem", 0,
                                                false));
    resources[1962] = makeRes(
        17101, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_000_midorian_lod_2.aem", 0,
                                                false));
    resources[1963] = makeRes(
        18000, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_000_midorian_engine_add.aem",
                                                34600, false));
    resources[1964] = makeRes(
        18700, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_000_midorian_lights_add.aem",
                                                34600, false));
    resources[1965] = makeRes(
        17001, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_001_terran.aem", 0, false));
    resources[1966] = makeRes(
        17103, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_001_terran_lod_1.aem", 0,
                                                false));
    resources[1967] = makeRes(
        17104, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_001_terran_lod_2.aem", 0,
                                                false));
    resources[1968] = makeRes(
        18001, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_001_terran_engine_add.aem", 3,
                                                false));
    resources[1969] = makeRes(
        18701, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_001_terran_lights_add.aem", 3,
                                                false));
    resources[1970] = makeRes(
        17002, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_002_pirates.aem", 0, false));
    resources[1971] = makeRes(
        17106, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_002_pirates_lod_1.aem", 0, false));
    resources[1972] = makeRes(
        17107, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_002_pirates_lod_2.aem", 0, false));
    resources[1973] = makeRes(
        18002, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_002_pirates_engine_add.aem",
                                                34811, false));
    resources[1974] = makeRes(
        18702, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_002_pirates_lights_add.aem",
                                                34811, false));
    resources[1975] = makeRes(
        17003, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_003_midorian.aem", 0, false));
    resources[1976] = makeRes(
        17109, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_003_midorian_lod_1.aem", 0,
                                                false));
    resources[1977] = makeRes(
        17110, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_003_midorian_lod_2.aem", 0,
                                                false));
    resources[1978] = makeRes(
        18003, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_003_midorian_engine_add.aem",
                                                34600, false));
    resources[1979] = makeRes(
        18703, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_003_midorian_lights_add.aem",
                                                34600, false));
    resources[1980] = makeRes(
        17004, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_004_nivelian.aem", 0, false));
    resources[1981] = makeRes(
        17112, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_004_nivelian_lod_1.aem", 0,
                                                false));
    resources[1982] = makeRes(
        17113, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_004_nivelian_lod_2.aem", 0,
                                                false));
    resources[1983] = makeRes(
        18004, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_004_nivelian_engine_add.aem",
                                                34810, false));
    resources[1984] = makeRes(
        18704, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_004_nivelian_lights_add.aem",
                                                34810, false));
    resources[1985] = makeRes(
        17005, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_005_terran.aem", 0, false));
    resources[1986] = makeRes(
        17115, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_005_terran_lod_1.aem", 0,
                                                false));
    resources[1987] = makeRes(
        17116, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_005_terran_lod_2.aem", 0,
                                                false));
    resources[1988] = makeRes(
        18005, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_005_terran_engine_add.aem", 3,
                                                false));
    resources[1989] = makeRes(
        18705, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_005_terran_lights_add.aem", 3,
                                                false));
    resources[1990] = makeRes(
        17006, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_006_midorian.aem", 0, false));
    resources[1991] = makeRes(
        17118, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_006_midorian_lod_1.aem", 0,
                                                false));
    resources[1992] = makeRes(
        17119, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_006_midorian_lod_2.aem", 0,
                                                false));
    resources[1993] = makeRes(
        18006, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_006_midorian_engine_add.aem",
                                                34600, false));
    resources[1994] = makeRes(
        18706, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_006_midorian_lights_add.aem",
                                                34600, false));
    resources[1995] = makeRes(
        17007, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_007_terran.aem", 0, false));
    resources[1996] = makeRes(
        17121, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_007_terran_lod_1.aem", 0,
                                                false));
    resources[1997] = makeRes(
        17122, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_007_terran_lod_2.aem", 0,
                                                false));
    resources[1998] = makeRes(
        18007, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_007_terran_engine_add.aem", 3,
                                                false));
    resources[1999] = makeRes(
        18707, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_007_terran_lights_add.aem", 3,
                                                false));
    resources[2000] = makeRes(
        17008, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_008_void.aem", 0, false));
    resources[2001] = makeRes(
        17124, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_008_void_lod_1.aem", 0, false));
    resources[2002] = makeRes(
        17125, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_008_void_lod_2.aem", 0, false));
    resources[2003] = makeRes(
        18008, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_008_void_engine_add.aem", 0,
                                                false));
    resources[2004] = makeRes(
        18708, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_008_void_lights_add.aem", 0,
                                                false));
    resources[2005] = makeRes(
        17009, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_009_vossk.aem", 0, false));
    resources[2006] = makeRes(
        17127, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_009_vossk_lod_1.aem", 0, false));
    resources[2007] = makeRes(
        17128, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_009_vossk_lod_2.aem", 0, false));
    resources[2008] = makeRes(
        18009, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_009_vossk_engine_add.aem", 65535,
                                                false));
    resources[2009] = makeRes(
        18709, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_009_vossk_lights_add.aem", 65535,
                                                false));
    resources[2010] = makeRes(
        17010, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_010_terran.aem", 0, false));
    resources[2011] = makeRes(
        17130, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_010_terran_lod_1.aem", 0,
                                                false));
    resources[2012] = makeRes(
        17131, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_010_terran_lod_2.aem", 0,
                                                false));
    resources[2013] = makeRes(
        18010, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_010_terran_engine_add.aem", 3,
                                                false));
    resources[2014] = makeRes(
        18710, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_010_terran_lights_add.aem", 3,
                                                false));
    resources[2015] = makeRes(
        17011, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_011_pirates.aem", 0, false));
    resources[2016] = makeRes(
        17133, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_011_pirates_lod_1.aem", 0, false));
    resources[2017] = makeRes(
        17134, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_011_pirates_lod_2.aem", 0, false));
    resources[2018] = makeRes(
        18011, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_011_pirates_engine_add.aem",
                                                34811, false));
    resources[2019] = makeRes(
        18711, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_011_pirates_lights_add.aem",
                                                34811, false));
    resources[2020] = makeRes(
        17012, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_012_nivelian.aem", 0, false));
    resources[2021] = makeRes(
        17136, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_012_nivelian_lod_1.aem", 0,
                                                false));
    resources[2022] = makeRes(
        17137, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_012_nivelian_lod_2.aem", 0,
                                                false));
    resources[2023] = makeRes(
        18012, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_012_nivelian_engine_add.aem",
                                                34810, false));
    resources[2024] = makeRes(
        18712, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_012_nivelian_lights_add.aem",
                                                34810, false));
    resources[2025] = makeRes(
        17016, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_016_nivelian.aem", 0, false));
    resources[2026] = makeRes(
        17148, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_016_nivelian_lod_1.aem", 0,
                                                false));
    resources[2027] = makeRes(
        17149, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_016_nivelian_lod_2.aem", 0,
                                                false));
    resources[2028] = makeRes(
        18016, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_016_nivelian_engine_add.aem",
                                                34810, false));
    resources[2029] = makeRes(
        18716, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_016_nivelian_lights_add.aem",
                                                34810, false));
    resources[2030] = makeRes(
        17017, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_017_terran.aem", 0, false));
    resources[2031] = makeRes(
        17151, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_017_terran_lod_1.aem", 0,
                                                false));
    resources[2032] = makeRes(
        17152, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_017_terran_lod_2.aem", 0,
                                                false));
    resources[2033] = makeRes(
        18017, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_017_terran_engine_add.aem", 3,
                                                false));
    resources[2034] = makeRes(
        18717, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_017_terran_lights_add.aem", 3,
                                                false));
    resources[2035] = makeRes(
        17018, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_018_nivelian.aem", 0, false));
    resources[2036] = makeRes(
        17154, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_018_nivelian_lod_1.aem", 0,
                                                false));
    resources[2037] = makeRes(
        17155, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_018_nivelian_lod_2.aem", 0,
                                                false));
    resources[2038] = makeRes(
        18018, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_018_nivelian_engine_add.aem",
                                                34810, false));
    resources[2039] = makeRes(
        18718, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_018_nivelian_lights_add.aem",
                                                34810, false));
    resources[2040] = makeRes(
        17019, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_019_midorian.aem", 0, false));
    resources[2041] = makeRes(
        17157, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_019_midorian_lod_1.aem", 0,
                                                false));
    resources[2042] = makeRes(
        17158, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_019_midorian_lod_2.aem", 0,
                                                false));
    resources[2043] = makeRes(
        18019, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_019_midorian_engine_add.aem",
                                                34600, false));
    resources[2044] = makeRes(
        18719, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_019_midorian_lights_add.aem",
                                                34600, false));
    resources[2045] = makeRes(
        17020, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_020_midorian.aem", 0, false));
    resources[2046] = makeRes(
        17160, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_020_midorian_lod_1.aem", 0,
                                                false));
    resources[2047] = makeRes(
        17161, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_020_midorian_lod_2.aem", 0,
                                                false));
    resources[2048] = makeRes(
        18020, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_020_midorian_engine_add.aem",
                                                34600, false));
    resources[2049] = makeRes(
        18720, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_020_midorian_lights_add.aem",
                                                34600, false));
    resources[2050] = makeRes(
        17021, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_021_nivelian.aem", 0, false));
    resources[2051] = makeRes(
        17163, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_021_nivelian_lod_1.aem", 0,
                                                false));
    resources[2052] = makeRes(
        17164, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_021_nivelian_lod_2.aem", 0,
                                                false));
    resources[2053] = makeRes(
        18021, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_021_nivelian_engine_add.aem",
                                                34810, false));
    resources[2054] = makeRes(
        18721, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_021_nivelian_lights_add.aem",
                                                34810, false));
    resources[2055] = makeRes(
        17022, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_022_terran.aem", 0, false));
    resources[2056] = makeRes(
        17166, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_022_terran_lod_1.aem", 0,
                                                false));
    resources[2057] = makeRes(
        17167, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_022_terran_lod_2.aem", 0,
                                                false));
    resources[2058] = makeRes(
        18022, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_022_terran_engine_add.aem", 3,
                                                false));
    resources[2059] = makeRes(
        18722, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_022_terran_lights_add.aem", 3,
                                                false));
    resources[2060] = makeRes(
        17023, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_023_pirates.aem", 0, false));
    resources[2061] = makeRes(
        17169, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_023_pirates_lod_1.aem", 0, false));
    resources[2062] = makeRes(
        17170, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_023_pirates_lod_2.aem", 0, false));
    resources[2063] = makeRes(
        18023, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_023_pirates_engine_add.aem",
                                                34811, false));
    resources[2064] = makeRes(
        18723, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_023_pirates_lights_add.aem",
                                                34811, false));
    resources[2065] = makeRes(
        17024, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_024_pirates.aem", 0, false));
    resources[2066] = makeRes(
        17172, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_024_pirates_lod_1.aem", 0, false));
    resources[2067] = makeRes(
        17173, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_024_pirates_lod_2.aem", 0, false));
    resources[2068] = makeRes(
        18024, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_024_pirates_engine_add.aem",
                                                34811, false));
    resources[2069] = makeRes(
        18724, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_024_pirates_lights_add.aem",
                                                34811, false));
    resources[2070] = makeRes(
        17025, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_025_pirates.aem", 0, false));
    resources[2071] = makeRes(
        17175, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_025_pirates_lod_1.aem", 0, false));
    resources[2072] = makeRes(
        17176, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_025_pirates_lod_2.aem", 0, false));
    resources[2073] = makeRes(
        18025, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_025_pirates_engine_add.aem",
                                                34811, false));
    resources[2074] = makeRes(
        18725, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_025_pirates_lights_add.aem",
                                                34811, false));
    resources[2075] = makeRes(
        17026, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_026_terran.aem", 0, false));
    resources[2076] = makeRes(
        17178, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_026_terran_lod_1.aem", 0,
                                                false));
    resources[2077] = makeRes(
        17179, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_026_terran_lod_2.aem", 0,
                                                false));
    resources[2078] = makeRes(
        18026, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_026_terran_engine_add.aem", 3,
                                                false));
    resources[2079] = makeRes(
        18726, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_026_terran_lights_add.aem", 3,
                                                false));
    resources[2080] = makeRes(
        17027, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_027_terran.aem", 0, false));
    resources[2081] = makeRes(
        17181, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_027_terran_lod_1.aem", 0,
                                                false));
    resources[2082] = makeRes(
        17182, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_027_terran_lod_2.aem", 0,
                                                false));
    resources[2083] = makeRes(
        18027, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_027_terran_engine_add.aem", 3,
                                                false));
    resources[2084] = makeRes(
        18727, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_027_terran_lights_add.aem", 3,
                                                false));
    resources[2085] = makeRes(
        17028, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_028_terran.aem", 0, false));
    resources[2086] = makeRes(
        17184, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_028_terran_lod_1.aem", 0,
                                                false));
    resources[2087] = makeRes(
        17185, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_028_terran_lod_2.aem", 0,
                                                false));
    resources[2088] = makeRes(
        18028, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_028_terran_engine_add.aem", 3,
                                                false));
    resources[2089] = makeRes(
        18728, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_028_terran_lights_add.aem", 3,
                                                false));
    resources[2090] = makeRes(
        17029, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_029_pirates.aem", 0, false));
    resources[2091] = makeRes(
        17187, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_029_pirates_lod_1.aem", 0, false));
    resources[2092] = makeRes(
        17188, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_029_pirates_lod_2.aem", 0, false));
    resources[2093] = makeRes(
        18029, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_029_pirates_engine_add.aem",
                                                34811, false));
    resources[2094] = makeRes(
        18729, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_029_pirates_lights_add.aem",
                                                34811, false));
    resources[2095] = makeRes(
        17030, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_030_midorian.aem", 0, false));
    resources[2096] = makeRes(
        17190, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_030_midorian_lod_1.aem", 0,
                                                false));
    resources[2097] = makeRes(
        17191, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_030_midorian_lod_2.aem", 0,
                                                false));
    resources[2098] = makeRes(
        18030, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_030_midorian_engine_add.aem",
                                                34600, false));
    resources[2099] = makeRes(
        18730, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_030_midorian_lights_add.aem",
                                                34600, false));
    resources[2100] = makeRes(
        17031, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_031_nivelian.aem", 0, false));
    resources[2101] = makeRes(
        17193, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_031_nivelian_lod_1.aem", 0,
                                                false));
    resources[2102] = makeRes(
        17194, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_031_nivelian_lod_2.aem", 0,
                                                false));
    resources[2103] = makeRes(
        18031, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_031_nivelian_engine_add.aem",
                                                34810, false));
    resources[2104] = makeRes(
        18731, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_031_nivelian_lights_add.aem",
                                                34810, false));
    resources[2105] = makeRes(
        17032, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_032_pirates.aem", 0, false));
    resources[2106] = makeRes(
        17196, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_032_pirates_lod_1.aem", 0, false));
    resources[2107] = makeRes(
        17197, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_032_pirates_lod_2.aem", 0, false));
    resources[2108] = makeRes(
        18032, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_032_pirates_engine_add.aem",
                                                34811, false));
    resources[2109] = makeRes(
        18732, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_032_pirates_lights_add.aem",
                                                34811, false));
    resources[2110] = makeRes(
        17033, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_033_terran.aem", 0, false));
    resources[2111] = makeRes(
        17199, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_033_terran_lod_1.aem", 0,
                                                false));
    resources[2112] = makeRes(
        17200, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_033_terran_lod_2.aem", 0,
                                                false));
    resources[2113] = makeRes(
        18033, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_033_terran_engine_add.aem", 3,
                                                false));
    resources[2114] = makeRes(
        18733, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_033_terran_lights_add.aem", 3,
                                                false));
    resources[2115] = makeRes(
        17034, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_034_terran.aem", 0, false));
    resources[2116] = makeRes(
        17202, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_034_terran_lod_1.aem", 0,
                                                false));
    resources[2117] = makeRes(
        17203, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_034_terran_lod_2.aem", 0,
                                                false));
    resources[2118] = makeRes(
        18034, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_034_terran_engine_add.aem", 3,
                                                false));
    resources[2119] = makeRes(
        18734, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_034_terran_lights_add.aem", 3,
                                                false));
    resources[2120] = makeRes(
        17035, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_035_nivelian.aem", 0, false));
    resources[2121] = makeRes(
        17205, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_035_nivelian_lod_1.aem", 0,
                                                false));
    resources[2122] = makeRes(
        17206, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_035_nivelian_lod_2.aem", 0,
                                                false));
    resources[2123] = makeRes(
        18035, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_035_nivelian_engine_add.aem",
                                                34810, false));
    resources[2124] = makeRes(
        18735, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_035_nivelian_lights_add.aem",
                                                34810, false));
    resources[2125] = makeRes(
        17036, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_036_terran.aem", 0, false));
    resources[2126] = makeRes(
        17208, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_036_terran_lod_1.aem", 0,
                                                false));
    resources[2127] = makeRes(
        17209, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_036_terran_lod_2.aem", 0,
                                                false));
    resources[2128] = makeRes(
        18036, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_036_terran_engine_add.aem", 3,
                                                false));
    resources[2129] = makeRes(
        18736, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_036_terran_lights_add.aem", 3,
                                                false));
    resources[2130] = makeRes(
        17211, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_037_deep_science.aem",
                                                65535, false));
    resources[2131] = makeRes(17215, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_037_deep_science_lights_anim_add.aem",
                                  32537, false));
    resources[2132] = makeRes(18037, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_037_deep_science_engine_add.aem", 32537,
                                  false));
    resources[2133] = makeRes(
        17216, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_038_deep_science.aem",
                                                65535, false));
    resources[2134] = makeRes(18738, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_038_deep_science_lights_anim_add.aem",
                                  32538, false));
    resources[2135] = makeRes(18038, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_038_deep_science_engine_add.aem", 32538,
                                  false));
    resources[2136] = makeRes(
        17221, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_039_vossk.aem", 0, false));
    resources[2137] = makeRes(
        17222, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_039_vossk_lod_1.aem", 0,
                                                false));
    resources[2138] = makeRes(
        17223, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_039_vossk_lod_2.aem", 0,
                                                false));
    resources[2139] = makeRes(
        18739, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_039_vossk_lights_add.aem",
                                                32539, false));
    resources[2140] = makeRes(
        18039, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_039_vossk_engine_add.aem",
                                                32539, false));
    resources[2141] = makeRes(
        17227, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_040_deep_science.aem",
                                                65535, false));
    resources[2142] = makeRes(17231, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_040_deep_science_lights_add.aem", 32540,
                                  false));
    resources[2143] = makeRes(18040, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/ships/v_ship_040_deep_science_engine_add.aem", 32540,
                                  false));
    resources[2144] = makeRes(
        17233, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_041_vossk.aem", 0, false));
    resources[2145] = makeRes(
        17234, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_041_vossk_lod_1.aem", 0,
                                                false));
    resources[2146] = makeRes(
        17235, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_041_vossk_lod_2.aem", 0,
                                                false));
    resources[2147] = makeRes(
        18741, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_041_vossk_lights_add.aem",
                                                32541, false));
    resources[2148] = makeRes(
        18041, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/ships/v_ship_041_vossk_engine_add.aem",
                                                32541, false));
    resources[2149] = makeRes(
        17239, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_042_retro.aem", 0, false));
    resources[2150] = makeRes(
        17240, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_042_retro_lod_1.aem", 0, false));
    resources[2151] = makeRes(
        17241, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_042_retro_lod_2.aem", 0, false));
    resources[2152] = makeRes(
        17243, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_042_retro_lights_add.aem", 32542,
                                                false));
    resources[2153] = makeRes(
        17245, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_043_retro.aem", 0, false));
    resources[2154] = makeRes(
        17246, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_043_retro_lod_1.aem", 0, false));
    resources[2155] = makeRes(
        17247, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_043_retro_lod_2.aem", 0, false));
    resources[2156] = makeRes(
        17249, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/ships/ship_043_retro_lights_add.aem", 32543,
                                                false));
    resources[2157] = makeRes(16800, 4, new AbyssEngine::ResourceMesh("data/meshes/col_box.aem", 20087, false));
    resources[2158] = makeRes(16801, 4, new AbyssEngine::ResourceMesh("data/meshes/col_sphere.aem", 20087, false));
    resources[2159] = makeRes(
        14243, 4,
        new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_pirates.aem", 20104, false));
    resources[2160] = makeRes(
        14244, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_pirates_emissive.aem",
                                                20105, false));
    resources[2161] = makeRes(
        14245, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_pirates_lights_add.aem",
                                                34811, false));
    resources[2162] = makeRes(
        14246, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/stations/station_pirates_explosion_anim.aem", 20104, false));
    resources[2163] = makeRes(
        16851, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_fog_layer_0.aem", 20025,
                                                false));
    resources[2164] = makeRes(
        16852, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_fog_layer_1.aem", 20026,
                                                false));
    resources[2165] = makeRes(
        16850, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_bg_plane.aem", 20024, false));
    resources[2166] = makeRes(
        16853, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/galaxymap/map_planet_ring.aem", 20030,
                                                false));
    resources[2167] = makeRes(
        16904, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_asteroid_ice.aem", 20134,
                                                false));
    resources[2168] = makeRes(
        16905, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_asteroid_ice_lod_1.aem", 20134,
                                                false));
    resources[2169] = makeRes(
        16906, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_asteroid_ice_lod_2.aem", 20134,
                                                false));
    resources[2170] = makeRes(
        16907, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_asteroid_ice_lod_3.aem", 20134,
                                                false));
    resources[2171] = makeRes(
        16908, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/misc/v_asteroid_ice_explosion_anim.aem",
                                                20134, false));
    resources[2172] = makeRes(16909, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/misc/v_asteroid_ice_explosion_anim_alpha.aem", 20133,
                                  false));
    resources[2173] = makeRes(
        16900, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_01.aem", 20023, false));
    resources[2174] = makeRes(
        16901, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_01_lod_1.aem", 20023, false));
    resources[2175] = makeRes(
        16902, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_01_lod_2.aem", 20023, false));
    resources[2176] = makeRes(
        16903, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_01_lod_3.aem", 20023, false));
    resources[2177] = makeRes(
        16913, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_01_explosion_anim.aem", 20023,
                                                false));
    resources[2178] = makeRes(
        16915, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/misc/asteroid_explosion_anim_alpha.aem",
                                                20051, false));
    resources[2179] = makeRes(
        18836, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_asteroid_magma.aem", 27312,
                                                false));
    resources[2180] = makeRes(
        18837, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_asteroid_magma_lod_1.aem",
                                                27312, false));
    resources[2181] = makeRes(
        18838, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_asteroid_magma_lod_2.aem",
                                                27312, false));
    resources[2182] = makeRes(
        18839, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/misc/sn_asteroid_magma_lod_3.aem",
                                                27312, false));
    resources[2183] = makeRes(18840, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/misc/sn_asteroid_magma_explosion_anim.aem", 27312,
                                  false));
    resources[2184] = makeRes(18841, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/misc/sn_asteroid_magma_explosion_anim_alpha.aem",
                                  27313, false));
    resources[2185] = makeRes(16928, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_anim.aem", 20135,
                                  false));
    resources[2186] = makeRes(16929, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_anim_emissive.aem",
                                  20136, false));
    resources[2187] = makeRes(16930, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_battlestation_anim_add.aem", 20158,
                                  false));
    resources[2188] = makeRes(
        16931, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/stations/v_station_deep_science.aem",
                                                27145, false));
    resources[2189] = makeRes(16932, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_emissive.aem", 27146,
                                  false));
    resources[2190] = makeRes(
        16933, 4, new AbyssEngine::ResourceMesh(
            "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_add.aem", 27147, false));
    resources[2191] = makeRes(16936, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_emitters_anim_add.aem",
                                  0, false));
    resources[2192] = makeRes(16934, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_lod_1.aem", 27145,
                                  false));
    resources[2193] = makeRes(16935, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_lod_1_emissive.aem",
                                  27146, false));
    resources[2194] = makeRes(14367, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_explosion_anim.aem",
                                  27145, false));
    resources[2195] = makeRes(14368, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_explosion_emissive.aem",
                                  27146, false));
    resources[2196] = makeRes(14369, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_explosion_anim_add.aem",
                                  27147, false));
    resources[2197] = makeRes(14370, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_explosion_laser_anim_add.aem",
                                  27147, false));
    resources[2198] = makeRes(16937, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_explosion_emitters_anim_add.aem",
                                  0, false));
    resources[2199] = makeRes(14371, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_damaged.aem", 27145,
                                  false));
    resources[2200] = makeRes(14372, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_damaged_emissive.aem",
                                  27146, false));
    resources[2201] = makeRes(14373, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_damaged_add.aem",
                                  27147, false));
    resources[2202] = makeRes(16938, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_deep_science_damaged_emitters_anim_add.aem",
                                  0, false));
    resources[2203] = makeRes(
        14374, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/fx/v_shield.aem", 27150, false));
    resources[2204] = makeRes(
        14375, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/skyboxes/skybox_asteroid_belt_alpha.aem",
                                                27160, false));
    resources[2205] = makeRes(
        21108, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_kaamo_club.aem", 20007,
                                                false));
    resources[2206] = makeRes(
        21908, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_kaamo_club_emissive.aem",
                                                20008, false));
    resources[2207] = makeRes(
        22900, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_kaamo_club_fx_anim.aem",
                                                27162, false));
    resources[2208] = makeRes(22901, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_kaamo_club_fx_anim_emissive.aem", 27163,
                                  false));
    resources[2209] = makeRes(
        22902, 4, new AbyssEngine::ResourceMesh(
            "data/assets/main/3d/meshes/stations/station_kaamo_club_fx_anim_add.aem", 27164, false));
    resources[2210] = makeRes(22903, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_kaamo_club_fx_anim_alpha.aem", 27165,
                                  false));
    resources[2211] = makeRes(19080, 4, new AbyssEngine::ResourceMesh("data/meshes/test_dock.aem", 20004, false));
    resources[2212] = makeRes(18830, 4, new AbyssEngine::ResourceMesh("data/meshes/carrier.aem", 20034, false));
    resources[2213] = makeRes(
        18834, 4, new AbyssEngine::ResourceMesh("data/meshes/gas_cloud_a_anim_lookat_add.aem", 27311, false));
    resources[2214] = makeRes(
        18835, 4, new AbyssEngine::ResourceMesh("data/meshes/gas_cloud_b_anim_lookat_add.aem", 27311, false));
    resources[2215] = makeRes(
        21000, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_000_nivelian.aem", 20005,
                                                false));
    resources[2216] = makeRes(
        21001, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_001_nivelian.aem", 20005,
                                                false));
    resources[2217] = makeRes(
        21002, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_002_nivelian.aem", 20005,
                                                false));
    resources[2218] = makeRes(
        21003, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_003_nivelian.aem", 20005,
                                                false));
    resources[2219] = makeRes(
        21004, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_004_nivelian.aem", 20005,
                                                false));
    resources[2220] = makeRes(
        21005, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_005_terran.aem", 20004,
                                                false));
    resources[2221] = makeRes(
        21006, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_006_terran.aem", 20004,
                                                false));
    resources[2222] = makeRes(
        21007, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_007_terran.aem", 20004,
                                                false));
    resources[2223] = makeRes(
        21008, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_008_terran.aem", 20004,
                                                false));
    resources[2224] = makeRes(
        21009, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_009_terran.aem", 20004,
                                                false));
    resources[2225] = makeRes(
        21010, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_010_terran.aem", 20004,
                                                false));
    resources[2226] = makeRes(
        21020, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_020_midorian.aem", 20007,
                                                false));
    resources[2227] = makeRes(
        21021, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_021_midorian.aem", 20007,
                                                false));
    resources[2228] = makeRes(
        21022, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_022_midorian.aem", 20007,
                                                false));
    resources[2229] = makeRes(
        21023, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_023_midorian.aem", 20007,
                                                false));
    resources[2230] = makeRes(
        21024, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_024_midorian.aem", 20007,
                                                false));
    resources[2231] = makeRes(
        21030, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_030_nivelian.aem", 20005,
                                                false));
    resources[2232] = makeRes(
        21031, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_031_nivelian.aem", 20005,
                                                false));
    resources[2233] = makeRes(
        21032, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_032_nivelian.aem", 20005,
                                                false));
    resources[2234] = makeRes(
        21033, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_033_nivelian.aem", 20005,
                                                false));
    resources[2235] = makeRes(
        21035, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_035_terran.aem", 20004,
                                                false));
    resources[2236] = makeRes(
        21036, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_036_terran.aem", 20004,
                                                false));
    resources[2237] = makeRes(
        21037, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_037_terran.aem", 20004,
                                                false));
    resources[2238] = makeRes(
        21038, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_038_terran.aem", 20004,
                                                false));
    resources[2239] = makeRes(
        21039, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_039_terran.aem", 20004,
                                                false));
    resources[2240] = makeRes(
        21040, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_040_terran.aem", 20004,
                                                false));
    resources[2241] = makeRes(
        21041, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_041_terran.aem", 20004,
                                                false));
    resources[2242] = makeRes(
        21042, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_042_terran.aem", 20004,
                                                false));
    resources[2243] = makeRes(
        21043, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_043_terran.aem", 20004,
                                                false));
    resources[2244] = makeRes(
        21044, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_044_terran.aem", 20004,
                                                false));
    resources[2245] = makeRes(
        21045, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_045_nivelian.aem", 20005,
                                                false));
    resources[2246] = makeRes(
        21046, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_046_nivelian.aem", 20005,
                                                false));
    resources[2247] = makeRes(
        21047, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_047_nivelian.aem", 20005,
                                                false));
    resources[2248] = makeRes(
        21048, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_048_nivelian.aem", 20005,
                                                false));
    resources[2249] = makeRes(
        21049, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_049_nivelian.aem", 20005,
                                                false));
    resources[2250] = makeRes(
        21055, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_055_terran.aem", 20004,
                                                false));
    resources[2251] = makeRes(
        21056, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_056_terran.aem", 20004,
                                                false));
    resources[2252] = makeRes(
        21057, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_057_terran.aem", 20004,
                                                false));
    resources[2253] = makeRes(
        21065, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_065_terran.aem", 20004,
                                                false));
    resources[2254] = makeRes(
        21066, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_066_terran.aem", 20004,
                                                false));
    resources[2255] = makeRes(
        21067, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_067_terran.aem", 20004,
                                                false));
    resources[2256] = makeRes(
        21068, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_068_terran.aem", 20004,
                                                false));
    resources[2257] = makeRes(
        21069, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_069_terran.aem", 20004,
                                                false));
    resources[2258] = makeRes(
        21070, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_070_terran.aem", 20004,
                                                false));
    resources[2259] = makeRes(
        21071, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_071_terran.aem", 20004,
                                                false));
    resources[2260] = makeRes(
        21072, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_072_terran.aem", 20004,
                                                false));
    resources[2261] = makeRes(
        21073, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_073_terran.aem", 20004,
                                                false));
    resources[2262] = makeRes(
        21074, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_074_terran.aem", 20004,
                                                false));
    resources[2263] = makeRes(
        21075, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_075_midorian.aem", 20007,
                                                false));
    resources[2264] = makeRes(
        21076, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_076_midorian.aem", 20007,
                                                false));
    resources[2265] = makeRes(
        21077, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_077_midorian.aem", 20007,
                                                false));
    resources[2266] = makeRes(
        21078, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_078_midorian.aem", 20007,
                                                false));
    resources[2267] = makeRes(
        21079, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_079_midorian.aem", 20007,
                                                false));
    resources[2268] = makeRes(
        21080, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_080_terran.aem", 20004,
                                                false));
    resources[2269] = makeRes(
        21081, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_081_terran.aem", 20004,
                                                false));
    resources[2270] = makeRes(
        21082, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_082_terran.aem", 20004,
                                                false));
    resources[2271] = makeRes(
        21083, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_083_terran.aem", 20004,
                                                false));
    resources[2272] = makeRes(
        21084, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_084_terran.aem", 20004,
                                                false));
    resources[2273] = makeRes(
        21085, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_085_nivelian.aem", 20005,
                                                false));
    resources[2274] = makeRes(
        21086, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_086_nivelian.aem", 20005,
                                                false));
    resources[2275] = makeRes(
        21087, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_087_nivelian.aem", 20005,
                                                false));
    resources[2276] = makeRes(
        21088, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_088_nivelian.aem", 20005,
                                                false));
    resources[2277] = makeRes(
        21089, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_089_nivelian.aem", 20005,
                                                false));
    resources[2278] = makeRes(
        21090, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_090_terran.aem", 20004,
                                                false));
    resources[2279] = makeRes(
        21091, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_091_terran.aem", 20004,
                                                false));
    resources[2280] = makeRes(
        21092, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_092_terran.aem", 20004,
                                                false));
    resources[2281] = makeRes(
        21093, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_093_terran.aem", 20004,
                                                false));
    resources[2282] = makeRes(
        21094, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_094_terran.aem", 20004,
                                                false));
    resources[2283] = makeRes(
        21095, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_095_terran.aem", 20004,
                                                false));
    resources[2284] = makeRes(
        21096, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_096_terran.aem", 20004,
                                                false));
    resources[2285] = makeRes(
        21097, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_097_terran.aem", 20004,
                                                false));
    resources[2286] = makeRes(
        21098, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_098_terran.aem", 20004,
                                                false));
    resources[2287] = makeRes(
        21099, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_099_terran.aem", 20004,
                                                false));
    resources[2288] = makeRes(
        21105, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/stations/v_station_105_loma.aem", 20007,
                                                false));
    resources[2289] = makeRes(
        21106, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/stations/v_station_106_loma.aem", 20007,
                                                false));
    resources[2290] = makeRes(
        21107, 4, new AbyssEngine::ResourceMesh("data/assets/valkyrie/3d/meshes/stations/v_station_107_loma.aem", 20007,
                                                false));
    resources[2291] = makeRes(
        21112, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_112_midorian.aem",
                                                20007, false));
    resources[2292] = makeRes(
        21113, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_113_midorian.aem",
                                                20007, false));
    resources[2293] = makeRes(
        21114, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_114_midorian.aem",
                                                20007, false));
    resources[2294] = makeRes(
        21115, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_115_midorian.aem",
                                                20007, false));
    resources[2295] = makeRes(
        21116, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_116_midorian.aem",
                                                20007, false));
    resources[2296] = makeRes(
        21117, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_117_midorian.aem",
                                                20007, false));
    resources[2297] = makeRes(
        21118, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_118_midorian.aem",
                                                20007, false));
    resources[2298] = makeRes(
        21119, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_119_nivelian.aem",
                                                20005, false));
    resources[2299] = makeRes(
        21120, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_120_nivelian.aem",
                                                20005, false));
    resources[2300] = makeRes(
        21121, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_121_nivelian.aem",
                                                20005, false));
    resources[2301] = makeRes(
        21122, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_122_nivelian.aem",
                                                20005, false));
    resources[2302] = makeRes(
        21123, 4, new AbyssEngine::ResourceMesh("data/assets/supernova/3d/meshes/stations/sn_station_123_nivelian.aem",
                                                20005, false));
    resources[2303] = makeRes(
        21800, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_000_nivelian_emissive.aem",
                                                20006, false));
    resources[2304] = makeRes(
        21801, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_001_nivelian_emissive.aem",
                                                20006, false));
    resources[2305] = makeRes(
        21802, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_002_nivelian_emissive.aem",
                                                20006, false));
    resources[2306] = makeRes(
        21803, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_003_nivelian_emissive.aem",
                                                20006, false));
    resources[2307] = makeRes(
        21804, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_004_nivelian_emissive.aem",
                                                20006, false));
    resources[2308] = makeRes(
        21805, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_005_terran_emissive.aem",
                                                20003, false));
    resources[2309] = makeRes(
        21806, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_006_terran_emissive.aem",
                                                20003, false));
    resources[2310] = makeRes(
        21807, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_007_terran_emissive.aem",
                                                20003, false));
    resources[2311] = makeRes(
        21808, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_008_terran_emissive.aem",
                                                20003, false));
    resources[2312] = makeRes(
        21809, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_009_terran_emissive.aem",
                                                20003, false));
    resources[2313] = makeRes(
        21810, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_010_terran_emissive.aem",
                                                20003, false));
    resources[2314] = makeRes(
        21820, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_020_midorian_emissive.aem",
                                                20008, false));
    resources[2315] = makeRes(
        21821, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_021_midorian_emissive.aem",
                                                20008, false));
    resources[2316] = makeRes(
        21822, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_022_midorian_emissive.aem",
                                                20008, false));
    resources[2317] = makeRes(
        21823, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_023_midorian_emissive.aem",
                                                20008, false));
    resources[2318] = makeRes(
        21824, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_024_midorian_emissive.aem",
                                                20008, false));
    resources[2319] = makeRes(
        21830, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_030_nivelian_emissive.aem",
                                                20006, false));
    resources[2320] = makeRes(
        21831, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_031_nivelian_emissive.aem",
                                                20006, false));
    resources[2321] = makeRes(
        21832, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_032_nivelian_emissive.aem",
                                                20006, false));
    resources[2322] = makeRes(
        21833, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_033_nivelian_emissive.aem",
                                                20006, false));
    resources[2323] = makeRes(
        21835, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_035_terran_emissive.aem",
                                                20003, false));
    resources[2324] = makeRes(
        21836, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_036_terran_emissive.aem",
                                                20003, false));
    resources[2325] = makeRes(
        21837, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_037_terran_emissive.aem",
                                                20003, false));
    resources[2326] = makeRes(
        21838, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_038_terran_emissive.aem",
                                                20003, false));
    resources[2327] = makeRes(
        21839, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_039_terran_emissive.aem",
                                                20003, false));
    resources[2328] = makeRes(
        21840, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_040_terran_emissive.aem",
                                                20003, false));
    resources[2329] = makeRes(
        21841, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_041_terran_emissive.aem",
                                                20003, false));
    resources[2330] = makeRes(
        21842, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_042_terran_emissive.aem",
                                                20003, false));
    resources[2331] = makeRes(
        21843, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_043_terran_emissive.aem",
                                                20003, false));
    resources[2332] = makeRes(
        21844, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_044_terran_emissive.aem",
                                                20003, false));
    resources[2333] = makeRes(
        21845, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_045_nivelian_emissive.aem",
                                                20006, false));
    resources[2334] = makeRes(
        21846, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_046_nivelian_emissive.aem",
                                                20006, false));
    resources[2335] = makeRes(
        21847, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_047_nivelian_emissive.aem",
                                                20006, false));
    resources[2336] = makeRes(
        21848, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_048_nivelian_emissive.aem",
                                                20006, false));
    resources[2337] = makeRes(
        21849, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_049_nivelian_emissive.aem",
                                                20006, false));
    resources[2338] = makeRes(
        21855, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_055_terran_emissive.aem",
                                                20003, false));
    resources[2339] = makeRes(
        21856, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_056_terran_emissive.aem",
                                                20003, false));
    resources[2340] = makeRes(
        21857, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_057_terran_emissive.aem",
                                                20003, false));
    resources[2341] = makeRes(
        21865, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_065_terran_emissive.aem",
                                                20003, false));
    resources[2342] = makeRes(
        21866, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_066_terran_emissive.aem",
                                                20003, false));
    resources[2343] = makeRes(
        21867, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_067_terran_emissive.aem",
                                                20003, false));
    resources[2344] = makeRes(
        21868, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_068_terran_emissive.aem",
                                                20003, false));
    resources[2345] = makeRes(
        21869, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_069_terran_emissive.aem",
                                                20003, false));
    resources[2346] = makeRes(
        21870, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_070_terran_emissive.aem",
                                                20003, false));
    resources[2347] = makeRes(
        21871, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_071_terran_emissive.aem",
                                                20003, false));
    resources[2348] = makeRes(
        21872, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_072_terran_emissive.aem",
                                                20003, false));
    resources[2349] = makeRes(
        21873, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_073_terran_emissive.aem",
                                                20003, false));
    resources[2350] = makeRes(
        21874, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_074_terran_emissive.aem",
                                                20003, false));
    resources[2351] = makeRes(
        21875, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_075_midorian_emissive.aem",
                                                20008, false));
    resources[2352] = makeRes(
        21876, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_076_midorian_emissive.aem",
                                                20008, false));
    resources[2353] = makeRes(
        21877, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_077_midorian_emissive.aem",
                                                20008, false));
    resources[2354] = makeRes(
        21878, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_078_midorian_emissive.aem",
                                                20008, false));
    resources[2355] = makeRes(
        21879, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_079_midorian_emissive.aem",
                                                20008, false));
    resources[2356] = makeRes(
        21880, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_080_terran_emissive.aem",
                                                20003, false));
    resources[2357] = makeRes(
        21881, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_081_terran_emissive.aem",
                                                20003, false));
    resources[2358] = makeRes(
        21882, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_082_terran_emissive.aem",
                                                20003, false));
    resources[2359] = makeRes(
        21883, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_083_terran_emissive.aem",
                                                20003, false));
    resources[2360] = makeRes(
        21884, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_084_terran_emissive.aem",
                                                20003, false));
    resources[2361] = makeRes(
        21885, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_085_nivelian_emissive.aem",
                                                20006, false));
    resources[2362] = makeRes(
        21886, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_086_nivelian_emissive.aem",
                                                20006, false));
    resources[2363] = makeRes(
        21887, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_087_nivelian_emissive.aem",
                                                20006, false));
    resources[2364] = makeRes(
        21888, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_088_nivelian_emissive.aem",
                                                20006, false));
    resources[2365] = makeRes(
        21889, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_089_nivelian_emissive.aem",
                                                20006, false));
    resources[2366] = makeRes(
        21890, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_090_terran_emissive.aem",
                                                20003, false));
    resources[2367] = makeRes(
        21891, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_091_terran_emissive.aem",
                                                20003, false));
    resources[2368] = makeRes(
        21892, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_092_terran_emissive.aem",
                                                20003, false));
    resources[2369] = makeRes(
        21893, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_093_terran_emissive.aem",
                                                20003, false));
    resources[2370] = makeRes(
        21894, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_094_terran_emissive.aem",
                                                20003, false));
    resources[2371] = makeRes(
        21895, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_095_terran_emissive.aem",
                                                20003, false));
    resources[2372] = makeRes(
        21896, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_096_terran_emissive.aem",
                                                20003, false));
    resources[2373] = makeRes(
        21897, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_097_terran_emissive.aem",
                                                20003, false));
    resources[2374] = makeRes(
        21898, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_098_terran_emissive.aem",
                                                20003, false));
    resources[2375] = makeRes(
        21899, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_099_terran_emissive.aem",
                                                20003, false));
    resources[2376] = makeRes(21905, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_105_loma_emissive.aem", 20008,
                                  false));
    resources[2377] = makeRes(21906, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_106_loma_emissive.aem", 20008,
                                  false));
    resources[2378] = makeRes(21907, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_107_loma_emissive.aem", 20008,
                                  false));
    resources[2379] = makeRes(21912, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_112_midorian_emissive.aem",
                                  20008, false));
    resources[2380] = makeRes(21913, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_113_midorian_emissive.aem",
                                  20008, false));
    resources[2381] = makeRes(21914, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_114_midorian_emissive.aem",
                                  20008, false));
    resources[2382] = makeRes(21915, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_115_midorian_emissive.aem",
                                  20008, false));
    resources[2383] = makeRes(21916, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_116_midorian_emissive.aem",
                                  20008, false));
    resources[2384] = makeRes(21917, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_117_midorian_emissive.aem",
                                  20008, false));
    resources[2385] = makeRes(21918, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_118_midorian_emissive.aem",
                                  20008, false));
    resources[2386] = makeRes(21919, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_119_nivelian_emissive.aem",
                                  20006, false));
    resources[2387] = makeRes(21920, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_120_nivelian_emissive.aem",
                                  20006, false));
    resources[2388] = makeRes(21921, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_121_nivelian_emissive.aem",
                                  20006, false));
    resources[2389] = makeRes(21922, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_122_nivelian_emissive.aem",
                                  20006, false));
    resources[2390] = makeRes(21923, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_123_nivelian_emissive.aem",
                                  20006, false));
    resources[2391] = makeRes(22000, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_000_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2392] = makeRes(22001, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_001_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2393] = makeRes(22002, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_002_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2394] = makeRes(22003, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_003_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2395] = makeRes(22004, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_004_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2396] = makeRes(
        22005, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_005_terran_lights_add.aem",
                                                3, false));
    resources[2397] = makeRes(
        22006, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_006_terran_lights_add.aem",
                                                3, false));
    resources[2398] = makeRes(
        22007, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_007_terran_lights_add.aem",
                                                3, false));
    resources[2399] = makeRes(
        22008, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_008_terran_lights_add.aem",
                                                3, false));
    resources[2400] = makeRes(
        22009, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_009_terran_lights_add.aem",
                                                3, false));
    resources[2401] = makeRes(
        22010, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_010_terran_lights_add.aem",
                                                3, false));
    resources[2402] = makeRes(22020, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_020_midorian_lights_add.aem", 34600,
                                  false));
    resources[2403] = makeRes(22021, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_021_midorian_lights_add.aem", 34600,
                                  false));
    resources[2404] = makeRes(22022, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_022_midorian_lights_add.aem", 34600,
                                  false));
    resources[2405] = makeRes(22023, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_023_midorian_lights_add.aem", 34600,
                                  false));
    resources[2406] = makeRes(22024, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_024_midorian_lights_add.aem", 34600,
                                  false));
    resources[2407] = makeRes(22030, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_030_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2408] = makeRes(22031, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_031_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2409] = makeRes(22032, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_032_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2410] = makeRes(22033, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_033_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2411] = makeRes(
        22035, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_035_terran_lights_add.aem",
                                                3, false));
    resources[2412] = makeRes(
        22036, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_036_terran_lights_add.aem",
                                                3, false));
    resources[2413] = makeRes(
        22037, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_037_terran_lights_add.aem",
                                                3, false));
    resources[2414] = makeRes(
        22038, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_038_terran_lights_add.aem",
                                                3, false));
    resources[2415] = makeRes(
        22039, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_039_terran_lights_add.aem",
                                                3, false));
    resources[2416] = makeRes(
        22040, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_040_terran_lights_add.aem",
                                                3, false));
    resources[2417] = makeRes(
        22041, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_041_terran_lights_add.aem",
                                                3, false));
    resources[2418] = makeRes(
        22042, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_042_terran_lights_add.aem",
                                                3, false));
    resources[2419] = makeRes(
        22043, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_043_terran_lights_add.aem",
                                                3, false));
    resources[2420] = makeRes(
        22044, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_044_terran_lights_add.aem",
                                                3, false));
    resources[2421] = makeRes(22045, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_045_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2422] = makeRes(22046, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_046_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2423] = makeRes(22047, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_047_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2424] = makeRes(22048, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_048_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2425] = makeRes(22049, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_049_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2426] = makeRes(
        22055, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_055_terran_lights_add.aem",
                                                3, false));
    resources[2427] = makeRes(
        22056, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_056_terran_lights_add.aem",
                                                3, false));
    resources[2428] = makeRes(
        22057, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_057_terran_lights_add.aem",
                                                3, false));
    resources[2429] = makeRes(
        22065, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_065_terran_lights_add.aem",
                                                3, false));
    resources[2430] = makeRes(
        22066, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_066_terran_lights_add.aem",
                                                3, false));
    resources[2431] = makeRes(
        22067, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_067_terran_lights_add.aem",
                                                3, false));
    resources[2432] = makeRes(
        22068, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_068_terran_lights_add.aem",
                                                3, false));
    resources[2433] = makeRes(
        22069, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_069_terran_lights_add.aem",
                                                3, false));
    resources[2434] = makeRes(
        22070, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_070_terran_lights_add.aem",
                                                3, false));
    resources[2435] = makeRes(
        22071, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_071_terran_lights_add.aem",
                                                3, false));
    resources[2436] = makeRes(
        22072, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_072_terran_lights_add.aem",
                                                3, false));
    resources[2437] = makeRes(
        22073, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_073_terran_lights_add.aem",
                                                3, false));
    resources[2438] = makeRes(
        22074, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_074_terran_lights_add.aem",
                                                3, false));
    resources[2439] = makeRes(22075, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_075_midorian_lights_add.aem", 34600,
                                  false));
    resources[2440] = makeRes(22076, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_076_midorian_lights_add.aem", 34600,
                                  false));
    resources[2441] = makeRes(22077, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_077_midorian_lights_add.aem", 34600,
                                  false));
    resources[2442] = makeRes(22078, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_078_midorian_lights_add.aem", 34600,
                                  false));
    resources[2443] = makeRes(22079, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_079_midorian_lights_add.aem", 34600,
                                  false));
    resources[2444] = makeRes(
        22080, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_080_terran_lights_add.aem",
                                                3, false));
    resources[2445] = makeRes(
        22081, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_081_terran_lights_add.aem",
                                                3, false));
    resources[2446] = makeRes(
        22082, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_082_terran_lights_add.aem",
                                                3, false));
    resources[2447] = makeRes(
        22083, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_083_terran_lights_add.aem",
                                                3, false));
    resources[2448] = makeRes(
        22084, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_084_terran_lights_add.aem",
                                                3, false));
    resources[2449] = makeRes(22085, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_085_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2450] = makeRes(22086, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_086_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2451] = makeRes(22087, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_087_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2452] = makeRes(22088, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_088_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2453] = makeRes(22089, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/main/3d/meshes/stations/station_089_nivelian_lights_add.aem", 34810,
                                  false));
    resources[2454] = makeRes(
        22090, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_090_terran_lights_add.aem",
                                                3, false));
    resources[2455] = makeRes(
        22091, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_091_terran_lights_add.aem",
                                                3, false));
    resources[2456] = makeRes(
        22092, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_092_terran_lights_add.aem",
                                                3, false));
    resources[2457] = makeRes(
        22093, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_093_terran_lights_add.aem",
                                                3, false));
    resources[2458] = makeRes(
        22094, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_094_terran_lights_add.aem",
                                                3, false));
    resources[2459] = makeRes(
        22095, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_095_terran_lights_add.aem",
                                                3, false));
    resources[2460] = makeRes(
        22096, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_096_terran_lights_add.aem",
                                                3, false));
    resources[2461] = makeRes(
        22097, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_097_terran_lights_add.aem",
                                                3, false));
    resources[2462] = makeRes(
        22098, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_098_terran_lights_add.aem",
                                                3, false));
    resources[2463] = makeRes(
        22099, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_099_terran_lights_add.aem",
                                                3, false));
    resources[2464] = makeRes(22105, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_105_loma_lights_add.aem", 34600,
                                  false));
    resources[2465] = makeRes(22106, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_106_loma_lights_add.aem", 34600,
                                  false));
    resources[2466] = makeRes(22107, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/valkyrie/3d/meshes/stations/v_station_107_loma_lights_add.aem", 34600,
                                  false));
    resources[2467] = makeRes(22112, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_112_midorian_add.aem", 34600,
                                  false));
    resources[2468] = makeRes(22113, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_113_midorian_add.aem", 34600,
                                  false));
    resources[2469] = makeRes(22114, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_114_midorian_add.aem", 34600,
                                  false));
    resources[2470] = makeRes(22115, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_115_midorian_add.aem", 34600,
                                  false));
    resources[2471] = makeRes(22116, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_116_midorian_add.aem", 34600,
                                  false));
    resources[2472] = makeRes(22117, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_117_midorian_add.aem", 34600,
                                  false));
    resources[2473] = makeRes(22118, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_118_midorian_add.aem", 34600,
                                  false));
    resources[2474] = makeRes(22119, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_119_nivelian_add.aem", 34810,
                                  false));
    resources[2475] = makeRes(22120, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_120_nivelian_add.aem", 34810,
                                  false));
    resources[2476] = makeRes(22121, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_121_nivelian_add.aem", 34810,
                                  false));
    resources[2477] = makeRes(22122, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_122_nivelian_add.aem", 34810,
                                  false));
    resources[2478] = makeRes(22123, 4, new AbyssEngine::ResourceMesh(
                                  "data/assets/supernova/3d/meshes/stations/sn_station_123_nivelian_add.aem", 34810,
                                  false));
    resources[2479] = makeRes(
        16436, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_vossk.aem", 20032, false));
    resources[2480] = makeRes(
        16439, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_vossk_emissive.aem", 20031,
                                                false));
    resources[2481] = makeRes(
        16442, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_vossk_lights_add.aem",
                                                20033, false));
    resources[2482] = makeRes(
        16443, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_void.aem", 20045, false));
    resources[2483] = makeRes(
        16446, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_void_emissive.aem", 20048,
                                                false));
    resources[2484] = makeRes(
        16449, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/stations/station_void_add.aem", 20047,
                                                false));
    resources[2485] = makeRes(
        14285, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_void_station_add.aem", 27301,
                                                false));
    resources[2486] = makeRes(
        14286, 4, new AbyssEngine::ResourceMesh("data/assets/main/3d/meshes/fx/explosion_void_station_add_lookat.aem",
                                                27301, false));
    canvas->SetResourceList(resources, 2488);

    for (int i = 0; i < 152; ++i)
        canvas->AddResource(makeRes((unsigned short) (i + 5000), 3,
                                    newImage((unsigned short) (i + 10200), 0)));

    canvas->AddResource(
        makeRes(11635, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_ar_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11637, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_cht_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11639, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_chs_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11643, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_kr_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11645, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_ja_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11641, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_langselect_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10062, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface_ipad_1440.aei", 0.0f)));
    canvas->AddResource(makeRes(10002, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_logos_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(27340, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_credits_1440.aei", 0.0f)));
    canvas->AddResource(makeRes(
        27341, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_challenge_interface_ipad_1440.aei", 0.0f)));
    canvas->AddResource(makeRes(
        27342, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_campaign_select_ipad_1440.aei", 0.0f)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(
        makeRes(10089, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface3_ipad_large.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10063, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface2_ipad_large.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10064, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_items_ipad_large.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11632, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_items_ipad_2_large.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11630, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_dlc_interface_ipad_large.aei", 0.0f)));
    canvas->AddResource(makeRes(
        27341, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_challenge_interface_ipad_large.aei", 0.0f)));
    canvas->AddResource(makeRes(
        27342, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_campaign_select_ipad_large.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11635, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_ar_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11637, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_cht_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11639, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_chs_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11643, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_kr_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11645, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_ja_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11641, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_font_langselect_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10062, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10002, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_logos_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(27340, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_credits_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10064, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_items_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11632, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_items_iphone4_2.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11630, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_dlc_interface_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10089, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface3_iphone4.aei", 0.0f)));
    canvas->AddResource(makeRes(
        27341, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_challenge_interface_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(27342, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_campaign_select_iphone4.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11630, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_dlc_interface.aei", 0.0f)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(
        makeRes(10089, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface3_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10063, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_interface2_ipad_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(10064, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_items_ipad_1440.aei", 0.0f)));
    canvas->AddResource(
        makeRes(11632, 2, new AbyssEngine::ResourceTexture("data/textures/gof2_items_ipad_2_1440.aei", 0.0f)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3001, 3, newImage(0x2d6eu, 0x0u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3002, 3, newImage(0x2d6eu, 0x2u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3104, 3, newImage(0x2d6eu, 0x4u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3106, 3, newImage(0x2d6eu, 0x6u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3006, 3, newImage(0x2d6eu, 0x8u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3008, 3, newImage(0x2d6eu, 0xau)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3010, 3, newImage(0x2d6eu, 0xcu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3012, 3, newImage(0x2d6eu, 0xeu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3014, 3, newImage(0x2d6eu, 0x10u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(3016, 3, newImage(0x2d6eu, 0x12u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1701, 3, newImage(0x274fu, 0x1u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1703, 3, newImage(0x274fu, 0x3u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1705, 3, newImage(0x274fu, 0x5u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1312, 3, newImage(0x274fu, 0x7u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1222, 3, newImage(0x274fu, 0x9u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1204, 3, newImage(0x274fu, 0xbu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1257, 3, newImage(0x274fu, 0xdu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1214, 3, newImage(0x274fu, 0xfu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1212, 3, newImage(0x274fu, 0x11u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1210, 3, newImage(0x274fu, 0x13u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1208, 3, newImage(0x274fu, 0x15u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1199, 3, newImage(0x274fu, 0x17u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1192, 3, newImage(0x274fu, 0x19u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1207, 3, newImage(0x274fu, 0x1bu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1217, 3, newImage(0x274fu, 0x1du)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1202, 3, newImage(0x274fu, 0x1fu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1200, 3, newImage(0x274fu, 0x21u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1708, 3, newImage(0x274fu, 0x23u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1273, 3, newImage(0x274fu, 0x25u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1275, 3, newImage(0x274fu, 0x27u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1271, 3, newImage(0x274fu, 0x29u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1709, 3, newImage(0x274fu, 0x2bu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1711, 3, newImage(0x274fu, 0x2du)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1713, 3, newImage(0x274fu, 0x2fu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1715, 3, newImage(0x274fu, 0x31u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1717, 3, newImage(0x274fu, 0x33u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1719, 3, newImage(0x274fu, 0x35u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1721, 3, newImage(0x274fu, 0x37u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1723, 3, newImage(0x274fu, 0x39u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1316, 3, newImage(0x274fu, 0x3bu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1318, 3, newImage(0x274fu, 0x3du)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1320, 3, newImage(0x274fu, 0x3fu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1322, 3, newImage(0x274fu, 0x41u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1324, 3, newImage(0x274fu, 0x43u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1104, 3, newImage(0x274fu, 0x45u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1100, 3, newImage(0x274fu, 0x47u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1105, 3, newImage(0x274fu, 0x49u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1264, 3, newImage(0x274fu, 0x4bu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1225, 3, newImage(0x274fu, 0x4du)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1227, 3, newImage(0x274fu, 0x4fu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1262, 3, newImage(0x274fu, 0x51u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1234, 3, newImage(0x274fu, 0x53u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1229, 3, newImage(0x274fu, 0x55u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1224, 3, newImage(0x274fu, 0x57u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1239, 3, newImage(0x274fu, 0x59u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1241, 3, newImage(0x274fu, 0x5bu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1238, 3, newImage(0x274fu, 0x5du)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1236, 3, newImage(0x274fu, 0x5fu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1325, 3, newImage(0x274fu, 0x61u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1327, 3, newImage(0x274fu, 0x63u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1329, 3, newImage(0x274fu, 0x65u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1331, 3, newImage(0x274fu, 0x67u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1332, 3, newImage(0x274eu, 0xe6u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1343, 3, newImage(0x274fu, 0x6bu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1345, 3, newImage(0x274fu, 0x6du)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1347, 3, newImage(0x274fu, 0x6fu)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(1350, 3, newImage(0x274fu, 0x71u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(7005, 3, newImage(0x2712u, 0x5u)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(
        10011, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_000_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10012, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_001_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10013, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_002_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10014, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_003_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10015, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_004_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10016, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_005_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10017, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_006_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10018, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_007_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10019, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_008_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10020, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_009_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10021, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_010_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10022, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_011_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10023, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_012_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10024, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_013_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10025, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_014_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10026, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_015_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10027, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_016_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10028, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_017_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10029, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_018_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        10030, 2, new AbyssEngine::ResourceTexture("data/assets/main/3d/textures/low/etc/planets/planet_019_small.aei",
                                                   0.0f)));
    canvas->AddResource(makeRes(
        11621, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/planets/v_planet_020_small.aei", 0.0f)));
    canvas->AddResource(makeRes(
        11622, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/planets/v_planet_021_small.aei", 0.0f)));
    canvas->AddResource(makeRes(
        11623, 2, new AbyssEngine::ResourceTexture(
            "data/assets/valkyrie/3d/textures/low/etc/planets/v_planet_022_small.aei", 0.0f)));
    canvas->AddResource(makeRes(
        11784, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/planets/sn_planet_024_small.aei", 0.0f)));
    canvas->AddResource(makeRes(
        11785, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/planets/sn_planet_025_small.aei", 0.0f)));
    canvas->AddResource(makeRes(
        11786, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/planets/sn_planet_026_small.aei", 0.0f)));
    canvas->AddResource(makeRes(
        29080, 2, new AbyssEngine::ResourceTexture(
            "data/assets/supernova/3d/textures/low/etc/planets/sn_planet_ring.aei", 0.0f)));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(0, 0, nullptr));
    canvas->AddResource(makeRes(9510, 3, newImage(0x6acfu, 0x5u)));

    loadPortraits(engine);
    loadLowTexturesAndMaterials(engine);
}
