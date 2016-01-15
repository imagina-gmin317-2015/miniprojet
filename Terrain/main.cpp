/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */

#include "terrain.h"
#include "gui.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include <QtGui>
#include <QMainWindow>

#include <QtCore/qmath.h>

#include <iostream>

#include <QApplication>
#include <QScrollArea>

using namespace std;

int main(int argc, char *argv[])
{
    //Démarrage du programme
    QApplication* app = new QApplication(argc, argv);

    //Modification de la surface OpenGL
    QSurfaceFormat format;
    format.setSamples(16);//Bonne taille
    format.setDepthBufferSize(24);//High precision

    //Initialisation du terrain en fonction des images par défaut
    Terrain* terrain = new Terrain("./breaking-default-256energy.bmp", "./breaking-default-256color.bmp", app);
    terrain->setTitle("BreakinEngine - Terrain");
    terrain->setFormat(format);
    terrain->resize(640, 480);
    terrain->setX(500);
    terrain->setY(0);
    terrain->show();
    terrain->setAnimating(true);


    // Fenêtre de contrôles de Perlin
    QMainWindow window;
    QScrollArea* scrollArea = new QScrollArea;
    GUI* gui = new GUI(terrain);
    scrollArea->setWidget(gui);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    window.setCentralWidget(scrollArea);
    //scrollArea->setBaseSize(500,1200);
    window.setWindowTitle("Breaking Engine - Contrôles");
    window.move(0,0);
    window.show();

    //Exécution du programme
    return app->exec();

}

