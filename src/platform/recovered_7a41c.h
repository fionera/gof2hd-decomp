#ifndef GOF2_RECOVERED_7A41C_H
#define GOF2_RECOVERED_7A41C_H


extern "C" unsigned int F(unsigned int value);

int GetKeyState(char *name);

int GetKeyState(int index);

void MouseInput(int dx, int dy);

void MouseWheel(float delta, float residual);

void SetKeyCode(const char *name, int slot, int code);

int IsSubMenuActive(int player);

bool ArrowKeyPressed();

bool keyIsPressed();

void LowerMouseWheel();

void keyPressed(AbyssEngine::Engine *engine, int key);

void keyEventPressed(AbyssEngine::Engine *engine, char *name);

void keyReleased(AbyssEngine::Engine *engine, int key);

void keyReleasedWithDelay(AbyssEngine::Engine *engine, int key);

void SendStoredKeyUpEvents(AbyssEngine::Engine * engine);

void keyEventReleased(AbyssEngine::Engine *engine, char *name);

void KeyboardAnimationTimer(AbyssEngine::Engine * engine);

int ActualizeMouseVisibilty(int force);

void actualizeButtonPositions(AbyssEngine::Engine * engine);

void simulateTouch(AbyssEngine::Engine * engine);

#endif
