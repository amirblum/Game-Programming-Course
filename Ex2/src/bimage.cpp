#include "bimage.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

BImage::BImage(){
  _width = 0;
  _height = 0;
  _data = NULL ;
  _pdata = NULL ;
}

BImage::~BImage() {
   if (_data!=NULL)
    delete[] _data;
}

BImage::BImage(const BImage& other){
  _width = 0;
  _height = 0;
  _pdata = NULL ;
  _data = NULL ;
  *this=other;
}

BImage::BImage(int width, int height) {
  _width = width;
  _height = height;
  _data = new unsigned char[_width * _height * DEPTH];
  _pdata = (Bpixel*)_data ;
}

BImage::BImage(const char * Image_name) {
  _width = 0;
  _height = 0;
  _pdata = NULL ;
  _data = NULL;
  readImage(Image_name);
}

void BImage::set(int width, int height) {
  _width = width ;
  _height = height ; 

  if(_data != NULL)
    delete[] _data ;

  _data = new  unsigned char[_width * _height * DEPTH] ;
  _pdata = (Bpixel*)_data ;
}

void BImage::flip() {

  if(_data == NULL)
    return ;
  
  int len = _width * _height * 3 ;
  unsigned char* data = new unsigned char[len] ;

  for(int y = 0 ; y < _height ; y++) 
    for(int x = 0 ; x < _width ; x++) {
      int ind = (x + y * _width) * 3 ;
      int iind = (x + (_height - 1 - y) * _width) * 3 ;
      
      data[ind] = _data[iind + 2] ;
      data[ind+1] = _data[iind + 1] ;
      data[ind+2] = _data[iind] ;
    }


  delete[] _data ;

  _data = data ;
  _pdata = (Bpixel*)_data ;
}

int BImage::readImage(const char * Image_name) {
  ifstream f(Image_name,ios::binary);

  if(!f)
    return FAILURE;

  BITMAPFILEHEADER header1;
  BITMAPINFOHEADER header2;
  
  f.read((char*)&header1.bfType,sizeof(header1.bfType)) ;
  f.read((char*)&header1.bfSize,sizeof(header1.bfSize)) ;
  f.read((char*)&header1.bfReserved1,sizeof(header1.bfReserved1)) ;
  f.read((char*)&header1.bfReserved2,sizeof(header1.bfReserved2)) ;
  f.read((char*)&header1.bfOffBits,sizeof(header1.bfOffBits)) ;

 
  f.read((char*)&header2,sizeof(header2));


//   cout << header1.bfSize << endl ;
//   cout <<  header2.biWidth << " " <<  header2.biHeight << "  " <<   header2.biWidth * header2.biHeight * 3 << endl ;
//   cout << header2.biSizeImage << endl ;
  
  // calculate the size (assuming 24 bits or 3 bytes per pixel).
  _height = header2.biHeight ;
  _width = header2.biWidth ;

  long size = _width * _height * 3 ;

  if(_data!=NULL)
    delete[] _data;
  
  _data = new unsigned char[size] ;
  _pdata = (Bpixel*)_data ;
  f.read((char*)_data, size * sizeof(unsigned char));

  flip() ;
//   for (int i = 0 ; i < _width * _height * 3 ; i+=3) { 
//     unsigned char temp = _data[i];
//     _data[i] = _data[i+2];
//     _data[i+2] = temp;
//   }
  
  f.close();
    
  return SUCCESS;
}

int BImage::writeImage(const char * Image_name) {

  if(_data==NULL)
    return FAILURE;
  
  BITMAPFILEHEADER header1;
  BITMAPINFOHEADER header2;
	
  header1.bfType= 256 * 'M' + 'B' ;
  header1.bfSize= _width * _height * 3 + 54 ;
  header1.bfReserved1 = 0 ;
  header1.bfReserved2 = 0 ;	
  header1.bfOffBits = 54 ;	

  header2.biSize = 40 ;
  header2.biHeight = _height ;
  header2.biWidth = _width ;
  header2.biBitCount = 24 ;
  header2.biCompression = 0 ;
  header2.biSizeImage = _height * _width * 3 ;
  header2.biPlanes = 1 ;
  header2.biXPelsPerMeter = 100 ;
  header2.biYPelsPerMeter = 100 ;
  header2.biClrUsed = 0 ;
  header2.biClrImportant = 0 ;
  ofstream f(Image_name,ios::binary);
  
  f.write((char*)&header1.bfType,sizeof(header1.bfType)) ;
  f.write((char*)&header1.bfSize,sizeof(header1.bfSize)) ;
  f.write((char*)&header1.bfReserved1,sizeof(header1.bfReserved1)) ;
  f.write((char*)&header1.bfReserved2,sizeof(header1.bfReserved2)) ;
  f.write((char*)&header1.bfOffBits,sizeof(header1.bfOffBits)) ;
  
  f.write((char*)&header2,sizeof(header2));

  // unsigned char *data = new unsigned char[_width * _height * 3];
  // _pdata = (Bpixel*)_data ;

 //  for (int i = 0 ; i < _width * _height * 3 ; i+=3) { 
//     data[i+2] = _data[i];
//     data[i] = _data[i+2];
//     data[i+1] = _data[i+1];
//   }

  flip() ;
  
  f.write((char*)_data, _width * _height * 3);

  flip() ;
 
  f.close();
	
  return SUCCESS;
}


BImage& BImage::operator=(const BImage& other){
  _width = other._width;
  _height = other._height;
  
  if(_data!=NULL){
    delete[] _data;
    _pdata = NULL ;
    _data = NULL ;
  }
  
  if(other._data!=NULL) {
    long size = _width * _height * DEPTH;
    _data = new unsigned char[size];
    _pdata = (Bpixel*)_data ;
    
    for(long i=0;i<size;i++)
      _data[i] = other._data[i];
  }

  return *this;
}

void BImage::resize(int fact) {
  int new_width, new_height ;
  
  if(fact < 0) {
    fact = - fact ;
     
     new_width = _width / fact ;
     new_height = _height / fact ;

     BImage img(new_width, new_height) ;

     for(int y = 0 ; y < new_height ; y++) {
       for(int x = 0 ; x < new_width ; x++) {
	 double r = 0, g = 0, b = 0 ;
	 
	 for(int dy = 0 ; dy < fact ; dy++)
	   for(int dx = 0 ; dx < fact ; dx++) {
	  	     r += (*this)(x*fact+dx,y*fact+dy).r ;
	     g += (*this)(x*fact+dx,y*fact+dy).g ;
	     b += (*this)(x*fact+dx,y*fact+dy).b ;
	    
	   }

	 r /= (double)(fact * fact) ;
	 g /= (double)(fact * fact) ;
	 b /= (double)(fact * fact) ;
	 
	 img(x,y).r = (uchar)r ; img(x,y).g = (uchar)g ; img(x,y).b = (uchar)b ; 
       }
     }
     (*this) = img ;
  }
  else {
     new_width = _width * fact ;
     new_height = _height * fact ;
     
     BImage img(new_width, new_height) ;
     
     for(int y = 0 ; y < _height ; y++) {
       for(int x = 0 ; x < _width ; x++) {
	 
	 for(int dy = 0 ; dy < fact ; dy++)
	   for(int dx = 0 ; dx < fact ; dx++)
	     img(x * fact + dx, y * fact + dy) = (*this)(x,y) ;
       }
     }

     (*this) = img ;
  }

 
}


void BImage::clear(uchar r, uchar g, uchar b, uchar a) {
  if(_data==NULL)
    return;
  
  long size = _width * _height * DEPTH;

  for(long i=0;i<size;){
    _data[i++] = r;
    _data[i++] = g;
    _data[i++] = b;
    //    _data[i++] = a;
  }
}

uchar BImage::min() {
  uchar m = 255 ;
  int len = _width * _height * DEPTH ;
  
  for(int i = 0 ; i < len ; i++)
    if(_data[i] < m)
      m = _data[i] ;

  return m ;
}

uchar BImage::max() {
  uchar m = 0 ;
  int len = _width * _height * DEPTH ;
  
  for(int i = 0 ; i < len ; i++)
    if(_data[i] > m)
      m = _data[i] ;

  return m ;
}

void BImage::normalize(){
  if(_data==NULL)
    return;
  
  long size = _width * _height * DEPTH;

  double mmax = max() ;
  double mmin = min() ;
  
  if(mmax == mmin)
    return;
  
  double factor = 255.499 / (mmax-mmin) ;
  
  for(long i=0;i<size;i++) {
    _data[i] = (unsigned char)((_data[i]-mmin)*factor);
  }
}

void BImage::invert(){
  if(_data==NULL)
    return;
  
  long size = _width * _height * DEPTH;
  
  for(long i=0;i<size;i++) {
    _data[i] = 255 - _data[i] ;
  }
}


void BImage::blt(BImage& source, int x_src, int y_src,
		 int x_dst, int y_dst, int x_size, int y_size){
  if(_data==NULL)
    return;

   int sx,sy,dx,dy ;
   int w = source.width() ;
   int h = source.height() ;
     
  for(int x=0;x<x_size;x++){
    for(int y=0;y<y_size;y++){
       sx = x_dst+x ;
       sy = y_dst+y ;
       dx = x_src+x ;
       dy = y_src+y ;
       
       if(sx < 0 || sy < 0 || sx >= _width || sy >= _height || dx < 0 || dy < 0 || dx >= w || dy >= h)
	 continue ;
       
      operator()(x_dst+x,y_dst+y)=source(x_src+x,y_src+y);
    }
  }
}
/*
void BImage::resize(int new_width, int new_height, FilterType ft, double blur) {
  if(_data==NULL)
    return ;

  ExceptionInfo ei ;
  
  GetExceptionInfo(&ei) ;
  
  Image *Image = ConstituteImage ( _width, _height, "RGB", CharPixel, _data, &ei) ;

  if(Image==NULL)
    return ;

  GetExceptionInfo(&ei) ;
  
  Image *resized_Image = ResizeImage (Image, new_width, new_height, (FilterTypes)ft, blur, &ei) ;
 
  if(_data)
    delete[] _data ;

  _width = new_width ;
  _height = new_height ;
  
  _data = new unsigned char[_width * _height * DEPTH] ;
  _pdata = (Bpixel*)_data ;
 
  if(_data == NULL)
    return ;

  PixelPacket * pp ;

  pp = GetImagePixels(resized_Image, 0, 0, resized_Image->columns, resized_Image->rows) ;

  for(int i = 0 ; i < _width * _height ; i++) {
    _data[i * DEPTH] = (pp[i].red >> 8) ;
    _data[i * DEPTH + 1] = (pp[i].green >> 8) ;
    _data[i * DEPTH + 2] = (pp[i].blue >> 8);
    //    _data[i * DEPTH + 3] = pp[i].opacity ;
  }
  
  DestroyImage(Image) ;
  DestroyImage(resized_Image) ;
}
*/  
void BImage::show() {
  char filename[20] ;
  char rand_name[9] ;

  for(int i = 0; i < 8 ; i++)
    rand_name[i] = (int)((double)rand() / RAND_MAX * 26) + 'a' ;
  rand_name[8] = 0 ;

  sprintf(filename,"%s.bmp", rand_name);
}
