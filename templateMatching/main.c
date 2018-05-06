#include "imageUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void templateMatchingGray(Image* src, Image* template, Point* position, double* distance)
{
	if (src->channel != 1 || template->channel != 1)
	{
		fprintf(stderr, "src and/or templeta image is not a gray image.\n");
		return;
	}

	int min_distance = INT_MAX;
	int ret_x = 0;
	int ret_y = 0;
	int x, y, i, j;
	for (y = 0; y < (src->height - template->height); y++)
	{
		for (x = 0; x < src->width - template->width; x++)
		{
			int distance = 0;
			//SSD
			for (j = 0; j < template->height; j++)
			{
				for (i = 0; i < template->width; i++)
				{
					int v = (src->data[(y + j)*src->width + (x + i)] - template->data[j*template->width + i]);
					distance += v*v;
				}
			}
			if (distance < min_distance)
			{
				min_distance = distance;
				ret_x = x;
				ret_y = y;
			}
		}
	}

	position->x = ret_x;
	position->y = ret_y;
	*distance = sqrt(min_distance) / (template->width*template->height);
}

void templateMatchingColor(Image* src, Image* template, Point* position, double* distance)
{
	if (src->channel != 3 || template->channel != 3)
	{
		fprintf(stderr, "src and/or templeta image is not a color image.\n");
		return;
	}

	int min_distance = INT_MAX;
	int ret_x = 0;
	int ret_y = 0;
	int x, y, i, j;
	for (y = 0; y < (src->height - template->height); y++)
	{
		for (x = 0; x < src->width - template->width; x++)
		{
			int distance = 0;
			//SSD
			for (j = 0; j < template->height; j++)
			{
				for (i = 0; i < template->width; i++)
				{
					int pt = 3 * ((y + j)*src->width + (x + i));
					int pt2 = 3 * (j*template->width + i);
					int r = (src->data[pt + 0] - template->data[pt2 + 0]);
					int g = (src->data[pt + 1] - template->data[pt2 + 1]);
					int b = (src->data[pt + 2] - template->data[pt2 + 2]);

					distance += (r*r + g*g + b*b);
				}
			}
			if (distance < min_distance)
			{
				min_distance = distance;
				ret_x = x;
				ret_y = y;
			}
		}
	}

	position->x = ret_x;
	position->y = ret_y;
	*distance = sqrt(min_distance) / (template->width*template->height);
}

//templateMatching test/beach0.ppm template/ocean_beach_kinzokutanchi.ppm 1.0 cwp
int main(int argc, char** argv)
{
	if (argc < 4)
	{
		fprintf(stderr, "Usage: templateMatching src_image temlate_image threshold option(c,w,p,g)\n");
		fprintf(stderr, "Option:\nc) clear a txt result. \nw) write result a image with rectangle.\np) print results.\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0  \n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 c\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 w\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 p\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 g\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 cw\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 cwp\n");
		fprintf(stderr, "ex: templateMatching src_image.ppm temlate_image.ppm 1.0 cwpg\n");
		return -1;
	}

	char* input_file = argv[1];
	char* template_file = argv[2];
	double threshold = atof(argv[3]);

	char output_name_base[256];
	char output_name_txt[256];
	char output_name_img[256];
	strcpy(output_name_base, "result/");
	strcat(output_name_base, getBaseName(input_file));
	strcpy(output_name_txt, output_name_base);
	strcat(output_name_txt, ".txt");

	int isWriteImageResult = 0;
	int isPrintResult = 0;
	int isGray = 0;
	if (argc == 5)
	{
		char *p = NULL;
		if (p = strchr(argv[4], 'c') != NULL) clearResult(output_name_txt);
		if (p = strchr(argv[4], 'w') != NULL) isWriteImageResult = 1;
		if (p = strchr(argv[4], 'p') != NULL) isPrintResult = 1;
		if (p = strchr(argv[4], 'g') != NULL) isGray = 1;
	}

	Image* img = readPXM(input_file);
	Image* template = readPXM(template_file);

	Point result;
	double distance = 0.0;

	if (isGray &&img->channel==3)
	{
		Image* img_gray = createImage(img->width, img->height, 1);
		Image* template_gray = createImage(template->width, template->height, 1);
		cvtColorGray(img, img_gray);
		cvtColorGray(template, template_gray);

		templateMatchingGray(img_gray, template_gray, &result, &distance);

		freeImage(img_gray);
		freeImage(template_gray);
	}
	else
	{
		templateMatchingColor(img, template, &result, &distance);
	}

	if (distance < threshold)
	{
		writeResult(output_name_txt, getBaseName(template_file), result, distance);
		if (isPrintResult)
		{
			printf("[Found    ] %s %d %d %f\n", getBaseName(template_file), result.x, result.y, distance);
		}
	}
	else
	{
		if (isPrintResult)
		{
			printf("[Not found] %s %d %d %f\n", getBaseName(template_file), result.x, result.y, distance);
		}
	}

	if (isWriteImageResult)
	{
		drawRectangle(img, result, template->width, template->height);
		
		if (img->channel == 3) strcat(output_name_img, ".ppm");
		else if (img->channel == 1)strcat(output_name_img, ".pgm");

		writePXM(output_name_img, img);
	}

	freeImage(img);
	freeImage(template);

	return 0;
}
