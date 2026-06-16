#ifndef GOF2_MISSIONSWINDOW_H
#define GOF2_MISSIONSWINDOW_H
#include "common.h"

// MissionsWindow -- the in-station "Missions" screen. Hosts a left (campaign) and a
// right (freelance) scroll window, a row of tab buttons, the accept/reject/show-on-map
// buttons, and overlays the star-map and wanted-board sub-windows.

class ScrollTouchWindow;
class StarMap;
class ChoiceWindow;
class WantedWindow;
class TouchButton;
class ImagePart;

class MissionsWindow {
public:
    ScrollTouchWindow*        m_pCampaignWindow;   // left (campaign) scroll window
    ScrollTouchWindow*        m_pFreelanceWindow;  // right (freelance) scroll window
    StarMap*                  m_pStarMap;          // star-map overlay (active when m_starMapActive)
    ChoiceWindow*             m_pChoiceWindow;     // confirm/choice dialog
    WantedWindow*             m_pWantedWindow;     // wanted-board window (active when m_mode == 1)
    Array<TouchButton*>*      m_pTabButtons;       // tab buttons (campaign / freelance)
    Array<ImagePart*>*        m_pAgentImageParts;  // freelance-agent portrait image parts
    int                       m_textHalfHeight;    // cached (font line height / 2 - 1)
    uint8_t                   m_choiceActive;      // confirm dialog open flag
    uint8_t                   m_field_0x21;        // secondary choice-window draw flag
    uint8_t                   m_starMapActive;     // star-map overlay active flag
    uint8_t                   m_hangarNeedsUpdate; // hangar-needs-update flag
    TouchButton*              m_pAcceptButton;     // accept-mission button
    TouchButton*              m_pRejectButton;     // reject-mission button
    TouchButton*              m_pMapButton;        // show-on-map button
    int                       m_x;                 // window x
    int                       m_y;                 // window y
    int                       m_width;             // window width
    int                       m_height;            // window height
    int                       m_mode;              // active mode (1 == wanted board)

    MissionsWindow();
    ~MissionsWindow();

    int  OnTouchMove(int x, int y);
    int  OnTouchBegin(int x, int y);
    void OnTouchEnd(int x, int y);
    void setHangarUpdate(bool v);
    uint8_t hangarNeedsUpdate();
    void render3D();
    int  init();
    void draw();
    void update(int dt);

    // Mode-specific sub-window dispatchers, reached from draw()/update() when an
    // overlay is active: the wanted board (m_pWantedWindow, active when m_mode == 1)
    // or the star-map overlay (m_pStarMap, active when m_starMapActive is set).
    void drawWanted();   // paint the active wanted-board window
    void drawStarMap();  // paint the active star-map overlay
    void acceptAction(); // advance the wanted-board window one frame
    void cancelAction(); // advance the star-map overlay one frame
};
#endif
