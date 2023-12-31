#pragma once

#include "pixel.h"
#include "ImageString.h"

class ImageMatrix {
protected:
	unsigned int img_Height = 0;                    // ������ �����������
	unsigned int img_Width = 0;                     // ������ �����������

	Pixel<BYTE>** matrix;       //������� ����������� 

	ImageMatrix();

public:
	ImageMatrix(unsigned int height, unsigned int width);

	~ImageMatrix();

	
	void set_pixel(Pixel<BYTE> value, unsigned int i, unsigned int j);
	void set_pixel(BYTE canal_R, BYTE canal_G, BYTE canal_B, unsigned int i, unsigned int j);

	Pixel<BYTE> get_pixel(unsigned int i, unsigned int j);
	void get_row_matrix(Pixel<BYTE>* matr, unsigned int i);
	void get_row_matrix(Pixel<BYTE>* matr, unsigned int i, unsigned int y, unsigned int x);

	// ������ ������ �����������
	unsigned int get_height();
	// ������ ������ �����������
	unsigned int get_width();

	// ��������� �������
	void zeroing();

	// ���������� ������� Bitmap � ������� matrix (������� � ������� matrix[x][y])
	void recording(ImageMatrix* Bitmap, unsigned int y, unsigned int x);

	// ���������� ������� matrix ���������� ������� Bitmap (������� � ������� Bitmap[x][y])
	void cut_out(ImageMatrix* Bitmap, unsigned int y, unsigned int x);

	// ������� ������ �������� �������� ������� 
	Pixel<double> finding_avg();
	// ������� ������ c������������������� ���������� ������� 
	Pixel<double> finding_sd();

	// ������� ������ �������� �������� ����� �������
	Pixel<double> finding_avg(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	// ������� ������ c������������������� ���������� ����� ������� 
	Pixel<double> finding_sd(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

	Pixel<double> finding_cor(unsigned int x, unsigned int y, ImageMatrix& matr);

	//����� ������������ ���������� � �������� 
	boolean finding_correlation(Pixel<double>& coef_cor, unsigned int x, unsigned int y, ImageMatrix& matr);
};