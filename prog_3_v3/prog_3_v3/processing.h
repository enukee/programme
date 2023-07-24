#include <iostream>
#include <thread>
#include <future>
#include "BmpFile.h"

using namespace System::Drawing;

std::mutex mtx;

// проверка
boolean is_bmp_file(char* way) {
	int last_symbol = strlen(way) - 1;
	boolean is_bmp = (way[last_symbol] == 'p');
	is_bmp &= (way[last_symbol - 1] == 'm');
	is_bmp &= (way[last_symbol - 2] == 'b');
	is_bmp &= (way[last_symbol - 3] == '.');
	return is_bmp;
}

void search_inside_img(ImageMatrix* Bitmap_1, ImageMatrix* Bitmap_2, unsigned int a, unsigned int b, bool* similar, coordinates* coord) {
	unsigned int Height = Bitmap_1->get_height();
	unsigned int Width = Bitmap_1->get_width();

	unsigned int search_area_w = Bitmap_2->get_width() - Width + 1;

	// координаты пересечения
	coordinates coord_img;

	// флаг(было найдено хотя бы одно значение корреляции больше максимального)
	bool flag = 0;

	// структура хранения значений коэффициента корреляции для каждого канала
	Pixel<double> coef_cor;
	Pixel<double> max_coef_cor;
	max_coef_cor.canal_R = 0.5;
	max_coef_cor.canal_G = 0.5;
	max_coef_cor.canal_B = 0.5;
	
	for (unsigned int i = a; i < b; i++) {
		for (unsigned int j = 0; j < search_area_w; j++) {
			coef_cor = Bitmap_2->finding_cor(i, j, *Bitmap_1);

			if (*similar) {
				return;
			}

			if ((coef_cor.canal_R > max_coef_cor.canal_R) &&
				(coef_cor.canal_G > max_coef_cor.canal_G) &&
				(coef_cor.canal_B > max_coef_cor.canal_B)) {
				max_coef_cor = coef_cor;
				coord_img.x = j;
				coord_img.y = i;
				flag = 1;
			}

			if ((coef_cor.canal_R > 0.8) &&
				(coef_cor.canal_G > 0.8) &&
				(coef_cor.canal_B > 0.8)) {
				*similar = 1;
				*coord = coord_img;
				return;
			}
		}
	}

	*similar = flag;
	*coord = coord_img;
	return;
}

// метод поиска пересечения 2х изображений (поиск img_1 в img_2)
boolean finding_intersection(BmpFile* img_1, BmpFile* img_2, coordinates* coord_img_1, coordinates* coord_img_2, prog3v3::MyForm^ mb) {
	// Создаём матрицу части изображения img_1
	ImageMatrix Bitmap_1(coord_img_1->height, coord_img_1->width);
	// Записываем в неё нужный фрагмент
	Bitmap_1.cut_out(img_1, coord_img_1->y, coord_img_1->x);
	mb->increasing_value(4);

	// Создаём матрицу части изображения img_2
	ImageMatrix Bitmap_2(coord_img_2->height, coord_img_2->width);
	// Записываем в неё нужный фрагмент
	Bitmap_2.cut_out(img_2, coord_img_2->y, coord_img_2->x);
	mb->increasing_value(4);

	unsigned int Height = Bitmap_1.get_height();

	unsigned int search_area_h = Bitmap_2.get_height() - Height + 1;

	bool similar = 0;

	auto f1 = std::async(std::launch::async, search_inside_img, &Bitmap_1, &Bitmap_2,
		0, search_area_h / 2, &similar, coord_img_2);

	f1 = std::async(std::launch::async, search_inside_img, &Bitmap_1, &Bitmap_2,
		search_area_h / 2, search_area_h, &similar, coord_img_2);

	f1.get();

	return similar;
}

//метод объединения изображений
ImageMatrix* combining(BmpFile* img_1, BmpFile* img_2, coordinates coord_img_1, coordinates coord_img_2, prog3v3::MyForm^ mb) {
	unsigned int img_1_height = img_1->get_height();
	unsigned int img_1_width = img_1->get_width();

	unsigned int img_2_height = img_2->get_height();
	unsigned int img_2_width = img_2->get_width();

	unsigned int Height;
	unsigned int Width;

	coordinates coord_general_image_1;
	coordinates coord_general_image_2;

	if (coord_img_1.x < coord_img_2.x) {
		coord_general_image_1.x = coord_img_2.x - coord_img_1.x;
		coord_general_image_2.x = 0;
		Width = img_1->get_width() + coord_general_image_1.x;
	}

	if (coord_img_1.x > coord_img_2.x) {
		coord_general_image_1.x = 0; 
		coord_general_image_2.x = coord_img_1.x - coord_img_2.x;
		Width = img_2->get_width() + coord_general_image_2.x;
	}

	if (coord_img_1.y < coord_img_2.y) {
		coord_general_image_1.y = coord_img_2.y - coord_img_1.y;
		coord_general_image_2.y = 0;
		Height = img_1->get_height() + coord_general_image_1.y;
	}

	if (coord_img_1.y > coord_img_2.y) {
		coord_general_image_1.y = 0;
		coord_general_image_2.y = coord_img_1.y - coord_img_2.y;
		Height = img_2->get_height() + coord_general_image_2.y;
	}

	ImageMatrix* Bitmap = new ImageMatrix(Height, Width);

	Bitmap->zeroing();

	Bitmap->recording(img_2, coord_general_image_2.y, coord_general_image_2.x);
	mb->increasing_value(6);
	Bitmap->recording(img_1, coord_general_image_1.y, coord_general_image_1.x);
	mb->increasing_value(14);

	return Bitmap;
}

//сохранение изображения
void save_img(ImageMatrix* matrix) {
	BmpFile img_o(matrix);
	img_o.bmp_writer((char*)"D:\\GitHub_rep\\foton\\pro_3_v2\\image\\п_4\\image.bmp");
}

// коррекция пикселя
Pixel<BYTE> color_correction(Pixel<double> pixel, Pixel<double>sco, Pixel<double>mo) {
	Pixel<double> value;
	value = pixel * sco + mo;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

	Pixel<BYTE> pixel_1;
	pixel_1 = value.to_BYTE();

	return pixel_1;
}

// метод корректирования яркости
System::String^ brightness_correction(BmpFile* img_1, BmpFile* img_2, coordinates coord_img_1, coordinates coord_img_2, prog3v3::MyForm^ form) {

	// создание матрицы части пересечения первого изображения
	ImageMatrix Bitmap_1(coord_img_1.height, coord_img_1.width);

	// заполнения первой матрицы 
	Bitmap_1.cut_out(img_1, coord_img_1.y, coord_img_1.x);
	form->increasing_value(4);

	// создание матрицы части пересечения второго изображения
	ImageMatrix Bitmap_2(coord_img_2.height, coord_img_2.width);

	// заполнения второй матрицы 
	Bitmap_2.cut_out(img_2, coord_img_2.y, coord_img_2.x);
	form->increasing_value(4);

	Pixel<double> sco_1 = Bitmap_1.finding_sd();
	Pixel<double> sco_2 = Bitmap_2.finding_sd();
	Pixel<double> mo_1 = Bitmap_1.finding_avg();
	Pixel<double> mo_2 = Bitmap_2.finding_avg();

	System::String^ info = "Изображение 1";
	info += "\r\n Математическое ожидание";
	info += "\r\n    канал R: " + mo_1.canal_R;
	info += "\r\n    канал G: " + mo_1.canal_G;
	info += "\r\n    канал B: " + mo_1.canal_B;

	info += "\r\n Среднеквадратичное отклонение";
	info += "\r\n    канал R: " + sco_1.canal_R;
	info += "\r\n    канал G: " + sco_1.canal_G;
	info += "\r\n    канал B: " + sco_1.canal_B;

	info += "\r\n Изображение 2";
	info += "\r\n Математическое ожидание";
	info += "\r\n    канал R: " + mo_2.canal_R;
	info += "\r\n    канал G: " + mo_2.canal_G;
	info += "\r\n    канал B: " + mo_2.canal_B;

	info += "\r\n Среднеквадратичное отклонение";
	info += "\r\n    канал R: " + sco_2.canal_R;
	info += "\r\n    канал G: " + sco_2.canal_G;
	info += "\r\n    канал B: " + sco_2.canal_B;

	info += "\r\nКоордината 1:\r\n x: " + coord_img_1.x + "   y: " + coord_img_1.y;
	info += "\r\nКоордината 2:\r\n x: " + coord_img_2.x + "   y: " + coord_img_2.y;

	Pixel<double> sco;
	Pixel<double> mo;

	if (((sco_1.canal_R > -0.5) && (sco_1.canal_R < 0.5)) || ((sco_2.canal_R > -0.5) && (sco_2.canal_R < 0.5))) {
		sco.canal_R = 1;
	}
	else {
		sco.canal_R = sco_1.canal_R / sco_2.canal_R;
	}

	if (((sco_1.canal_G > -0.5) && (sco_1.canal_G < 0.5)) || ((sco_2.canal_G > -0.5) && (sco_2.canal_G < 0.5))) {
		sco.canal_G = 1;
	}
	else {
		sco.canal_G = sco_1.canal_G / sco_2.canal_G;
	}

	if (((sco_1.canal_B > -0.5) && (sco_1.canal_B < 0.5)) || ((sco_2.canal_B > -0.5) && (sco_2.canal_B < 0.5))) {
		sco.canal_B = 1;
	}
	else {
		sco.canal_B = sco_1.canal_B / sco_2.canal_B;
	}

	mo = mo_1 - (mo_2 * sco);

	Pixel<BYTE> byte;
	unsigned int height = img_2->get_height();
	unsigned int width = img_2->get_width();

	double value_progressbar = form->getProgressBarValue();
	double step = (double)12 / (double)height;

	for (unsigned int i = 0; i < height; i++) {
		value_progressbar += step;
		form->set_value((int)round(value_progressbar));

		for (unsigned int j = 0; j < width; j++) {
			byte = img_2->get_pixel(i, j);
			byte = color_correction(byte.to_double(), sco, mo);
			img_2->set_pixel(byte, i, j);
		}
	}

	return info;
}
