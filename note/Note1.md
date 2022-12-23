# 什么是OPENGL
1. 它仅仅是一个由Khronos组织制定并维护的规范(Specification)。
2. 严格规定了每个函数该如何执行，以及它们的输出值
3. 实际的OpenGL库的开发者通常是显卡的生产商

# 核心模式和立即模式
1. 早期的OpenGL使用立即渲染模式，固定渲染管线，不够灵活
2. 3.2开始使用核心模式：灵活效率高
3. 使用3.3因为核心架构没变

# 其他
1. 扩展：使用硬件支持的全新的现代特性

# 入门

## 窗口
### 关于glew-glad / glfw的实际工作
GLFW 提供 OpenGL, OpenGL ES 和 Vulkan 的桌面开发环境，是一个跨平台的开源库。
利用它可以创建窗口、处理键盘鼠标等事件。windows下它使用win32 api创建窗口，WGL关联OPENGL上下文。
Linux 下使用 X11创建窗口，GLX关联OPENGL上下文。MAC下使用cocoa创建窗口，AGL关联OPENGL上下文。
GLAD 的功能比较专一，专门加载OpenGL的函数指针。

窗口类的都是glfw..()，而实际渲染相关的代码是gl..()


## 三角形
### 管线
图形渲染管线接受一组3D坐标，通过着色器将之转变屏幕上的有色2D像素输出。
OpenGL着色器是用OpenGL着色器语言(OpenGL Shading Language, GLSL)写成的。


顶点着色器-形状装配-几何着色器-光栅化-片段着色器-测试与混合

1. **顶点着色器**(Vertex Shader)主要的目的是把3D坐标转为标准化设备坐标，同时顶点着色器允许我们对顶点属性进行一些基本处理。
2. **图元装配**(Primitive Assembly)阶段将顶点着色器输出的所有顶点作为输入（如果是GL_POINTS，那么就是一个顶点），并所有的点装配成指定图元的形状
3. **几何着色器**(Geometry Shader)把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。
4. **光栅化阶段**(Rasterization Stage)，这里它会把图元映射为最终屏幕上相应的像素，生成供片段着色器(Fragment Shader)使用的片段(Fragment)。
5. **裁切**(Clipping)。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。
6. **片段着色器**的主要目的是计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。
7. 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做Alpha测试和混合(Blending)阶段。这个阶段检验某个像素深度以判断是否改被丢弃，并检查alpha值以混合。
### VBO
顶点缓冲对象存储顶点信息。每个类型可以绑定一个vbo
### 两个着色器的编译
我们需要自己写顶点着色器和几何着色器。
### 着色器程序
多个着色器合并之后并最终链接完成的版本
### 链接顶点属性
### VAO
可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了.
VAO也会自动绑定IBO
![](2022-12-23-15-59-26.png)

### GLSL
着色器的开头总是要声明版本，接着是输入和输出变量、uniform和main函数。
![](2022-12-23-17-02-05.png)
每个输入变量也叫顶点属性(Vertex Attribute)
### 数据类型
int、float、double、uint和bool，vector和matrix

#### vector

类型 	含义
vecn 	包含n个float分量的默认向量
bvecn 	包含n个bool分量的向量
ivecn 	包含n个int分量的向量
uvecn 	包含n个unsigned int分量的向量
dvecn 	包含n个double分量的向量

向量允许重组：vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

### I/O
语法：
in/out type name;
每一级shader的out会进入下一级作为in，需要名称和类型相同。
对于顶点缓冲区，特别地需要layout(index)以表达顶点属性输入。
### uniform 变量
uniform type name; 此名称是全局的，并能够从cpu函数中修改。