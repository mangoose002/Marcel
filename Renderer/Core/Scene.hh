
#ifndef _SCENE_HH_
#define _SCENE_HH_

#include  "Core/Octree.hh"
#include  "Core/Point.hh"
#include  "Core/QuadTree.hh"

#include  "Core/Light.hh"
#include  "Core/Material.hh"
#include  "Core/Texture.hh"
#include  "Core/Object.hh"
#include  "Core/Color.hh"

#include  "Core/Camera.hh"
#include  "Core/Image.hh"
#include  "Core/RowDispatcher.hh"

#include <string>
#include <vector>
#include <algorithm>

namespace Marcel{

struct SceneContext{
    int             XResolution;
    int             YResolution;
    Image*          File;
    RowDispatcher*  Dispatcher;
    string          InputFileName;
    string          OutputFileName;
    int             MAX_OCTREE_LEVEL;
    int             MAX_QUADTREE_LEVEL;
    int             MAX_LEVEL;
    int             isAliased;
    int             isProgressive;
    int             isSoftshadowed;
    int             Threshold;
    int             Blocksize;
    int             ThreadNumber;
    int             isRaycasting;

    string          hostname;
    int             port;
};

class Scene
{
protected:
    Octree    *OctreeScene;
    QuadTree  *QuadTreeScene;

    int        ResX;
    int        ResY;
    int        FrameNumber;
    Point      Cam;
    Point      LookAt;
    Plan       *ViewPlane;
    Camera     *camera;

    string     OutputFileName;
    string     InputFileName;
    string     FilePath;
    string     CompressedInputFileName;

    int        numThreads;

    int        MAX_LEVEL;
    int        MAX_OCTREE_LEVEL;
    int        MAX_QUADTREE_LEVEL;
    bool       Progressive;
    int        BlockSize;
    int        Seuil;
    bool       antialias;
    bool       softshadow;
    bool       isRaycasting;

    vector<Light *>    *LightList;
    vector<Objet *>    *ObjectList;
    vector<Texture *>  *MapList;
    vector<Material *> *MaterialList;
    Color               BackGroundColor;
    Color               AmbientColor;
    RowDispatcher      *Dispatcher;
    Image              *File;

protected:
    void ParseFile(string);
    void CreateSmallestOctree();
    void CreateSmallestQuadTree();

    bool ParseScnFile(const char *);
    bool Parse3dsFile(string);
    bool ParseObjFile(string);

    void StandardCalculation(int,RowDispatcher*);
    void ProgressiveComputation(int,RowDispatcher*);
public:
    /// Constructs the scene from the input file and the command line
    Scene();
    Scene(SceneContext *context);
    virtual ~Scene();

    /// Will be called from the parser;
    void addObject(Objet *);
    void addLight(Light *);
    void addTexture(Texture *);
    void addMaterial(Material *);

    //Setters
    void setCamera(Point, Point);
    void setCamera(Camera *);
    void setAmbientColor(Color);
    void setBackGroundColor(Color);
    void setFrameNumber(int);
    void setMaterial(Objet *, int);

    //Getters
    inline Camera*   getCamera()           { return camera;           }
    inline Octree*   getOctree()           { return OctreeScene;      }
    inline QuadTree* getQuadTree()         { return QuadTreeScene;    }
    inline Color     getAmbientColor()     { return AmbientColor;     }
    inline Color     getBackGroundColor()  { return BackGroundColor;  }
    inline vector<Light *>* getLightList() { return LightList;        }
    inline Image*    getImage()            { return File;             }
    inline Point     getLookAt()           { return LookAt;           }
    inline Point     getCam()              { return Cam;              }
    inline bool      is_SoftShadow()       { return softshadow;       }
    inline bool      is_Antialias()        { return antialias;        }
    inline bool      is_Progressive()      { return Progressive;      }
    inline bool      is_Raycasting()       { return isRaycasting;     }
    inline int       getBlockSize()        { return BlockSize;        }
    inline int       getThreshold()        { return Seuil;            }

    void CreateCullingBox(Objet*);

    vector<Texture*> *getMappingList(void); // Used to link mapping coordinates to loaded textures.
    int               getFrameNumber();


    /// Will be called from the main
    void Start();
    static void Standard_helper(void *);
    static void Alias_helper(void *);
    static void Progressive_helper(void *);
};

}
#endif
