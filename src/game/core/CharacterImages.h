#ifndef GOF2_CHARACTERIMAGES_H
#define GOF2_CHARACTERIMAGES_H

// Shared by every TU that shows a story character (Radio, DialogueWindow, ModStation, MGame,
// Level): like the original header, each including TU gets its own file-static copy of the
// tables it references (the original .data holds one CHAR_* copy per such TU, and the
// CHAR_IMAGES pointer table once per TU that indexes it).
// Face descriptors of the story characters (5 image-part indices each), indexed by a radio
// message's image id. Values taken from the original's .data (CHAR_IMAGES @ 0x20df04).
static int CHAR_BRENT[5] = {0, 0, 0, 0, 0};
static int CHAR_GUNANT[5] = {0, 8, 8, 1, 8};
static int CHAR_NORRIS[5] = {2, 1, 1, 1, 1};
static int CHAR_MKKT_BKKT[5] = {0, 9, 9, 8, 9};
static int CHAR_TOMMY[5] = {4, 2, 0, 2, 2};
static int CHAR_CARLA[5] = {0, 2, 3, 3, 10};
static int CHAR_ERRKT[5] = {10, 0, 0, 1, 0};
static int CHAR_JEAN[5] = {1, 1, 0, 4, 0};
static int CHAR_PIRATE_CHIEF[5] = {0, 6, 8, 8, 7};
static int CHAR_PIRATE_1[5] = {0, 4, 8, 8, 10};
static int CHAR_PIRATE_2[5] = {0, 3, 5, 8, 6};
static int CHAR_SECURITY_GUY[5] = {0, 7, 7, 8, 5};
static int CHAR_SECURITY_WOMAN[5] = {0, 5, 4, 2, 7};
static int CHAR_KIDNAPPER[5] = {10, 1, 1, 2, 4};
static int CHAR_COMPUTER[5] = {2, 2, 2, 4, 2};
static int CHAR_INFO_PIC[5] = {11, 1, 0, 0, 1};
static int CHAR_STORY[5] = {11, 1, 0, 0, 2};
static int CHAR_TERRAN_OFFICER[5] = {11, 1, 0, 0, 3};
static int CHAR_ALIEN[5] = {0, 8, 10, 10, 4};
static int CHAR_KHADOR[5] = {9, 0, 0, 0, 0};
static int CHAR_MIDO[5] = {7, 1, 1, 0, 0};
static int CHAR_NIVELIAN_SECURITY[5] = {2, 0, 0, 1, 2};
static int CHAR_AGENT[5] = {2, 0, 0, 0, 0};
static int CHAR_TARET[5] = {0, 2, 10, 9, 2};
static int CHAR_MEKANT[5] = {1, 2, 4, 3, 1};
static int CHAR_ALICE[5] = {1, 3, 1, 5, 0};
static int CHAR_ALICE_MERC_1[5] = {10, 3, 3, 4, 6};
static int CHAR_PIRATE_KHADOR_1[5] = {0, 0, 5, 8, 6};
static int CHAR_PIRATE_KHADOR_2[5] = {0, 8, 8, 8, 0};
static int CHAR_PIRATE_KHADOR_3[5] = {0, 4, 8, 8, 4};
static int CHAR_TENNER[5] = {0, 0, 5, 8, 10};
static int CHAR_NETOR[5] = {0, 2, 10, 9, 2};
static int CHAR_CONVOY_CAPTAIN[5] = {7, 0, 0, 2, 1};
static int CHAR_TROT_LYKKT[5] = {0, 5, 4, 8, 3};
static int CHAR_SMUGGLER[5] = {4, 1, 1, 1, 1};
static int CHAR_CONNY[5] = {0, 4, 8, 8, 4};
static int CHAR_BARGAND_SURR[5] = {0, 2, 10, 9, 2};
static int CHAR_MOONSPROCKET[5] = {2, 4, 1, 4, 1};
static int CHAR_TRUNT_HARVAL[5] = {0, 8, 3, 8, 3};
static int CHAR_BRILLO[5] = {2, 2, 1, 3, 2};
static int CHAR_DRUNK_WOMAN[5] = {6, 0, 0, 2, 0};
static int CHAR_BARKEEPER[5] = {10, 3, 3, 1, 4};
static int CHAR_DEALER[5] = {0, 0, 5, 4, 6};
static int CHAR_MUSEUM_CURATOR[5] = {1, 0, 3, 1, 2};
static int CHAR_PRISM_PIRATE_1[5] = {2, 4, 1, 2, 3};
static int CHAR_PRISM_PIRATE_2[5] = {1, 0, 1, 1, 3};
static int CHAR_PRISM_PIRATE_3[5] = {7, 0, 0, 0, 0};
static int CHAR_INDUSTRIALIST[5] = {2, 4, 2, 0, 0};
static int CHAR_MINER[5] = {2, 0, 1, 1, 2};
static int CHAR_VOSSK_ON_BAKRRAM[5] = {2, 1, 0, 4, 0};
static int CHAR_BROKER_1[5] = {1, 0, 0, 3, 5};
static int CHAR_BROKER_2[5] = {0, 3, 3, 0, 5};
static int CHAR_BROKER_3[5] = {10, 0, 3, 3, 5};
static int CHAR_BROKER_4[5] = {0, 3, 6, 0, 8};
static int CHAR_HANS[5] = {10, 3, 0, 3, 3};
static int CHAR_CONNY_VOID[5] = {0, 10, 7, 2, 1};
static int CHAR_BARKEEPER_WRONG_1[5] = {12, 0, 0, 0, 0};
static int CHAR_BARKEEPER_WRONG_2[5] = {0, 0, 10, 0, 7};
static int CHAR_MIDO_FREIGHTER_CAPTAIN[5] = {0, 6, 0, 7, 4};
static int CHAR_GUNANTS_SON[5] = {2, 3, 1, 1, 4};
static int CHAR_DEEP_SCIENCE_ESCORT[5] = {2, 1, 4, 2, 0};
static int CHAR_BARKEEPER_2[5] = {0, 0, 4, 3, 5};
static int CHAR_KEITH[5] = {0, 10, 2, 9, 4};
static int *const CHAR_IMAGES[63] = {
    CHAR_BRENT, CHAR_GUNANT, CHAR_NORRIS, CHAR_MKKT_BKKT,
    CHAR_TOMMY, CHAR_CARLA, CHAR_ERRKT, CHAR_JEAN,
    CHAR_PIRATE_CHIEF, CHAR_PIRATE_1, CHAR_PIRATE_2, CHAR_SECURITY_GUY,
    CHAR_SECURITY_WOMAN, CHAR_KIDNAPPER, CHAR_COMPUTER, CHAR_INFO_PIC,
    CHAR_STORY, CHAR_TERRAN_OFFICER, CHAR_ALIEN, CHAR_KHADOR,
    CHAR_MIDO, CHAR_NIVELIAN_SECURITY, CHAR_AGENT, CHAR_TARET,
    CHAR_MEKANT, CHAR_ALICE, CHAR_ALICE_MERC_1, CHAR_PIRATE_KHADOR_1,
    CHAR_PIRATE_KHADOR_2, CHAR_PIRATE_KHADOR_3, CHAR_TENNER, CHAR_NETOR,
    CHAR_CONVOY_CAPTAIN, CHAR_TROT_LYKKT, CHAR_SMUGGLER, CHAR_CONNY,
    CHAR_BARGAND_SURR, CHAR_MOONSPROCKET, CHAR_TRUNT_HARVAL, CHAR_BRILLO,
    CHAR_DRUNK_WOMAN, CHAR_BARKEEPER, CHAR_DEALER, CHAR_MUSEUM_CURATOR,
    CHAR_PRISM_PIRATE_1, CHAR_PRISM_PIRATE_2, CHAR_PRISM_PIRATE_3, CHAR_INDUSTRIALIST,
    CHAR_MINER, CHAR_VOSSK_ON_BAKRRAM, CHAR_BROKER_1, CHAR_BROKER_2,
    CHAR_BROKER_3, CHAR_BROKER_4, CHAR_HANS, CHAR_CONNY_VOID,
    CHAR_BARKEEPER_WRONG_1, CHAR_BARKEEPER_WRONG_2, CHAR_MIDO_FREIGHTER_CAPTAIN, CHAR_GUNANTS_SON,
    CHAR_DEEP_SCIENCE_ESCORT, CHAR_BARKEEPER_2, CHAR_KEITH,
};

#endif
