#include "terrain2.h"

#include <QImage>
#include <QDebug>

namespace BreakingEngine {

Terrain2::Terrain2(QImage heightMap) : Mesh()
{
    this->width = width;
    this->height = height;
    init(heightMap.width(),heightMap.height(),heightMap);
    float gap = 0.5f;
    float posX = -(gap * width/2.f);
    float posY = -10.f;
    float posZ = -(gap * height/2.f);

    int index = 0;
    float x = 1.f / width;
    float y = 1.f / height;
    for(int i = 0 ; i < height; i++){
        for(int j = 0 ; j < width; j++){
            QRgb h = heightMap.pixel(j,i);
            float hr = qGray(h);
            vertices[index].position = QVector3D(posX + gap * j, posY + hr / 20.f, posZ + gap * i);
            vertices[index].color = QVector3D(1.0f,1.0f,1.0f);
            vertices[index].texCoord = QVector2D(x * j, y * i);
            index++;
        }
    }

    index = 0;

    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            if(j == width - 1){
                vertices[index].normal = vertices[index-1].normal;
            }
            else if(i == height - 1){
                vertices[index].normal = vertices[index - width].normal;
            }else{
                vertices[index].normal = QVector3D::normal
                        (vertices[index].position, vertices[index+1].position, vertices[index + width].position);
            }
            index++;
        }
    }
    //INDICES /////////////////////////////////////////////////////////////////
    if(indices!=nullptr)
        delete indices;
    indexSize = (width-1)*(height-1)*6;
    indices = new GLushort[indexSize];

    index = 0;

    for(int i = 0 ; i < height-1 ; i++){
        for(int j = 0 ; j < width-1 ; j++){

            indices[index++] = GLushort(i * width + j);
            indices[index++] = GLushort((i+1) * width + j);
            indices[index++] = GLushort((i+1) * width + j + 1);

            indices[index++] = GLushort(i * width + j);
            indices[index++] = GLushort((i+1) * width + j + 1);
            indices[index++] = GLushort(i * width + j + 1);
        }
    }
}

Terrain2::Terrain2(QImage heightMap, QImage colorMap) : Mesh()
{
    this->width = width;
    this->height = height;
    init(heightMap.width(),heightMap.height(),heightMap,colorMap);
}

Terrain2::Terrain2(QString filename): Mesh()
{
    QImage image;
    if(!image.load(filename))
        exit(102);


    init(image.width(),image.height(),image);
}

void Terrain2::init(int width, int height, QImage heightMap)
{
        this->width = width;
        this->height = height;
        Mesh::init(width*height);
        float gap = 0.5f;
        float posX = -(gap * width/2.f);
        float posY = -10.f;
        float posZ = -(gap * height/2.f);

        int index = 0;
        float x = 1.f / width;
        float y = 1.f / height;
        for(int i = 0 ; i < height; i++){
            for(int j = 0 ; j < width; j++){
                QRgb h = heightMap.pixel(j,i);
                float hr = qGray(h);
                vertices[index].position = QVector3D(posX + gap * j, posY + hr / 100.f, posZ + gap * i);
                vertices[index].texCoord = QVector2D(x * j, y * i);
                index++;
            }
        }

        index = 0;

        for(int i = 0 ; i < height ; i++){
            for(int j = 0 ; j < width ; j++){
                if(j == width - 1){
                    vertices[index].normal = vertices[index-1].normal;
                }
                else if(i == height - 1){
                    vertices[index].normal = vertices[index - width].normal;
                }else{
                    vertices[index].normal = QVector3D::normal
                            (vertices[index].position, vertices[index+1].position, vertices[index + width].position);
                }
                index++;
            }
        }
        //INDICES /////////////////////////////////////////////////////////////////
        if(indices!=nullptr)
            delete indices;
        indexSize = (width-1)*(height-1)*6;
        indices = new GLushort[indexSize];

        index = 0;

        for(int i = 0 ; i < height-1 ; i++){
            for(int j = 0 ; j < width-1 ; j++){

                indices[index++] = GLushort(i * width + j);
                indices[index++] = GLushort((i+1) * width + j);
                indices[index++] = GLushort((i+1) * width + j + 1);

                indices[index++] = GLushort(i * width + j);
                indices[index++] = GLushort((i+1) * width + j + 1);
                indices[index++] = GLushort(i * width + j + 1);
            }
        }

}
void Terrain2::init(int width, int height, QImage heightMap, QImage colorMap)
{
        this->width = width;
        this->height = height;


        Mesh::init(width*height);
        float gap = 0.5f;
        float posX = -(gap * width/2.f);
        float posY = -10.f;
        float posZ = -(gap * height/2.f);

        int index = 0;
        float x = 1.f / width;
        float y = 1.f / height;
        for(int i = 0 ; i < height; i++){
            for(int j = 0 ; j < width; j++){
                QRgb h = heightMap.pixel(j,i);
                QRgb p = colorMap.pixel(j,i);
                vertices[index].position = QVector3D(posX + gap * j, posY + qGray(h) / 20.f, posZ + gap * i);
                vertices[index].color = QVector3D(qRed(p)/255.f,qGreen(p)/255.f,qBlue(p)/255.f);
                vertices[index].texCoord = QVector2D(x * j, y * i);
                index++;
            }
        }

        index = 0;

        for(int i = 0 ; i < height ; i++){
            for(int j = 0 ; j < width ; j++){
                if(j == width - 1){
                    vertices[index].normal = vertices[index-1].normal;
                }
                else if(i == height - 1){
                    vertices[index].normal = vertices[index - width].normal;
                }else{
                    vertices[index].normal = QVector3D::normal
                            (vertices[index].position, vertices[index+1].position, vertices[index + width].position);
                }
                index++;
            }
        }
        //INDICES /////////////////////////////////////////////////////////////////
        if(indices!=nullptr)
            delete indices;
        indexSize = (width-1)*(height-1)*6;
        indices = new GLushort[indexSize];

        index = 0;

        for(int i = 0 ; i < height-1 ; i++){
            for(int j = 0 ; j < width-1 ; j++){

                indices[index++] = GLushort(i * width + j);
                indices[index++] = GLushort((i+1) * width + j);
                indices[index++] = GLushort((i+1) * width + j + 1);

                indices[index++] = GLushort(i * width + j);
                indices[index++] = GLushort((i+1) * width + j + 1);
                indices[index++] = GLushort(i * width + j + 1);
            }
        }
    }


}

