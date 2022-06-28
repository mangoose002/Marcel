#ifndef _MATERIAL_HH_
#define _MATERIAL_HH_

#include <string>
#include "Core/Color.hh"
#include "Core/Texture.hh"

namespace Marcel {

    class Material {
    private:
    	string   Name;
    	int      MatId;

    	Color  Ambient;
    	Color  Diffuse;
    	Color  Specular;
    	double Shininess;
    	double Transparency;
    	double Reflection;

    	Texture *Map1;
    	Texture *Map2;
    	Texture *BumpMap;
    	Texture *SpecularMap;
    	Texture *OpacityMap;
    	Texture *ReflectionMap;

    public:
    	Material();
    	~Material();

    	void setId           (int);
    	void setName         (char *);
    	void setMap1         (Texture *);
    	void setMap2         (Texture *);
    	void setBumpMap      (Texture *);
    	void setSpecularMap  (Texture *);
    	void setOpacityMap   (Texture *);
    	void setReflectionMap(Texture *);
    	void setAmbientColor (Color);
    	void setDiffuseColor (Color);
    	void setSpecularColor(Color);
    	void setShininess    (double);
    	void setTransparency (double);
    	void setReflection   (double);

    	int    getId(void);
    	Color  getAmbientColor(void);
    	Color  getDiffuseColor(void);
    	double getTransparency(void);
    	double getReflection(void);

    	Texture *getMap1();
    	Texture *getBumpMap();
    };
}
#endif
