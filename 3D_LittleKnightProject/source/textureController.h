#ifndef textureController_h
#define textureController_h

#include <map>
#include <string>
using namespace std;

#include "library/bmpImageProcess.h"

#define TEXTURE_COUNT    7 //這必須和下面map內含的數量相同!!
GLuint  textures[TEXTURE_COUNT];

map<char*,int> getTextureNumMap()
{
    static map<char*, int> textureNumMap;
    if(textureNumMap.empty())
    {
        textureNumMap["leadingRole"] = 0;
        textureNumMap["groundGrass"] = 1;
        textureNumMap["dirt"] = 2;
        textureNumMap["demonTexture"] = 3;
        textureNumMap["backgroundSky"] = 4;
        textureNumMap["gameover"] = 5;
        textureNumMap["win"] = 6;
        //記得修改上面數量與下面map!!!
    }
    return textureNumMap;
}

map<int,char*> getTextureFileMap()
{
    static map<int, char*> textrueFileMap;
    if(textrueFileMap.empty())
    {
        textrueFileMap[0] = "image\\mini_knight.bmp";
        textrueFileMap[1] = "image\\groundGrass.bmp";
        textrueFileMap[2] = "image\\dirt.bmp";
        textrueFileMap[3] = "image\\demonTexture.bmp";
        textrueFileMap[4] = "image\\backgroundSky.bmp";
        textrueFileMap[5] = "image\\gameover.bmp";
        textrueFileMap[6] = "image\\win.bmp";
        //記得修改上面數量與上面map!!!
    }
    return textrueFileMap;
}

void generateTexture()
{
    //texture
    glGenTextures(TEXTURE_COUNT, textures);
    for(int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        // Bind to next texture object
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

        int width;
        int height;
        unsigned char *image;                     //放置圖檔，已經不是BMP圖了，是能直接讓OpenGL使用的資料了
        BITMAPINFO bmpinfo;                        //用來存放HEADER資訊

        GLint iWidth, iHeight, iComponents;
        GLenum eFormat;

        //GLbyte *image = gltLoadTGA("bmp\\huge_tree.tga", &iWidth, &iHeight, &iComponents, &eFormat);
        image = TransIntoRGBA(getTextureFileMap()[iLoop], &bmpinfo);

        width = bmpinfo.bmiHeader.biWidth;
        height = bmpinfo.bmiHeader.biHeight;

        //材質控制
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        //使用多材質
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D,4,width,height,GL_RGBA,GL_UNSIGNED_BYTE,image);

    }
}

#endif
