/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */

#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <gradientcolorwidget.h>

#include "terrain.h"

/**
 * @brief The GUI class est un widget servant à manipuler les contrôles du bruit de Perlin
 * de notre terrain
 */
class GUI : public QWidget
{
    Q_OBJECT
public:
    explicit GUI(Terrain* t, QWidget* parent = 0);

private:


    QSlider* octaveSlider;
    QSlider* frequencySlider;
    QSlider* persistenceSlider;

    QLabel* octaveValue;
    QLabel* frequencyValue;
    QLabel* persistenceValue;

    QList<GradientColorWidget*> list;

    QPushButton* lightButton;
    QPushButton* addGradientButton;


    QLabel* imgLabel;

    Terrain* terrain;

    bool lightEnabled = true;



    QVBoxLayout* layout;
    QHBoxLayout* octaveHLayout;
    QHBoxLayout* frequencyHLayout;
    QHBoxLayout* persistenceHLayout;
    QHBoxLayout* buttonHLayout;

signals:

public slots:
    void SwapLight();
    void GeneratePerlinNoise();
    void OctaveChanged(int);
    void FrequencyChanged(int);
    void PersistenceChanged(int);
    void AddGradiantPoint();
    void RemoveGradiantPoint(GradientColorWidget* gcw);
    void UpdateLayout();

};

#endif // GUI_H
