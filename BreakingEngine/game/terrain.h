#ifndef TERRAIN_H
#define TERRAIN_H

#include "engine/rendering/breakingwindow.h"

#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QString>

struct VertexData
{
    QVector3D position;
    QVector3D color;
    QVector2D texCoord;
    QVector3D normal;
};

class Terrain : public BreakingWindow
{
public:
    Terrain(QApplication* app, QString map);
    ~Terrain();

    void initialize();
    void render();
    bool event(QEvent *event);

    GLuint loadShader(GLenum type, const char *source);

private:
    void openImage(QString str);
    void createTerrain();
    void displayTerrain();

    void keyPressEvent(QKeyEvent *event);

signals:

public slots:

private:
    int m_frame;

    QOpenGLTexture* texture;
    QImage image;
    GLuint m_texture_location;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_texAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;

    QImage heightmap;                           //Image de heightmap

    /// TERRAIN DATA //////////////////////////////////////////////////////////////////////////////////////
    int* hauteur;                               //Tableau des hauteurs, correspond aux pixels de l'image
    int terrain_height, terrain_width;          //Largeur et longueur du terrain

    VertexData* vertices;                       //Tableau de vertices
    GLushort* indices;                          //Tableau d'indices
    QOpenGLBuffer arrayBuf;                     //Buffer du tableau de vertices
    QOpenGLBuffer indexBuf;                     //Buffer du tableau d'indices
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    bool wireframe;                             //Affichage fil de fer ou pas
};

#endif // TERRAIN_H
