#pragma once

typedef struct Image
{
	int width;
	int height;
	int channel;
	unsigned char* data;
}Image;

typedef struct Point
{
	int x;
	int y;
}Point;

void freeImage(Image* img);
Image* createImage(const int width, const int height, const int channels);
Image* cloneImage(Image* src);
Image* readPXM(const char* name);
void writePXM(const char* name, Image* src);
void cvtColorGray(Image* src, Image* dest);

void drawRectangle(Image* src, Point pt, const int template_width, const int template_height);

char* getBaseName(char* name);
char* getDirAndBaseName(char* name);

void writeResult(char* file_name, char* template_name, Point result, double distance);
void clearResult(char* file_name);