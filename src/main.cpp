#include "morph.h"
#include <time.h>

using namespace std;

int main(int argc, char** argv) {

    if(argc != 3){
        cout << "Error arguments !" << endl;
        cout << "example : IRMorph.exe image1 image2" << endl;
        cout << "Accepted images : bmp format with same height and width" << endl;
        exit(EXIT_FAILURE);
    }

    clock_t fps = clock();
    TEST test;
    bool transform = false;   
    SDL_bool done = SDL_FALSE;
    SDL_Texture* tex = NULL,*tex3 = NULL;
    SDL_Event event;
    list<LineCoordinates> srcList,dstList,*curList;
    list<SDL_Texture*> texList1,texList2;
    PixelArray pixelArrayfirstsrc,pixelArrayfirstmod,pixelArraysndsrc,pixelArraysndmod;
    list<SDL_Texture*>::iterator tex1It,tex2It;
    SDL_Texture *curTex1,*curTex2;  
    bool animation = false;
    const char* path1 = argv[1];
    const char* path2 = argv[2];

    if(!loadMedia(test.firstimgsrc,path1))
        exit(EXIT_FAILURE);
    
    if(!loadMedia(test.firstimgmod,path1))
        exit(EXIT_FAILURE);

    if(!loadMedia(test.sndeimgsrc,path2))
        exit(EXIT_FAILURE);
    
    if(!loadMedia(test.sndeimgmod,path2))
        exit(EXIT_FAILURE);

    if(!init(test))
        exit(EXIT_FAILURE);
  
    
	if(!getPixelsArray(test.firstimgsrc,pixelArrayfirstsrc))
        exit(EXIT_FAILURE);
    if(!getPixelsArray(test.firstimgmod,pixelArrayfirstmod))
        exit(EXIT_FAILURE);
    if(!getPixelsArray(test.sndeimgsrc,pixelArraysndsrc))
         exit(EXIT_FAILURE);
     if(!getPixelsArray(test.sndeimgmod,pixelArraysndmod))
         exit(EXIT_FAILURE);

    tex = SDL_CreateTextureFromSurface(test.renderer,test.firstimgsrc);
    tex3 = SDL_CreateTextureFromSurface(test.renderer,test.sndeimgsrc);

    SDL_SetTextureBlendMode(tex,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(tex3,SDL_BLENDMODE_BLEND);

    bool firstClick = true;
    int xMouse,yMouse;
	int currentMouseX, currentMouseY;
    Vecteur2D temp;
    bool isGenerated = false;
    int alpha = 255;
    double weight = 0;
    curList = &srcList;
    SDL_SetTextureAlphaMod(tex,255);
    SDL_SetRenderDrawColor(test.renderer, 255, 0, 0, 255);
    int disableLines = 0;
    double pourcentage = 2;
    /*
	
		MAIN LOOP
	
	*/
    while(!done){
		SDL_GetMouseState(&currentMouseX, &currentMouseY);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                done = SDL_TRUE;
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(firstClick){
					firstClick = false;
					xMouse = currentMouseX;yMouse = currentMouseY;
					if (dstList.size() == srcList.size() && srcList.size() != 0)
						dstList.pop_back();
                }else{
					firstClick = true;
					temp = Vecteur2D(xMouse,yMouse);
					xMouse = currentMouseX;yMouse = currentMouseY;
                    curList->push_back({temp,Vecteur2D(xMouse,yMouse)});
                    if (dstList.size() == srcList.size() && dstList.size() != 0)
                        pourcentage = 0;
                        
					
                } 
                //cout << xMouse << ", " << yMouse << endl;     //DEBUG PURPOSES      
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
                if(!firstClick)
                    firstClick = true;
                else if(curList->size() != 0)
                    curList->pop_back();
            }   
            if(event.type == SDL_KEYUP){
                if(event.key.keysym.scancode == SDL_SCANCODE_R){
                    // Reset
                    SDL_FreeSurface(test.firstimgmod);
                    if(!loadMedia(test.firstimgmod,path1))
                        exit(EXIT_FAILURE);
                    getPixelsArray(test.firstimgmod,pixelArrayfirstmod);

                    SDL_FreeSurface(test.sndeimgmod);
                    if(!loadMedia(test.sndeimgmod,path2))
                        exit(EXIT_FAILURE);
                    getPixelsArray(test.sndeimgmod,pixelArraysndmod);

                    firstClick = true;   
                    srcList.clear();
                    dstList.clear();
                    curList = &srcList;
                    //DEBUG
	                //tex2 = SDL_CreateTextureFromSurface(test.renderer,test.firstimgmod);
                    //SDL_SetTextureBlendMode(tex2,SDL_BLENDMODE_BLEND);
                    //SDL_SetTextureAlphaMod(tex,255);
                    //SDL_SetTextureAlphaMod(tex2,0);
                    //SDL_SetTextureAlphaMod(tex3,255);               
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
                    curList = &dstList;
                    cout << "SWITCHED TO DESLIST !"<<endl;
                    cout << "number of lines allowed : " << srcList.size() << endl;
                    SDL_SetTextureAlphaMod(tex,0);
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_D){
                    if(disableLines)
                        disableLines = 0;
                    else
                        disableLines = 1;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_A){
                    tex1It = texList1.begin(); tex2It = texList2.begin();
                    animation = true;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                    if(isGenerated && texList1.begin() != tex1It && texList2.begin() != tex2It){
                        tex1It--;tex2It--;
                        if(texList1.begin() != tex1It && texList2.begin() != tex2It)
                            moveFrames(curTex1,curTex2,tex1It,tex2It,SDL_SCANCODE_LEFT);                 
                        else{
                            cout << "Unavailable" << endl;
                            tex1It++;tex2It++;
                        }
                    }

                        
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                    if(isGenerated && texList1.end() != tex1It && texList2.end() != tex2It){
                        tex1It++;tex2It++;
                        if(texList1.end() != tex1It && texList2.end() != tex2It){ 
                            moveFrames(curTex1,curTex2,tex1It,tex2It,SDL_SCANCODE_RIGHT);
                        }                        
                        else{
                            cout << "Unavailable" << endl;
                            tex1It--;tex2It--;
                        }
                            
                    }
                        
                }
                
            }                
        } 
           
        SDL_RenderClear(test.renderer);
        SDL_RenderCopy(test.renderer,tex3,NULL,NULL);
        SDL_RenderCopy(test.renderer,tex,NULL,NULL);
        
        //Image generation
        while (pourcentage < 2){
            if(pourcentage > 1)
                pourcentage = 1;
            LineCoordinates current;
            list<LineCoordinates>::iterator dstIt,srcIt;
            dstIt = dstList.begin(); srcIt = srcList.begin();
            list<LineCoordinates> temp;
            while(dstList.end() != dstIt){
                LineCoordinates curDst = *dstIt;
                LineCoordinates curSrc = *srcIt;
                timeFrame(curSrc,curDst,&current,pourcentage);
                temp.push_back(current);
                dstIt++;
                srcIt++;

            }
            //Hide source images
            SDL_SetTextureAlphaMod(tex3,0);
            SDL_SetTextureAlphaMod(tex,0);

            //Texture list 1
            SDL_SetSurfaceBlendMode(test.firstimgmod,SDL_BLENDMODE_ADD);
            SDL_SetSurfaceAlphaMod(test.firstimgmod,(Uint8)(255*(1-pourcentage)));
            if(pourcentage == 0){
                texList1.push_back(SDL_CreateTextureFromSurface(test.renderer,test.firstimgsrc));
            }else{
                calculateDestination(test.firstimgsrc,test.firstimgmod, pixelArrayfirstsrc, pixelArrayfirstmod, srcList,temp);
                texList1.push_back(SDL_CreateTextureFromSurface(test.renderer,test.firstimgmod));
            }
            
            //Texture List 2
            SDL_SetSurfaceBlendMode(test.sndeimgmod,SDL_BLENDMODE_ADD);
            SDL_SetSurfaceAlphaMod(test.sndeimgmod,(Uint8)(255*(pourcentage)));
            if(pourcentage == 1)
                texList2.push_back(SDL_CreateTextureFromSurface(test.renderer,test.sndeimgsrc));
            else{
                calculateDestination(test.sndeimgsrc,test.sndeimgmod, pixelArraysndsrc, pixelArraysndmod, dstList,temp);
                texList2.push_back(SDL_CreateTextureFromSurface(test.renderer,test.sndeimgmod));
            }

            temp.clear();
            isGenerated = true;           
            tex1It = texList1.begin(); tex2It = texList2.begin();
            cout << "Generating animation : " << pourcentage * 100 << "%" << endl;
            animation = true;
            if(pourcentage >= 1)
                pourcentage = 2;
            pourcentage += .05;

        }

        //The image has been generated
        if(isGenerated){ 
            if(animation){
                if(texList1.end() != tex1It){
                    curTex1 = *tex1It; curTex2 = *tex2It;
                    clock_t difference = clock() - fps;
                    int msec = difference * 1000 / CLOCKS_PER_SEC;
                    if(msec > 66){
                        tex2It++;
                        tex1It++;
                        fps = clock();
                    }
                }else
                    animation = false;               
            }
            SDL_RenderCopy(test.renderer,curTex2,NULL,NULL); 
            SDL_RenderCopy(test.renderer,curTex1,NULL,NULL);
            
        }

        //Drawing the construction lines
        SDL_SetRenderDrawColor(test.renderer, 0, 0, 0, 255);
		if (!firstClick) 
			if(SDL_RenderDrawLine(test.renderer,xMouse,yMouse,currentMouseX,currentMouseY) == -1)
			    printf(SDL_GetError());
        
        
        if(!disableLines){
            int nbLines =0;
            for (list<LineCoordinates>::iterator it=srcList.begin(); it!=srcList.end(); ++it){
            if(nbLines == dstList.size() && curList == &dstList)
                SDL_SetRenderDrawColor(test.renderer, 255, 255, 0, 255);
            else
                SDL_SetRenderDrawColor(test.renderer, 255, 0, 0, 255);
            LineCoordinates* current = (LineCoordinates*)&*it;
            if(SDL_RenderDrawLine(test.renderer,(int)current->a.x,(int)current->a.y,(int)current->b.x,(int)current->b.y) == -1)
                printf(SDL_GetError());
            nbLines++;
            }
            SDL_SetRenderDrawColor(test.renderer, 0, 255, 0, 255);
            for (list<LineCoordinates>::iterator it=dstList.begin(); it!=dstList.end(); ++it){
                LineCoordinates* current = (LineCoordinates*)&*it;
                if(SDL_RenderDrawLine(test.renderer,(int)current->a.x,(int)current->a.y,(int)current->b.x,(int)current->b.y) == -1)
                    printf(SDL_GetError());
            }
        }
        
        SDL_SetRenderDrawColor(test.renderer,0, 0,0, 0);
        SDL_RenderPresent(test.renderer);   
    }

    close(test.firstimgsrc,test.window);
    return EXIT_SUCCESS;
}