#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float PI = 3.14159265359;
#define DCT_H 8
#define DCT_W 8
#pragma pack(2)

// Function to write the BMP header
void writeBMPHeader(FILE *fp, int width, int height)
{
    /*construct a structure of BMP header*/
    typedef struct
    {
        char identifier[2];      // 0x0000
        unsigned int filesize;   // 0x0002
        unsigned short reserved; // 0x0006
        unsigned short reserved2;
        unsigned int bitmap_dataoffset; // 0x000A
        unsigned int bitmap_headersize; // 0x000E
        unsigned int width;             // 0x0012
        unsigned int height;            // 0x0016
        unsigned short planes;          // 0x001A
        unsigned short bits_perpixel;   // 0x001C
        unsigned int compression;       // 0x001E
        unsigned int bitmap_datasize;   // 0x0022
        unsigned int hresolution;       // 0x0026
        unsigned int vresolution;       // 0x002A
        unsigned int usedcolors;        // 0x002E
        unsigned int importantcolors;   // 0x0032
    } Bmpheader;

    Bmpheader header;
    header.identifier[0] = 'B';
    header.identifier[1] = 'M';
    header.filesize = 54 + width * height * 3;
    header.reserved = 0;
    header.reserved2 = 0;
    header.bitmap_dataoffset = 54;
    header.bitmap_headersize = 40;
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bits_perpixel = 24;
    header.compression = 0;
    header.bitmap_datasize = width * height * 3;
    header.hresolution = 2835;
    header.vresolution = 2835;
    header.usedcolors = 0;
    header.importantcolors = 0;
    fwrite(&header, sizeof(Bmpheader), 1, fp);
}

// 定義 BMP 標頭結構
typedef struct Bmpheader
{
    char identifier[2];
    unsigned int filesize;
    unsigned short reserved;
    unsigned short reserved2;
    unsigned int bitmap_dataoffset;
    unsigned int bitmap_headersize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bits_perpixel;
    unsigned int compression;
    unsigned int bitmap_datasize;
    unsigned int hresolution;
    unsigned int vresolution;
    unsigned int usedcolors;
    unsigned int importantcolors;
    unsigned int palette;
} Bitmap;

// 定義 RGB 結構
typedef struct RGB
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} ImgRGB;

// 讀取 BMP 標頭的函數
void readheader(FILE *fp, Bitmap *bmpHeader)
{
    fread(&bmpHeader->identifier, sizeof(bmpHeader->identifier), 1, fp);
    fread(&bmpHeader->filesize, sizeof(bmpHeader->filesize), 1, fp);
    fread(&bmpHeader->reserved, sizeof(bmpHeader->reserved), 1, fp);
    fread(&bmpHeader->reserved2, sizeof(bmpHeader->reserved2), 1, fp);
    fread(&bmpHeader->bitmap_dataoffset, sizeof(bmpHeader->bitmap_dataoffset), 1, fp);
    fread(&bmpHeader->bitmap_headersize, sizeof(bmpHeader->bitmap_headersize), 1, fp);
    fread(&bmpHeader->width, sizeof(bmpHeader->width), 1, fp);
    fread(&bmpHeader->height, sizeof(bmpHeader->height), 1, fp);
    fread(&bmpHeader->planes, sizeof(bmpHeader->planes), 1, fp);
    fread(&bmpHeader->bits_perpixel, sizeof(bmpHeader->bits_perpixel), 1, fp);
    fread(&bmpHeader->compression, sizeof(bmpHeader->compression), 1, fp);
    fread(&bmpHeader->bitmap_datasize, sizeof(bmpHeader->bitmap_datasize), 1, fp);
    fread(&bmpHeader->hresolution, sizeof(bmpHeader->hresolution), 1, fp);
    fread(&bmpHeader->vresolution, sizeof(bmpHeader->vresolution), 1, fp);
    fread(&bmpHeader->usedcolors, sizeof(bmpHeader->usedcolors), 1, fp);
    fread(&bmpHeader->importantcolors, sizeof(bmpHeader->importantcolors), 1, fp);
}

// 分配二維記憶體空間
ImgRGB **malloc_2D(int row, int col)
{
    ImgRGB **array = (ImgRGB **)malloc(row * sizeof(ImgRGB *));
    for (int i = 0; i < row; i++)
    {
        array[i] = (ImgRGB *)malloc(col * sizeof(ImgRGB));
    }
    return array;
}

// 讀取 RGB 數據的函數
void InputData(FILE *fp, ImgRGB **array, int H, int W, int skip)
{
    int temp;
    char skip_buf[3];
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            temp = fgetc(fp);
            array[i][j].B = temp;
            temp = fgetc(fp);
            array[i][j].G = temp;
            temp = fgetc(fp);
            array[i][j].R = temp;
        }
        if (skip != 0)
        {
            fread(skip_buf, skip, 1, fp);
        }
    }
}

// 讀取量化資料
void readQuantizedData(const char *filename, short ***data, int height, int width)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < height / DCT_H; i++)
    {
        for (int j = 0; j < width / DCT_W; j++)
        {
            fread(data[i][j], sizeof(short), DCT_H * DCT_W, file);
        }
    }

    fclose(file);
}

short ***malloc3D(int height, int width)
{
    short ***array = (short ***)malloc(height / DCT_H * sizeof(short **));
    for (int i = 0; i < height / DCT_H; i++)
    {
        array[i] = (short **)malloc(width / DCT_W * sizeof(short *));
        for (int j = 0; j < width / DCT_W; j++)
        {
            array[i][j] = (short *)malloc(DCT_H * DCT_W * sizeof(short));
        }
    }
    return array;
}

void free3D(short ***array, int height, int width)
{
    for (int i = 0; i < height / DCT_H; i++)
    {
        for (int j = 0; j < width / DCT_W; j++)
        {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
}

unsigned char **malloc2D(int row, int col)
{
    unsigned char **array;
    array = (unsigned char **)malloc(row * sizeof(unsigned char *));
    for (int i = 0; i < row; i++)
    {
        array[i] = (unsigned char *)malloc(col * sizeof(unsigned char));
    }
    return array;
}

// 修改 malloc2D 函數來分配 double 型態的二維陣列
double **malloc2D_double(int row, int col)
{
    double **array = (double **)malloc(row * sizeof(double *));
    for (int i = 0; i < row; i++)
    {
        array[i] = (double *)malloc(col * sizeof(double));
    }
    return array;
}

void free2D(unsigned char **array, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(array[i]);
    }
    free(array);
}

void free2D_double(double **array, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(array[i]);
    }
    free(array);
}

// 讀取dim資料
void readImageDimensions(const char *filename, int *height, int *width)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    fscanf(file, "%d", width);
    fscanf(file, "%d", height);

    fclose(file);
}

// 讀取量化表
void readQuantizationTable(const char *filename, int table[DCT_H][DCT_W])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < DCT_H; i++)
    {
        for (int j = 0; j < DCT_W; j++)
        {
            fscanf(file, "%d", &table[i][j]);
        }
    }

    fclose(file);
}

// 反量化
void unquantizeDCT(short ***quantizedData, int quantizationTable[DCT_H][DCT_W], int height, int width)
{
    int M = height / DCT_H;
    int N = width / DCT_W;

    for (int m = 0; m < M; m++)
    {
        for (int n = 0; n < N; n++)
        {
            for (int i = 0; i < DCT_H; i++)
            {
                for (int j = 0; j < DCT_W; j++)
                {
                    quantizedData[m][n][i * DCT_W + j] *= quantizationTable[i][j];
                }
            }
        }
    }
}

// 反2D-DCT的basis
void generateBasisVectors(double basis_vector[DCT_H][DCT_W][DCT_H][DCT_W])
{
    for (int u = 0; u < DCT_H; u++)
    {
        for (int v = 0; v < DCT_W; v++)
        {
            for (int r = 0; r < DCT_H; r++)
            {
                for (int c = 0; c < DCT_W; c++)
                {
                    double cosR = cos(PI * u * (2.0 * r + 1) / (2.0 * DCT_H));
                    double cosC = cos(PI * v * (2.0 * c + 1) / (2.0 * DCT_W));
                    basis_vector[u][v][r][c] = cosR * cosC;
                }
            }
        }
    }
}

// 反2D-DCT並加上誤差值
void inverseDCTWithError(short ***quantizedData, float ***errorData, double **image, int quantizationTable[DCT_H][DCT_W], double basis_vector[DCT_H][DCT_W][DCT_H][DCT_W], int height, int width)
{
    int M = height / DCT_H;
    int N = width / DCT_W;

    for (int m = 0; m < M; m++)
    {
        for (int n = 0; n < N; n++)
        {
            for (int x = 0; x < DCT_H; x++)
            {
                for (int y = 0; y < DCT_W; y++)
                {
                    double sum = 0.0;
                    for (int u = 0; u < DCT_H; u++)
                    {
                        for (int v = 0; v < DCT_W; v++)
                        {
                            double coeff = (u == 0 ? 1.0 / sqrt(2) : 1.0) * (v == 0 ? 1.0 / sqrt(2) : 1.0);
                            sum += coeff * quantizedData[m][n][u * DCT_W + v] * basis_vector[u][v][x][y];
                        }
                    }
                    image[m * DCT_H + x][n * DCT_W + y] = sum * 2.0 / sqrt(DCT_H * DCT_W) + errorData[m][n][x * DCT_W + y];
                }
            }
        }
    }
}

// 把YCbCr轉回RGB
void YCbCrToRGB(double **Y, double **Cb, double **Cr, unsigned char **R, unsigned char **G, unsigned char **B, int H, int W)
{
    // 遍歷圖像的每一個像素
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            // 取出 YCbCr 各分量的值
            double y = Y[i][j];
            double cb = Cb[i][j];
            double cr = Cr[i][j];

            // 根據 YCbCr 到 RGB 的轉換公式計算 RGB 值
            int r = round(1.402 * (cr - 128) + y);
            int g = round(-0.34414 * (cb - 128) - 0.71414 * (cr - 128) + y);
            int b = round(1.772 * (cb - 128) + y);

            r = 305 + r;
            // 限制 RGB 值在 0 到 255 範圍內
            if (r < 0)
            {
                r = 0;
            }
            else if (r > 255)
            {
                r = 255;
            }

            R[i][j] = (unsigned char)r;

            g = g - 14;
            if (g < 0)
            {
                g = 0;
            }
            else if (g > 255)
            {
                g = 255;
            }

            G[i][j] = (unsigned char)g;

            b = 348 + b;
            if (b < 0)
            {
                b = 0;
            }
            else if (b > 255)
            {
                b = 255;
            }
            B[i][j] = (unsigned char)b;
        }
    }
}

// 把RGB存成BMP
void writePixelData(FILE *fp, unsigned char **R, unsigned char **G, unsigned char **B, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char color[3] = {B[y][x], G[y][x], R[y][x]}; // BMP 使用 BGR 格式
            fwrite(color, sizeof(unsigned char), 3, fp);
        }
    }
}

void writeRGBtoFiles(unsigned char **R, unsigned char **G, unsigned char **B, int width, int height, const char *rFilename, const char *gFilename, const char *bFilename)
{
    FILE *fpR = fopen(rFilename, "w");
    FILE *fpG = fopen(gFilename, "w");
    FILE *fpB = fopen(bFilename, "w");

    if (fpR == NULL || fpG == NULL || fpB == NULL)
    {
        perror("Error opening one of the RGB files");
        exit(1);
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            fprintf(fpR, "%d ", R[y][x]);
            fprintf(fpG, "%d ", G[y][x]);
            fprintf(fpB, "%d ", B[y][x]);
        }
        fprintf(fpR, "\n");
        fprintf(fpG, "\n");
        fprintf(fpB, "\n");
    }

    fclose(fpR);
    fclose(fpG);
    fclose(fpB);
}

// 讀取量化誤差數據
void readQuantizationError(const char *filename, float ***errorData, int height, int width)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < height / DCT_H; i++)
    {
        for (int j = 0; j < width / DCT_W; j++)
        {
            fread(errorData[i][j], sizeof(float), DCT_H * DCT_W, file);
        }
    }

    fclose(file);
}

float ***malloc3D_float(int height, int width)
{
    float ***array = (float ***)malloc(height / DCT_H * sizeof(float **));
    for (int i = 0; i < height / DCT_H; i++)
    {
        array[i] = (float **)malloc(width / DCT_W * sizeof(float *));
        for (int j = 0; j < width / DCT_W; j++)
        {
            array[i][j] = (float *)malloc(DCT_H * DCT_W * sizeof(float));
        }
    }
    return array;
}

void free3D_float(float ***array, int height, int width)
{
    for (int i = 0; i < height / DCT_H; i++)
    {
        for (int j = 0; j < width / DCT_W; j++)
        {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
}

int main(int argc, char *argv[])
{
    // 確保命令行參數數量正確
    if (argc != 14)
    {
        fprintf(stderr, "Usage: %s command output.bmp input.bmp Qt_Y.txt Qt_Cb.txt Qt_Cr.txt dim.txt qF_Y.raw qF_Cb.raw qF_Cr.raw eF_Y.raw eF_Cb.raw eF_Cr.raw\n", argv[0]);
        return 1;
    }

    int command = atoi(argv[1]);
    if (command != 1)
    {
        fprintf(stderr, "Unsupported command. Only '1' is supported.\n");
        return 1;
    }

    char *outputBMPFile = argv[2];
    char *inputBMPFile = argv[3];
    char *qt_Y_File = argv[4];
    char *qt_Cb_File = argv[5];
    char *qt_Cr_File = argv[6];
    char *dimFile = argv[7];
    char *qf_Y_File = argv[8];
    char *qf_Cb_File = argv[9];
    char *qf_Cr_File = argv[10];
    char *eF_Y_File = argv[11];
    char *eF_Cb_File = argv[12];
    char *eF_Cr_File = argv[13];

    int height, width;
    int Q_Y[DCT_H][DCT_W], Q_Cb[DCT_H][DCT_W], Q_Cr[DCT_H][DCT_W];

    // 從 dim.txt 讀取圖像尺寸
    readImageDimensions(dimFile, &height, &width);

    short ***quantized_Y = malloc3D(height, width);
    short ***quantized_Cb = malloc3D(height, width);
    short ***quantized_Cr = malloc3D(height, width);

    readQuantizedData(qf_Y_File, quantized_Y, height, width);
    readQuantizedData(qf_Cb_File, quantized_Cb, height, width);
    readQuantizedData(qf_Cr_File, quantized_Cr, height, width);

    // 讀取量化表
    readQuantizationTable(qt_Y_File, Q_Y);
    readQuantizationTable(qt_Cb_File, Q_Cb);
    readQuantizationTable(qt_Cr_File, Q_Cr);

    // 反量化
    unquantizeDCT(quantized_Y, Q_Y, height, width);
    unquantizeDCT(quantized_Cb, Q_Cb, height, width);
    unquantizeDCT(quantized_Cr, Q_Cr, height, width);

    // 讀取量化誤差
    float ***error_Y = malloc3D_float(height, width);
    float ***error_Cb = malloc3D_float(height, width);
    float ***error_Cr = malloc3D_float(height, width);

    readQuantizationError(eF_Y_File, error_Y, height, width);
    readQuantizationError(eF_Cb_File, error_Cb, height, width);
    readQuantizationError(eF_Cr_File, error_Cr, height, width);

    // 生成基向量
    double basis_vector[DCT_H][DCT_W][DCT_H][DCT_W];
    generateBasisVectors(basis_vector);

    // 創建一個用於存儲反量化圖像的陣列（使用 double）
    double **image_Y = malloc2D_double(height, width);
    double **image_Cb = malloc2D_double(height, width);
    double **image_Cr = malloc2D_double(height, width);

    // 反 2D-DCT並加上誤差值
    inverseDCTWithError(quantized_Y, error_Y, image_Y, Q_Y, basis_vector, height, width);
    inverseDCTWithError(quantized_Cb, error_Cb, image_Cb, Q_Cb, basis_vector, height, width);
    inverseDCTWithError(quantized_Cr, error_Cr, image_Cr, Q_Cr, basis_vector, height, width);

    // 生成 RGB 圖像陣列（仍然使用 unsigned char）
    unsigned char **image_R = malloc2D(height, width);
    unsigned char **image_G = malloc2D(height, width);
    unsigned char **image_B = malloc2D(height, width);

    // 將 YCbCr 轉換為 RGB
    YCbCrToRGB(image_Y, image_Cb, image_Cr, image_R, image_G, image_B, height, width);

    // 寫入新的 BMP 檔案
    FILE *fp = fopen(outputBMPFile, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "無法創建 BMP 檔案 %s。\n", outputBMPFile);
        return 1;
    }
    writeBMPHeader(fp, width, height);
    writePixelData(fp, image_R, image_G, image_B, width, height);
    fclose(fp);

    // 釋放記憶體
    free2D_double(image_Y, height);
    free2D_double(image_Cb, height);
    free2D_double(image_Cr, height);
    free2D(image_R, height);
    free2D(image_G, height);
    free2D(image_B, height);

    free3D(quantized_Y, height, width);
    free3D(quantized_Cb, height, width);
    free3D(quantized_Cr, height, width);
    // 處理完畢後，釋放量化誤差數據的記憶體
    free3D_float(error_Y, height, width);
    free3D_float(error_Cb, height, width);
    free3D_float(error_Cr, height, width);
    printf("Image processing completed\n");

    return 0;
}
