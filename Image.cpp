//
// Created by floodd on 26/01/2026.
//

#include "Image.h"
#include <algorithm>
#include <iostream>

MyImage::MyImage(string filename) : fileName(filename) {
    this->load(filename);
}

void MyImage::reload() {
    this->load(fileName);
}

void MyImage::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    float x = 0, y = 0;
    int type = this->size.x > this->size.y ? LANDSCAPE : PORTRAIT;
    float step = 1;
    float startX = this->position.x, startY = this->position.y;
    float dimW = this->targetSize.x;
    float dimH = this->targetSize.y;

    if (this->size.x < this->targetSize.x && this->size.y < this->targetSize.y) {

        dimW = this->size.x;
        startY += (this->targetSize.y - this->size.y) / 2.0f;
        dimH = this->size.y;
        startX += (this->targetSize.x - this->size.x) / 2.0f;
    } else {
        if (this->size.x / this->targetSize.x > this->size.y / this->targetSize.y) {
            step = this->size.x / this->targetSize.x;
            dimH = this->size.y / step;
            startY += (this->targetSize.y - dimH) / 2;
        } else {
            step = this->size.y / (float) this->targetSize.y;
            dimW = this->size.x / step;
            startX += (this->targetSize.x - dimW) / 2;
        }
    }

    float xStep = 0, yStep = 0;
    for (float i = 0; i < dimH; ++i) {
        for (float j = 0; j < dimW; ++j) {
            x = (int) xStep;
            y = (int) yStep;
            int pix = (y * this->size.x) + x;

            sf::VertexArray circle(sf::PrimitiveType::Points, 1);
            circle[0].color = sf::Color(this->pixels[pix].r, this->pixels[pix].g, this->pixels[pix].b);
            circle[0].position = {j + startX, i + startY};

            target.draw(circle);
            xStep += step;
        }
        xStep = 0;
        yStep += step;
    }
}

void MyImage::setTargetSize(sf::Vector2f target) {
    this->targetSize = target;
}

sf::Vector2f MyImage::getSize() {
    return this->size;
}

void MyImage::load(string filename) {
    ifstream ifs;

    string dummy;
    ifs.open(filename, std::ios::binary);
    try {
        if (ifs.fail()) { throw ("Can't open input file"); }
        std::string header;
        int w, h, b;

        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw ("Can't read input file");
        getline(ifs, dummy);

        char c;
        c = ifs.peek();
        if (c == '#') {
            getline(ifs, dummy);
        }

        ifs >> w >> h >> b;
        this->size = {(float) w, (float) h};

        ifs.ignore(256, '\n');
        unsigned char pix[3];
        this->pixels.clear();

        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            RGB pixel;
            pixel.r = pix[0];
            pixel.g = pix[1];
            pixel.b = pix[2];
            this->pixels.push_back(pixel);
        }

        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
}

void MyImage::save() {
    string filename = "Output/SavedImage.ppm";
    if (this->size.x == 0 || this->size.y == 0) {
        fprintf(stderr, "Can't save an empty image\n");
        return;
    }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail()) throw ("Can't open output file");
        ofs << "P6\n" << this->size.x << " " << this->size.y << "\n255\n";

        unsigned char r, g, b;
        for (int i = 0; i < this->size.x * this->size.y; ++i) {
            r = static_cast<unsigned char>(this->pixels[i].r);
            g = static_cast<unsigned char>(this->pixels[i].g);
            b = static_cast<unsigned char>(this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

void MyImage::filterRed() {
    cout << "Filter Red" << endl;
    for (int i = 0; i < this->pixels.size(); ++i) {
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }
}

void MyImage::filterGreen() {
    cout << "Filter Green" << endl;
    for (int i = 0; i < this->pixels.size(); ++i) {
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }
}

void MyImage::filterBlue() {
    cout << "Filter Blue" << endl;
    for (int i = 0; i < this->pixels.size(); ++i) {
        this->pixels[i].g = 0;
        this->pixels[i].r = 0;
    }
}

void MyImage::greyScale() {
    cout << "Filter Greyscale" << endl;
    for (int i = 0; i < this->pixels.size(); ++i) {
        float avg = (this->pixels[i].r + this->pixels[i].g + this->pixels[i].b) / 3;
        this->pixels[i].r = avg;
        this->pixels[i].g = avg;
        this->pixels[i].b = avg;
    }
}

void MyImage::flipHorizontal() {
    cout << "Flip Horizontal" << endl;
    int width = this->size.x;
    int height = this->size.y;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            int leftIndex = (i * width) + j;
            int rightIndex = (i * width) + (width - j - 1);
            RGB temp = this->pixels[leftIndex];
            this->pixels[leftIndex] = this->pixels[rightIndex];
            this->pixels[rightIndex] = temp;
        }
    }
}

void MyImage::flipVertical() {
    cout << "Flip Vertical" << endl;
    int width = this->size.x;
    int height = this->size.y;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height / 2; ++j) {
            int topIndex = (j * width) + i;
            int bottomIndex = ((height - 1 - j) * width) + i;
            RGB temp = this->pixels[topIndex];
            this->pixels[topIndex] = this->pixels[bottomIndex];
            this->pixels[bottomIndex] = temp;
        }
    }
}

void MyImage::advancedFeature1() {
    cout << "Rotate 90" << endl;

    int width = static_cast<int>(this->size.x);
    int height = static_cast<int>(this->size.y);
    if (width <= 0 || height <= 0) return;

    vector<RGB> rotated;
    rotated.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int newX = height - 1 - y;
            int newY = x;

            int oldIndex = y * width + x;
            int newIndex = newY * height + newX;

            rotated[newIndex] = this->pixels[oldIndex];
        }
    }

    this->pixels = std::move(rotated);
    this->size = {static_cast<float>(height), static_cast<float>(width)};
}

void MyImage::advancedFeature2() {
    cout << "Advanced Feature 2: Mirror Image" << endl;
    int width = this->size.x;
    int height = this->size.y;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            int leftIndex = (i * width) + j;
            int rightIndex = (i * width) + (width - j - 1);
            this->pixels[leftIndex] = this->pixels[rightIndex];
        }
    }
}

void MyImage::advancedFeature3() {
    cout << "Advanced Feature 3: Oil Painting" << endl;

    const int radius = 5;
    const int intensityLevels = 20;

    int width = static_cast<int>(this->size.x);
    int height = static_cast<int>(this->size.y);
    if (width <= 0 || height <= 0) return;

    vector<RGB> out(this->pixels.size());

    vector<int> intensityCount(intensityLevels);
    vector<int> sumR(intensityLevels);
    vector<int> sumG(intensityLevels);
    vector<int> sumB(intensityLevels);

    auto clampInt = [](int v, int lo, int hi) {
        return std::max(lo, std::min(v, hi));
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            std::fill(intensityCount.begin(), intensityCount.end(), 0);
            std::fill(sumR.begin(), sumR.end(), 0);
            std::fill(sumG.begin(), sumG.end(), 0);
            std::fill(sumB.begin(), sumB.end(), 0);

            int y0 = std::max(0, y - radius);
            int y1 = std::min(height - 1, y + radius);
            int x0 = std::max(0, x - radius);
            int x1 = std::min(width - 1, x + radius);

            for (int ny = y0; ny <= y1; ++ny) {
                for (int nx = x0; nx <= x1; ++nx) {

                    int index = ny * width + nx;
                    RGB p = this->pixels[index];

                    double avg = (static_cast<double>(p.r) + p.g + p.b) / 3.0;
                    int bin = static_cast<int>((avg * intensityLevels) / 255.0);
                    bin = clampInt(bin, 0, intensityLevels - 1);

                    intensityCount[bin]++;
                    sumR[bin] += p.r;
                    sumG[bin] += p.g;
                    sumB[bin] += p.b;
                }
            }

            int maxIndex = 0;
            int curMax = intensityCount[0];
            for (int i = 1; i < intensityLevels; ++i) {
                if (intensityCount[i] > curMax) {
                    curMax = intensityCount[i];
                    maxIndex = i;
                }
            }

            int outIndex = y * width + x;
            if (curMax > 0) {
                RGB result;
                result.r = static_cast<unsigned char>(sumR[maxIndex] / curMax);
                result.g = static_cast<unsigned char>(sumG[maxIndex] / curMax);
                result.b = static_cast<unsigned char>(sumB[maxIndex] / curMax);
                out[outIndex] = result;
            } else {
                out[outIndex] = this->pixels[outIndex];
            }
        }
    }

    this->pixels = std::move(out);
}