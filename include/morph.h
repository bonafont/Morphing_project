#include <SDL.h>
#include "Tools/vecteur2D.h"
#include <list>
#include <vector>
#include <iostream>
#include <math.h>

// little-endian pixel
typedef struct Color_t{
    Uint8 b;
    Uint8 g;
    Uint8 r;
    Uint8 a;
} ColorPixelBGRA;

typedef struct test_t{
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Surface* firstimgsrc;
    SDL_Surface* firstimgmod;
    SDL_Surface* sndeimgsrc;
    SDL_Surface* sndeimgmod;
    SDL_Renderer* renderer;

} TEST;

typedef struct LineCoordinates_t{
    Vecteur2D a;
    Vecteur2D b;
} LineCoordinates;

typedef vector<vector<ColorPixelBGRA*>> PixelArray;

//Starts up SDL and creates window
bool init(TEST& surface);

/**
 *  @param Xdst : Destination pixel
 *  @param Pdst : P coordinate from PQ line from destination file
 *  @param Qdst : Q coordinate from PQ line from destination file
 *  @param Psrc : P coordinate from PQ line from source file
 *  @param Qsrc : Q coordinate from PQ line from source file
 * 
 *  @short Single-line algorithm for morphing
**/
Vecteur2D algotest(Vecteur2D Xdst, Vecteur2D Pdst, Vecteur2D Qdst,Vecteur2D Psrc, Vecteur2D Qsrc);

/**
 * @short Calculate the shrotest distance between two vectors
 * 
**/
double shortestdistance(Vecteur2D l1,Vecteur2D l2, Vecteur2D point);

/**
 * @short Calculate the shrotest distance between two vectors alternate version
 * 
**/
double shortestdistance2(Vecteur2D a,Vecteur2D b, Vecteur2D point);

//Frees media and shuts down SDL
void close(SDL_Surface*& hello,SDL_Window*& window);

/**
 * @short Get the pointers of the pixel Array of a SDL_Surface
 * 
**/
bool getPixelsArray(SDL_Surface* surface,PixelArray& arrayPixels);


/**
 * @short Load image to SDL
 * 
**/
bool loadMedia(SDL_Surface*& surface,const char* path);


/**
 * @short Multi-line algorithm for morphing
 * 
**/
void calculateDestination(SDL_Surface*& srcSurface, SDL_Surface*& dstSurface, const PixelArray& pixelArraysrc, PixelArray& pixelArraydst,const list<LineCoordinates>& srcList,const list<LineCoordinates>& dstList);
/**
 * @short Calculate the weighting of a line
 * 
**/
double weightcalc(double a, double b, double p,double length,double dist);

/**
 * @short Calculate the intermediary lines necessary for animated morphs
 * 
**/
bool timeFrame(const LineCoordinates& srcLine, const LineCoordinates& dstLine, LineCoordinates* res,double keyframes);

/**
 * @short moves the frames independently
 * 
**/
void moveFrames(SDL_Texture*& tex,SDL_Texture*& tex2,list<SDL_Texture*>::iterator& morph1,list<SDL_Texture*>::iterator& morph2,int direction);