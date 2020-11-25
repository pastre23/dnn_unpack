#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "utility/bmp.h"
#include "utility/draw.h"

#define clamp(value) ((value<0)?0:(value>255?255:value))

#define FRM_WIDTH       1920
#define FRM_HEIGHT      156

#define IMG_WIDTH       300
#define IMG_HEIGHT      300
#define IMG_PADDING     20
#define IMG_WIDTH_PAD   (IMG_WIDTH + IMG_PADDING)

#define VP_RED          0
#define VP_GREEN        1
#define VP_BLUE         2

#pragma pack(1)
typedef struct {
    unsigned char r[IMG_WIDTH * IMG_HEIGHT];
    unsigned char g[IMG_WIDTH * IMG_HEIGHT];
    unsigned char b[IMG_WIDTH * IMG_HEIGHT];
} rgbp_buffer_t;

#pragma pack(1)
typedef struct {
    unsigned char y[IMG_WIDTH * IMG_HEIGHT];
    unsigned char cb[IMG_WIDTH * IMG_HEIGHT];
    unsigned char cr[IMG_WIDTH * IMG_HEIGHT];
} yc_buffer_t;

#define draw_text_rgb(buffer, img_w, img_h, left, top, text, color) \
    draw_text(buffer.r, img_w, img_h, left, top, text, R(color));   \
    draw_text(buffer.g, img_w, img_h, left, top, text, G(color));   \
    draw_text(buffer.b, img_w, img_h, left, top, text, B(color))

#define draw_rect_rgb(buffer, img_w, img_h, left, top, right, bottom, color, lw) \
    draw_rect(buffer.r, img_w, img_h, left, top, right, bottom, R(color), lw);   \
    draw_rect(buffer.g, img_w, img_h, left, top, right, bottom, G(color), lw);   \
    draw_rect(buffer.b, img_w, img_h, left, top, right, bottom, B(color), lw)

/**************** PROFILER ********************/
static double start_time = 0;
static double stop_time = 0;
static double elapsed_time = 0;

double get_time()
{
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

#define PROFILE_START() do{start_time = get_time();} while(0)
#define PROFILE_STOP()  do{stop_time = get_time(); elapsed_time=stop_time-start_time; printf("Execution time at line %d: %f s\n", __LINE__, elapsed_time);} while(0)
/***********************************************/

int main(int argc, char **argv)
{
    rgbp_buffer_t rgbp_data;
    yc_buffer_t   yc_data;
    rgb_buffer_t  rgb_data[IMG_WIDTH*IMG_HEIGHT];
    
    // for (int y = 0; y < IMG_HEIGHT; y++)
    // {
    //     for (int x = 0; x < IMG_WIDTH; x++)
    //     {
    //         // /*compute some pixel values*/
    //         rgbp_data.r[x+(IMG_WIDTH*y)] = 255*((float)x/IMG_WIDTH);
    //         rgbp_data.g[x+(IMG_WIDTH*y)] = 255*((float)y/IMG_HEIGHT);
    //         rgbp_data.b[x+(IMG_WIDTH*y)] = 0;
    //     }
    // }

    DIR *dir;    
    FILE *raw_file;
    struct dirent *ent;
    
    char out_path[256];
    size_t size;
    unsigned char *raw_data;
    unsigned char *raw_ptr;

    uint32_t normVal = 0x180;
    uint32_t normShift = 1; //  = 0 --> pow(2,normShift)

    if((normVal >> 8) == 0) {
        normVal = (normVal & 0xff);
    }
    else {
        normVal = -(normVal & 0xff);
    }
        
    if ((dir = opendir("data")) != NULL) 
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) 
        {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

            printf ("Extract: data/%s\n", ent->d_name);
    
            char path[256] = "";
            sprintf(path, "data/%s", ent->d_name);            
            raw_file = fopen(path,"rb");
            fseek(raw_file, 0, SEEK_END); 
            size = ftell(raw_file);      
            fseek(raw_file, 0, SEEK_SET); 
            raw_data = (unsigned char *)malloc(size);
            
            if (raw_file == NULL){
                printf("[ERROR] File %s not found.\n", ent->d_name);           
                return EXIT_FAILURE;
            }
            else if (fread((unsigned char*)raw_data, sizeof(unsigned char), size, raw_file) != size){
                printf("[ERROR] There was an error in reading the file %s\n", ent->d_name, size);
                return EXIT_FAILURE;
            }
            
            fclose(raw_file);
            
            raw_ptr = (raw_data + (FRM_WIDTH*2)); // Split first two lines
            int vp_size = IMG_WIDTH_PAD * IMG_HEIGHT;

            PROFILE_START();
            for (int i = 0; i < IMG_WIDTH*IMG_HEIGHT; i++)
            {
                int i_padded = i + (IMG_PADDING * (i/IMG_WIDTH));
                rgbp_data.r[i] = ((raw_ptr[i_padded+(vp_size*VP_RED)] * normShift - normVal) & 0xFF);
                rgbp_data.g[i] = ((raw_ptr[i_padded+(vp_size*VP_GREEN)] * normShift - normVal) & 0xFF) + 128;
                rgbp_data.b[i] = ((raw_ptr[i_padded+(vp_size*VP_BLUE)] * normShift - normVal) & 0xFF) + 128;
            }
            PROFILE_STOP();

            free(raw_data);
            
            // PROFILE_START();
            // char class[32] = "";
            // sprintf(class, "%s: %d%%", "human", 68);
            // draw_text_rgb(rgbp_data, IMG_WIDTH, IMG_HEIGHT, 50+4, 50+4, class, RGB(128,32,0));
            // draw_rect_rgb(rgbp_data, IMG_WIDTH, IMG_HEIGHT, 50, 50, 273, 257, RGB(128,32,0), 3);
            // PROFILE_STOP();

            // // Conversion from RGB to YCbCr
            // PROFILE_START();
            // for (int i = 0; i < IMG_WIDTH*IMG_HEIGHT; i++)
            // {
            //     yc_data.y[i]  = (( 66 * rgbp_data.r[i] + 129 * rgbp_data.g[i] +  25 * rgbp_data.b[i]) >> 8) + 16;
            //     yc_data.cb[i] = ((-38 * rgbp_data.r[i] + -74 * rgbp_data.g[i] + 112 * rgbp_data.b[i]) >> 8) + 128;
            //     yc_data.cr[i] = ((112 * rgbp_data.r[i] + -94 * rgbp_data.g[i] + -18 * rgbp_data.b[i]) >> 8) + 128;
            // }
            // PROFILE_STOP();

            for (int y = 0; y < IMG_HEIGHT; y++)
            {
                for (int x = 0; x < IMG_WIDTH; x++)
                {
                    // /*compute some pixel values*/
                    int pixel = x+(IMG_WIDTH*y);
                    rgb_data[pixel].r = rgbp_data.r[pixel];
                    rgb_data[pixel].g = rgbp_data.g[pixel];
                    rgb_data[pixel].b = rgbp_data.b[pixel];
                }
            }

            sprintf(out_path, "e_%s.ppm", ent->d_name);
            to_ppm(out_path, rgb_data, IMG_WIDTH, IMG_HEIGHT);
    
        }
        closedir (dir);
    } else {
        /* could not open directory */
        return EXIT_FAILURE;
    }
}

