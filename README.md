# RGBScale
RGB图像缩放算法：最邻近插值，双线性插值

## 最近邻插值法
目标像素的色值 等于 等比原图距离最近像素点的色值。
![](MdPicture/01.png)
![](MdPicture/02.png)

## 双线性插值
目标像素的色值 等于 等比原图像素点周围的四个像素点色值的双线性插值。

### 线性插值：
![](MdPicture/03.png)
### 像素转换：
![](MdPicture/04.png)
### 双线性插值应用公式推导：
![](MdPicture/05.png)
![](MdPicture/06.png)