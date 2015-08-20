#include <windows.h>
#include <stdio.h>
#define GetPixel(Bmp, X, Y) (((Bmp)->Pixels[(Y) * (Bmp)->Width + (X)]) & 0x00FFFFFF)
#define PixelToColour(Pixel) (COLORREF)(((Pixel) & 0xFF00FF00) | (((Pixel) & 0xFF0000) >> 16) | (((Pixel) & 0xFF) << 16))
typedef struct
{
    HBITMAP hBmp;
    int Width, Height;
    unsigned int* Pixels;
    unsigned short BitsPerPixel;
} BmpData;
typedef enum {false = 0, true = 1} bool;
bool ScreenShot(BmpData* Data)
{
    bool Result = false;
    memset(Data, 0, sizeof(BmpData));
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(NULL);
    Data->Width = GetDeviceCaps(hdcScreen, HORZRES);
    Data->Height = GetDeviceCaps(hdcScreen, VERTRES);
    unsigned char* Pixels = NULL;
    unsigned short BitsPerPixel = 32;
    BITMAPINFO Info = {{sizeof(BITMAPINFOHEADER), Data->Width, -Data->Height, 1, BitsPerPixel, BI_RGB, 0, 0, 0, 0, 0}};
    Data->hBmp = CreateDIBSection(hdcScreen, &Info, DIB_RGB_COLORS, (void**)&Pixels, NULL, 0);
    if(Data->hBmp)
    {
        HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, Data->hBmp);
        BitBlt(hdcMem, 0, 0, Data->Width, Data->Height, hdcScreen, 0, 0, SRCCOPY);
        SelectObject(hdcMem, hOld);
        Data->Height *= -1;
        Data->BitsPerPixel = BitsPerPixel;
        Data->Pixels = (unsigned int*)Pixels;
        Result = true;
    }
    DeleteDC(hdcMem);
    DeleteDC(hdcScreen);
    return Result;
}
void SaveBitmap(const char* FilePath, const BmpData* Data)
{
    FILE* hFile = fopen(FilePath, "wb+");
    if (!hFile)
    {
        printf("%s", "Error. Cannot Create Bitmap.");
        return;
    }
    unsigned int Trash = 0;
    unsigned short Planes = 1;
    unsigned int biSize = 40;
    unsigned short Type = 0x4D42;
    unsigned int compression = 0;
    unsigned int PixelsOffsetBits = 54;
    int Width = Data->Width;
    int Height = -Data->Height;
    unsigned short BitsPerPixel = Data->BitsPerPixel;
    unsigned int size = ((Width * BitsPerPixel + 31) / 32) * 4 * Height;
    unsigned int bfSize = 54 + size;
    Height *= -1;
    fwrite(&Type, sizeof(char), sizeof(Type), hFile);
    fwrite(&bfSize, sizeof(char), sizeof(bfSize), hFile);
    fwrite(&Trash, sizeof(char), sizeof(Trash), hFile);
    fwrite(&PixelsOffsetBits, sizeof(char), sizeof(PixelsOffsetBits), hFile);
    fwrite(&biSize, sizeof(char), sizeof(biSize), hFile);
    fwrite(&Width, sizeof(char), sizeof(Width), hFile);
    fwrite(&Height, sizeof(char), sizeof(Height), hFile);
    fwrite(&Planes, sizeof(char), sizeof(Planes), hFile);
    fwrite(&BitsPerPixel, sizeof(char), sizeof(BitsPerPixel), hFile);
    fwrite(&compression, sizeof(char), sizeof(compression), hFile);
    fwrite(&size, sizeof(char), sizeof(size), hFile);
    fwrite(&Trash, sizeof(char), sizeof(Trash), hFile);
    fwrite(&Trash, sizeof(char), sizeof(Trash), hFile);
    fwrite(&Trash, sizeof(char), sizeof(Trash), hFile);
    fwrite(&Trash, sizeof(char), sizeof(Trash), hFile);
    fwrite(&Data->Pixels, sizeof(char), size, hFile);
    fclose(hFile);
}









void SaveBitmap1(const char* FilePath, const BmpData* Data)
{
    std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
    if (!hFile.is_open())
    {
        printf("%s", "Error. Cannot Create Bitmap.");
        return;
    }
    unsigned int Trash = 0;
    unsigned short Planes = 1;
    unsigned int biSize = 40;
    unsigned short Type = 0x4D42;
    unsigned int compression = 0;
    unsigned int PixelsOffsetBits = 54;
    int Width = Data->Width;
    int Height = -Data->Height;
    unsigned short BitsPerPixel = Data->BitsPerPixel;
    unsigned int size = ((Width * BitsPerPixel + 31) / 32) * 4 * Height;
    unsigned int bfSize = 54 + size;
    Height *= -1;
    hFile.write(reinterpret_cast<char*>(&Type), sizeof(Type));
    hFile.write(reinterpret_cast<char*>(&bfSize), sizeof(bfSize));
    hFile.write(reinterpret_cast<char*>(&Trash), sizeof(Trash));
    hFile.write(reinterpret_cast<char*>(&PixelsOffsetBits), sizeof(PixelsOffsetBits));
    hFile.write(reinterpret_cast<char*>(&biSize), sizeof(biSize));
    hFile.write(reinterpret_cast<char*>(&Width), sizeof(Width));
    hFile.write(reinterpret_cast<char*>(&Height), sizeof(Height));
    hFile.write(reinterpret_cast<char*>(&Planes), sizeof(Planes));
    hFile.write(reinterpret_cast<char*>(&BitsPerPixel), sizeof(BitsPerPixel));
    hFile.write(reinterpret_cast<char*>(&compression), sizeof(compression));
    hFile.write(reinterpret_cast<char*>(&size), sizeof(size));
    hFile.write(reinterpret_cast<char*>(&Trash), sizeof(Trash));
    hFile.write(reinterpret_cast<char*>(&Trash), sizeof(Trash));
    hFile.write(reinterpret_cast<char*>(&Trash), sizeof(Trash));
    hFile.write(reinterpret_cast<char*>(&Trash), sizeof(Trash));
    hFile.write(reinterpret_cast<char*>(Data->Pixels), size);
    hFile.close();
}
int main()
{
    int X = 15, Y = 16;
    BmpData Data = {0};
    ScreenShot(&Data);
    printf("The colour at: (%i, %i) is: %lu\n\n", X, Y, PixelToColour(GetPixel(&Data, X, Y)));
    SaveBitmap1("C:/Users/Brandon/Desktop/Foo.bmp", &Data);
    DeleteObject(Data.hBmp);
    return 0;
}