#ifndef GOF2_DIALOGUEWINDOW_H
#define GOF2_DIALOGUEWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Mission;
class Level;
class Agent;
class ImagePart;
class TouchButton;
class ChoiceWindow;
class ScrollTouchWindow;

// Modal mission/message dialogue: a framed window showing an agent portrait,
// title and body text, optional briefing/success paging and a yes/no choice.
class DialogueWindow {
public:
    TouchButton* prevButton;
    TouchButton* nextButton;
    TouchButton* moreButton;
    Array<ImagePart*>* faceParts;        // face image-part list (loadChar result)
    int campaignMission;
    int frameX;
    int frameY;
    int frameWidth;
    int frameHeight;
    void* clientImage;
    String bodyText;                     // dialogue body text
    String agentName;                    // agent / client name (title text)
    ScrollTouchWindow* scrollWindow;
    int kind;
    int page;
    Mission* mission;
    ChoiceWindow* choiceWindow;
    uint8_t choiceActive;
    Level* level;
    int* briefingOffsets;                // campaign briefing offset table
    int* successOffsets;                 // campaign success offset table
    int voiceSound;
    int autoAdvanceTimer;
    int pauseLength;
    uint8_t mirrorFace;                  // draw the portrait mirrored

    DialogueWindow();
    DialogueWindow(Mission* mission, Level* level, int kind);
    DialogueWindow(String* text, String* agentName, int* parts);
    ~DialogueWindow();

    int OnTouchBegin(int x, int y);
    int OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void draw();
    bool hasLevel();
    int init();
    bool isFirstPage();
    bool isLastPage();
    int length();
    void loadContent();
    int nextPage();
    int pickGermanGenericTextBecauseWeSaved100EurosWithThat(int kind, Agent* agent);
    int previousPage();
    void set(Mission* mission, int kind, int campaign);
    void setLevel(Level* level);
    void update(int dt);

    // Static campaign-dialogue lookups: do the offset tables list a briefing /
    // success dialogue for this campaign-mission id?  (id capped at 0xa1.)
    static bool hasBriefingDialogue(int id);
    static bool hasSuccessDialogue(int id);
};
#endif
