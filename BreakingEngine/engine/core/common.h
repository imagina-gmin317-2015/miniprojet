#ifndef COMMON_H
#define COMMON_H
#include <QList>

class GameObject;

//Components
class Component;
class Transform;
class Camera;
class FreeFlyCamera;
class EulerCamera;
class Light;

class Updatable;
class Renderer;

class Scene;

class ColliderTriggerable;
class Behaviour;//Component, Updatable, ColliderTriggerable

class MeshRenderer;
class SpriteRenderer;

typedef QList<Component*> ComponentList;
typedef QList<GameObject*> GameObjectList;


//Collision Components
class Collider;
class SphereCollider;
class AABBCollider;
class OBBCollider;
class BoxCollider;
class CapsuleCollider;
class ConvexCollider;

class Collider2D;
class Box2DCollider;
class CircleCollider;

class RigidBody;
class RigiBody2D;


class Cube;//Mesh
class Plane;//Mesh
class Sphere;//Mesh

class Quad;//Mesh

class Tree;
class Terrain;

//IO
class Input;
class InputManager;
class AxisInput;

class FileManager;
class BigFile;
class BigFileHeader;
class BigFileReader;
class BigFileWriter;


class MeshReader;
class MeshReaderFactory;
class PLYMeshReaderFactory;
class STLMeshReaderFactory;
class OBJMeshReaderFactory;

//Meshing
class Mesh;
class NormalMesh;
class ColorMesh;
class CompleteMesh;
class ConvexCreator;

class Shader;
class Texture;


//System
class ShaderManager;
class PrefabManager;
class TimeManager;
class PhysicsManager;
class Physics2DManager;
class AudioManager;
class TagAndLayerManager;
class PlayerSettings;

#endif // COMMON
