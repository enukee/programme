#pragma once

#include "ImageMatrix.h"
#define ALING4(x) ((((x)-1)/4+1)*4)

ImageMatrix::ImageMatrix() {
}

ImageMatrix::ImageMatrix(unsigned int height, unsigned int width) {
	img_Height = height;
	img_Width = width;

	matrix = new Pixel<BYTE> *[img_Height];
	for (unsigned int i = 0; i < img_Height; i++) {
		matrix[i] = new Pixel<BYTE>[img_Width];
	}
}

ImageMatrix::~ImageMatrix() {
	for (unsigned int i = 0; i < img_Height; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

// ������ �������
void ImageMatrix::set_pixel(Pixel<BYTE> value, unsigned int i, unsigned int j) {
	matrix[i][j] = value;
}

// ������ ������� (�������� �� �������)
void ImageMatrix::set_pixel(BYTE canal_R, BYTE canal_G, BYTE canal_B, unsigned int i, unsigned int j) {
	matrix[i][j].canal_R = canal_R;
	matrix[i][j].canal_G = canal_G;
	matrix[i][j].canal_B = canal_B;
}

// ������ �������
Pixel<BYTE> ImageMatrix::get_pixel(unsigned int i, unsigned int j) {
	return matrix[i][j];
}

// ������ ������ �������
void ImageMatrix::get_row_matrix(Pixel<BYTE>* matr, unsigned int i) {
	for (unsigned int j = 0; j < img_Width; j++) {
		matr[j] = matrix[i][j];
	}
}

// ������ ������ ������� (�� ������ ������� �� s_1 ������� ������ s_2 �������� ������)
void ImageMatrix::get_row_matrix(Pixel<BYTE>* matr, unsigned int i, unsigned int s_1, unsigned int s_2) {
	for (unsigned int j = 0; j < s_2; j++) {
		matr[j] = matrix[i][s_1 + j];
	}
  }

// ������ ������ �����������
unsigned int ImageMatrix::get_height() {
	return img_Height;
}

// ������ ������ �����������
unsigned int ImageMatrix::get_width() {
	return img_Width;
}

// ��������� �������
void ImageMatrix::zeroing() {
	for (unsigned int i = 0; i < img_Height; i++) {
		for (unsigned int j = 0; j < img_Width; j++) {
			matrix[i][j].zeroing();
		}
	}
}

// ���������� ������� Bitmap � ������� matrix (������� � ������� matrix[x][y])
void ImageMatrix::recording(ImageMatrix* Bitmap, unsigned int y, unsigned int x) {
	for (unsigned int i = 0; i < Bitmap->get_height(); i++) {
		for (unsigned int j = 0; j < Bitmap->get_width(); j++) {
			matrix[i + y][j + x] = Bitmap->matrix[i][j];
		}
	}
}

// ���������� ������� matrix ���������� ������� Bitmap (������� � ������� Bitmap[x][y])
void ImageMatrix::cut_out(ImageMatrix* Bitmap, unsigned int y, unsigned int x) {
	for (unsigned int i = 0; i < img_Height; i++) {
		for (unsigned int j = 0; j < img_Width; j++) {
			matrix[i][j] = (*Bitmap).matrix[i + y][j + x];
		}
	}
}

Pixel<double> ImageMatrix::finding_avg() { // ������� ������ �������� �������� ������� 
	Pixel<double> avg;

	for (unsigned int i = 0; i < img_Height; i++) {
		for (unsigned int j = 0; j < img_Width; j++) {
			avg += matrix[i][j].to_double();
		}
	}
	avg /= img_Height;
	avg /= img_Width;

	return avg;
}

Pixel<double> ImageMatrix::finding_sd() {// ������� ������ c������������������� ���������� ������� 
	Pixel<double> sd;
	Pixel<double> _avg = finding_avg();

	for (unsigned int i = 0; i < img_Height; i++) {
		for (unsigned int j = 0; j < img_Width; j++) {
			sd += ((matrix[i][j].to_double() - _avg) * (matrix[i][j].to_double() - _avg));
		}
	}
	sd /= img_Height;
	sd /= img_Width;
	sd = pixel_sqrt(sd);

	return sd;
}

Pixel<double> ImageMatrix::finding_cor(unsigned int x, unsigned int y, ImageMatrix& matr) {
	unsigned int Height = matr.get_height();
	unsigned int Width = matr.get_width();

	Pixel<double> avg_1 = finding_avg();
	Pixel<double> avg_2 = matr.finding_avg();

	Pixel<double> avg_1_2;
	for (unsigned int i = 0; i < Height; i++) {
		for (unsigned int j = 0; j < Width; j++) {
			avg_1_2 += matrix[i + x][j + y].to_double() * matr.matrix[i][j].to_double();
		}
	}
	avg_1_2 /= Height * Width;

	Pixel<double> sd_1 = finding_sd();
	Pixel<double> sd_2 = matr.finding_sd();

	Pixel<double> kof_kor;
	kof_kor = (avg_1_2 - avg_1 * avg_2) / (sd_1 * sd_2);

	return kof_kor;
}

//����� ������������ ���������� � �������� matr (x � y ������� matr ������ ������ �������)
boolean ImageMatrix::finding_correlation(Pixel<double>& coef_cor, unsigned int x, unsigned int y, ImageMatrix& matr) {
	unsigned int Height = matr.get_height();
	unsigned int Width = matr.get_width();

	// ����������� ���������� ����� ������
	Pixel<double> coef_cor_str;

	class RowOfPixels {
	public:
		Pixel<BYTE>* Bitmap;

		RowOfPixels(unsigned int Width);
		~RowOfPixels();
	};

	ImageString Bitmap_im_1(Width);
	ImageString Bitmap_im_2(Width);

	for (unsigned int t = 0; t < Height; t++) {
		get_row_matrix(Bitmap_im_1.mass, x + t, y, Width);
		matr.get_row_matrix(Bitmap_im_2.mass, t);
		coef_cor_str = Bitmap_im_2.kcor(Bitmap_im_1.mass, Bitmap_im_2.mass, Width);
		coef_cor += coef_cor_str;
		if (((coef_cor_str.canal_R < 0.6) ||
			(coef_cor_str.canal_G < 0.6) ||
			(coef_cor_str.canal_B < 0.6))) {
			return 0;
		}
	}
	coef_cor /= Height;

	return 1;
}