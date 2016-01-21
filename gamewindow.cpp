#include "gamewindow.h"
#include "player.h"

#include <QVector3D>
#include <QVector>

static const int NB_PLAYER = 4;
static const int HALF_DIM_BOARD = 1;


GameWindow::GameWindow() : _playerProgram(0), _tailsProgram(0)
{
    QVector<Qt::Key> controller;
    controller.push_back(Qt::Key_Q);
    controller.push_back(Qt::Key_D);
    _controller.push_back(controller);
    controller.clear();
    controller.push_back(Qt::Key_Left);
    controller.push_back(Qt::Key_Right);
    _controller.push_back(controller);
    controller.clear();
    controller.push_back(Qt::Key_I);
    controller.push_back(Qt::Key_P);
    _controller.push_back(controller);
    controller.clear();
    controller.push_back(Qt::Key_F);
    controller.push_back(Qt::Key_H);
    _controller.push_back(controller);

    _colorList.push_back(QVector4D(1.0, 0.2, 0.2, 1.0));
    _colorList.push_back(QVector4D(0.2, 1.0, 0.2, 1.0));
    _colorList.push_back(QVector4D(0.2, 0.2, 1.0, 1.0));
    _colorList.push_back(QVector4D(1.0, 1.0, 0.2, 1.0));

    _particlesSystem.push_back(new Particles(QVector4D(1.0, 0.2, 0.2, 1.0)));
    _particlesSystem.push_back(new Particles(QVector4D(0.2, 1.0, 0.2, 1.0)));
    _particlesSystem.push_back(new Particles(QVector4D(0.2, 0.2, 1.0, 1.0)));
    _particlesSystem.push_back(new Particles(QVector4D(1.0, 1.0, 0.2, 1.0)));

    _gameState = GameState::Menu;

    _physicTimer = new QTimer();
    connect(_physicTimer, SIGNAL(timeout()), &myWorld, SLOT(tick()));

    _renderTimer = new QTimer();
    connect(_renderTimer, SIGNAL(timeout()), this, SLOT(renderNow()));

    _tailTimer = new QTimer();
    connect(_tailTimer, SIGNAL(timeout()), this, SLOT(updateTails()));

    for(Particles *p : _particlesSystem)
    {
        connect(_renderTimer, SIGNAL(timeout()), p, SLOT(update()));
    }

    connect(_renderTimer, SIGNAL(timeout()), &_bonus, SLOT(update()));

    connect(myWorld.getMyColliderInstance(),SIGNAL(playerExplodes(Player*)),this,SLOT(playerExplodes(Player*)));
    connect(myWorld.getMyColliderInstance(),SIGNAL(destroyBody(b2Body*)),&myWorld,SLOT(addToBin(b2Body*)));
    //connect(&_bonus,SIGNAL(addBonusSignal(Food*)),this,SLOT(addBonus(Food*)));

    //initializeGame();
    endGame = false;
}

GameWindow::~GameWindow()
{
}

void GameWindow::initialize()
{
    initPlayerShaderPrograme();
    initParticlesShaderPrograme();
    initTailsShaderPrograme();
    initBorderShaderPrograme();
    initBonusShaderPrograme();
}


void GameWindow::initPlayerShaderPrograme()
{
    _playerProgram = new QOpenGLShaderProgram(this);

    _playerProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/player.vert");
    _playerProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/player.frag");

    _playerProgram->link();

    _playerProgram->bind();

    _playerPosAttr = _playerProgram->attributeLocation("posAttr");
    _playerCenterUni = _playerProgram->uniformLocation("playerCenter");
    _playerColUni = _playerProgram->uniformLocation("colUni");
    _matrixUniform = _playerProgram->uniformLocation("matrix");

    _playerVao.create();
    _playerVao.bind();

    _playerVbo.create();
    _playerVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _playerVbo.bind();

    _playerVao.bind();

    _playerProgram->setAttributeBuffer(_playerPosAttr, GL_FLOAT, 0, 3, 0);

    _playerProgram->enableAttributeArray(_playerPosAttr);

    _playerVao.release();

    _playerProgram->release();
}

void GameWindow::initParticlesShaderPrograme()
{
    _particlesProgram = new QOpenGLShaderProgram(this);

    _particlesProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/tail.vert");
    _particlesProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/tail.frag");

    _particlesProgram->link();

    _particlesProgram->bind();

    _particlesPosAttr = _particlesProgram->attributeLocation("posAttr");
    _particlesColAttr = _particlesProgram->uniformLocation("colAttr");
    _matrixUniform = _particlesProgram->uniformLocation("matrix");

    _particlesVao.create();
    _particlesVao.bind();

    _particlesVbo.create();
    _particlesVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _particlesVbo.bind();

    _particlesProgram->setAttributeBuffer(_particlesPosAttr, GL_FLOAT, 0, 3, 0);

    _particlesProgram->enableAttributeArray(_particlesPosAttr);
    _particlesProgram->enableAttributeArray(_particlesColAttr);

    _particlesVao.release();

    _particlesProgram->release();
}

void GameWindow::initTailsShaderPrograme()
{
    _tailsProgram = new QOpenGLShaderProgram(this);

    _tailsProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/tail.vert");
    _tailsProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/tail.frag");

    _tailsProgram->link();

    _tailsPosAttr = _tailsProgram->attributeLocation("posAttr");
    _tailsColAttr = _tailsProgram->uniformLocation("colAttr");
    _matrixUniform = _tailsProgram->uniformLocation("matrix");

    _tailsVao.create();
    _tailsVao.bind();

    _tailsVbo.create();
    _tailsVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _tailsVbo.bind();

    _tailsProgram->setAttributeBuffer(_tailsPosAttr, GL_FLOAT, 0, 3, 0);
    _tailsProgram->enableAttributeArray(_tailsPosAttr);

    _tailsVao.release();
    _tailsProgram->release();
}



void GameWindow::initBorderShaderPrograme()
{

    _borderProgram = new QOpenGLShaderProgram(this);

    _borderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/border.vert");
    _borderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/border.frag");

    _borderProgram->link();

    _borderPosAttr = _borderProgram->attributeLocation("posAttr");
    _borderColUni = _borderProgram->uniformLocation("colUni");
    _matrixUniform = _borderProgram->uniformLocation("matrix");

    _borderVao.create();
    _borderVao.bind();

    _borderVbo.create();
    _borderVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _borderVbo.bind();

    _borderProgram->setAttributeBuffer(_borderPosAttr, GL_FLOAT, 0, 3, 0);
    _borderProgram->enableAttributeArray(_borderPosAttr);

    _borderVao.release();
    _borderProgram->release();
}

void GameWindow::initBonusShaderPrograme()
{
    _bonusProgram = new QOpenGLShaderProgram(this);

    _bonusProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/bonus.vert");
    _bonusProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/bonus.frag");

    _bonusProgram->link();

    _bonusPosAttr = _bonusProgram->attributeLocation("posAttr");
    _bonusColAttr = _bonusProgram->attributeLocation("colAttr");
    _matrixUniform = _bonusProgram->uniformLocation("matrix");

    _bonusVao.create();
    _bonusVao.bind();

    _bonusVbo.create();
    _bonusVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _bonusVbo.bind();

    _bonusProgram->enableAttributeArray(_bonusPosAttr);
    _bonusProgram->enableAttributeArray(_bonusColAttr);

    _bonusVao.release();
    _bonusProgram->release();
}

void GameWindow::initializeGame()
{

    _player.clear();

    for(int i = 0; i < NB_PLAYER; i++)
    {
        _player.push_back(Player(i + 1,_controller[i], QVector3D((-HALF_DIM_BOARD+0.2)+((2*HALF_DIM_BOARD-0.4)/(NB_PLAYER+1))*(i), -HALF_DIM_BOARD+0.3, 0), _colorList[i]));
    }

    QVector<Player *> temp;
    for (int i=0 ;i <_player.size();i++){
        temp<< &_player[i];
    }


    QVector< QVector2D> vec;
    vec<<QVector2D(-1,-1)<<QVector2D(-1,1)<<QVector2D(1,1)<<QVector2D(1,-1);
    _border = new Border(vec);

    myWorld.setBorder(_border);
    myWorld.setPlayers(temp);
    myWorld.init();

    _physicTimer->start(30);
    _renderTimer->start(30);
    _tailTimer->start(30);
}


GLuint GameWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

Border *GameWindow::border() const
{
    return _border;
}

void GameWindow::setBorder(Border *border)
{
    _border = border;
}


void GameWindow::render(){
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    switch(_gameState)
    {
    case GameState::Menu:
        renderMenu();
        break;
    case GameState::Game:
        renderGame();
        break;
    default:
        break;
    }
}

void GameWindow::renderMenu()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    QPainter painter(m_device);
    painter.setFont(QFont("Arial", 80));
    painter.setPen(Qt::blue);
    painter.drawText(0, 0, 600, 500, Qt::AlignTop, "SneakyTron");
    painter.setFont(QFont("Arial", 40));
    painter.drawText(0, 0, 800, 500, Qt::AlignBottom, "Press enter to start !");
    painter.end();
}

void GameWindow::renderEnd()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    QPainter painter(m_device);
    painter.setFont(QFont("Arial", 80));

    for(Player player : _player)
    {
        if(player.getAlive())
        {
            QVector4D color = player.color();
            QColor c = QColor(color.x(), color.y(), color.z());
            painter.setPen(c);
        }
    }
    //painter.setPen(Qt::yellow);
    painter.drawText(0, 0, 600, 500, Qt::AlignTop, "Victory !");
    painter.end();
}

void GameWindow::renderGame()
{
    QMatrix4x4 matrix;

    for(Player &player : _player)
    {
        if (!player.getAlive()) continue;
        _playerProgram->bind();
        _playerProgram->setUniformValue(_matrixUniform, matrix);
        _playerProgram->setUniformValue(_playerColUni, player.color());
        _playerProgram->setUniformValue(_playerCenterUni, QVector3D(10,10,10));

        _playerVao.bind();
        _playerVbo.bind();

        QVector<QVector3D> playerShape;
        QVector3D cornerDistanceToCenter = QVector3D(0.01f, 0.01f, 0.0f);
        playerShape << cornerDistanceToCenter +player.position();
        cornerDistanceToCenter = QVector3D(0.01f, -0.01f, 0.0f);
        playerShape << cornerDistanceToCenter+player.position();
        cornerDistanceToCenter = QVector3D(-0.01f, -0.01f, 0.0f);
        playerShape << cornerDistanceToCenter+player.position();
        cornerDistanceToCenter = QVector3D(-0.01f, 0.01f, 0.0f);
        playerShape << cornerDistanceToCenter+player.position();

        size_t posSize = playerShape.size() * sizeof(QVector3D);
        _playerVbo.allocate(posSize);
        _playerVbo.write(0, playerShape.constData(), posSize);

        glDrawArrays(GL_POLYGON, 0, playerShape.size());

        _playerVao.release();
        _playerProgram->release();
    }

    for(Player player : _player)
    {
        if (!player.getAlive()) continue;
        _tailsProgram->bind();
        _tailsProgram->setUniformValue(_matrixUniform, matrix);

        _tailsProgram->setUniformValue(_tailsColAttr, player.color());

        _tailsVao.bind();
        _tailsVbo.bind();

        size_t tailSize = player.tail()->size()*sizeof(QVector3D);
        _tailsVbo.allocate(tailSize);
        _tailsVbo.write(0, player.tail()->getChain().constData(), tailSize);

        glDrawArrays(GL_LINE_STRIP, 0, player.tail()->size());

        _tailsVao.release();
        _tailsProgram->release();


    }

    for(Particles *p : _particlesSystem)
    {
        if(p->getParticlesPosition().size() <= 0)
            continue;
        _particlesProgram->bind();
        _particlesProgram->setUniformValue(_matrixUniform, matrix);

        _particlesProgram->setUniformValue(_particlesColAttr, p->getColor());

        _particlesVao.bind();
        _particlesVbo.bind();

        size_t particlesSize = p->getParticlesPosition().size()*sizeof(QVector3D);
        _particlesVbo.allocate(particlesSize);
        _particlesVbo.write(0, p->getParticlesPosition().constData(), particlesSize);

        glDrawArrays(GL_POINTS, 0, p->getParticlesPosition().size());

        _particlesVao.release();
        _particlesProgram->release();
    }

    _borderProgram->bind();
    _borderProgram->setUniformValue(_matrixUniform, matrix);

    _borderProgram->setUniformValue(_borderColUni, QVector4D(0.7,0.7,0.7,1));

    _borderVao.bind();
    _borderVbo.bind();

    QVector<QVector3D> vec;
    for (QVector2D v : _border->getPositions()) vec.push_back(QVector3D(v.x(), v.y(), 0.0));
    size_t borderSize = vec.size()*sizeof(QVector3D);
    _borderVbo.allocate(borderSize);
    _borderVbo.write(0, vec.constData(), borderSize);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_POLYGON, 0, vec.size());
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    _borderVao.release();
    _borderProgram->release();

    _bonusProgram->bind();
    _bonusProgram->setUniformValue(_matrixUniform, matrix);

    _bonusVao.bind();
    _bonusVbo.bind();

    size_t bonusPosSize = _bonus.getPositions().size()*sizeof(QVector3D), bonusColSize = _bonus.getColors().size()*sizeof(QVector4D);


    _bonusProgram->setAttributeBuffer(_bonusPosAttr, GL_FLOAT, 0, 3, 0);
    _bonusProgram->setAttributeBuffer(_bonusColAttr, GL_FLOAT, bonusPosSize, 4, 0);
    _bonusVbo.allocate(bonusPosSize + bonusColSize);
    _bonusVbo.write(0, _bonus.getPositions().constData(), bonusPosSize);
    _bonusVbo.write(bonusPosSize, _bonus.getColors().constData(), bonusColSize);

    glDrawArrays(GL_POINTS, 0, _bonus.getPositions().size());

    _bonusVao.release();
    _bonusProgram->release();
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(_gameState)
    {
    case GameState::Menu:
        switch(event->key())
        {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Return:
            startWorld();
            QTimer::singleShot(37,this,SLOT(switchGameMenu()));
            break;
        }
        break;
    case GameState::Game:
        for(Player &player : _player)
        {
            player.keyPressEvent(event);
        }

        switch(event->key())
        {
        case Qt::Key_Escape:
            destroyWorld();
            QTimer::singleShot(37,this,SLOT(switchGameMenu()));
            break;
        case Qt::Key_R:
            destroyWorld();
            startWorld();
            break;
        }
        break;
    case GameState::EndGame:
        switch(event->key())
        {
        case Qt::Key_Escape:
            close();
            break;
        }
        break;
    default:
        break;
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    for(Player &player : _player)
    {
        player.keyReleaseEvent(event);
    }
}

// SLOT

void GameWindow::playerExplodes(Player *player)
{
    static int cpt = 0;
    cpt++;
    if(cpt >= NB_PLAYER - 1)
        endGame = true;
    if(_particlesSystem[player->getId()-1]->animationDone())
        _particlesSystem[player->getId()-1]->initParticles(player->position());
    myWorld.addToBin(player->getBody());
    myWorld.addToBin(player->tail()->getBody());
    player->setAlive(false);
    qDebug()<<myWorld.getWorld()->GetBodyCount();

}


void GameWindow::startWorld()
{
   QTimer::singleShot(35,this, SLOT(initializeGame()));
}

void GameWindow::destroyWorld()
{
    myWorld.resetWorldContent();
    QTimer::singleShot(35,&myWorld, SLOT(tick()));
}

void GameWindow::switchGameMenu()
{
    (_gameState==Menu) ? _gameState = Game : _gameState = Menu;
}

void GameWindow::addBonus(Food *food)
{
    myWorld.addFood(food);
}

void GameWindow::updateTails()
{
    for(Player &player : _player)
    {
        player.updateTail();
    }
}

