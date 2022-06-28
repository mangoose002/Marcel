#ifndef _UTILS_HH_
#define _UTILS_HH_

#include <string>
#include "Core/Scene.hh"

#define _LOCAL_MODE_    1
#define _SERVER_MODE_   2
#define _CLIENT_MODE_   3

namespace Marcel{

class Utils
{
public:
  Utils();
  ~Utils();
  static int CompressFile(string,string);
  static int UncompressFile(string,string);
  static SceneContext* ParseCommandLine(int argc, char* argv[],int mode=_LOCAL_MODE_);
  static int filesize(string);
  static string filepath(string);
};

}
#endif
