
/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagin1UV
{
  FLT U, V; /* компоненты текстурных координат */
} in1UV;

/* Функиця инициализации текстурных координат */
__inline in1UV IN1_UVSet( FLT U, FLT V )
{
  in1UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'IN1_UVSet' function */

/* Теп представления вершины */
typedef struct tagin1VERTEX
{
  VEC P;    /* координаты */
  in1UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} in1VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagin1PRIM_TYPE
{
  IN1_PRIM_TRIMESH, IN1_PRIM_GRID
} in1PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagin1PRIM
{
  in1PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  in1VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} in1PRIM;

/* Цвет по-умолчанию */
extern COLOR IN1_DefaultColor;

/* Функция создания примитива */
BOOL IN1_PrimCreate( in1PRIM *P, in1PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL IN1_PrimCreateSphere( in1PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID IN1_PrimFree( in1PRIM *P );

/* Функция отрисовки примитива */
VOID IN1_PrimDraw( in1PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagin1MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} in1MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagin1GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  in1MATERIAL *Mtls;         /* таблица материалов */
  in1PRIM *Prims;            /* массив примитивов */
} in1GEOM;

/* Функция добавления материала к объекту */
INT IN1_GeomAddMaterial( in1GEOM *G,
                         in1MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT IN1_GeomAddPrim( in1GEOM *G, in1PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID IN1_GeomFree( in1GEOM *G );

/* Функция рисования */
VOID IN1_GeomDraw( in1GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL IN1_GeomLoad( in1GEOM *G, CHAR *FileName );

