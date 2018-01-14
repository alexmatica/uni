#include "lodepng.h"
#include <iostream>
#include <thread>

struct raw_image {
	std::vector<unsigned char> img;
	unsigned width, height;
};

struct image {
	std::vector<std::vector<float>>red;
	std::vector<std::vector<float>>green;
	std::vector<std::vector<float>>blue;
	std::vector<std::vector<float>>alpha;
};

struct acumulator {
	float red = 0;
	float green = 0;
	float blue = 0;
};

float filter[3][3] = { {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0 }, { 2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0 }, { 1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0 } };
image newImage;
image img;
int height;
int width;
int numberOfThreads;
const char* filename;
const char* nrOfThreads;

raw_image decodeOneStep(const char* filename){
	std::cout << "Number of threads:" << numberOfThreads << std::endl;
	std::cout << "Loading image " << filename << "..." << std::endl;
	std::vector<unsigned char> img; //the raw pixels
	unsigned width, height;
	unsigned error = lodepng::decode(img, width, height, filename);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	std::cout << "Image loaded!" << std::endl;
	return raw_image{ img, width, height };
}

void encodeOneStep(const char* filename,raw_image inputImg){
	std::cout << "Writing image..."<<std::endl;
	unsigned error = lodepng::encode(filename, inputImg.img, inputImg.width, inputImg.height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	std::cout << "Image written!" << std::endl;;
}

image raw_to_image(raw_image img) {
	image new_image;
	int k = 0;
	for (int i = 0; i <(signed)img.height; i++) {
		new_image.red.push_back(std::vector<float>());
		new_image.green.push_back(std::vector<float>());
		new_image.blue.push_back(std::vector<float>());
		new_image.alpha.push_back(std::vector<float>());

		for (int j = 0; j < (signed)img.width; j++) {
			new_image.red[i].push_back(img.img[k]);
			new_image.green[i].push_back(img.img[k + 1]);
			new_image.blue[i].push_back(img.img[k + 2]);
			new_image.alpha[i].push_back(img.img[k + 3]);
			k += 4;
		}
	}
	return new_image;
}

raw_image image_to_raw(image img) {
	raw_image new_image;
	new_image.height = img.red.size();
	new_image.width = img.red[0].size();
	for (int i = 0; i < (signed)img.red.size(); i++){
		for (int j = 0; j < (signed)img.red[0].size(); j++) {
			new_image.img.push_back((unsigned char)img.red[i][j]);
			new_image.img.push_back((unsigned char)img.green[i][j]);
			new_image.img.push_back((unsigned char)img.blue[i][j]);
			new_image.img.push_back((unsigned char)img.alpha[i][j]);
		}
	}
	return new_image;
}

void worker(int start, int stop) {
	int i = 0;
	int j = 0;
	for (int k = start; k < stop; k++){
		i = k / height;
		j = k % width;
		if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
			continue;
		acumulator a;
		for (int h = -1; h < 2; h++) {
			for (int w = -1; w < 2; w++) {
				a.red += img.red[i + h][j + w] * filter[h + 1][w + 1];
				a.green += img.green[i + h][j + w] * filter[h + 1][w + 1];
				a.blue += img.blue[i + h][j + w] * filter[h + 1][w + 1];
			}
		}
		newImage.red[i-1][j-1] = a.red;
		newImage.green[i-1][j-1]= a.green;
		newImage.blue[i-1][j-1] = a.blue;
		newImage.alpha[i-1][j-1] = img.alpha[i][j];
	}
}

image applyFilter(raw_image inputImage) {
	std::cout << "Processing image.." << std::endl;
	img = raw_to_image(inputImage);
	height = img.red.size();
	width = img.red[0].size();

	for (int i = 0; i < height - 2; i++) {
		newImage.red.push_back(std::vector<float>(width - 2));
		newImage.green.push_back(std::vector<float>(width - 2));
		newImage.blue.push_back(std::vector<float>(width - 2));
		newImage.alpha.push_back(std::vector<float>(width - 2));
	}
	int size = inputImage.height * inputImage.width;
	std::vector<std::thread> threads;
	for (int i = 0; i < numberOfThreads; i++) {
		threads.push_back(std::thread(worker, size / numberOfThreads * i, size / numberOfThreads * (i + 1)));
	}
	for (int i = 0; i < numberOfThreads; i++) {
		threads[i].join();
	}
	std::cout << "Image processed!" << std::endl;
	return newImage;
}

int main(int argc, char *argv[]){
	if (argc < 2) {
		filename = "lenna.png";
		nrOfThreads = "8";
	}
	else {
		filename = argv[1];
		nrOfThreads = argv[2];
	}
	numberOfThreads = std::stoi(nrOfThreads);
	raw_image img;
	image img2;
	img = decodeOneStep(filename);
	img2 = applyFilter(img);
	encodeOneStep("output.png", image_to_raw(img2));
}
