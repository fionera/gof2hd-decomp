#ifndef GOF2_MISSIONSWINDOW_H
#define GOF2_MISSIONSWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class ScrollTouchWindow;
class StarMap;
class ChoiceWindow;
class WantedWindow;
class TouchButton;
class ImagePart;

class MissionsWindow {
public:
    ScrollTouchWindow *m_pCampaignWindow; // +0x00 left (campaign) scroll window
    ScrollTouchWindow *m_pFreelanceWindow; // +0x04 right (freelance) scroll window
    StarMap *m_pStarMap; // +0x08 star-map overlay (active when m_starMapActive)
    ChoiceWindow *m_pChoiceWindow; // +0x0c confirm/choice dialog
    WantedWindow *m_pWantedWindow; // +0x10 wanted-board window (active when m_mode == 1)
    Array<TouchButton *> *m_pTabButtons; // +0x14 tab buttons (campaign / freelance)
    Array<ImagePart *> *m_pAgentImageParts; // +0x18 freelance-agent portrait image parts
    int m_textHalfHeight; // +0x1c cached (font line height / 2 - 1)
    uint8_t m_choiceActive; // +0x20 confirm dialog open flag
    uint8_t m_field_0x21; // +0x21 secondary choice-window draw flag
    uint8_t m_starMapActive; // +0x22 star-map overlay active flag
    uint8_t m_hangarNeedsUpdate; // +0x23 hangar-needs-update flag
    TouchButton *m_pAcceptButton; // +0x24 accept-mission button
    TouchButton *m_pRejectButton; // +0x28 reject-mission button
    TouchButton *m_pMapButton; // +0x2c show-on-map button
    int m_x; // +0x30 window x
    int m_y; // +0x34 window y
    int m_width; // +0x38 window width
    int m_height; // +0x3c window height
    int m_mode; // +0x40 active mode (1 == wanted board)

    MissionsWindow();

    ~MissionsWindow();

    int OnTouchMove(int x, int y);

    int OnTouchBegin(int x, int y);

    void OnTouchEnd(int x, int y);

    void setHangarUpdate(bool v);

    uint8_t hangarNeedsUpdate();

    void render3D();

    int init();

    void draw();

    void update(int dt);
};
#endif
