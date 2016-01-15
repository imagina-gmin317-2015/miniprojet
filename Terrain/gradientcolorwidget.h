/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#ifndef GRADIENTCOLORWIDGET_H
#define GRADIENTCOLORWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QVector3D>
#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>

/**
 * @brief The GradiantColorWidget class est un widget permettant de saisir
 * une couleur en rgb et une valeur de gradient pour un bruit de Perlin
 */
class GradientColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GradientColorWidget(QWidget* parent=0);
    virtual ~GradientColorWidget();

    float getGradiantPoint() {
        return m_slider->value()/100.0f;
    }

    QVector3D getColor() const {
        return QVector3D(r->value(),g->value(),b->value());
    }
    QColor getCColor();

private:
    QSlider* m_slider;
    QLabel* gradiantValue;
    QSlider* r;
    QLabel* rLabel;
    QLabel* rValue;
    QSlider* g;
    QLabel* gLabel;
    QLabel* gValue;
    QSlider* b;
    QLabel* bLabel;
    QLabel* bValue;
    QVBoxLayout *subLayout;
    QPushButton* showButton;
    QPushButton* removeButton;
    bool show = true;
signals:
    void RequestToDestroy(GradientColorWidget*);

public slots:
    void modifyGP(int val);
    void modifyR(int val);
    void modifyG(int val);
    void modifyB(int val);
    void ShowOrHide();
    void Destroy();


};

#endif // GRADIANTCOLORWIDGET_H
