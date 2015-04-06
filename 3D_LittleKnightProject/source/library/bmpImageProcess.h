#ifndef bmpImageProcess_h
#define bmpImageProcess_h

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char *LoadBitmapFile(char *fileName, BITMAPINFO *bitmapInfo)
{
   FILE            *fp;
   BITMAPFILEHEADER   bitmapFileHeader;   // Bitmap file header
   unsigned char       *bitmapImage;      // Bitmap image data
   unsigned int      lInfoSize;         // Size of information
   unsigned int      lBitSize;         // Size of bitmap

   unsigned char change;
    int pixel;
    int p=0;

   fp = fopen(fileName, "rb");
   fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);         //Ū�� bitmap header

   lInfoSize = bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);   //Info��size
   fread(bitmapInfo, lInfoSize, 1, fp);


   lBitSize = bitmapInfo->bmiHeader.biSizeImage;                  //�t�m�O����
   bitmapImage = new BYTE[lBitSize];
   fread(bitmapImage, 1, lBitSize, fp);                        //Ū���v����

   fclose(fp);

   //���ɶǦ^bitmapImage���ܡA�C��|�OBGR���ǡA�U���j��|�ﶶ�Ǭ�RGB
   pixel = (bitmapInfo->bmiHeader.biWidth)*(bitmapInfo->bmiHeader.biHeight);

   for( int i=0 ; i<pixel ; i++, p+=3 )
   {
      //�洫bitmapImage[p]�MbitmapImage[p+2]����
      change = bitmapImage[p];
      bitmapImage[p] = bitmapImage[p+2];
      bitmapImage[p+2]  = change;
   }

   return bitmapImage;
}

unsigned char *TransIntoRGBA(char *fileName, BITMAPINFO *bitmapInfo)
{
   unsigned char *rgb;         //�x�s��qbmp���Ӫ�RGB��
   unsigned char *rgba;      //�x�s�̫᧹����RGBA�Ϩæ^��
   int x,y;

   unsigned char *rgb_ptr;
   unsigned char *rgba_ptr;
   rgb = LoadBitmapFile(fileName, bitmapInfo);
   rgba= (unsigned char *)malloc(bitmapInfo->bmiHeader.biWidth*bitmapInfo->bmiHeader.biHeight*4*sizeof(unsigned char));

   for( y=0 ; y<bitmapInfo->bmiHeader.biWidth ; y++ )
   {
      rgb_ptr = rgb+y*bitmapInfo->bmiHeader.biWidth*3;
      rgba_ptr= rgba+y*bitmapInfo->bmiHeader.biWidth*4;

      for( x=0 ; x<bitmapInfo->bmiHeader.biWidth ; x++, rgb_ptr+=3, rgba_ptr+=4 )
      {
         rgba_ptr[0]=rgb_ptr[0];
         rgba_ptr[1]=rgb_ptr[1];
         rgba_ptr[2]=rgb_ptr[2];
         rgba_ptr[3]=0xff;//�z����
         //if(rgb_ptr[0]==rgb_ptr[1] && rgb_ptr[0]==rgb_ptr[2] && abs(rgb_ptr[0]-32)<30)
         //{
             //rgba_ptr[3]=0x0;
         //}
      }
   }
   free(rgb);
   return rgba;
}

#endif
