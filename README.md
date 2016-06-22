#JGraphics
This project is a graphic editor which is curriculum design for Principle of Computer Graphics.

It is based on OpenGL and WIN32 API.

You can control from the user-friendly graphics interface.

##Function

* draw
    * circle
    * polygon
    * line
    * bezier curve
    * pencil mode
* change
    * cut(in/out)
	* fill
    * translation
    * rotation
    * scaling
* import/export

##Specil

* keep a 2d-array to save the pixels.When mouse move(draw frequently),draw graphs by OpenGL.Otherwise,draw from the 2d-array to avoid repeated calculate.
* save as a binary file,the format is defined myself.and I set a magic number to judge file format.
* user-friendly graphics interface

##bug record

* 在一些模式下，点击clear，再在主面板进行点击会崩溃。(fixed)
* 裁剪模式，绘制了裁剪多边形后不进行裁剪，切换到其它模式,遗留的红色多边形可能会造成错误。(fixed)
* 裁剪模式，裁剪窗口边框(红色)覆盖了绿色点后，再将裁剪窗口移开，绿色不会恢复。(fixed)
* 裁剪模式，对某些顶点判断错误。(fixed)
* 由于使用了int表示坐标，将多边形过度缩小再放大会变形。(fixed:进行了较大的改动:使用了float表示坐标，几乎修改了所有的文件)