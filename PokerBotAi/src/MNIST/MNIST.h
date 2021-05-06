#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief see: https://stackoverflow.com/questions/8286668/how-to-read-mnist-data-in-c
 * 
 */


unsigned char** read_mnist_images(string full_path, int& number_of_images, int& image_size);

unsigned char* read_mnist_labels(string full_path, int& number_of_labels);

void printImg(ostream& out, unsigned char** images, const int imgSize, const int imgNum);

vector<float> charArrImgToVector(unsigned char* img, const int imgSize);

vector<vector<float>> charArrImgsToVectors(unsigned char** imgs, const int imgSize, const int fromImgIdx, const int toImgIdx);

vector<vector<float>> charArrLabelsToVectors(unsigned char* labels, const int fromLabelIdx, const int toLabelIdx);
