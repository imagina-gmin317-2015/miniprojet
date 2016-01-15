#include "engine/rendering/breakingwindow.h"
#include "engine/core/scene.h"
#include "engine/rendering/renderer.h"
#include "engine/core/gameobject.h"
#include "engine/core/transform.h"
#include "engine/core/updatable.h"

#include "engine/core/camera.h"
#include <QApplication>
#include "scene1.h"
#include "terrain.h"
int main(int argc, char* argv[])
{

    //QTApplication
    QApplication a(argc, argv);
    //Surface
    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24);

    //BreakingWindow
    BreakingWindow w(&a);
    w.setFormat(format);
    //w.setSurfaceType(QWindow::SurfaceType::OpenGLSurface);
    w.resize(640, 480);
    w.setAnimating(true);

    // Create Scene
    Scene* scene = new Scene1();
    w.setupScene(scene);
    w.show();

    return a.exec();
}
