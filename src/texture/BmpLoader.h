#include <stdio.h>

#ifndef _BMPLOADER_H_
#define _BMPLOADER_H_

#define BLUE 0
#define GREEN 1
#define RED 2

#define swapcolor(a, b){ \
        (a) ^= (b);     \
        (b) ^= (a);     \
        (a) ^= (b);     \
}

class BmpLoader {
private:
    unsigned char *header;//�ļ�ͷ
    unsigned int dataPos;//��ȡλ��
    unsigned int imageSize;//ͼƬ���ݴ�С

public:
    unsigned int width, height;//ͼƬ��� �߶�
    unsigned char *data;//ͼƬ���� rgb
    BmpLoader();

    ~BmpLoader();

    int getWidth();

    int getHeight();

    bool loadBitmap(const char *fileName);
};

#endif
