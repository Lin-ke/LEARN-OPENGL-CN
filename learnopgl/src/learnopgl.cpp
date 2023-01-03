

#include <iostream>
#include <glad/glad.h> // 管理opengl指针
#include <GLFW/glfw3.h> // 使用opengl并提供一些接口
#include <fstream>
#include <sstream>
#include "shader.h"
#include "stb_image.h"
#define uint32 unsigned int

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 当窗口大小改变时调用
void processInput(GLFWwindow *window);//返回esc按键是否正在被按下
void load_texturepic(const char* fpath,uint32 type); //加载纹理图片
int main()
{

	glfwInit(); //初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // GLFW配置(hint,value)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // glfw 3.3,核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//if use mac-os
	// 创建窗口对象

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);// 宽，高，标题
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // 设置为主线程上下文
	// 初始化Glad。Glad需要在主线程上下文
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// 定义视口，一般和窗口大小相同

	glViewport(0, 0, 800, 600);
	// 注册回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int indices[] = {   
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	// 纹理
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	//绑定顶点数组对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 绑定顶点缓冲区
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 元素缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性指针
// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//绑定纹理
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	load_texturepic("res/pic/container.jpg", GL_RGB);
	

	// 纹理2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	load_texturepic("res/pic/awesomeface.png", GL_RGBA);

	
	
	// 编译着色器
	Shader shader("res/shader/vertex.shader", "res/shader/fragment.shader");

	// 激活着色器
	shader.use();
	// 设置uniform
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); // 手动设置
	shader.setInt("texture2", 1); // 或者使用着色器类设置	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// 渲染代码

		// 清空颜色缓冲

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //背景
		glClear(GL_COLOR_BUFFER_BIT); //GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
				// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// 绑定VAO
		shader.use();
		glBindVertexArray(VAO);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// 使用线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




		//前缓冲保存着最终输出的图像，它会在屏幕上显示，而所有的的渲染指令都会在后缓冲上绘制。交换缓冲
		glfwSwapBuffers(window);
		//检查有没有触发什么事件
		glfwPollEvents();
	}
	// 释放资源
	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void setpos_callback(GLFWwindow* window, double width, double height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void load_texturepic(const char* fpath,uint32 type) {
	// 加载并生成纹理
	int width1, height1, nrChannels1;
	unsigned char *data1 = stbi_load(fpath, &width1, &height1, &nrChannels1, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, type, width1, height1, 0, type, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data1);

}