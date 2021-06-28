#include <stdio.h>
#include <Windows.h>

#pragma warning(disable:4996)

//最邻近插值
void ImageScaleNearest(const char* src_file_path, const char* dst_file_path, int src_width, int src_height, float scale)
{
    FILE * src_rgb_file = fopen(src_file_path, "rb");
    if (!src_rgb_file) {
        return;
    }

    //rgb 格式 channel=3, rgba 格式 channel=4
    int channel = 3;

    unsigned char *src_rgb = (unsigned char *)malloc(src_width * src_height * channel);
    fread(src_rgb, src_width * src_height * channel, 1, src_rgb_file);

    int dst_width = src_width * scale;
    int dst_height = src_height * scale;

    //转换后图像每一行的字节数
    int dst_line_byte_num = dst_width * channel;
	
    //为缩放后图像分配存储空间
    unsigned char *dst_rgb = (unsigned char *)malloc(dst_line_byte_num * dst_height);

    //最临近插值转换
    for (int dst_y = 0; dst_y < dst_height; dst_y++)
    {
        for (int dst_x = 0; dst_x < dst_width; dst_x++)
        {
            int src_x = (int)(dst_x / scale);
            int src_y = (int)(dst_y / scale);
            for (int k = 0; k < channel; k++) {
                *(dst_rgb + dst_y * dst_line_byte_num + dst_x * channel + k) = *(src_rgb + src_y * src_width * channel + src_x * channel + k);
            }
        }
    }

    FILE * des_rgb_file = fopen(dst_file_path, "wb");
    if (!des_rgb_file) {
        return;
    }
    fwrite(dst_rgb, dst_line_byte_num * dst_height, 1, des_rgb_file);

    fclose(src_rgb_file);
    fclose(des_rgb_file);

    free(src_rgb);
    free(dst_rgb);
}

//双线性插值
void ImageScalingDoubleLine(const char* src_file_path, const char* dst_file_path, int src_width, int src_height, float scale)
{
    FILE * src_rgb_file = fopen(src_file_path, "rb");
    if (!src_rgb_file) {
        return;
    }

    //rgb 格式 channel=3, rgba 格式 channel=4
    int channel = 3;

    unsigned char *src_rgb = (unsigned char *)malloc(src_width * src_height * channel);
    fread(src_rgb, src_width * src_height * channel, 1, src_rgb_file);

    int dst_width = (int)(src_width * scale);
    int dst_height = (int)(src_height * scale);

    // 图像每一行的字节数
    int src_line_byte_num = src_width * channel;
    int dst_line_byte_num = dst_width * channel;
    
    //为缩放后图像分配存储空间
    unsigned char *dst_rgb = (unsigned char *)malloc(dst_line_byte_num * dst_height);

    for (int dst_y = 0; dst_y < dst_height; dst_y++) 
    {
        for (int dst_x = 0; dst_x < dst_width; dst_x++) 
        {
            float src_point_y = dst_y / scale;
            float src_point_x = dst_x / scale;
            
            int src_left_point_y = src_point_y;
            int src_left_point_x = src_point_x;
            
            float distance_y = src_point_y - src_left_point_y;
            float distance_x = src_point_x - src_left_point_x;
            
            int point_a = src_left_point_y       * src_line_byte_num + src_left_point_x       * channel;//偏移量a点的rgb起点。
            int point_b = src_left_point_y       * src_line_byte_num + (src_left_point_x + 1) * channel;//偏移量b点的rgb起点。 
            int point_c = (src_left_point_y + 1) * src_line_byte_num + src_left_point_x       * channel;//偏移量c点的rgb起点。  
            int point_d = (src_left_point_y + 1) * src_line_byte_num + (src_left_point_x + 1) * channel;//偏移量d点的rgb起点。

            if (src_left_point_y == src_height - 1) {
                point_c = point_a;
                point_d = point_b;
            }
            if (src_left_point_x == src_width - 1) {
                point_b = point_a;
                point_d = point_c;
            }

            int pixel_point = dst_y * dst_line_byte_num + dst_x * channel;

            for (int k = 0; k < channel; k++)
            {
                dst_rgb[pixel_point + k] =
                    src_rgb[point_a + k] * (1 - distance_x) * (1 - distance_y) +
                    src_rgb[point_b + k] * distance_x       * (1 - distance_y) +
                    src_rgb[point_c + k] * distance_y       * (1 - distance_x) +
                    src_rgb[point_d + k] * distance_y       * distance_x;
            }
        }
    }

    FILE * des_rgb_file = fopen(dst_file_path, "wb");
    if (!des_rgb_file) {
        return;
    }
    fwrite(dst_rgb, dst_line_byte_num * dst_height, 1, des_rgb_file);

    fclose(src_rgb_file);
    fclose(des_rgb_file);

    free(src_rgb);
    free(dst_rgb);
}

int main() 
{
    //ImageScalingDoubleLine("Rgba32.rgba", "DoublelineRgb32.rgb", 1920, 1080, 0.5);
    //ImageScaleNearest("Rgba32.rgba", "NearestRgb32.rgb", 1920, 1080, 0.5);

    ImageScalingDoubleLine("Rgb24.rgb", "DoublelineRgb24.rgb", 423, 223, 0.5);
    ImageScaleNearest("Rgb24.rgb", "NearestRgb24.rgb", 423, 223, 0.5);
	return 0;
}