#include "go_button.hpp"

namespace {

const vector<string> goIconPattern = {
    "GGGGGGGGGBGGGGG",
    "GGGGGGGGGBBGGGG",
    "GGGGGGGGGBUBGGG",
    "ccccccccccUuBGG",
    "cUUUUUUUUUUvvBG",
    "cUvMMMMMMMMMMdB",
    "cUMMMMMMMMMMddB",
    "cMDDDDDDDDMdDBG",
    "cbbbbbbbbbdDBGG",
    "GGGGGGGGGBDBGGG",
    "GGGGGGGGGBBGGGG",
    "GGGGGGGGGBGGGGG"
};

ImageSlice createActiveGoIcon() {
    return ImageSlice::createImageFromStrings(
        goIconPattern,
        {
            {'G', {192, 192, 192}},
            {'B', {0, 0, 0}},
            {'b', {32, 32, 32}},
            {'c', {64, 64, 64}},
            {'W', {255, 255, 255}},
            {'U', {120, 255, 120}},
            {'u', {109, 236, 109}},
            {'v', {102, 226, 102}},
            {'M', {96, 216, 96}},
            {'d', {82, 188, 82}},
            {'D', {68, 160, 68}},
        }
    );
}

ImageSlice createPassiveGoIcon() {
    return ImageSlice::createImageFromStrings(
        goIconPattern,
        {
            {'G', {192, 192, 192}},
            {'B', {0, 0, 0}},
            {'b', {32, 32, 32}},
            {'c', {64, 64, 64}},
            {'W', {255, 255, 255}},
            {'U', {255, 255, 255}},
            {'u', {232, 232, 232}},
            {'v', {214, 214, 214}},
            {'M', {200, 200, 200}},
            {'d', {172, 172, 172}},
            {'D', {144, 144, 144}},
        }
    );
}

ImageSlice activeGoIcon = createActiveGoIcon();
ImageSlice passiveGoIcon = createPassiveGoIcon();

}

GoButton::GoButton(CKey,
    weak_ptr<WidgetParent> widgetParent,
    weak_ptr<GoButtonEventHandler> eventHandler
)
    : Widget(widgetParent)
{
    CEF_REQUIRE_UI_THREAD();

    eventHandler_ = eventHandler;

    mouseOver_ = false;
    mouseDown_ = false;
}

void GoButton::mouseMove_(int x, int y) {
    ImageSlice viewport = getViewport();

    bool newMouseOver =
        x >= 0 && x < viewport.width() &&
        y >= 0 && y < viewport.height();
    
    if(newMouseOver != mouseOver_) {
        mouseOver_ = newMouseOver;
        signalViewDirty_();
    }
}

void GoButton::widgetRender_() {
    CEF_REQUIRE_UI_THREAD();

    ImageSlice viewport = getViewport();

    // Background
    viewport.fill(0, Width, 0, Height, 192);

    const int IconX = 4;
    const int IconY = 5;

    if(mouseOver_) {
        // Frame
        viewport.fill(0, Width - 1, 0, 1, mouseDown_ ? 128 : 255);
        viewport.fill(0, 1, 1, Height - 1, mouseDown_ ? 128 : 255);
        viewport.fill(0, Width - 1, Height - 1, Height, mouseDown_ ? 255 : 128);
        viewport.fill(Width - 1, Width, 0, Height, mouseDown_ ? 255 : 128);

        // Icon
        int d = mouseDown_ ? 1 : 0;
        viewport.putImage(activeGoIcon, IconX + d, IconY + d);
    } else {
        viewport.putImage(passiveGoIcon, IconX, IconY);
    }
}

void GoButton::widgetMouseDownEvent_(int x, int y, int button) {
    CEF_REQUIRE_UI_THREAD();

    if(button == 0) {
        mouseDown_ = true;
        signalViewDirty_();
    }
}

void GoButton::widgetMouseUpEvent_(int x, int y, int button) {
    CEF_REQUIRE_UI_THREAD();

    if(button == 0) {
        if(mouseDown_ && mouseOver_) {
            postTask(eventHandler_, &GoButtonEventHandler::onGoButtonPressed);
        }

        mouseDown_ = false;
        signalViewDirty_();
    }
}

void GoButton::widgetMouseMoveEvent_(int x, int y) {
    CEF_REQUIRE_UI_THREAD();
    mouseMove_(x, y);
}

void GoButton::widgetMouseEnterEvent_(int x, int y) {
    CEF_REQUIRE_UI_THREAD();
    mouseMove_(x, y);
}

void GoButton::widgetMouseLeaveEvent_(int x, int y) {
    CEF_REQUIRE_UI_THREAD();

    if(mouseOver_) {
        mouseOver_ = false;
        signalViewDirty_();
    }
}
