#include "input.h"
#include <QList>
#include <QDebug>

Input Input::singleton;
Input::Input()
{

}

Input::~Input()
{

}

///Must be run after
void Input::purge()
{
    QMapIterator<Qt::Key, InputState> i(keyboardState);
    QList<Qt::Key> released;
    while (i.hasNext()) {
        i.next();
        if(i.value()==InputState::RELEASED) {
            released.append(i.key());
        }
    }
    foreach(Qt::Key key, released) {
        keyboardState[key] = InputState::NONE;
    }

    QMapIterator<Qt::MouseButton, InputState> i2(mouseState);
    QList<Qt::MouseButton> released2;
    while (i2.hasNext()) {
        i2.next();
        if(i2.value()==InputState::RELEASED) {
            released2.append(i2.key());
        }
    }
    foreach(Qt::MouseButton key, released2) {
        mouseState[key] = InputState::NONE;
    }


}

void Input::handlePressedKeyEvent(QKeyEvent *event)
{
    Qt::Key key = (Qt::Key)event->key();
    if(!keyboardState.contains(key) && keyboardState[key] != InputState::MAINTAIN
            && keyboardState[key] != InputState::DOWN) {
        keyboardState[key] = InputState::DOWN;
    }
    else {
        keyboardState[key] = InputState::MAINTAIN;
    }

}
QVector2D Input::mousePosition() {
    return singleton.mp;
}

void Input::handleReleasedKeyEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();
    keyboardState[key] = InputState::RELEASED;
}

void Input::handleMouseEvent(QMouseEvent *event) {
    QVector2D mousePosition2( event->globalPos().x(), event->globalPos().y());
    mp = mousePosition2;
    Qt::MouseButton button = (Qt::MouseButton) event->button();


    if(event->type() == QEvent::MouseButtonPress) {
        if(mouseState.contains(button) && (mouseState[button] == InputState::DOWN || mouseState[button] == InputState::MAINTAIN)) {
            mouseState[button] = InputState::MAINTAIN;

        }else
            mouseState[button] = InputState::DOWN;

    }else
        if(event->type() == QEvent::MouseButtonRelease)
            mouseState[button] = InputState::RELEASED;
}

bool Input::getKey(Qt::Key key)
{
    return singleton.keyboardState.contains(key) && singleton.keyboardState[key] == InputState::MAINTAIN;
}

bool Input::getKeyDown(Qt::Key key) {
    return singleton.keyboardState.contains(key) && singleton.keyboardState[key] == InputState::DOWN;
}

bool Input::getKeyUp(Qt::Key key) {
    return singleton.keyboardState.contains(key) && singleton.keyboardState[key] == InputState::RELEASED;
}

bool Input::getMouse(Qt::MouseButton button) {
    return singleton.mouseState.contains(button) && singleton.mouseState[button] == InputState::MAINTAIN;
}

bool Input::getMouseDown(Qt::MouseButton button) {
    return singleton.mouseState.contains(button) && singleton.mouseState[button] == InputState::DOWN;
}

bool Input::getMouseUp(Qt::MouseButton button) {
    return singleton.mouseState.contains(button) && singleton.mouseState[button] == InputState::RELEASED;
}
