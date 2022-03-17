#include "cJSON.h"
#include "bmp_header.h"
#include "write_digit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 150000
#define FILE_NAME_SIZE 50
#define DIMENSION 73

void verify_parsing(const cJSON *verify)
{
    if (verify == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
}

bmp_fileheader * get_file_header(const char * json_input)
{
    cJSON * input_info = cJSON_Parse(json_input); //in input info se parseaza continutul json file
    verify_parsing(input_info); //ser verifica parsarea

    const cJSON *file_header = NULL;
    const cJSON *signature = NULL;
    const cJSON *file_size = NULL;
    const cJSON *reserved = NULL;
    const cJSON *offset = NULL;

    file_header = cJSON_GetObjectItemCaseSensitive(input_info, "file_header"); //se citeste toata informatia pentru
    signature = cJSON_GetObjectItemCaseSensitive(file_header, "signature"); // structura respectiva
    file_size = cJSON_GetObjectItemCaseSensitive(file_header, "file_size");
    reserved = cJSON_GetObjectItemCaseSensitive(file_header, "reserved");
    offset = cJSON_GetObjectItemCaseSensitive(file_header, "offset");

    bmp_fileheader * bmp_file_header = calloc(6, sizeof(bmp_fileheader)); //se aloca spatiu pentru structura
    if (bmp_file_header == NULL)
    {
        printf("Allocation error of file header\n");
        exit(EXIT_FAILURE);
    }
    bmp_file_header->bfSize = file_size->valueint;  //se scriu datele in structura
    bmp_file_header->fileMarker1 = signature->valuestring[0];
    bmp_file_header->fileMarker2 = signature->valuestring[1];
    bmp_file_header->unused1 = reserved->valueint;
    bmp_file_header->unused2 = reserved->valueint;
    bmp_file_header->imageDataOffset = offset->valueint;
    cJSON_Delete(input_info); //se sterge input_info de tip cJSON(deoarece aloca dinamic)
    return bmp_file_header;
}

bmp_infoheader * get_header_info(const char * json_input) //analog ca in get file_header
{
    cJSON * input_info = cJSON_Parse(json_input); 
    verify_parsing(input_info);

    const cJSON *info_header = NULL;
    const cJSON *size = NULL;
    const cJSON *width = NULL;
    const cJSON *height = NULL;
    const cJSON *planes = NULL;
    const cJSON *bit_count = NULL;
    const cJSON *compr = NULL;
    const cJSON *image_size = NULL;
    const cJSON *x_per_met = NULL;
    const cJSON *y_per_met = NULL;
    const cJSON *imp_col = NULL;
    const cJSON *used_col = NULL;

    info_header = cJSON_GetObjectItemCaseSensitive(input_info, "info_header");
    size = cJSON_GetObjectItemCaseSensitive(info_header, "size");
    width = cJSON_GetObjectItemCaseSensitive(info_header, "width");
    height = cJSON_GetObjectItemCaseSensitive(info_header, "height");
    planes = cJSON_GetObjectItemCaseSensitive(info_header, "planes");
    bit_count = cJSON_GetObjectItemCaseSensitive(info_header, "bit_count");
    compr = cJSON_GetObjectItemCaseSensitive(info_header, "compression");
    image_size = cJSON_GetObjectItemCaseSensitive(info_header, "image_size");
    x_per_met = cJSON_GetObjectItemCaseSensitive(info_header, "x_pixels_per_meter");
    y_per_met = cJSON_GetObjectItemCaseSensitive(info_header, "y_pixels_per_meter");
    used_col = cJSON_GetObjectItemCaseSensitive(info_header, "colors_used");
    imp_col = cJSON_GetObjectItemCaseSensitive(info_header, "colors_important");

    bmp_infoheader * bmp_info_header = calloc(11, sizeof(bmp_infoheader));
    if (bmp_info_header == NULL)
    {
        printf("Allocation error of info header\n");
        exit(EXIT_FAILURE);
    }

    bmp_info_header->biSize = size->valueint;
    bmp_info_header->width = width->valueint;
    bmp_info_header->height = height->valueint;
    bmp_info_header->planes = planes->valueint;
    bmp_info_header->bitPix = bit_count->valueint;
    bmp_info_header->biCompression = compr->valueint;
    bmp_info_header->biSizeImage = image_size->valueint;
    bmp_info_header->biXPelsPerMeter = x_per_met->valueint;
    bmp_info_header->biYPelsPerMeter = y_per_met->valueint;
    bmp_info_header->biClrUsed = used_col->valueint;
    bmp_info_header->biClrImportant = imp_col->valueint;
    cJSON_Delete(input_info);
    return bmp_info_header;
}

unsigned char *** bmp_matrix(const char * json_input, signed int height, signed int width)
{   //primeste imputul, dimensiunea x si y
    cJSON * input_info = cJSON_Parse(json_input);
    verify_parsing(input_info);

    cJSON * iterator = NULL; //iterator
    const cJSON *pxl_bit_map = NULL; //salveaza informatia din json din Obiectul bitmap
    pxl_bit_map = cJSON_GetObjectItem(input_info, "bitmap");

    int height_count = 0, width_count = 0, color_byte_count = 0;    //numara x, y si bitul de culoare
    
    unsigned char *** bit_map = (unsigned char ***) calloc(height, sizeof(unsigned char**));    //alocam o matrice 3D
    if (bit_map == NULL)
    {
        printf("Allocation error of bit map\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < height; i++)
    {
        bit_map[i] = (unsigned char **)calloc(width, sizeof(unsigned char *));
        if (bit_map[i] == NULL)
        {
            printf("Allocation error bit map\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < width; j++)
        {
            bit_map[i][j] = (unsigned char *)calloc(3, sizeof(unsigned char));
            if (bit_map[i][j] == NULL)
            {
                printf("Allocation error bitmap\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    cJSON_ArrayForEach(iterator, pxl_bit_map){  //citim din pxl_bit_map fiecare element intr-un iterator
        if (cJSON_IsNumber(iterator))
        {
            if( width_count == width )  //daca am numarat 73 de ori
            {
                width_count = 0;    //se anuleaza
                height_count++;     //inscriem in linia urmatoare
            }
            bit_map[height_count][width_count][color_byte_count] = iterator->valueint; //salvam in bit_map elementul curent citit(iterator)
            color_byte_count++;//am citit un bit si trecem la altul
            if (color_byte_count == 3)  //daca am citit 3 biti de culoare
            {
                width_count++;  //trecem la urmatorul pixel
                color_byte_count = 0;   //citim de la 0
            }

        }
    }
    cJSON_Delete(input_info);
    return bit_map;
}

void write_to_bmp (bmp_fileheader * file_header, bmp_infoheader * info_header, unsigned char *** pixel_matrix, char * outputFileName)
{
    const unsigned char bmp_padding[3]  = {0, 0, 0};    //paddingul
    FILE * bmp_output = fopen(outputFileName,"wb"); //deschide fisierul
    /* ==================Writing file header to output====================*/
    fwrite( &(file_header->fileMarker1), sizeof(unsigned char), 1, bmp_output); 
    fwrite( &(file_header->fileMarker2), sizeof(unsigned char), 1, bmp_output);
    fwrite( &(file_header->bfSize), sizeof(unsigned int), 1, bmp_output);
    fwrite( &(file_header->unused1), sizeof(unsigned short), 1, bmp_output);
    fwrite( &(file_header->unused2), sizeof(unsigned short), 1, bmp_output);
    fwrite( &(file_header->imageDataOffset), sizeof(unsigned int), 1, bmp_output);
    /* ==================Writing info header to output====================*/
    fwrite( &(info_header->biSize), sizeof(unsigned int), 1, bmp_output);
    fwrite( &(info_header->width), sizeof(signed int), 1, bmp_output);
    fwrite( &(info_header->height), sizeof(signed int), 1, bmp_output);
    fwrite( &(info_header->planes), sizeof(unsigned short), 1, bmp_output);
    fwrite( &(info_header->bitPix), sizeof(unsigned short), 1, bmp_output);
    fwrite( &(info_header->biCompression), sizeof(unsigned int), 1, bmp_output);
    fwrite( &(info_header->biSizeImage), sizeof(unsigned int), 1, bmp_output);
    fwrite( &(info_header->biXPelsPerMeter), sizeof(int), 1, bmp_output);
    fwrite( &(info_header->biYPelsPerMeter), sizeof(int), 1, bmp_output);
    fwrite( &(info_header->biClrUsed), sizeof(unsigned int), 1, bmp_output);
    fwrite( &(info_header->biClrImportant), sizeof(unsigned int), 1, bmp_output);
    /* ==================Writing bit map to output====================*/
    for (int i = info_header->height - 1; i >= 0; i--) //incepem sa inscriem de la capatul matricii de biti
    {                                                  //din cauza structurii fisierelor BMP
        for (int j = 0; j < info_header->width; j++)
        {
                fwrite(pixel_matrix[i][j], sizeof(unsigned char), 3, bmp_output); //scriem pixel cu culoarea lui
        }
        fwrite(bmp_padding, sizeof(unsigned char), 1, bmp_output);//la sfarsitul randului inscris adaugam paddingul
    }
    fclose(bmp_output);
    free(outputFileName);
}

char * ReadFromJson(char * inputFileName)
{
    char * buffer = calloc(BUFFER_SIZE, sizeof(char));  //aloca un string pentru stocare
    if (buffer == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    FILE * f_input;
    f_input = fopen(inputFileName, "r+");
    fread(buffer, BUFFER_SIZE, 1, f_input); //citeste in buffer
    fclose(f_input);
    return buffer;
}

void mirror_matrix(unsigned char ***initial_matrix, signed int height, signed int width)
{
    unsigned char aux1[3] = {0, 0, 0};  //auxiliar pentru salvare primului pixel
    unsigned char aux2[3] = {0, 0, 0};  //auxiliar pentru salvarea bitului ce trebuie interschimbat
    for (int i = height - 2; i >= 2 ; i--)
    {
        for (int j = 2; j < width; j += 8)
        {
            for(int k = 0; k < 2; k++)
            {
                aux1[0] = initial_matrix[i][j + k][0];  //salvam in aux1 primul pixel
                aux1[1] = initial_matrix[i][j + k][1];
                aux1[2] = initial_matrix[i][j + k][2];

                aux2[0] = initial_matrix[i][j + 4 - k][0];  //salvam in aux2 al doilea pixel
                aux2[1] = initial_matrix[i][j + 4 - k][1];
                aux2[2] = initial_matrix[i][j + 4 - k][2];

                initial_matrix[i][j + k][0] =  aux2[0];     //interschimbam
                initial_matrix[i][j + k][1] =  aux2[1];
                initial_matrix[i][j + k][2] =  aux2[2];

                initial_matrix[i][j + 4 - k][0] =  aux1[0];
                initial_matrix[i][j + 4 - k][1] =  aux1[1];
                initial_matrix[i][j + 4 - k][2] =  aux1[2];
            }
        }
    }
}

void write_rose_to_aux(unsigned char *** initial_matrix, short ** aux)
{
    int i, j;
    for (i = 2; i < DIMENSION - 2; i++)
    {
        for (j = 2; j < DIMENSION - 2; j++)
        {
            if(initial_matrix[i][j][0] == 175)
                if(initial_matrix[i][j][1] == 175)
                    if(initial_matrix[i][j][2] == 255)
                        aux[i][j] = 1;
        }
    }
}

short recognize_digit(short ** arr, short i, short j)
{
    if(arr[i][j] == 0 && arr[i][j + 4] == 1) //recunoastere cifra 1
        return 1;
    if(arr[i][j] == 1 && arr[i + 1][j] == 0 && arr[i + 2][j] == 1 
                      && arr[i + 3][j] == 1 && arr[i + 4][j] == 1)
        return 2;//recunoastere cifra 2 si asa mai departe
    if(arr[i][j] == 1 && arr[i + 1][j] == 0 && arr[i + 2][j] == 1 
                      && arr[i + 3][j] == 0 && arr[i + 4][j] == 1)
        return 3;
    if(arr[i][j] == 1 && arr[i + 1][j] == 1 && arr[i + 2][j] == 1 
                      && arr[i + 3][j] == 0 && arr[i + 4][j] == 0)
        return 4;
    if(arr[i][j] == 1 && arr[i + 1][j] == 1 && arr[i + 2][j] == 1 
                      && arr[i + 3][j] == 0 && arr[i + 4][j] == 1)
                        if(arr[i + 1][j + 4] == 0)
                            return 5;
    if(arr[i][j] == 1 && arr[i + 1][j] == 1 && arr[i + 2][j] == 1 
                      && arr[i + 3][j] == 1 && arr[i + 4][j] == 1)
                        if(arr[i + 1][j + 4] == 0)
                            return 6;
    if(arr[i][j + 0] == 1 && arr[i][j + 1] == 1 && arr[i][j + 1] == 1 
                          && arr[i][j + 3] == 1 && arr[i][j + 4] == 1)
                        if(arr[i + 4][j] == 0)
                            return 7;
    if(arr[i][j] == 1 && arr[i + 1][j] == 1 && arr[i + 2][j] == 1 
                      && arr[i + 3][j] == 1 && arr[i + 4][j] == 1)
                        if(arr[i][j + 4] == 1 && arr[i + 1][j + 4] == 1 
                            && arr[i + 2][j + 4] == 1 
                            && arr[i + 3][j + 4] == 1 && arr[i + 4][j + 4] == 1)
                            return 8;
    if(arr[i + 3][j] == 0)
        if(arr[i][j + 4] == 1 && arr[i + 1][j + 4] == 1 && arr[i + 2][j + 4] == 1 
                              && arr[i + 3][j + 4] == 1 && arr[i + 4][j + 4] == 1)
            return 9;    
    return 0;//in caz ca cifra nu s-a recunoscut, scrie 0
}

void write_to_sudoku(short ** rose, short ** sudoku) 
{
    int row = 0, column = 0;    //parcurgem matricea auxiliara din 8 in 8 randuri si coloane, 
                                //adica, exact din pixelul de unde incepe sa fie scrisa cifra
    for ( int i = 2; i < DIMENSION - 2; i += 8)
    {
        for (int j = 2; j < DIMENSION - 2; j += 8)
        {
            sudoku[row][column] = recognize_digit(rose, i, j);//functie de recunoastere a cifrei  
            column += 1;//counter pentru a determina indexul de coloana si rand a sudoku_matrix
        }
        column = 0;
        row += 1;   //counter pentru a determina indexul de coloana si rand a sudoku_matrix
    }
}

int verify_sudoku(short ** sudoku)
{
    int sum;

    //verificare rand
    for (int i = 0; i < 9; i++)
    {
        sum = 0;
        for (int j = 0; j < 9; j++)
        {
            sum += sudoku[i][j];
        }
        if (sum != 45)
        {
            return 0; 
        }
    }

    //verificare coloana
    for (int j = 0; j < 9; j++)
    {
        sum = 0;
        for (int i = 0; i < 9; i++)
            sum += sudoku[i][j];
        if (sum != 45)
        {
            return 0; 
        }
    }

    //verifica 3x3 patrat
    for (int sq_i = 0; sq_i < 3; sq_i++)
    {
        for (int sq_j = 0; sq_j < 3; sq_j++)
        {
            sum = 0;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    sum += sudoku[sq_i * 3 + i][sq_j * 3 + j];
        }
        if ( sum != 45)
        {
            return 0;
        }
    }
    return 1;
}

void write_to_json(char * inputFileName, char * outputFileName, int valide)
{
    char * string = NULL;
    cJSON *input = NULL;
    cJSON *game_state = NULL;

    cJSON *file = cJSON_CreateObject();
    if (file == NULL)
    {
        printf("Error creating JSON object\n");
        exit(EXIT_FAILURE);
    }

    input = cJSON_CreateString(inputFileName);
    cJSON_AddItemToObject(file, "input_file", input);
    if (valide) //valide = 1 in cazul in care sudoku a fost rezolvat corect
        game_state = cJSON_CreateString("Win!");
    else //valide = 0 in cazul in care sudoku a fost rezolvat gresit
        game_state = cJSON_CreateString("Loss :(");
    cJSON_AddItemToObject(file, "game_state", game_state);

    string = cJSON_Print(file); //scrie in string continutul obiectului file json
    cJSON_Delete(file);

    FILE * output = fopen(outputFileName, "wt");
    fprintf(output,"%s\n", string); //scrie in fisierul .json
    fclose(output);

    free(string);//eliberam stringul, deoarece cJSON_Print intoarce o adresa catre un string alocat dinamic
    free(outputFileName);//eliberam stringul alocat dinamic mai devreme
}

unsigned char *** Task01(char * outputFileName, char * input, 
                            bmp_fileheader * file_header, 
                            bmp_infoheader * info_header)
{
    char * output_name_task1 = calloc(FILE_NAME_SIZE, sizeof(char));//alocare si
    sprintf(output_name_task1,"output_task1_%s", outputFileName);   //atribuire nume fisierului output
    unsigned char ***arr = bmp_matrix(input, DIMENSION, DIMENSION); //citirea matricii de pixeli
    write_to_bmp(file_header, info_header, arr, output_name_task1); //functie de scriere in bmp
    return arr; //returneaza array necesar pentru taskul urmator
}

void Task02(char * outputFileName, unsigned char *** arr, 
                                bmp_fileheader * file_header,
                                bmp_infoheader * info_header)
{
    char * output_name_task2 = calloc(FILE_NAME_SIZE, sizeof(char));
    sprintf(output_name_task2,"output_task2_%s", outputFileName);
    mirror_matrix(arr, DIMENSION, DIMENSION);   //functie de oglindire a fiecarei cifre din bitmap
    write_to_bmp(file_header, info_header, arr, output_name_task2);
}

void Task03(char * inputFileName, unsigned char *** arr)
{
    char * output_name_task3 = calloc(FILE_NAME_SIZE, sizeof(char));
    sprintf(output_name_task3, "output_task3_%s", inputFileName);
    short ** aux_matrix = (short **)calloc(DIMENSION, sizeof(short *));//aloc o matrice auxiliara 2D de 73x73
    if (aux_matrix == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < DIMENSION; i++)
    {
        aux_matrix[i] = (short *)calloc(DIMENSION, sizeof(short));
        if (aux_matrix[i] == NULL)
        {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }

    write_rose_to_aux(arr, aux_matrix); //din array identificam bitii roz si ii scriu in aux_matrix ca valoare 1

    short ** sudoku_matrix = (short **)calloc(9, sizeof(short *));  //aloc o matrice 9x9 2d de sudoku
    if (sudoku_matrix == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 9; i++)
    {
        sudoku_matrix[i] = (short *)calloc(9, sizeof(short));
        if (sudoku_matrix[i] == NULL)
        {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }

    write_to_sudoku(aux_matrix, sudoku_matrix);     //functie de scriere din aux matrix fiecare cifra in sudoku matrix sub forma de short
    write_to_json(inputFileName, output_name_task3, verify_sudoku(sudoku_matrix));//scrie in json
                                                    //verify sudoku verifica daca sudoku a fost rezolvat corect sau nu

    for (int i = 0; i < DIMENSION; i++)//eliberam aux_matrix
    {
            free(aux_matrix[i]);
    }
    free(aux_matrix);

    for (int i = 0; i < 9; i++) //eliberam sudoku_matrix
    {
            free(sudoku_matrix[i]);
    }
    free(sudoku_matrix);
}

/*=================================================================================================================*/

bmp_fileheader * get_file_header_from_bmp(char * inputFile)
{
    FILE * F_bmp;
    bmp_fileheader * file_header = calloc(6, sizeof(bmp_fileheader *));
    F_bmp = fopen(inputFile, "rb");
    fread(&file_header->fileMarker1, sizeof(unsigned char), 1, F_bmp);
    fread(&file_header->fileMarker2, sizeof(unsigned char), 1, F_bmp);
    fread(&file_header->bfSize, sizeof(unsigned int), 1, F_bmp);
    fread(&file_header->unused1, sizeof(unsigned short), 1, F_bmp);
    fread(&file_header->unused2, sizeof(unsigned short), 1, F_bmp);
    fread(&file_header->imageDataOffset, sizeof(unsigned int), 1, F_bmp);
    fclose(F_bmp);
    return file_header;
}

bmp_infoheader * get_info_header_from_bmp(char * inputFile)
{
    FILE * F_bmp;
    bmp_infoheader * info_header = calloc(11, sizeof(bmp_infoheader *));
    F_bmp = fopen(inputFile, "rb");
    fseek(F_bmp, 14, SEEK_SET);
    fread(&info_header->biSize, sizeof(unsigned int), 1, F_bmp);
    fread(&info_header->width, sizeof(signed int), 1, F_bmp);
    fread(&info_header->height, sizeof(signed int), 1, F_bmp);
    fread(&info_header->planes, sizeof(unsigned short), 1, F_bmp);
    fread(&info_header->bitPix, sizeof(unsigned short), 1, F_bmp);
    fread(&info_header->biCompression, sizeof(unsigned int), 1, F_bmp);
    fread(&info_header->biSizeImage, sizeof(unsigned int), 1, F_bmp);
    fread(&info_header->biXPelsPerMeter, sizeof(int), 1, F_bmp);
    fread(&info_header->biYPelsPerMeter, sizeof(unsigned int), 1, F_bmp);
    fread(&info_header->biClrUsed, sizeof(unsigned int), 1, F_bmp);
    fread(&info_header->biClrImportant, sizeof(unsigned int), 1, F_bmp);
    fclose(F_bmp);
    return info_header;
}

unsigned char *** read_bitmap(char * inputFile, unsigned int offset)
{
    FILE * F_bmp;
    unsigned char *** bit_map = (unsigned char ***) calloc(DIMENSION, sizeof(unsigned char**));
    if (bit_map == NULL)
    {
        printf("Allocation error of bit map\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < DIMENSION; i++)
    {
        bit_map[i] = (unsigned char **)calloc(DIMENSION, sizeof(unsigned char *));
        if (bit_map[i] == NULL)
        {
            printf("Allocation error bit map\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < DIMENSION; j++)
        {
            bit_map[i][j] = (unsigned char *)calloc(3, sizeof(unsigned char));
            if (bit_map[i][j] == NULL)
            {
                printf("Allocation error bitmap\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    F_bmp = fopen(inputFile, "rb");
    fseek(F_bmp, offset, SEEK_SET);
    for (int i = DIMENSION - 1; i >= 0; i--)
    {
        for(int j = 0; j < DIMENSION; j++)
        {
            fread(bit_map[i][j], sizeof(unsigned char), 3, F_bmp);
        }
        fseek(F_bmp, 1, SEEK_CUR);
    }
    fclose(F_bmp);
    return bit_map; 
}

int valid ( short ** sudoku_matrix, int r, int c, int n)
{
    for (int i = 0; i < 9; i++)
    {
        if (sudoku_matrix[r][i] == n)
            return 0;
    }

    for (int i = 0; i < 9; i++)
    {
        if (sudoku_matrix[i][c] == n)
            return 0;
    }

    int current_square_x = r - r % 3;
    int current_square_y = c - c % 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (sudoku_matrix[i + current_square_x][j + current_square_y] == n)
                return 0;
        }
    }
    return 1;
}

int fill_sudoku(short ** sudoku_matrix, int r, int c)
{
    if (r == 8 && c == 9)
        return 1;

    if (c == 9)
    {
        r++;
        c = 0;
    }

    if(sudoku_matrix[r][c] > 0)
    {
        return fill_sudoku(sudoku_matrix, r, c + 1);
    }

    for (int val = 1; val <= 9; val++) 
    {
        if (valid(sudoku_matrix, r, c, val) == 1) 
        {
            sudoku_matrix[r][c] = val;

            if (fill_sudoku(sudoku_matrix, r, c + 1) == 1)
                return 1;
        }
        sudoku_matrix[r][c] = 0;
    }
    return 0;
}

void write_digits_to_bitmap(unsigned char ***bitmap, 
                            short **sudoku_matrix, int *empty_coords, 
                            int empty_counter)
{
    int x, y;
    for(int i = 0; i < empty_counter; i++)
    {
        x = *(empty_coords + 2 * i); //coordonata de rand unde trebuie scria cifra
        y = *(empty_coords + 2 * i + 1);    //coordonata de coloana unde trebuie scrisa cifra
        write_digit_to_bmp[sudoku_matrix[x][y] - 1](bitmap, x, y); //in header, array de * la functii, care scrie cifra in bitmap
                                                                   //primeste bitmap, x, y
    }
}

void Task04(char * outputFileName, unsigned char *** arr, 
                bmp_fileheader * file_header, 
                bmp_infoheader * info_header)
{
    char * output_name_task4 = calloc(FILE_NAME_SIZE, sizeof(char));

    int empty_counter = 0;

    sprintf(output_name_task4, "output_task4_%s", outputFileName);
    /*==Allocation for aux matrix======================================*/
    short ** aux_matrix = (short **)calloc(info_header->height, sizeof(short *));
    if (aux_matrix == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < info_header->width; i++)
    {
        aux_matrix[i] = (short *)calloc(info_header->width, sizeof(short));
        if (aux_matrix[i] == NULL)
        {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    /*===================================================================*/

    write_rose_to_aux(arr, aux_matrix);
    /*==Allocation for sudoku matrix ====================================*/
    short ** sudoku_matrix = (short **)calloc(9, sizeof(short *));
    if (sudoku_matrix == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 9; i++)
    {
        sudoku_matrix[i] = (short *)calloc(9, sizeof(short));
        if (sudoku_matrix[i] == NULL)
        {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    /*===================================================================*/
    write_to_sudoku(aux_matrix, sudoku_matrix);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if(sudoku_matrix[i][j] == 0)
                empty_counter++;    //numar cate pozitii necompletate sunt
        }
    }
    /*==Allocation for empty spaces coords*/
    int * empty_coords = (int *)calloc(empty_counter * 2, sizeof(int)); //aloc pentru fiecare zerou 2 spatii pentru a salva coordonatele lui
    if (empty_coords == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    /*=====================================*/
    empty_counter = 0;  //anulez
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if(sudoku_matrix[i][j] == 0)
            {
                *(empty_coords + 2 * empty_counter) = i;     //salvez coordonata la empty_coords
                *(empty_coords + 2 * empty_counter + 1) = j; //la fel
                empty_counter++;
            }
        }
    }

    if(fill_sudoku(sudoku_matrix, 0, 0) == 1) //functie de completare sudoku, returneaza 1 daca sudoku a putut fi rezolvat
        {                                     //dar s-a garantat ca la task4 nu voi primi board-uri nerezolvabile
            write_digits_to_bitmap(arr, sudoku_matrix, empty_coords, empty_counter);    //functie de a scrie cifre in bit_map matrix
            write_to_bmp(file_header, info_header, arr, output_name_task4);//deja cu bitmap-ul scris, cream fisierul bmp
        }

    free(empty_coords); //eliberam totul

    for (int i = 0; i < DIMENSION; i++)
    {
            free(aux_matrix[i]);
    }
    free(aux_matrix);

    for (int i = 0; i < 9; i++)
    {
            free(sudoku_matrix[i]);
    }
    free(sudoku_matrix);
}

void Task_Bonus(char * outputFileName, unsigned char *** arr, 
                bmp_fileheader * file_header, 
                bmp_infoheader * info_header)
{       //Analog ca task 4, doar ca in cazul in care sudoku nu a putut fi completat, fill_sudoku va returna 0
        //si se va completa bitmapul cu "X" de culoarea rosie
    char * output_name_task4 = calloc(FILE_NAME_SIZE, sizeof(char));

    int empty_counter = 0;

    sprintf(output_name_task4, "output_bonus_%s", outputFileName);
    /*==Allocation for aux matrix======================================*/
    short ** aux_matrix = (short **)calloc(info_header->height, sizeof(short *));
    if (aux_matrix == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < info_header->width; i++)
    {
        aux_matrix[i] = (short *)calloc(info_header->width, sizeof(short));
        if (aux_matrix[i] == NULL)
        {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    /*===================================================================*/

    write_rose_to_aux(arr, aux_matrix);
    /*==Allocation for sudoku matrix ====================================*/
    short ** sudoku_matrix = (short **)calloc(9, sizeof(short *));
    if (sudoku_matrix == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 9; i++)
    {
        sudoku_matrix[i] = (short *)calloc(9, sizeof(short));
        if (sudoku_matrix[i] == NULL)
        {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    /*===================================================================*/
    write_to_sudoku(aux_matrix, sudoku_matrix);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if(sudoku_matrix[i][j] == 0)
                empty_counter++;    //numar cate sunt de 0
        }
    }
    /*==Allocation for empty spaces coords*/
    int * empty_coords = (int *)calloc(empty_counter * 2, sizeof(int)); //aloc pentru fiecare zerou 2 spatii pentru a salva coordonatele
    if (empty_coords == NULL)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    /*=====================================*/
    empty_counter = 0;  //anulez
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if(sudoku_matrix[i][j] == 0)
            {
                *(empty_coords + 2 * empty_counter) = i;     //salvez coordonata la empty_coords
                *(empty_coords + 2 * empty_counter + 1) = j; //la fel
                empty_counter++;
            }
        }
    }

    if(fill_sudoku(sudoku_matrix, 0, 0) == 1)
        {
            write_digits_to_bitmap(arr, sudoku_matrix, empty_coords, empty_counter);
            write_to_bmp(file_header, info_header, arr, output_name_task4);
        }
    else
        {
            write_failure(arr); //in write_digit.c
            write_to_bmp(file_header, info_header, arr, output_name_task4);
        }

    //eliberam totul
    free(empty_coords);

    for (int i = 0; i < DIMENSION; i++)
    {
            free(aux_matrix[i]);
    }
    free(aux_matrix);

    for (int i = 0; i < 9; i++)
    {
            free(sudoku_matrix[i]);
    }
    free(sudoku_matrix);
}

int main( int argc, char *argv[] )
{
    char * input_file_name; //string pentru input file name
    char * output_file_name; //string pentru output file name
    char * aux_name; //string pentru a edita output_file_name
    unsigned char *** bmp_matrix;   //matrice pentru bitmap
    bmp_fileheader * file_header;   //structura pentru file_header
    bmp_infoheader * info_header;   //          pentru info_header
    if(argc != 3)   //daca sunt mai putin de 3 argumente la rularea executabilului - exit
		return -1;
    if(!strcmp(argv[2],"123"))
    {
        input_file_name = strdup(argv[1]);  //alocare pentru stringuri
        output_file_name = strdup(argv[1]);
        
        aux_name = strstr(output_file_name, "json"); //gaseste json
        strcpy(aux_name,"bmp"); //inlocuieste cu bmp
        aux_name = strstr(output_file_name,"board"); //gaseste board
        strcpy(output_file_name, aux_name);//copie in output file name

        char * json_file; //string pentru a stoca continutul .json

        json_file = ReadFromJson(input_file_name); //functie de citire din json
        file_header = get_file_header(json_file); //functie extrage din json informatia si stocheaza in file_header
        info_header = get_header_info(json_file); //functie extrage din json informatia si stocheaza in info_header
        bmp_matrix = Task01(output_file_name, json_file, file_header, info_header); //functie de rezolvare task1
        Task02(output_file_name, bmp_matrix, file_header, info_header);//functie rezolvare task2
        aux_name = strstr(output_file_name, "bmp");//la task3 deoarece outputul eu un json
        strcpy(aux_name, "json");                   //modific sfarsitul output_file_name in json
        strcpy(input_file_name, output_file_name);
        Task03(input_file_name, bmp_matrix);    //functie rezolvare task3

        /* ==================Dealocating the bit map matrix====================*/
        for (int i = 0; i < info_header->height; i++)
        {
            for (int j = 0; j < info_header->width; j++)
            {
                free(bmp_matrix[i][j]);
            }
            free(bmp_matrix[i]);
        }
        free(bmp_matrix);
        /* ====================================================================*/
        free(json_file);
        free(info_header);
        free(file_header);
    }
    if(!strcmp(argv[2], "4") || !strcmp(argv[2], "bonus"))
    {
        input_file_name = strdup(argv[1]);
        output_file_name = strdup(argv[1]);
        file_header = get_file_header_from_bmp(input_file_name);
        info_header = get_info_header_from_bmp(input_file_name);
        bmp_matrix = read_bitmap(input_file_name, file_header->imageDataOffset);
        aux_name = strstr(output_file_name,"board");
        strcpy(output_file_name, aux_name);

        if(!strcmp(argv[2], "4")) 
            Task04(output_file_name, bmp_matrix, file_header, info_header);
        if(!strcmp(argv[2], "bonus")) 
            Task_Bonus(output_file_name, bmp_matrix, file_header, info_header);

        /* ==================Dealocating the bit map matrix====================*/
        for (int i = 0; i < info_header->height; i++)
        {
            for (int j = 0; j < info_header->width; j++)
            {
                free(bmp_matrix[i][j]);
            }
            free(bmp_matrix[i]);
        }
        free(bmp_matrix);
        /* ====================================================================*/
        free(info_header);
        free(file_header);
    }
    free(input_file_name);
    free(output_file_name);
    return 0;
}