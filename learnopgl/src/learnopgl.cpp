

#include <iostream>
#include <glad/glad.h> // 管理opengl指针
#include <GLFW/glfw3.h> // 使用opengl并提供一些接口
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 当窗口大小改变时调用
void processInput(GLFWwindow *window);//返回esc按键是否正在被按下
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
	
	
	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// 渲染代码
		// 清空颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT







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

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}