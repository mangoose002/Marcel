using namespace std;
#include "Core/Triangle.hh"
#include "Core/Raytrace.hh"

#include "Core/Image.hh"
#include "Core/ImageClient.hh"
#include "Core/Octree.hh"

#include <vector>
#include <list>

#include <time.h>
#include <errno.h>


//using namespace tthread;
namespace Marcel{

	#define   AMBIENT	.4
	#define   BIAS      0
	#define   IN(a)     Debug debug=Debug(a,__FILE__,__LINE__);
	#define   MIN(a,b)   a>b?b:a
	#define   MAX(a,b)   a>b?a:b

	//////////////////////////////////////////////////////////////////////////////////
	Raytracer::Raytracer(Scene *S, RowDispatcher *D){
		scene = S;
		Dispatcher = D;

		ResX = scene->getImage()->Width();
		ResY = scene->getImage()->Height();

		halfResX = ResX / 2;
		halfResY = ResY / 2;
		MAX_LEVEL = 5;

		Cam    = scene->getCam();
		Lookat = scene->getLookAt();

		Vector V = (Lookat - Cam); V.Normalize();
		Oeil = Cam - V * Cam.Distance(Lookat);
		CX = Vector(-V.z, 0, V.x);
		CY = CX / V;
		CZ = V;

		CoeffX = CoeffY = Cam.Distance(Lookat) / (sqrt(ResX * ResX + ResY * ResY));

		CX.Normalize();
		CY.Normalize();
		CZ.Normalize();

		Cx = CoeffX * CX.x;
		Cy = CoeffX * CX.y;
		Cz = CoeffX * CX.z;
	}
	///////////////////////////////////////////////////////////////////////////
	Raytracer::~Raytracer() {}
	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	Color Raytracer::Min(Color *C)
	{
		float R, G, B;
		R = C->getRed();
		G = C->getGreen();
		B = C->getBlue();

		R = MIN(1,R);
		G = MIN(1,G);
		B = MIN(1,B);

		return Color(R, G, B);
	}

	///////////////////////////////////////////////////////////////////////////
	Color Raytracer::computeDiffuse(Tuple *CP, const Ray *Ra)
	{
		if (CP->obj->getReflection() == 1) // Objet Brillant = Pas de diffsion
			return Color(0, 0, 0);

		Light *light;

		Ray R;  // Ray local de raytracing
		Point P2; // Point Local d'intersection
		Vector V, D, V1;
		Vector N1,N2;
		Vector Bias;
		Color Intensity;

		double NS = 0;
		double NL = 0;
		double VN = 0;
		double t;

		Intensity.setColor(0, 0, 0);
		R.setOrigin(&(CP->intersection));
		R.Level = 1;

		for(vector<Light *>::iterator it = scene->getLightList()->begin(); it != scene->getLightList()->end(); ++it) {
			light = (*it);

			V = light->getOrigin() - CP->intersection; // Ray from the point to the source

			if (V.x != 0 || V.y != 0)
				N1 = Vector(V.y, -V.x, 0);
			else
				N1 = Vector(0, V.z, -V.y);

			N1.Normalize();
			N2 = N1 / V;
			N2.Normalize();

			for (int i = 0; i < 8 * scene->is_SoftShadow() + 1; i++){
				switch(i){
					case 0: V = V;                         break;
					case 1: V = V + BIAS * N1;             break;
					case 2: V = V - BIAS * N1;             break;
					case 3: V = V + BIAS * N2;             break;
					case 4: V = V - BIAS * N2;             break;
					case 5: V = V + BIAS * N1 + BIAS * N2; break;
					case 6: V = V - BIAS * N1 + BIAS * N2; break;
					case 7: V = V + BIAS * N1 - BIAS * N2; break;
					case 8: V = V - BIAS * N1 - BIAS * N2; break;
				}

				t = V.Norme();
				V.divide(t);
				//V1.Normalize();

				R.setDirection(&V);

				if (light->CanIlluminate(&(CP->intersection))){
					NL = CP->normal * V;
					if (NL < 1e-8)
						NL = 0;

					if (CP->obj->getSurB() != 0){
						VN = 2 * NL;

						D.x = V.x - VN * CP->normal.x; // On determine le rayon reflechi
						D.y = V.y - VN * CP->normal.y;
						D.z = V.z - VN * CP->normal.z;

						NS = (Ra->D.x * D.x + Ra->D.y * D.y + Ra->D.z * D.z);
						if (NS > 1e-8)
							NS = pow(NS, CP->obj->getSurB()); // On calcul la surbrillance
						else
							NS = 0;
					} else{
						NS = 0;
					}

					if (light->CastShadows == true && scene->is_Raycasting()==false){
						scene->getOctree()->ComputeLight(&R, t, &NS, &NL, NULL);
					}

					if (CP->obj->getMaterial() != NULL)
						Intensity = Intensity + (light->getIntensity(&(CP->intersection))) * CP->color * ((1 - CP->obj->getMaterial()->getTransparency()) * NL) / (8.0 * scene->is_SoftShadow() + 1.0);

					Intensity = Intensity + light->getIntensity(&(CP->intersection)) * NS / (8.0 * scene->is_SoftShadow() + 1.0);
				}
			}
		}

		return Intensity;
	}

	///////////////////////////////////////////////////////////////////////
	void Raytracer::ComputeNonAliasedLine(int width, int y, int blocksize)
	{
		int x;
		int blksize = Dispatcher->getBlockSize();
		Ray Vision;
		Vision.setOrigin(&Oeil);
		Vision.setColor(1, 1, 1);

		for (x = 0; x < width; x += blksize){
			scene->getCamera()->LaunchRay(&Vision, x, y);
			scene->getImage()->setColor(Raytrace(&Vision, NULL), ResX - 1 - x, ResY - 1 - y);
		}
	}

	void Raytracer::ComputeAliasedPixel(int x, int y, int invert, int computepixel)
	{
		Ray Vision;
		Vision.setOrigin(&Oeil);
		Point GlobalP;
		Vector GlobalV;

		Vision.setColor(1, 1, 1);
		Color Result(0, 0, 0);


		if (computepixel){
			scene->getCamera()->LaunchRay(&Vision, x, y);
			Result = Result + Raytrace(&Vision, NULL);
		}

		scene->getCamera()->LaunchRay(&Vision, x - 0.2, y);
		Result = Result + Raytrace(&Vision, NULL);

		scene->getCamera()->LaunchRay(&Vision, x + 0.2, y);
		Result = Result + Raytrace(&Vision, NULL);

		scene->getCamera()->LaunchRay(&Vision, x, y - 0.2);
		Result = Result + Raytrace(&Vision, NULL);

		scene->getCamera()->LaunchRay(&Vision, x, y + 0.2);
		Result = Result + Raytrace(&Vision, NULL);

		if (computepixel)
		{
			if (invert != 0)
				scene->getImage()->setColor(Result * .2, ResX - 1 - x, ResY - 1 - y);
			else
				scene->getImage()->setColor(Result * .2, x, y);
		}
		else
		{
			if (invert != 0)
				scene->getImage()->setColor((scene->getImage()->getColor(ResX - 1 - x, ResY - 1 - y) + Result)*.2, ResX - 1 - x, ResY - 1 - y);
			else
				scene->getImage()->setColor((scene->getImage()->getColor(x, y) + Result)*.2, x, y);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	Color Raytracer::Raytrace(Ray *R, Objet* O)
	{
		if (R->Level != MAX_LEVEL){
			Tuple        CP;

			CP.t = 1e8;

			if (R->Level > 0 && scene->is_Raycasting()==false)
				scene->getOctree()->Intersect(R, &CP);
			else
				scene->getQuadTree()->Intersect(R, &CP);

			if (CP.t > 1e-8 && CP.t < 1e7){ // Le point d'intersection est valide et il est + pret
				Color Intensity;

				CP.intersection = R->getPoint(CP.t);
				Point UVW       = (CP.obj)->computeUVW(&(CP.intersection));  //Code factorisation
				CP.color        = (CP.obj)->getColor(&UVW);

				CP.normal       = (CP.obj)->getNormal(&(CP.intersection),&UVW); 
				CP.normal.Normalize();
				if( CP.normal * R->D > 1e-8){
					CP.normal = Vector(0,0,0)-CP.normal;
				}

				Intensity = scene->getAmbientColor() * (CP.color);
				Intensity = Intensity + computeDiffuse(&CP, R);

				if(scene->is_Raycasting() == false)
					if (CP.obj->getMaterial() != NULL)
						if (CP.obj->getMaterial()->getReflection() > 0){ // Objet Reflechissant
							Ray Reflechi = (CP.obj)->Reflect(R, &(CP.intersection), &(CP.normal));
							Intensity = Intensity + Raytrace(&Reflechi, (CP.obj)) * CP.obj->getMaterial()->getReflection();
						}

				if(scene->is_Raycasting() == false)
					if (CP.obj->getMaterial() != NULL)
						if (CP.obj->getMaterial()->getTransparency() > 0) { // Objet Transparent
							Ray Refracte = (CP.obj)->Refract(R, &(CP.intersection), &(CP.normal));
							Intensity = Intensity + Raytrace(&Refracte, (CP.obj)) * CP.obj->getMaterial()->getTransparency();
						}
				return Min(&Intensity);
			}
			else
				return scene->getBackGroundColor();
		}
		else
			return scene->getBackGroundColor();
	}
	///////////////////////////////////////////////////////////////////////////
	void Raytracer::Standard()
	{
		Ray Vision;
		Vision.setOrigin(&Oeil);
		Vision.setColor(1, 1, 1);
		int blksize = Dispatcher->getBlockSize();
		int y = scene->getImage()->Height()-1;
		int ComputedLine = 0;

		while(true){
			if(y<0 || y==65535){
				break;
			}
			y = Dispatcher->nextRow();
			ComputedLine++;

			for (int x = 0; x < ResX; x += blksize){
				scene->getCamera()->LaunchRay(&Vision, x, y);
				scene->getImage()->setColor(Raytrace(&Vision, NULL), ResX - 1 - x, ResY - 1 - y);
			}
			scene->getImage()->saveLine(y);
		}
	}

	void Raytracer::Alias(){
		int blksize = Dispatcher->getBlockSize();
		int y = scene->getImage()->Height()-1;
		while(true){
			if(y<0 || y==65535){
				break;
			}
			y = Dispatcher->nextRow();

			for (int x = 0; x < ResX; x += blksize){
				if (scene->getImage()->ComputeMaxDifference(1, ResX - 1 - x, ResY - 1 - y) > 0.125)
					ComputeAliasedPixel(x, y, 1);
			}
		}
	}

	void Raytracer::Progressive(char** table,int passe){
		int blksize = Dispatcher->getBlockSize();
		Ray Vision;
		Vision.setOrigin(&Oeil);
		Vision.setColor(1, 1, 1);

		int y = 0;
		while(true){
			if(y<0 || y==65535){
				break;
			}
			y = Dispatcher->nextRow();

			for (int x = 0; x < ResX; x += blksize){
				if (table[x][y] == passe)
				{
					scene->getCamera()->LaunchRay(&Vision, x, y);
					scene->getImage()->setBlock(Raytrace(&Vision, NULL),blksize, x, y);
				}
			}
		}
	}
}
