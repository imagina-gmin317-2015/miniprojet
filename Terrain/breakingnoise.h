/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#ifndef BREAKINGNOISE_H
#define BREAKINGNOISE_H

#include <QCoreApplication>
#include<iostream>
#include <libnoise/noise.h>
#include "noiseutils.h"
#include <QDebug>
#include <QMap>
#include <QVector3D>

using namespace std;
using namespace noise;

/**
 * Fonctions adaptant le code de la librairie libnoise
 * au besoin du projet
 *
 */


///Produit un bruit de perlin
utils::NoiseMap PerlinNoise(int width, int height, float octave, float frequency, float persistence);
///Produit un bruit de perlin en fonction d'un bound particulier
utils::NoiseMap PerlinNoise(float x, float y, float w, float h,
                            int width, int height, float octave, float frequency, float persistence);

///Produit une image à partie d'un bruit de perlin
utils::Image RenderImage(utils::NoiseMap heightMap,bool lightEnabled);
///Produit une image à partir d'un bruit de perlin avec des gradients particuliers
utils::Image RenderImage(utils::NoiseMap heightMap, bool lightEnabled, QMap<float,QVector3D> gradiants);
///Produit une image de gradient de terrain
utils::Image RenderNormalMapImage(utils::NoiseMap heightMap);
///Produit une image en niveau de gris d'un bruit de perlin
utils::Image RenderGrayScaleImage(utils::NoiseMap heightMap);
///Ecrit dans un fichier une image de libnoise::utils
void WriteImage(string title, utils::Image image);
#endif // BREAKINGNOISE_H
