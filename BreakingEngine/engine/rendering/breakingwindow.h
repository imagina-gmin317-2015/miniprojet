#ifndef BREAKINGRENDER_H
#define BREAKINGRENDER_H

#include "openglwindow.h"

#include "engine/core/common.h"
#include <QTimer>
#include <Qt>

class BreakingWindow : public virtual OpenGLWindow
{
    Q_OBJECT
private:
    static BreakingWindow* main_window;
    static QApplication* app;

    Scene* m_scene = nullptr;
    QTimer* m_timer;


public:
    explicit BreakingWindow(QApplication* app, QWindow* parent=0);
    virtual~BreakingWindow();

    virtual void setupScene(Scene* scene);

    virtual void initialize();
    virtual void render();

    void keyPressEvent(QKeyEvent * );
    void keyReleaseEvent(QKeyEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);


    float static widthWindow() {
        if(main_window!=nullptr)
            return main_window->width();
        return 0;
    }

    float static heightWindow() {
        if(main_window!=nullptr)
            return main_window->width();
        return 0;
    }


};

#endif // BREAKINGRENDER_H
