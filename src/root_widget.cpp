#include "root_widget.hpp"

#include "control_bar.hpp"
#include "browser_area.hpp"

RootWidget::RootWidget(CKey, weak_ptr<WidgetEventHandler> widgetEventHandler)
    : Widget(widgetEventHandler)
{
    CEF_REQUIRE_UI_THREAD();
    // Initialization is finalized in afterConstruct_
}

void RootWidget::onWidgetViewDirty() {
    CEF_REQUIRE_UI_THREAD();
    signalViewDirty_();
}

void RootWidget::afterConstruct_(shared_ptr<RootWidget> self) {
    controlBar_ = ControlBar::create(self);
    browserArea_ = BrowserArea::create(self);
}

void RootWidget::widgetViewportUpdated_() {
    CEF_REQUIRE_UI_THREAD();

    ImageSlice controlBarViewport, browserAreaViewport;
    tie(controlBarViewport, browserAreaViewport) =
        getViewport().splitY(ControlBar::Height);

    controlBar_->setViewport(controlBarViewport);
    browserArea_->setViewport(browserAreaViewport);
}

void RootWidget::widgetRender_() {
    CEF_REQUIRE_UI_THREAD();
    LOG(INFO) << "Rendering root widget";
}