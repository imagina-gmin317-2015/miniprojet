/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#include "gradientcolorwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

GradientColorWidget::GradientColorWidget(QWidget* parent) :QWidget(parent)
{
    QVBoxLayout *masterLayout = new QVBoxLayout;

    //Gradiant line
    QHBoxLayout *line1 = new QHBoxLayout;
    QLabel* gradiantTitle = new QLabel("Gradient :");
    gradiantValue = new QLabel("-1.0");
    m_slider = new QSlider(Qt::Orientation::Horizontal);
    m_slider->setMinimum(-100.0);
    m_slider->setMaximum(100.0);
    m_slider->setValue(-100);
    m_slider->setSingleStep(1);
    line1->addWidget(gradiantTitle);


    line1->addWidget(gradiantValue);
    removeButton = new QPushButton("-");
    showButton = new QPushButton("Hide");
    line1->addWidget(removeButton);
    line1->addWidget(showButton);
    gradiantValue->setAutoFillBackground(true);

    //line1->addWidget(m_slider);

    //RGB Line
    QHBoxLayout *line2 = new QHBoxLayout;
    rLabel = new QLabel("R");
    rValue = new QLabel("0");
    r = new QSlider(Qt::Orientation::Horizontal);
    r->setMinimum(0);
    r->setMaximum(255);
    r->setSingleStep(1);
    r->setValue(0);
    line2->addWidget(rLabel);
    line2->addWidget(rValue);
    //line2->addWidget(r);

    QHBoxLayout *line3 = new QHBoxLayout;
    gLabel = new QLabel("G");
    gValue = new QLabel("0");
    g = new QSlider(Qt::Orientation::Horizontal);
    g->setMinimum(0);
    g->setMaximum(255);
    g->setSingleStep(1);
    g->setValue(0);
    line3->addWidget(gLabel);
    line3->addWidget(gValue);
    //line3->addWidget(g);

    QHBoxLayout *line4 = new QHBoxLayout;
    bLabel = new QLabel("B");
    bValue = new QLabel("0");
    b = new QSlider(Qt::Orientation::Horizontal);
    b->setMinimum(0);
    b->setMaximum(255);
    b->setSingleStep(1);
    b->setValue(0);
    line4->addWidget(bLabel);
    line4->addWidget(bValue);
    //line4->addWidget(b);

    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(modifyGP(int)));
    connect(r, SIGNAL(valueChanged(int)), this, SLOT(modifyR(int)));
    connect(g, SIGNAL(valueChanged(int)), this, SLOT(modifyG(int)));
    connect(b, SIGNAL(valueChanged(int)), this, SLOT(modifyB(int)));
    connect(showButton, SIGNAL(clicked(bool)), this, SLOT(ShowOrHide()));
    connect(removeButton, SIGNAL(clicked(bool)), this, SLOT(Destroy()));

    subLayout = new QVBoxLayout;

    subLayout->addLayout(line1);
    subLayout->addWidget(m_slider);

    subLayout->addLayout(line2);
    subLayout->addWidget(r);

    subLayout->addLayout(line3);
    subLayout->addWidget(g);

    subLayout->addLayout(line4);
    subLayout->addWidget(b);

    masterLayout->addLayout(subLayout);
    setLayout(masterLayout);
}

GradientColorWidget::~GradientColorWidget()
{
    delete rValue;
    delete m_slider;
    delete r;
    delete g;
    delete b;
    delete gValue;
    delete bValue;
}

void GradientColorWidget::modifyGP(int val)
{
    gradiantValue->setText(QString::number(val/100.0f));
}


QColor GradientColorWidget::getCColor() {
    return QColor(r->value(),g->value(),b->value());
}


void GradientColorWidget::Destroy() {
    emit RequestToDestroy(this);
}

void GradientColorWidget::modifyR(int val)
{
    rValue->setText(QString::number(val));
    QPalette qp = gradiantValue->palette();
    qp.setColor(gradiantValue->backgroundRole(),getCColor());
    gradiantValue->setPalette(qp);

}

void GradientColorWidget::modifyG(int val)
{
    gValue->setText(QString::number(val));
    QPalette qp = gradiantValue->palette();
    qp.setColor(gradiantValue->backgroundRole(),getCColor());
    gradiantValue->setPalette(qp);
}

void GradientColorWidget::modifyB(int val)
{
    bValue->setText(QString::number(val));
    QPalette qp = gradiantValue->palette();
    qp.setColor(gradiantValue->backgroundRole(),getCColor());
    gradiantValue->setPalette(qp);
}

void GradientColorWidget::ShowOrHide() {
    show = !show;
    if(!show) {
        showButton->setText("Show");
        m_slider->hide();

        r->hide();
        rLabel->hide();
        rValue->hide();

        g->hide();
        gLabel->hide();
        gValue->hide();

        b->hide();
        bLabel->hide();
        bValue->hide();


    }
    else {
        showButton->setText("Hide");
        m_slider->show();

        r->show();
        rLabel->show();
        rValue->show();

        g->show();
        gLabel->show();
        gValue->show();

        b->show();
        bLabel->show();
        bValue->show();
    }
}
