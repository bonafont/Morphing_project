#include "morph.h"

bool init(TEST& test){
    //Ini flag
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr,"Erreur");
        success = false;
    }else{
        //Create window
        test.window = SDL_CreateWindow( "IR_Morph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, test.firstimgsrc->w, test.firstimgsrc->h, SDL_WINDOW_SHOWN );
        test.renderer = SDL_CreateRenderer(test.window,-1,0);
        if( test.window == NULL ){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else{
            //Get window surface
            test.screenSurface = SDL_GetWindowSurface(test.window);
        }

    }
    return success;
}

Vecteur2D algotest(Vecteur2D Xdst, Vecteur2D Psrc, Vecteur2D Qsrc,Vecteur2D Pdst, Vecteur2D Qdst){
    double u = (Xdst - Pdst) * (Qdst - Pdst) / pow(norme((Qdst - Pdst)),2);
    double v = ((Xdst - Pdst) * (Qdst - Pdst).rotationDirecteQuartDeTour())/norme(Qdst - Pdst);
    Vecteur2D Xres = Psrc + (u * (Qsrc - Psrc)) + ((v * (Qsrc - Psrc).rotationDirecteQuartDeTour())/norme(Qsrc - Psrc));
    return Xres;
}


double shortestdistance(Vecteur2D l1,Vecteur2D l2, Vecteur2D point){
    return abs((l2.y - l1.y) * point.x - (l2.x - l1.x)*point.y + l2.x*l1.y - l2.y*l1.x)/sqrt(pow(l2.y-l1.y,2)+pow(l2.x-l1.x,2));
}

double shortestdistance2(Vecteur2D a,Vecteur2D b, Vecteur2D point){
    Vecteur2D n = b - a;
    Vecteur2D pa = a - point;
    Vecteur2D c = n * ( pa * n ) /( n * n );
    Vecteur2D d = pa - c;
    return sqrt( d * d );
}

void close(SDL_Surface*& hello,SDL_Window*& window)
{
    //Deallocate surface
    SDL_FreeSurface( hello );
    hello = NULL;

    //Destroy window
    SDL_DestroyWindow( window );
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool getPixelsArray(SDL_Surface* surface,PixelArray& arrayPixels) {
    arrayPixels.clear();
	SDL_PixelFormat* fmt = surface->format;
	int pixelCount = surface->w * surface->h;
	int next = 0;
    vector<ColorPixelBGRA*> row;
	if (fmt->format == SDL_PIXELFORMAT_BGRA32) {
		for (int i = 0; i < pixelCount; i++) {
            if( i % surface->w ==0 && i != 0){
                arrayPixels.push_back(row);
                row.clear();
            }
            ColorPixelBGRA* pixel = (ColorPixelBGRA*)surface->pixels + next;
            row.push_back(pixel);
			next++;
		}
        arrayPixels.push_back(row);
        return true;
	}
	else {
		return false;
	}
}

void calculateDestination(SDL_Surface*& srcSurface, SDL_Surface*& dstSurface, const PixelArray& pixelArraysrc, PixelArray& pixelArraydst,const list<LineCoordinates>& srcList,const list<LineCoordinates>& dstList)
{
	//SDL_FillRect(dstSurface, NULL, SDL_MapRGBA(dstSurface->format, 255, 255, 255,0));
	//cout << "PQ : " << PQdst.a << PQdst.b << ", PQp : " << PQsrc.a << PQsrc.b << endl;
	//cout << test.helloWorlddst->w;

	for (int i = 0; i < dstSurface->h; i++) {
		for (int j = 0; j < dstSurface->w; j++) {
            Vecteur2D diweight;
            double weightsum = 0;
            Vecteur2D Xprime(j,i);
            list<LineCoordinates>::const_iterator dstIt,srcIt;
            dstIt = dstList.begin(); srcIt = srcList.begin();
            while(dstList.end() != dstIt){
                LineCoordinates curDst = *dstIt;
                LineCoordinates curSrc = *srcIt;
                //cout << curDst.a << endl;
                Vecteur2D res = algotest(Xprime, curDst.a, curDst.b, curSrc.a, curSrc.b);
                Vecteur2D di = res - Xprime;
                double dist = shortestdistance(curDst.a,curDst.b,Xprime);
                double weight = weightcalc(.5,.5,1,sqrt(pow(curDst.b.x-curDst.a.x,2)+ pow(curDst.b.y - curDst.a.y,2)),dist);
                diweight += di * weight;
                weightsum += weight;
                
                dstIt++;
                srcIt++;
            }
			Vecteur2D res = Xprime + diweight / weightsum;
			//cout << res << i << " , " << j << endl;
			if (res.x < srcSurface->w && res.x >= 0 && res.y < srcSurface->h && res.y >= 0) {
				//cout << (int)res.y << (int) res.y << endl;
				*pixelArraydst[res.y][res.x] = *pixelArraysrc[i][j];
                
			}
		}
	}
    //SDL_UpdateTexture(restex,NULL,dstSurface->pixels,dstSurface->pitch);
}

bool loadMedia(SDL_Surface*& surface,const char* path){
    //Loading success flag
    bool success = true;
    //Load splash image
    SDL_Surface * temp = SDL_LoadBMP(path);
    if( temp == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
        return false;
    }
    SDL_PixelFormat* bla = temp->format;
    if(bla->format != SDL_PIXELFORMAT_BGRA32){
        SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_BGRA32);
        surface = SDL_ConvertSurface(temp, format, 0);
        SDL_FreeSurface(temp);
        SDL_FreeFormat(format);
    }else
    {
        surface = temp;
    }
    return true;
}

double weightcalc(double a, double b, double p,double length,double dist){
    return pow((pow(length,p)/(a + dist)),b);
}

bool timeFrame(const LineCoordinates& srcLine, const LineCoordinates& dstLine, LineCoordinates* dstResLine,double keyframes){
    //if(keyframes == 0)
    //    keyframes = 1;
    LineCoordinates orthA;
    Vecteur2D test1 = dstLine.a - srcLine.a;
    Vecteur2D test2 = dstLine.b - srcLine.b;
    test1 *= abs(keyframes);
    test2 *= abs(keyframes);

    dstResLine->a = srcLine.a + test1;
    dstResLine->b = srcLine.b + test2;

    return true;
}

void moveFrames(SDL_Texture*& tex,SDL_Texture*& tex2,list<SDL_Texture*>::iterator& morph1,list<SDL_Texture*>::iterator& morph2,int direction){
    // if(direction == SDL_SCANCODE_LEFT){
    //     morph1--;morph2--;
    // }
    // else if(direction == SDL_SCANCODE_RIGHT){
    //     morph1++;morph2++;
    // }
    tex = *morph1;
    tex2 = *morph2;
}