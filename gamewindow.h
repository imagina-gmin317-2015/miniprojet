#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui>
#include <QTimer>


#include "openglwindow.h"
#include "player.h"
#include "border.h"
#include "physic.h"
#include "particles.h"
#include "bonus.h"
#include "food.h"

enum GameState
{
    Menu,
    Game,
    EndGame
};

class GameWindow : public OpenGLWindow
{
    Q_OBJECT
private:
    Physic myWorld;
    GLuint loadShader(GLenum type, const char *source);
    QVector<Player> _player;
    Border* _border;
    QVector<QVector4D> _colorList;
    QVector<Particles *> _particlesSystem;
    Bonus _bonus;

    GLuint _playerPosAttr, _playerCenterUni, _playerColUni;
    GLuint _particlesPosAttr, _particlesColAttr;
    GLuint _tailsPosAttr, _tailsColAttr;
    GLuint _borderPosAttr, _borderColUni;
    GLuint _bonusPosAttr, _bonusColAttr;
    GLuint _matrixUniform;

    QOpenGLShaderProgram *_playerProgram;
    QOpenGLVertexArrayObject _playerVao;
    QOpenGLBuffer _playerVbo;

    QOpenGLShaderProgram *_particlesProgram;
    QOpenGLVertexArrayObject _particlesVao;
    QOpenGLBuffer _particlesVbo;

    QOpenGLShaderProgram *_tailsProgram;
    QOpenGLVertexArrayObject _tailsVao;
    QOpenGLBuffer _tailsVbo;

    QOpenGLShaderProgram *_borderProgram;
    QOpenGLVertexArrayObject _borderVao;
    QOpenGLBuffer _borderVbo;

    QOpenGLShaderProgram *_bonusProgram;
    QOpenGLVertexArrayObject _bonusVao;
    QOpenGLBuffer _bonusVbo;

    QTimer *_renderTimer;
    QTimer *_tailTimer;
    QTimer *_physicTimer;

    QVector< QVector<Qt::Key> > _controller;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void initPlayerShaderPrograme();
    void initParticlesShaderPrograme();
    void initTailsShaderPrograme();
    void initBorderShaderPrograme();
    void initBonusShaderPrograme();

    void renderMenu();
    void renderGame();
    void renderEnd();

    GameState _gameState;

    bool endGame;

public:
    GameWindow();
    ~GameWindow();
    void render();
    void initialize();
    void startWorld();
    void destroyWorld();


    Border *border() const;
    void setBorder(Border *border);

public slots:
    void updateTails();
    void playerExplodes(Player *player);
    void initializeGame();
    void switchGameMenu();
    void addBonus(Food *food);
};

#endif // GAMEWINDOW_H
