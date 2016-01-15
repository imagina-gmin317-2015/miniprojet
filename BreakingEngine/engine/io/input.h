#ifndef INPUT_H
#define INPUT_H

#include "engine/core/common.h"
#include "engine/rendering/breakingwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMap>

class Input
{
    friend class BreakingWindow;
    enum InputState {NONE, DOWN, MAINTAIN, RELEASED};
private:
    Input();
    ~Input();

    QMap<Qt::Key, InputState> keyboardState;

    QMap<Qt::MouseButton, InputState> mouseState;

    QVector2D mp;

protected:
    static Input singleton;


public:
    void purge();

    void handlePressedKeyEvent(QKeyEvent* event);
    void handleReleasedKeyEvent(QKeyEvent* event);
    void handleMouseEvent(QMouseEvent* event);


    static bool getKey(Qt::Key);
    static bool getKeyDown(Qt::Key);
    static bool getKeyUp(Qt::Key);

    static QVector2D mousePosition();
    static bool getMouse(Qt::MouseButton);
    static bool getMouseDown(Qt::MouseButton);
    static bool getMouseUp(Qt::MouseButton);
signals:

public slots:
};

#endif // INPUT_H
