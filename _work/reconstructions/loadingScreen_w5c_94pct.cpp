
void loadingScreen(AbyssEngine::PaintCanvas *canvas, int progress, void *fontHolder) { // lint: void_ptr (exported free function; mangled signature is fixed)
    canvas->ClearBuffer(0xff);
    canvas->Begin2d();
    canvas->SetColor(0xffffffffu);
    if (fontHolder != nullptr) {
        unsigned int font = *static_cast<unsigned int *>(fontHolder);
        if (GameText::getLanguage() == 9 ||
            !canvas->ResourceLoaded(font, static_cast<AbyssEngine::ResourceType>(1))) {
            Globals::globals->loadFont(GameText::getLanguage());
            *static_cast<unsigned int *>(fontHolder) = font;
        }

        unsigned int spinner = 0xffffffffu;
        Globals::Canvas->Image2DCreate(0x504, spinner);
        unsigned int bar = 0xffffffffu;
        Globals::Canvas->Image2DCreate(0x505, bar);
        int barWidth = Globals::Canvas->GetImage2DWidth(bar);
        int spinnerWidth = Globals::Canvas->GetImage2DWidth(spinner);
        int barHeight = Globals::Canvas->GetImage2DHeight(bar);
        int topOffset = Globals::layout->field_0x27c;

        Globals::layout->drawBG();
        Globals::layout->drawHeader();
        Globals::layout->drawEmptyFooter(false);

        AbyssEngine::String caption(*Globals::gameText->getText(Globals::gameSaving ? 0x18a : 0x189), false);

        if (progress < 0) {
            progress = 100;
        }
        int remaining = 100;
        if (progress > 0) {
            remaining = 100 - progress;
        }

        Globals::Canvas->DrawString(font, caption, (Globals::w >> 1) - Globals::Canvas->GetTextWidth(font, caption) / 2,
                                    (Globals::h >> 1) - topOffset - Globals::layout->field_0x4 -
                                        Globals::layout->field_0x280,
                                    false);
        Globals::Canvas->DrawImage2D(spinner, (Globals::w >> 1) - (spinnerWidth >> 1), (Globals::h >> 1) - topOffset);

        if (remaining < 0) {
            remaining = 0;
        }
        if (GameText::getLanguage() == 9) {
            // Right-to-left bar: the fill width expression is repeated inline in the original.
            Globals::Canvas->DrawRegion2D(
                bar, (int) ((float) barWidth - (float) remaining * 0.01f * (float) barWidth), 0,
                (int) ((float) remaining * 0.01f * (float) barWidth), barHeight, 0.0f, 0, 0,
                (int) ((float) ((Globals::w >> 1) + (barWidth >> 1)) - (float) remaining * 0.01f * (float) barWidth),
                (Globals::h >> 1) - topOffset + Globals::layout->field_0x290);
        } else {
            Globals::Canvas->DrawRegion2D(bar, 0, 0, (int) ((float) remaining * 0.01f * (float) barWidth), barHeight,
                                          0.0f, 0, 0, (Globals::w >> 1) - (barWidth >> 1),
                                          (Globals::h >> 1) - topOffset + Globals::layout->field_0x290);
        }
    }
    canvas->End2d();
    canvas->SwapBuffer();
}
