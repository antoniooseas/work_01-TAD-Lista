
#include <imgStruct.h>

typedef enum funcUsed {
  NONE,
  FLIP_VERTICAL,
  FLIP_HORIZONTAL,
  TRANSPOSE,
  CLAHE,
  MEDIAN_BLUR,
  FLIP_ADD90,
  FLIP_NEQ90,
} FuncUsed;

typedef struct ButtonStatus{
  int flip_vertical;
  int flip_horizontal;
  int transpose;
  int clahe;
  int median_blur;
  int qtdFlipAdd90;
  int qtdFlipNeq90;
} ButtonStatus;

typedef struct imgHistoricoRGB
{
  ImageRGB *imgRGB;
  struct imgHistoricoRGB *prev;
  struct imgHistoricoRGB *next;
  ButtonStatus buttonStatus;
  FuncUsed funcUsed;
} ImgHistoricoRGB;

typedef struct imgHistoricoGray
{
  ImageGray *imgGray;
  struct imgHistoricoGray *prev;
  struct imgHistoricoGray *next;
  ButtonStatus buttonStatus;
  FuncUsed funcUsed;
} ImgHistoricoGray;

extern ImgHistoricoRGB *historicoRGBInicio;
extern ImgHistoricoRGB *historicoRGBAtual;
extern ImgHistoricoGray *historicoGrayInicio;
extern ImgHistoricoGray *historicoGrayAtual;

ImageGray *refrashHistoricFuncGray(FuncUsed funcUsed);

void iniciarHistoricoRGB();
void removerValoresAFrenteRGB();
void adicionarHistoricoRGB(ImageRGB *newImgRGB, FuncUsed funcUsed);
void SeguirHistoricoRGB();
void VoltarHistoricoRGB();
void removerHistoricoRGB();

ImageRGB *refrashHistoricFuncRGB(FuncUsed funcUsed);

void iniciarHistoricoGray();
void removerValoresAFrenteGray();
void adicionarHistoricoGray(ImageGray *newImgGray, FuncUsed funcUsed);
void SeguirHistoricoGray();
void VoltarHistoricoGray();
void removerHistoricoGray();

