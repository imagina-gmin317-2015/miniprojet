/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#include "breakingnoise.h"


utils::NoiseMap PerlinNoise(int width, int height, float octave, float frequency, float persistence)
{
    //Bruit "Montagneux"
    module::RidgedMulti mountainTerrain;

    //Bruit "Flat"
    module::Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency (2.0);

    //On utilise cet autre bruit pour modifier le baseflat et créer une base de terrain smooth
    module::ScaleBias flatTerrain;
    flatTerrain.SetSourceModule(0,baseFlatTerrain);
    flatTerrain.SetScale(0.125);//On applique un facteur pour lisser les reliefs
    flatTerrain.SetBias(-0.75);//On applique un facteur pour diminuer les valeur de 0.75

    //On a crée 2 types de bruits différents (Montagnes/Flat) et
    //on veut les faire coexister sur un seul bruit

    module::Perlin terrainType;
    terrainType.SetPersistence(0.25);

    module::Select finalTerrain;
    finalTerrain.SetSourceModule (0, flatTerrain);
    finalTerrain.SetSourceModule (1, mountainTerrain);
    finalTerrain.SetControlModule (terrainType);
    finalTerrain.SetBounds (0.0, 1000.0);
    finalTerrain.SetEdgeFalloff (0.125);//Transition entre les deux types de bruits

    //Modifier le nombre d'octaves
    terrainType.SetOctaveCount(octave); //6
    //Modifier la fréquence
    terrainType.SetFrequency(frequency); //1.0
    //Modifier la persistence
    terrainType.SetPersistence(persistence); //0.5


    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;

    heightMapBuilder.SetSourceModule(finalTerrain);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    //Setter la taille de la heightmap et les valeurs limites
    heightMapBuilder.SetDestSize(width,height);
    heightMapBuilder.SetBounds(6.0, 10.0, 1.0, 5.0);

    //Build
    heightMapBuilder.Build();

    return heightMap;
}

utils::Image RenderImage(utils::NoiseMap heightMap, bool lightEnabled) {
    //Après le build, le rendu
    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);

    //Coloration
    renderer.ClearGradient ();
    renderer.AddGradientPoint (-1.00, utils::Color ( 32, 160,   0, 255)); // grass
    renderer.AddGradientPoint (-0.25, utils::Color (224, 224,   0, 255)); // dirt
    renderer.AddGradientPoint ( 0.25, utils::Color (128, 128, 128, 255)); // rock
    renderer.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255)); // snow

    //Light
    if(lightEnabled) {
    renderer.EnableLight ();
    renderer.SetLightContrast (3.0); // Triple le contraste
    renderer.SetLightBrightness (2.0); // Double la luminosité
    renderer.SetLightAzimuth(45); //Modifie l'inclinaison de la lumière (East:0 North:90 West:180 South:270 Default:45)
    renderer.SetLightElev(45);//Rotate la lumière, Default:45
    }
    //renderer.SetLightColor();

    renderer.Render();
    return image;
}

utils::Image RenderImage(utils::NoiseMap heightMap, bool lightEnabled, QMap<float,QVector3D> gradiants) {
    //Après le build, le rendu
    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);

    //Coloration
    renderer.ClearGradient ();
    for(float gr : gradiants.keys()) {
        QVector3D color = gradiants[gr];
        renderer.AddGradientPoint (gr, utils::Color ( color.x(), color.y(), color.z(), 255)); // grass
    }

    //Light
    if(lightEnabled) {
    renderer.EnableLight ();
    renderer.SetLightContrast (3.0); // Triple le contraste
    renderer.SetLightBrightness (2.0); // Double la luminosité
    renderer.SetLightAzimuth(45); //Modifie l'inclinaison de la lumière (East:0 North:90 West:180 South:270 Default:45)
    renderer.SetLightElev(45);//Rotate la lumière, Default:45
    }
    //renderer.SetLightColor();

    renderer.Render();
    return image;
}

utils::Image RenderGrayScaleImage(utils::NoiseMap heightMap) {
    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.BuildGrayscaleGradient();
    renderer.Render();
    return image;
}

utils::Image RenderNormalMapImage(utils::NoiseMap heightMap) {
    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.BuildTerrainGradient();
    return image;
}



void WriteImage(string title, utils::Image image) {
    //Après le rendu, l'écriture
    utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename(title);
    writer.WriteDestFile();
}

utils::NoiseMap PerlinNoise(float x, float y, float w, float h, int width, int height, float octave, float frequency, float persistence)
{
    //Bruit "Montagneux"
    module::RidgedMulti mountainTerrain;

    //Bruit "Flat"
    module::Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency (2.0);

    //On utilise cet autre bruit pour modifier le baseflat et créer une base de terrain smooth
    module::ScaleBias flatTerrain;
    flatTerrain.SetSourceModule(0,baseFlatTerrain);
    flatTerrain.SetScale(0.125);//On applique un facteur pour lisser les reliefs
    flatTerrain.SetBias(-0.75);//On applique un facteur pour diminuer les valeur de 0.75

    //On a crée 2 types de bruits différents (Montagnes/Flat) et
    //on veut les faire coexister sur un seul bruit

    module::Perlin terrainType;
    terrainType.SetPersistence(0.25);

    module::Select finalTerrain;
    finalTerrain.SetSourceModule (0, flatTerrain);
    finalTerrain.SetSourceModule (1, mountainTerrain);
    finalTerrain.SetControlModule (terrainType);
    finalTerrain.SetBounds (0.0, 1000.0);
    finalTerrain.SetEdgeFalloff (0.125);//Transition entre les deux types de bruits

    //Modifier le nombre d'octaves
    terrainType.SetOctaveCount(octave); //6
    //Modifier la fréquence
    terrainType.SetFrequency(frequency); //1.0
    //Modifier la persistence
    terrainType.SetPersistence(persistence); //0.5


    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;

    heightMapBuilder.SetSourceModule(finalTerrain);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    //Setter la taille de la heightmap et les valeurs limites
    heightMapBuilder.SetDestSize(width,height);
    heightMapBuilder.SetBounds(x, x+w, y,y+h);

    //Build
    heightMapBuilder.Build();

    return heightMap;
}
