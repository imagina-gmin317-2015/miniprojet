/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */

#ifndef TERRAIN_H
#define TERRAIN_H


#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QString>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector2D>
#include <iostream>
#include <QGuiApplication>

#include "openglwindow.h"
#include "quadtree.h"
#include "voxel.h"

using namespace std;

class VertexData;

/**
 * @brief The Terrain class est notre fenêtre de démonstration de terrain.
 */
class Terrain : public OpenGLWindow
{
public:
    Terrain(QString str, QString strColor, QGuiApplication*);
    ~Terrain();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    void DisplayStaticQuatree();
    void BuildStaticQuatree();
    void BuildHighResTerrain();

    void UpdateQuadtree();
    void UpdateResolution();

    void Rebuild();

    void UpdateNoise();

    //GETTER
    QString getHeightmapColorPath() const { return heightmapColorPath; }
    QString getHeightmapPath() const { return heightmapPath; }

    void keyPressEvent( QKeyEvent * event );
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent* event);

private:
    void openImage(QString str);
    void createTerrain();
    void displayTerrain();

public:
    int terrain_height, terrain_width;          //Largeur et longueur du terrain



    float frequency=1.0f;
    float octave=1;
    float persistence=1.0f;
    bool lightEnabled=true;
    QMap<float,QVector3D> gradiants;
    float decalX=0.0f;
    float decalY=0.0f;
private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    ////////////////////////////////////////////////

    QGuiApplication* app;                       //Application

    QImage heightmap;                           //Image de heightmap
    QString heightmapPath;
    QString heightmapColorPath;

    int* hauteur;                               //Tableau des hauteurs, correspond aux pixels de l'image

    VertexData* vertices;                       //Tableau de vertices
    GLushort* indices;                          //Tableau d'indices
    QOpenGLBuffer arrayBuf;                     //Buffer du tableau de vertices
    QOpenGLBuffer indexBuf;                     //Buffer du tableau d'indices

    QVector3D position;                         //position camera
    float direction_vue_h, direction_vue_v;     //angle vue caméra
    bool souris_active;                         //clique gauche enfoncé ou pas

    QVector3D direction;                        //vecteur direction de la caméra
    QVector3D right;                            //vecteur right perpendiculaire au vecteur direction de la caméra
    QVector3D up;                               //vecteur indiquant le haut pour la caméra

    bool wireframe;                             //Affichage fil de fer ou pas

    Quadtree* q;                                //Quadtree du Terrain
    std::vector<Voxel*> voxels;
    float delta = 0.1f;
    //int resolution = 7;
    int resolution = 127;

    bool bBuildDecimateTerrain = false;
    bool bDisplayQuadtree = false;

};

#endif // TERRAIN_H
