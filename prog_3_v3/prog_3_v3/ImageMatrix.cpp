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

	//return matr;
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

// ���������� ������� Bitmap � ������� matrix (������� � ������� matrix[x][y])
void ImageMatrix::recording(ImageMatrix* Bitmap, unsigned int y, unsigned int x) {
	for (unsigned int i = 0; i < Bitmap->get_height(); i++) {
		for (unsigned int j = 0; j < Bitmap->get_width(); j++) {
			matrix[i + y][j + x] = Bitmap->matrix[i][j];
		}
	}
}

// ��������� ������� mdtrix ���������� ������� Bitmap (������� � ������� Bitmap[x][y])
void ImageMatrix::cut_out(ImageMatrix* Bitmap, unsigned int y, unsigned int x) {
	for (unsigned int i = 0; i < img_Height; i++) {
		for (unsigned int j = 0; j < img_Width; j++) {
			matrix[i][j] = (*Bitmap).matrix[i + y][j + x];
		}
	}
}

Pixel<double> ImageMatrix::avg() { // ������� ������ �������� �������� ������� 
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

Pixel<double> ImageMatrix::sd() {// ������� ������ c������������������� ���������� ������� 
	Pixel<double> sd;
	Pixel<double> _avg = avg();

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
