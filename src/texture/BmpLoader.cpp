#include "BmpLoader.h"

BmpLoader::BmpLoader() {
    header = new unsigned char[54];
}

BmpLoader::~BmpLoader() {
    delete[] header;
    delete[] data;
    printf("release image\n");
}

bool BmpLoader::loadBitmap(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        printf("Image %s could not be opened\n", fileName);
        return false;
    }
    if (fread(header, 1, 54, file) != 54) {//�ļ�ͷ����54�ֽ� ��ȡʧ��
        printf("Not a correct BMP file\n");
        return false;
    }
    if (header[0] != 'B' || header[1] != 'M') {//�ļ�ͷ��ͷ����BM ��ȡʧ��
        printf("Not a correct BMP file\n");
        return false;
    }

    dataPos = *(int *) &(header[0x0A]);//��ȡλ�� λ�����ļ�ͷ0x0A��
    imageSize = *(int *) &(header[0x22]);//ͼƬ���ݴ�С���� λ�����ļ�ͷ0x22��
    width = *(int *) &(header[0x12]);//ͼƬ������� λ�����ļ�ͷ0x12��
    height = *(int *) &(header[0x16]);//ͼƬ�߶����� λ�����ļ�ͷ0x16��

    if (imageSize == 0)
        imageSize = width * height * 3;//ͼƬ��������=��������x3
    if (dataPos == 0)
        dataPos = 54;//�ļ�ͷ���� λ����54�ֽڴ�

    data = new unsigned char[imageSize];//data��������Ϣ
    fread(data, 1, imageSize, file);//��ȡ����
    fclose(file);

    for (int i = 0; i < (int) imageSize; i += 3) swapcolor(data[i], data[i + 2]);//bgr��Ϊrgb

    return true;
}

int BmpLoader::getWidth() {
    return width;
}

int BmpLoader::getHeight() {
    return height;
}


