/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QSlider>
#include <breakingnoise.h>
#include <QDebug>
#include <QScrollArea>
#include <QVector3D>
#include "gui.h"

GUI::GUI(Terrain* t, QWidget *parent) : QWidget(parent)
{
    terrain = t;

    QPushButton* generateP = new QPushButton("Generate Perlin Noise");
    addGradientButton = new QPushButton("+");

    lightButton = new QPushButton("Disable light");

    QImage heightmap;
    heightmap.load(terrain->getHeightmapColorPath());

    imgLabel = new QLabel();
    imgLabel->setPixmap(QPixmap::fromImage(heightmap));

    octaveHLayout = new QHBoxLayout;
    QLabel* octave = new QLabel("Octave : ");
    octaveValue = new QLabel("0");
    octaveHLayout->addWidget(octave);
    octaveHLayout->addWidget(octaveValue);

    octaveSlider = new QSlider(Qt::Orientation::Horizontal);
    octaveSlider->setMinimum(1);
    octaveSlider->setMaximum(8);

    frequencyHLayout = new QHBoxLayout;
    QLabel* frequency = new QLabel("Frequency : ");
    frequencyValue = new QLabel("0");
    frequencyHLayout->addWidget(frequency);
    frequencyHLayout->addWidget(frequencyValue);

    frequencySlider = new QSlider(Qt::Orientation::Horizontal);
    frequencySlider->setMinimum(1);
    frequencySlider->setMaximum(10);

    persistenceHLayout = new QHBoxLayout;
    QLabel* persistence = new QLabel("Persistence : ");
    persistenceValue = new QLabel("0");
    persistenceHLayout->addWidget(persistence);
    persistenceHLayout->addWidget(persistenceValue);

    persistenceSlider = new QSlider(Qt::Orientation::Horizontal);
    persistenceSlider->setMinimum(0);
    persistenceSlider->setMaximum(10);


    layout = new QVBoxLayout;
    layout->addWidget(imgLabel);
    layout->addLayout(octaveHLayout);
    layout->addWidget(octaveSlider);
    layout->addLayout(frequencyHLayout);
    layout->addWidget(frequencySlider);
    layout->addLayout(persistenceHLayout);
    layout->addWidget(persistenceSlider);

    layout->addWidget(addGradientButton);
    layout->addWidget(lightButton);


    buttonHLayout = new QHBoxLayout;
    buttonHLayout->addWidget(generateP);

    layout->addLayout(buttonHLayout);
    this->setLayout(layout);
    //this->setFixedSize(heightmap.width(), this->height());

    connect(lightButton, SIGNAL(clicked(bool)), this, SLOT(SwapLight()));
    connect(addGradientButton,SIGNAL(clicked(bool)),this,SLOT(AddGradiantPoint()));
    connect(generateP, SIGNAL(clicked(bool)), this, SLOT(GeneratePerlinNoise()));
    connect(octaveSlider, SIGNAL(valueChanged(int)), this, SLOT(OctaveChanged(int)));
    connect(frequencySlider, SIGNAL(valueChanged(int)), this, SLOT(FrequencyChanged(int)));
    connect(persistenceSlider, SIGNAL(valueChanged(int)), this, SLOT(PersistenceChanged(int)));
}

/**
 * @brief GUI::SwapLight Modification de la lumière
 */
void GUI::SwapLight()
{
    lightEnabled = !lightEnabled;

    if(lightEnabled){
        lightButton->setText("Disable light");
    }else{
        lightButton->setText("Enable light");
    }
}

/**
 * @brief GUI::GeneratePerlinNoise Action génération d'un nouveau terrain
 */
void GUI::GeneratePerlinNoise()
{

    //Transfert vers données du modèle de Terrain
    terrain->octave = octaveSlider->value();
    terrain->frequency = frequencySlider->value();
    terrain->persistence = persistenceSlider->value()/10.0f;
    terrain->lightEnabled = lightEnabled;

    if(list.size() > 1) {
    QMap<float,QVector3D> gradiants;
    foreach(GradientColorWidget* gcw, list) {
        gradiants[gcw->getGradiantPoint()] = gcw->getColor();
    }

    terrain->gradiants = gradiants;
    }
    else
        terrain->gradiants.clear();

    terrain->Rebuild();
    //Update Image
    QImage heightmap;
    heightmap.load(terrain->getHeightmapColorPath());
    imgLabel->setPixmap(QPixmap::fromImage(heightmap));
}

/**
 * @brief GUI::OctaveChanged Modification de l'octave
 * @param i
 */
void GUI::OctaveChanged(int i)
{
    octaveValue->setText(QString::number(i));
}

/**
 * @brief GUI::FrequencyChanged Modification de la fréquence
 * @param i
 */
void GUI::FrequencyChanged(int i)
{
    frequencyValue->setText(QString::number(i));
}

/**
 * @brief GUI::PersistenceChanged Modification de la persistence
 * @param i
 */
void GUI::PersistenceChanged(int i)
{
    persistenceValue->setText(QString::number(i/10.0f));
}

/**
 * @brief GUI::AddGradiantPoint Ajout d'un gradient pour le terrain
 */
void GUI::AddGradiantPoint()
{
    GradientColorWidget* gcw = new GradientColorWidget;
    connect(gcw,SIGNAL(RequestToDestroy(GradientColorWidget*)),this,SLOT(RemoveGradiantPoint(GradientColorWidget*)));
    list.append(gcw);
    emit UpdateLayout();
}

/**
 * @brief GUI::RemoveGradiantPoint Retrait d'un gradient pour le terrain
 * @param gcw
 */
void GUI::RemoveGradiantPoint(GradientColorWidget* gcw) {
    disconnect(gcw,SIGNAL(RequestToDestroy(GradientColorWidget*)),this,SLOT(RemoveGradiantPoint(GradientColorWidget*)));
    layout->removeWidget(gcw);
    gcw->hide();
    gcw->close();
    list.removeOne(gcw);
    emit UpdateLayout();
}

/**
 * @brief GUI::UpdateLayout Mise à jour du layout
 */
void GUI::UpdateLayout() {

    layout->removeWidget(imgLabel);
    layout->removeItem(octaveHLayout);
    layout->removeWidget(octaveSlider);
    layout->removeItem(frequencyHLayout);
    layout->removeWidget(frequencySlider);
    layout->removeItem(persistenceHLayout);
    layout->removeWidget(persistenceSlider);

    for(GradientColorWidget* gcw : list)
        layout->removeWidget(gcw);

    layout->removeWidget(addGradientButton);
    layout->removeWidget(lightButton);
    layout->removeItem(buttonHLayout);

    delete layout;

    layout = new QVBoxLayout;

    layout->addWidget(imgLabel);
    layout->addLayout(octaveHLayout);
    layout->addWidget(octaveSlider);
    layout->addLayout(frequencyHLayout);
    layout->addWidget(frequencySlider);
    layout->addLayout(persistenceHLayout);
    layout->addWidget(persistenceSlider);

    for(GradientColorWidget* gcw : list)
        layout->addWidget(gcw);

    layout->addWidget(addGradientButton);
    layout->addWidget(lightButton);
    layout->addLayout(buttonHLayout);

    setLayout(layout);

}

