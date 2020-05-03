#include "display.h"
#include "utils/utils.h"

Thread ui::Display::m_threadUi;
Thread ui::Display::m_threadEvent;
Queue<ui::Display::screenevent_t, UI_DISPLAY_THREAD_QUEUE_SIZE> ui::Display::m_screenQueue;
MemoryPool<ui::Display::screenevent_t, UI_DISPLAY_THREAD_QUEUE_SIZE> ui::Display::m_screenEventPool;
SerialLCD* ui::Display::m_display = null;
ui::screen_t ui::Display::m_screens[UI_DISPLAY_MAX_SCREENS] = {0};
int8_t ui::Display::m_currentScreen = -1;

void ui::Display::run(){ 
    /* Setup display */
    Display::m_display = new SerialLCD(NC, NC);
    Display::m_display->noCursor();
    Display::m_display->noBlink();
    /* Start the UI and event threads */
    Display::m_threadUi.start(callback(&Display::uiThread));
    Display::m_threadEvent.start(callback(&Display::eventThread));
    /* Set priorities */
    Display::m_threadUi.set_priority(UI_DISPLAY_THREAD_PRIORITY_UI);
    Display::m_threadEvent.set_priority(UI_DISPLAY_THREAD_PRIORITY_EVENT);
}

bool ui::Display::add(uint8_t id, const screen_t &screen) {
    /* If exists, couldn't add. Return false */
    if (Display::m_screens[id].render) {
        return false;
    }
    /* Add it */
    Display::m_screens[id] = screen;
    /* Return */
    return true;
}

bool ui::Display::get(uint8_t id, screen_t &screen) {
    /* If not exists, couldn't get. Return false */
    if (!Display::m_screens[id].render) {
        return false;
    }
    /* Return it */
    screen = Display::m_screens[id];
    /* Return */
    return true;
}

bool ui::Display::go(uint8_t id) {
    /* If not exists, couldn't get. Return false */
    if (!Display::m_screens[id].render) { return false; }
    /* Allocate a message */
    screenevent_t *message = Display::m_screenEventPool.alloc();
    /* Check if message could be allocated */
    if (message == nullptr) { return false; }
    /* Fill it and send it */
    message->id = id;
    message->screen = &Display::m_screens[id];
    Display::m_screenQueue.put(message);
    /* Return */
    return true;
}

int8_t ui::Display::current() {
    /* Return the current display */
    return Display::m_currentScreen;
}

void ui::Display::dirty() {
    /* Send */
    Display::m_threadUi.flags_set(UI_DISPLAY_THREAD_FLAG_UI_DIRTY);
}

void ui::Display::repaint() {
    /* Check if id fine */
    if (Display::m_currentScreen < 0) { return; }
    /* Get screen */
    screen_t &screen = Display::m_screens[Display::m_currentScreen];
    /* Check if current screen is fine */
    if (!screen.render) { return; }
    /* Clear */
    Display::m_display->clear();
    /* Draw */
    screen.render(screen.state, Display::m_display);
}

void ui::Display::uiThread() {
    /* Thread loop */
    while (1) {
        /* Wait for dirty event */
        ThisThread::flags_wait_all(UI_DISPLAY_THREAD_FLAG_UI_DIRTY);
        /* Repaint */
        repaint();
        /* Send redraw ok */
        Display::m_threadEvent.flags_set(UI_DISPLAY_THREAD_FLAG_EVENT_DRAWN);
    }
}

void ui::Display::eventThread()
{
    /* Dirty flag & current screen */
    bool dirty = false;
    /* Thread loop */
    while (1) {
        /* Reset dirty flag */
        dirty = false;
        /* Check if screen exists */
        if (Display::m_currentScreen >= 0 && Display::m_screens[Display::m_currentScreen].update) {
            /* Get screen */
            screen_t &screen = Display::m_screens[Display::m_currentScreen];
            /* Do update */
            screen.update(screen.state, &dirty);
            /* Check dirty flag */
            if (dirty) {
                /* Set as dirty, but don't wait for drawn flag */
                Display::dirty();
            }
        }
        /* Check queue */
        auto result = Display::m_screenQueue.get(UI_DISPLAY_REFRESH_UI_RATE);
        /* If anything was found */
        if (result.status == osEventMessage) {
            /* Get message */
            screenevent_t *event = (screenevent_t *)result.value.p;
            /* Check if screen we want to go to is different */
            if (event->id != Display::m_currentScreen) {
                /* Set current id */
                Display::m_currentScreen = event->id;
                /* Free the event */
                Display::m_screenEventPool.free(event);
                /* Clear flags in case */
                ThisThread::flags_clear(UI_DISPLAY_THREAD_FLAG_EVENT_DRAWN);
                /* Set as dirty */
                Display::dirty();
                /* Wait for drawn flag */
                ThisThread::flags_wait_all(UI_DISPLAY_THREAD_FLAG_EVENT_DRAWN);
            }
        }
    }
}