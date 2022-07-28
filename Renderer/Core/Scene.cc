using namespace std;
#include "Core/Scene.hh"
#include "Core/Point.hh"
#include "Core/Polygon.hh"
#include "Core/Triangle.hh"
#include "Core/Raytrace.hh"
#include "Core/ColorTexture.hh"

#include "Core/ImageClient.hh"
#include "Core/RowDispatcher.hh"
#include "Core/RowDispatcherClient.hh"
#include "Utils/Network/SocketException.hh"

#include "Core/Spot.hh"
#include "Core/Light.hh"
#include "Core/Omni.hh"

#include "l3ds.h"
#include "tiny_obj_loader.h"

#include <getopt.h>
#include <zlib.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <list>
#include <thread>

#include "Core/Camera.hh"
#include "Utils/Utils.hh"

#define MIN(a,b)  a<b?a:b
#define MAX(a,b)  a>b?a:b

using namespace l3ds;

namespace Marcel{

	struct ThreadContext {
		Raytracer     *raytracer;
		char**         table = NULL; //For progressive computation
		int            passe;        //For progressive computation
	};

	Scene::Scene(SceneContext *context) {
		ResX               = context->XResolution;
		ResY               = context->YResolution;

		Progressive        = context->isProgressive;
		Seuil              = context->Threshold;
		BlockSize          = context->Blocksize;

		antialias          = context->isAliased;
		softshadow         = context->isSoftshadowed;

		InputFileName      = context->InputFileName;
		numThreads         = context->ThreadNumber;

		FrameNumber        = 1;                          // Initialize the Frame number.
		BackGroundColor    = Color(0, 0, 0);             // Setting the background color.
		AmbientColor       = Color(0.2, 0.2, 0.2);       // Setting the ambient color.

		ObjectList         = new vector<Objet *>;             // Creating the object list.
		LightList          = new vector<Light *>;             // Creating the light list.
		MapList            = new vector<Texture *>;           // Creating the map list.
		MaterialList       = new vector<Material *>;          // Creating the material list.

		camera             = NULL;

		Dispatcher         = context->Dispatcher;
		File               = context->File;

		isRaycasting       = context->isRaycasting;
		listing			   = context->listing;
		cameraidx		   = context->cameraidx;

		MAX_LEVEL          = context->MAX_LEVEL;
		MAX_OCTREE_LEVEL   = context->MAX_OCTREE_LEVEL;
		MAX_QUADTREE_LEVEL = context->MAX_QUADTREE_LEVEL;

		File       = new Image(context->OutputFileName.c_str(), context->XResolution, context->YResolution);
		Dispatcher = new RowDispatcher(File);

		FilePath = Utils::filepath(InputFileName);
	}

	Scene::Scene(){
		File = NULL;
		FilePath = ".";

		isRaycasting = false;

		Progressive = 0;
		Seuil       = 4;
		BlockSize   = 8;

		FrameNumber     = 1;                          // Initialize the Frame number.
		BackGroundColor = Color(0, 0, 0);             // Setting the background color.
		AmbientColor    = Color(0.2, 0.2, 0.2);       // Setting the ambient color.

		ObjectList   = new vector<Objet *>;             // Creating the object list.
		LightList    = new vector<Light *>;             // Creating the light list.
		MapList      = new vector<Texture *>;           // Creating the map list.
		MaterialList = new vector<Material *>;          // Creating the material list.

		camera       = NULL;

		// Determine the number of calculation threads to use
		numThreads = thread::hardware_concurrency();
		if (numThreads < 1)
			numThreads = 1;
	}

	bool Scene::Parse3dsFile(string filename)
	{
		// This function parses 3ds files
		// This needs to be ran first to ensure that the format
		// is correctly checked.
		L3DS *loader = new L3DS();
		if (!(loader->LoadFile(filename.c_str()))){
			delete loader;
			return false;
		}

		int	     TriangleNumber=0;

		LCamera    lCamera;
		LLight     lLight;
		LMaterial  lMat;
		LMap       lMap;
		LTriangle2 t;
		LMatrix4   lMatrix;

		Light     *CurrentLight;
		Material  *CurrentMaterial;
		Polygon   *CurrentObj;
		Triangle  *triangle;

		bool cameracreated = false;
		if (loader->GetCameraCount() > 0) {
			for (int i = 0; i < loader->GetCameraCount() ; i++){
				if(i==cameraidx){
					lCamera  = loader->GetCamera(i);
					camera = new Camera(Point((lCamera.GetOrigin()).x, (lCamera.GetOrigin()).y, (lCamera.GetOrigin()).z),
				                    Point((lCamera.GetTarget()).x, (lCamera.GetTarget()).y, (lCamera.GetTarget()).z),
				                    lCamera.GetFocus(),
				                    ResX,
				                    ResY);
					cameracreated=true;
				}
			}
		}

		if (loader->GetCameraCount() == 0 || cameracreated==false){
			cout << "No camera found, creating one" << endl;
			camera = new Camera(Point(10, -5, 10), Point(0, 0, 0), 45, ResX, ResY);
			camera->isGenerated(true);
		}


		for (int i = 0; i < loader->GetLightCount(); i++)
		{
			lLight = loader->GetLight(i);
			if (lLight.GetSpotlight())
			{
				CurrentLight = new Spot();
				((Spot *)CurrentLight)->setOrigin(Point(lLight.GetPosition().x, lLight.GetPosition().y, lLight.GetPosition().z));
				((Spot *)CurrentLight)->setTarget(Point(lLight.GetTarget().x, lLight.GetTarget().y, lLight.GetTarget().z));
				((Spot *)CurrentLight)->setFallOff(lLight.GetFalloff());
				((Spot *)CurrentLight)->setHotSpot(lLight.GetHotspot());
				((Spot *)CurrentLight)->setColor(Color(lLight.GetColor().r, lLight.GetColor().g, lLight.GetColor().b));
				LightList->push_back(CurrentLight);
				//cout << "Spot" << endl;
			}
			else
			{
				CurrentLight = new Omni();
				((Omni *)CurrentLight)->setOrigin(Point(lLight.GetPosition().x, lLight.GetPosition().y, lLight.GetPosition().z));
				((Omni *)CurrentLight)->setColor(Color(lLight.GetColor().r*0.5, lLight.GetColor().g*0.5, lLight.GetColor().b*0.5));
				LightList->push_back(CurrentLight);
				//cout << "Omni" << endl;
			}
		}
		if (loader->GetLightCount() == 0) {
			CurrentLight = new Omni();
			((Omni *)CurrentLight)->setOrigin(Point(50, 50, 50));
			((Omni *)CurrentLight)->setColor(Color(.75, .75, .75));
			LightList->push_back(CurrentLight);
		}

		for (int i = 0; i < loader->GetMaterialCount(); i++)
		{
			lMat = loader->GetMaterial(i);
			CurrentMaterial = new Material();
			CurrentMaterial->setId(lMat.GetID());
			CurrentMaterial->setName((char *)(lMat.GetName()).c_str());
			CurrentMaterial->setAmbientColor(Color(lMat.GetAmbientColor().r * .8, lMat.GetAmbientColor().g * .8, lMat.GetAmbientColor().b * .8));
			CurrentMaterial->setDiffuseColor(Color(lMat.GetDiffuseColor().r * .8, lMat.GetDiffuseColor().g * .8, lMat.GetDiffuseColor().b * .8));

			CurrentMaterial->setShininess(lMat.GetShininess());
			CurrentMaterial->setTransparency(lMat.GetTransparency());
			
			lMap = lMat.GetTextureMap1();
			if(strcmp(lMap.mapName,"")){
				CurrentMaterial->setMap1(new ColorTexture(lMap.mapName,FilePath + "/"  + lMap.mapName));
			}
			lMap = lMat.GetTextureMap2();
			if(strcmp(lMap.mapName,"")){
				CurrentMaterial->setMap2(new ColorTexture(lMap.mapName,FilePath + "/"  + lMap.mapName));
			}
			lMap = lMat.GetOpacityMap();
			if(strcmp(lMap.mapName,"")){
				cout << "Opacity" << endl;
				CurrentMaterial->setOpacityMap(new ColorTexture(lMap.mapName,FilePath + "/"  + lMap.mapName));
			}
			lMap = lMat.GetBumpMap();
			if(strcmp(lMap.mapName,"")){
				CurrentMaterial->setBumpMap(new ColorTexture(lMap.mapName,FilePath + "/"  + lMap.mapName));
			}
			lMap = lMat.GetReflectionMap();
			if (!strcmp(lMap.mapName, "auto")) {
				CurrentMaterial->setReflection(lMap.strength);
			}

			MaterialList->push_back(CurrentMaterial);
		}
		if(loader->GetMaterialCount() == 0){
			cout << "No material found, creating one" << endl;

			CurrentMaterial = new Material();
			CurrentMaterial->setId(0);
			CurrentMaterial->setName("Default");
			CurrentMaterial->setAmbientColor(Color(.1,.1,.1));
			CurrentMaterial->setDiffuseColor(Color(.8,.8,.8));

			CurrentMaterial->setShininess(.5);
			CurrentMaterial->setTransparency(0);

			MaterialList->push_back(CurrentMaterial);
		}

		for (int i = 0; i < loader->GetMeshCount(); i++)
		{
			CurrentObj = new Polygon();
			CurrentObj->setSurB(64);

			// Setting visual parameters
			CurrentObj->setColor(Color(1, 1, 1));
			CurrentObj->setDiffuse(1);

			ObjectList->push_back(CurrentObj);
			LMesh &lMesh = loader->GetMesh(i);
			CurrentObj->setName(lMesh.GetName());
			TriangleNumber += lMesh.GetTriangleCount();

			lMatrix = lMesh.GetMatrix();
			Matrix *mMatrix = new Matrix(4, 4);
			mMatrix->Identity();

			//if(fabs(lMatrix._11) > 1e-5)	mMatrix->setValue(0, 0, lMatrix._11);
			//if(fabs(lMatrix._21) > 1e-5)	mMatrix->setValue(0, 1, lMatrix._21);
			//if(fabs(lMatrix._31) > 1e-5)	mMatrix->setValue(0, 2, lMatrix._31);
			if(fabs(lMatrix._41) > 1e-5)	mMatrix->setValue(0, 3, lMatrix._41);

			//if(fabs(lMatrix._12) > 1e-5)	mMatrix->setValue(1, 0, lMatrix._12);
			//if(fabs(lMatrix._22) > 1e-5)	mMatrix->setValue(1, 1, lMatrix._22);
			//if(fabs(lMatrix._32) > 1e-5)	mMatrix->setValue(1, 2, lMatrix._32);
			if(fabs(lMatrix._42) > 1e-5)	mMatrix->setValue(1, 3, lMatrix._42);

			//if(fabs(lMatrix._13) > 1e-5)	mMatrix->setValue(2, 0, lMatrix._13);
			//if(fabs(lMatrix._23) > 1e-5)	mMatrix->setValue(2, 1, lMatrix._23);
			//if(fabs(lMatrix._33) > 1e-5)	mMatrix->setValue(2, 2, lMatrix._33);
			if(fabs(lMatrix._43) > 1e-5)	mMatrix->setValue(2, 3, lMatrix._43);

			//if(fabs(lMatrix._14) > 1e-5)	mMatrix->setValue(3, 0, lMatrix._14);
			//if(fabs(lMatrix._24) > 1e-5)	mMatrix->setValue(3, 1, lMatrix._24);
			//if(fabs(lMatrix._34) > 1e-5)	mMatrix->setValue(3, 2, lMatrix._34);
			if(fabs(lMatrix._44) > 1e-5)	mMatrix->setValue(3, 3, lMatrix._44);

			for (int j = 0; j < lMesh.GetTriangleCount(); j++)
			{
				t = lMesh.GetTriangle2(j);
				try	{
					triangle = new Triangle(Point(t.vertices[0].x, t.vertices[0].y, t.vertices[0].z),
					                        Point(t.vertices[1].x, t.vertices[1].y, t.vertices[1].z),
					                        Point(t.vertices[2].x, t.vertices[2].y, t.vertices[2].z));

					triangle->setShadingNormal(1, Vector(t.vertexNormals[0].x, t.vertexNormals[0].y, t.vertexNormals[0].z));
					triangle->setShadingNormal(2, Vector(t.vertexNormals[1].x, t.vertexNormals[1].y, t.vertexNormals[1].z));
					triangle->setShadingNormal(3, Vector(t.vertexNormals[2].x, t.vertexNormals[2].y, t.vertexNormals[2].z));

					triangle->setUVCoordinates(1,t.textureCoords[0].x,t.textureCoords[0].y);
					triangle->setUVCoordinates(2,t.textureCoords[1].x,t.textureCoords[1].y);
					triangle->setUVCoordinates(3,t.textureCoords[2].x,t.textureCoords[2].y);

					// Setting visual parameters
					triangle->setSurB(64);
					triangle->setColor(Color(1, 1, 1));
					triangle->setDiffuse(1);

					setMaterial(triangle, t.materialId);

					((Polygon *)CurrentObj)->AddTriangle(triangle);
				}
				catch (...)	{
					cout << "An error occured" << endl;
				}
			}
		}

		return true;
	}

	bool Scene::ParseObjFile(string filename) {

		Light     *CurrentLight;
		Material  *CurrentMaterial;
		Polygon   *CurrentObj;
		Triangle  *triangle;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), ".");

		cout << shapes.size() << endl;

		for (size_t i = 0; i < materials.size(); i++) {
			CurrentMaterial = new Material();

			cout << materials[i].name.c_str() << endl;
			CurrentMaterial->setId(i);
			CurrentMaterial->setName((char *)(materials[i].name.c_str()));
			CurrentMaterial->setAmbientColor(Color(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]));
			CurrentMaterial->setDiffuseColor(Color(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]));

			MaterialList->push_back(CurrentMaterial);
		}

		for (size_t i = 0; i < shapes.size(); i++) {
			CurrentObj = new Polygon();
			CurrentObj->setSurB(64);

			// Setting visual parameters
			CurrentObj->setColor(Color(1, 1, 1));
			CurrentObj->setDiffuse(1);

			ObjectList->push_back(CurrentObj);

			for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
	//      		printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);

				Point A = Point(
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 0] + 0]),
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 0] + 1]),
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 0] + 2])
				          );
				Point B = Point(
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 1] + 0]),
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 1] + 1]),
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 1] + 2])
				          );
				Point C = Point(
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 2] + 0]),
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 2] + 1]),
				              (double)(shapes[i].mesh.positions[3 * shapes[i].mesh.indices[3 * f + 2] + 2])
				          );


				triangle = new Triangle(A, B, C);

				triangle->setShadingNormal(1, Vector(
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 0] + 0]),
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 0] + 1]),
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 0] + 2])
				                           ));
				triangle->setShadingNormal(2, Vector(
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 1] + 0]),
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 1] + 1]),
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 1] + 2])
				                           ));
				triangle->setShadingNormal(3, Vector(
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 2] + 0]),
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 2] + 1]),
				                               (double)(shapes[i].mesh.normals[3 * shapes[i].mesh.indices[3 * f + 2] + 2])
				                           ));


				setMaterial(triangle, shapes[i].mesh.material_ids[f]);
				((Polygon *)CurrentObj)->AddTriangle(triangle);

			}
		}

		camera = new Camera(Point(10, 10, 10), Point(0, 0, 0), 45, ResX, ResY);

		CurrentLight = new Omni();
		((Omni *)CurrentLight)->setOrigin(Point(100, 100, 200));
		((Omni *)CurrentLight)->setColor(Color(1, 1, 1));
		LightList->push_back(CurrentLight);

		return true;
	}

	void Scene::listCameras(void){
		// This function parses 3ds files
		// This needs to be ran first to ensure that the format
		// is correctly checked.
		L3DS *loader = new L3DS();
		if (!(loader->LoadFile(InputFileName.c_str()))){
			cerr << "Listing only available on 3DS files" << endl;
			delete loader;
			exit(-1);
		}

		LCamera    lCamera;
		if(loader->GetCameraCount() == 0){
			cout << "No camera defined in this scene" << endl;
			return;
		}
		if (loader->GetCameraCount() > 0) {
			for (int i = 0; i < loader->GetCameraCount() ; i++){
				lCamera  = loader->GetCamera(i);
				cout << i << ": " << lCamera.GetName() << endl;
			}
		}
	}

	void Scene::ParseFile(string filename)
	{
		auto start = chrono::steady_clock::now();
		if (!Parse3dsFile(filename)) {
			if (!ParseObjFile(filename)) {
				cout << "Unable to read the input file" << endl;
				exit(-1);
			}
		}
		auto end = chrono::steady_clock::now();
		chrono::duration<double> elapsed_seconds = end-start;
    	cout << endl << "Parse Time: " << elapsed_seconds.count() << "s\n";

		int nbObject = ObjectList->size();
		int nbLight  = LightList->size();
		int nbMap    = MapList->size();
		int nbMat    = MaterialList->size();

		Cam = camera->getViewPoint();
		LookAt = camera->getTarget();

		camera->setResolution(ResX, ResY);

		cout << "-----------------------------------------------------------" << endl;
		cout << "The scene contains:" << endl;
		cout << nbObject << " objects" << endl;
		cout << nbLight  << " lights" << endl;
		cout << nbMap    << " maps" << endl;
		cout << nbMat    << " materials" << endl;
		cout << "-----------------------------------------------------------" << endl;
		cout << FrameNumber << " frames will be computed" << endl;
		cout << "-----------------------------------------------------------" << endl;
	}

	Scene::~Scene()
	{
		if(!listing){
			cout << endl;
			cout << "Deleting Objects." ;
		}

		Objet *mo;
		int i = 0;
		for (int yy = 0; yy < ObjectList->size(); yy++){
			mo = ObjectList->at(yy);
			if(!listing){
				i = i % 4;
				switch (i)
				{
				case 0:
					cout << "/"; cout.flush(); break;
				case 1:
					cout << "|"; cout.flush(); break;
				case 2:
					cout << "-"; cout.flush(); break;
				case 3:
					cout << "\\"; cout.flush(); break;
				}
				cout << "\b"; cout.flush();
			}
			if (mo = NULL)
				delete mo;
			i++;
		}
		if(!listing){
			cout << "\b Done ." << endl;
		}
		delete ObjectList;                            // Deleting the object list.

		i = 0;
		if(!listing){
			cout << "Deleting Lights: " ;
		}

		Light *ml;
		for (int yy = 0; yy < LightList->size(); yy++){
			ml = LightList->at(yy);
			if(!listing){
				i = i % 4;
				switch (i)
				{
				case 0:
					cout << "/"; cout.flush(); break;
				case 1:
					cout << "|"; cout.flush(); break;
				case 2:
					cout << "-"; cout.flush(); break;
				case 3:
					cout << "\\"; cout.flush(); break;
				}
				cout << "\b"; cout.flush();
			}
			if (ml != NULL)
				delete ml;
			i++;
		}
		if(!listing){
			cout << "\b Done ." << endl;
		}
		delete LightList;                             // Deleting the light list.

		i = 0;
		if(!listing){
			cout << "Deleting Maps: ";
		}
		Texture *mm;
		for (int yy = 0; yy < MapList->size(); yy++)
		{
			mm = MapList->at(yy);
			if(!listing){
				i = i % 4;
				switch (i){
					case 0:
						cout << "/"; cout.flush(); break;
					case 1:
						cout << "|"; cout.flush(); break;
					case 2:
						cout << "-"; cout.flush(); break;
					case 3:
						cout << "\\"; cout.flush(); break;
				}
				cout << "\b"; cout.flush();
				i++;
			}
			if (mm != NULL)
				delete mm;
		}
		if(!listing){
			cout << "\b Done ." << endl;
		}
		delete MapList;                               // Deleting the map list.

		i = 0;
		if(!listing){
			cout << "Deleting Materials: ";
		}
		Material *mmat;
		for (int yy = 0; yy < MaterialList->size(); yy++)
		{
			mmat = MaterialList->at(yy);
			if(!listing){
				i = i % 4;
				switch (i){
					case 0:
						cout << "/"; cout.flush(); break;
					case 1:
						cout << "|"; cout.flush(); break;
					case 2:
						cout << "-"; cout.flush(); break;
					case 3:
						cout << "\\"; cout.flush(); break;
				}
				cout << "\b"; cout.flush();
				i++;
			}
			if (mmat != NULL)
				delete mmat;
		}
		if(!listing){
			cout << "\b Done ." << endl;
		}
		delete MaterialList;                               // Deleting the map list.

		if(File!=NULL)
			delete File;                                  // Deleting the image buffer.
		if(camera!=NULL)
			delete camera;
	}

	void Scene::addObject(Objet *o)                { ObjectList->push_back(o);                      }
	void Scene::addTexture(Texture *t)             { MapList->push_back(t);                         }
	void Scene::addMaterial(Material *m)           { MaterialList->push_back(m);                    }
	void Scene::addLight(Light *l)                 { LightList->push_back(l);                       }
	void Scene::setAmbientColor(Color c)           { AmbientColor = c;                              }
	void Scene::setBackGroundColor(Color c)        { BackGroundColor = c;                           }
	void Scene::setFrameNumber(int f)              { FrameNumber = f;                               }
	int  Scene::getFrameNumber()                   { return FrameNumber;                            }
	vector<Texture *> *Scene::getMappingList(void) { return MapList;                                }
	void Scene::setCamera(Camera *c)               { camera = c; camera->setResolution(ResX, ResY); }
	void Scene::setCamera(Point c, Point l){
		Cam = c;
		LookAt = l;

		Vector V = (LookAt - Cam); V.Normalize();
		//Eye = Cam - V * Cam.Distance(LookAt);
		ViewPlane = new Plan(Cam, V);
	}

	void Scene::Start() {

		ParseFile(InputFileName);

		for (int i = 0; i < FrameNumber; i++) {
			Dispatcher->reset();   //We reset the count of line to compute
			if (FrameNumber > 1 && i > 0)
			{
				cout << "Computing frame: " << i << endl;

				for (int yy = 0; yy < ObjectList->size(); yy++)
					(ObjectList->at(yy))->applyFrameAnimation(i);

				for (int yy = 0; yy < MapList->size(); yy++)
					(MapList->at(yy))->Animate(i);
			}
			else
				cout << "Computing..." << endl;

			cout << "Creating Octrees... ";
			CreateSmallestOctree();
			cout << "Done." << endl;

			if(camera->isGenerated()){
				cout << "Setting Camera target" << endl;
				camera->setTarget(OctreeScene->getCenter());
			}

			cout << "Creating CullingBoxes ";
			for(Objet*& mObject: *ObjectList){
				// Create the Culling boxes for the object
				camera->CreateCullingBox(mObject);

				if (mObject->isPolygon()){
					// Create Octree for polygons.
					((Polygon *)mObject)->CreateSmallestOctree(MAX_OCTREE_LEVEL);
					//cout << "Octree Before: " << ((Polygon *)mObject)->getOctree()->getTotalCount() << endl;

					// Create Quadtree for polygons.
					((Polygon *)mObject)->CreateSmallestQuadtree(MAX_QUADTREE_LEVEL);
				}
			}
			cout << "Done." << endl;

			cout << "Creating Quadtree ";
			CreateSmallestQuadTree();
			cout << "Done." << endl;

			for(Objet*& mObject: *ObjectList){
				OctreeScene->Add(mObject);
				QuadTreeScene->Add(mObject);
			}

			for(Objet*& mObject: *ObjectList){
				if(mObject->isPolygon()){
					((Polygon *)mObject)->getOctree()->Deduplicate();
					int c = 0;
					((Polygon *)mObject)->getOctree()->UpdateCount(&c);
					//cout << "Octree After: " << ((Polygon *)mObject)->getOctree()->getTotalCount() << endl;
				}
			}
			OctreeScene->Deduplicate();

			if (FrameNumber > 1)
				File->setFrame(i);

			auto start = chrono::steady_clock::now();
			if (Progressive == false) {
				StandardCalculation(numThreads, Dispatcher);
			} else {
				ProgressiveComputation(numThreads, Dispatcher);
			}
			auto end = chrono::steady_clock::now();

			chrono::duration<double> elapsed_seconds = end-start;
    		cout << endl << "Total Time: " << elapsed_seconds.count() << "s\n";
    		cout << (ResX * ResY) / (elapsed_seconds.count()) << " PPS" << endl;

			if (FrameNumber > 1)
				cout << "\rDone:  " << i + 1 << endl;
			else
				cout << "\rDone" << endl;
			File->saveImage();

			delete OctreeScene;
			delete QuadTreeScene;
		}
	}

	void Scene::CreateSmallestQuadTree()
	{
		int x = 5000;
		int X = -5000;
		int y = 5000;
		int Y = -5000;

		CullingBox *C;

		for(Objet*& m: *ObjectList){
			if (m != NULL)
			{
				C = m->getCullingBox();

				if (C->getXmin() < x)
					x = C->getXmin();

				if (C->getXmax() > X)
					X = C->getXmax();

				if (C->getYmin() < y)
					y = C->getYmin();

				if (C->getYmax() > Y)
					Y = C->getYmax();
			}
		}

		QuadTreeScene = new QuadTree(x, X, y, Y);
		QuadTreeScene->setMaxLevel(MAX_QUADTREE_LEVEL);
	}

	void Scene::CreateSmallestOctree()
	{
		double x = 50000;		double X = -50000;
		double y = 50000;		double Y = -50000;
		double z = 50000;		double Z = -50000;

		for(Objet*& m: *ObjectList){
			if (m != NULL){
				x = MIN(x,m->getBoundingBox()->getBoundingMin().x);
				X = MAX(X,m->getBoundingBox()->getBoundingMax().x);
				y = MIN(y,m->getBoundingBox()->getBoundingMin().y);
				Y = MAX(Y,m->getBoundingBox()->getBoundingMax().y);
				z = MIN(z,m->getBoundingBox()->getBoundingMin().z);
				Z = MAX(Z,m->getBoundingBox()->getBoundingMax().z);
			}
		}

		OctreeScene = new Octree(x,X,y,Y,z,Z);
		OctreeScene->setMaxLevel(MAX_OCTREE_LEVEL);

		cout << "_________________________________________" << endl;
		OctreeScene->getBoundingBox().show();
	}

	void Scene::setMaterial(Objet *o, int id)
	{
		Material *mmat;

		for (int yy = 0; yy < MaterialList->size(); yy++) {
			{
				mmat = MaterialList->at(yy);
				if (mmat != NULL)
				{
					if (mmat->getId() == id)
					{
						o->setMaterial(mmat);
						return;
					}
				}
			}
		}
	}

	void Scene::StandardCalculation(int ThreadNumber, RowDispatcher *Dispatcher) {
		// Start calculation threads (we run one thread on each processor core)
		ThreadContext context;
		context.raytracer = new Raytracer(this, Dispatcher);

		cout << "Running " << ThreadNumber << " calculation thread(s)..." << endl;
		vector<thread *> threadList;

		for (int j = 0; j < ThreadNumber; ++ j) {
			thread * t = new thread(&Scene::Standard_helper, (void *) &context);
			threadList.push_back(t);
		}

		// Wait for the threads to complete...
		for (auto& t : threadList) {
            	t->join();
				delete t;
        }
	
		if (antialias == true) {

			Dispatcher->reset(); //We reset the Dispatcher line number;
			
			auto start = chrono::steady_clock::now();
			// Start calculation threads (we run one thread on each processor core)
			cout << "\rAliasing...            " << endl;
			vector<thread *> threadList;

			for (int j = 0; j < ThreadNumber; ++ j) {
				thread * t = new thread(&Scene::Alias_helper, (void *) &context);
				threadList.push_back(t);
			}
			// Wait for the threads to complete...
			for (auto& t : threadList) {
            	t->join();
				delete t;
        	}

			auto end = chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = end-start;
    		cout << "\rAliasing Time: " << elapsed_seconds.count() << "s\n";
		}

		delete context.raytracer;
	}

	void Scene::ProgressiveComputation(int ThreadNumber, RowDispatcher *Dispatcher) {
		Ray Vision;
		Vector V = (LookAt - Cam); V.Normalize();
		Point Eye = Cam - V * Cam.Distance(LookAt);
		Vision.setOrigin(&Eye);
		Vision.setColor(1, 1, 1);

		int blocksize   = BlockSize;
		float seuil     = ((double)Seuil) / 255;
		int x;
		int y;
		int i;
		int passe       = 1;
		ThreadContext context;

		char **table;
		table = new char*[ResX + blocksize];
		for (i = 0; i < ResX; i++)
			table[i] = new char[ResY + blocksize];

		for (x = 0; x < ResX; x++)
			for (y = 0; y < ResY; y++)
				table[x][y] = 1;

		while (blocksize > 0) {
			Dispatcher->setDirection(DISPATCHER_ASC);
			Dispatcher->setBlockSize(blocksize);

			context.raytracer = new Raytracer(this, Dispatcher);
			context.passe =  passe;
			context.table =  table;

			cout << "Step: " << passe << endl;
			list<thread *> threadList;

			for (int j = 0; j < ThreadNumber; ++ j) {
				thread * t = new thread(&Scene::Progressive_helper, (void *) &context);
				threadList.push_back(t);
			}

			// Wait for the threads to complete...
			for (auto& t : threadList) {
					t->join();
					delete t;
			}
			
			for (x = 0; x < ResX; x += blocksize)
				for (y = 0 ; y < ResY; y += blocksize) {
					if (File->ComputeMaxDifference(blocksize, x, y) > seuil)
					{
						if (blocksize > 1)
						{
							table[x + blocksize / 2][y]                 = passe + 1;
							table[x + blocksize / 2][y + blocksize / 2] = passe + 1;
							table[x][y + blocksize / 2]                 = passe + 1;
						}
						else
							table[x][y] = passe + 1;
					}
				}

			blocksize = blocksize / 2;
			passe++;
		}

		Color c;
		for (x = 0; x < ResX / 2; x++) {
			for (y = 0; y < ResY; y++) {
				c = File->getColor(x, y);
				File->setColor(File->getColor(ResX - 1 - x, ResY - 1 - y), x, y);
				File->setColor(c, ResX - 1 - x, ResY - 1 - y);
			}
		}

		if (antialias == true) {
			Dispatcher->setDirection(DISPATCHER_DSC);
			Dispatcher->setBlockSize(1);

			auto start = chrono::steady_clock::now();
			// Start calculation threads (we run one thread on each processor core)
			cout << "\rAliasing...            " << endl;
			list<thread *> threadList;

			for (int j = 0; j < ThreadNumber; ++ j) {
				thread * t = new thread(&Scene::Alias_helper, (void *) &context);
				threadList.push_back(t);
			}

			// Wait for the threads to complete...
			for (auto& t : threadList) {
            	t->join();
				delete t;
       	 	}

			auto end = chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = end-start;
    		cout << "\rAliasing Time: " << elapsed_seconds.count() << "s\n";
		}

	}

	void Scene::Standard_helper(void *arg) {
		ThreadContext *threadcontext = (ThreadContext *)arg;
		threadcontext->raytracer->Standard();
	}

	void Scene::Alias_helper(void *arg) {
		ThreadContext *threadcontext = (ThreadContext *)arg;
		threadcontext->raytracer->Alias();
	}

	void Scene::Progressive_helper(void *arg) {
		ThreadContext *threadcontext = (ThreadContext *)arg;
		threadcontext->raytracer->Progressive(threadcontext->table, threadcontext->passe);
	}
}
