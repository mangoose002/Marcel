using namespace std;
#include "Core/Material.hh"
#include "Core/Color.hh"
#include "Core/Texture.hh"

namespace Marcel{
  Material::Material()
  {
    Ambient  = Color(0,0,0);
    Diffuse  = Color(0,0,0);
    Specular = Color(0,0,0);

    Name  = "unknown";
    MatId = -1;

    Shininess = Transparency = Reflection = 0;
    Map1 =  Map2 = BumpMap = SpecularMap = OpacityMap = ReflectionMap = NULL;
  }

  Material:: ~Material()
  {
    if(Map1!=NULL)          { delete Map1;          }
    if(Map2!=NULL)          { delete Map2;          }
    if(BumpMap!=NULL)       { delete BumpMap;       }
    if(SpecularMap!=NULL)   { delete SpecularMap;   }
    if(OpacityMap!=NULL)    { delete OpacityMap;    }
    if(ReflectionMap!=NULL) { delete ReflectionMap; }
  }

  void Material::setId           (int id)     { MatId = id;        }
  void Material::setName         (char *name) { Name = name;       }
  void Material::setMap1         (Texture *m) { Map1 = m;          }
  void Material::setMap2         (Texture *m) { Map2 = m;          }
  void Material::setBumpMap      (Texture *m) { BumpMap = m;       }
  void Material::setSpecularMap  (Texture *m) { SpecularMap = m;   }
  void Material::setOpacityMap   (Texture *m) { OpacityMap = m;    }
  void Material::setReflectionMap(Texture *m) { ReflectionMap = m; }
  void Material::setAmbientColor (Color c)    { Ambient  = c;      }
  void Material::setDiffuseColor (Color c)    { Diffuse  = c;      }
  void Material::setSpecularColor(Color c)    { Specular = c;      }
  void Material::setShininess    (double s)   { Shininess = s;     }
  void Material::setTransparency (double s)   { Transparency = s;  }
  void Material::setReflection   (double s)   { Reflection = s;    }

  int    Material::getId(void)                 { return MatId;        }
  Color  Material::getAmbientColor(void)       { return Ambient;      }
  Color  Material::getDiffuseColor(void)       { return Diffuse;      }
  double Material::getTransparency(void)       { return Transparency; }
  double Material::getReflection(void)         { return Reflection;   }

  Texture *Material::getMap1()                 { return Map1;         }
  Texture *Material::getBumpMap()              { return BumpMap;      }
}