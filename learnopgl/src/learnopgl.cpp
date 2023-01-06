

#include <iostream>
#include <glad/glad.h> // 管理opengl指针
#include <GLFW/glfw3.h> // 使用opengl并提供一些接口
#include <fstream>
#include <sstream>
#include "shader.h"
#include "camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#define uint32 unsigned int
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__,__LINE__))
void GLClearError() {
	while (glGetError() != GL_NO_ERROR) {
	}
}
bool GLLogCall(const char* function, const char* filename, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "error : " << function << " line:" << line << " error: " << error << " " << std::endl;
		return false;
	}
	return true;
}

#define uint32 unsigned int

// 屏幕设置
const float screenWidth = 800;
const float screenHeight = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 当窗口大小改变时调用
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn); // 鼠标
// glfw: 鼠标滚轮回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);//键盘输入回调
void load_texturepic(const char* fpath,uint32 type); //加载纹理图片
int main()
{

	//// 模型矩阵：物体坐标-世界坐标
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//// 观察矩阵：世界坐标-摄像坐标
	//glm::mat4 view = glm::mat4(1.0f);
	//// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//// 投影矩阵
	//glm::mat4 projection = glm::mat4(1.0f);
	//// FOV, aspect, n ,f
	//projection = glm::perspective(glm::radians(90.0f), screenWidth / screenHeight, 0.1f, 100.0f);
	//std::cout << glm::to_string(projection) << std::endl;
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

	GLCall(glViewport(0, 0, screenWidth, screenHeight));
	// 注册回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// 捕获鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 开启深度测试
	glEnable(GL_DEPTH_TEST); 
	//unsigned int indices[] = {
	//	// 注意索引从0开始! 
	//	// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
	//	// 这样可以由下标代表顶点组合成矩形

	//	0, 1, 3, // 第一个三角形
	//	1, 2, 3  // 第二个三角形
	//};
	//// 纹理
	//float vertices[] = {
	//	// positions               // texture coords
	//	 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right
	//	 0.5f, -0.5f, 0.0f,	   1.0f, 0.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
	//};
	float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//绑定顶点数组对象
	unsigned int VAO;
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));
	// 绑定顶点缓冲区
	unsigned int VBO;
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	//// 元素缓冲对象
	//unsigned int EBO;
	//GLCall(glGenBuffers(1, &EBO));
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// 设置顶点属性指针
	// 位置属性
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	// 纹理属性
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));

	//绑定纹理
	unsigned int texture1;
	GLCall(glGenTextures(1, &texture1));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
	// 为当前绑定的纹理对象设置环绕、过滤方式
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	stbi_set_flip_vertically_on_load(true);

	load_texturepic("res/pic/container.jpg", GL_RGB);
	
	glm::vec4 v = glm::vec4(0.0f, 0, 1, 1);
	auto lc = glm::lookAt(glm::vec3(1, 1, 0.0f), glm::vec3(0.0f, 0, 0), glm::vec3(0, 0, 1.0f));
	std::cout << glm::to_string(lc) << glm::to_string(lc*v);
	// 纹理2
	unsigned int texture2;
	GLCall(glGenTextures(1, &texture2));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture2));
	// 为当前绑定的纹理对象设置环绕、过滤方式
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	load_texturepic("res/pic/awesomeface.png", GL_RGBA);
	//// 摄像机测试
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//// 摄像机方向
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	//// 右方向（格拉姆施密特）
	//// trick : 用一个0,0,1和摄像机方向cross product（右手系）
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//// 上方向
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	// view matrix

	// 编译着色器
	Shader shader("res/shader/vertex.shader", "res/shader/fragment.shader");

	// 激活着色器
	shader.use();
	// 设置transform uniform
	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");

	// 设置uniform
	GLCall(glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0)); // 手动设置
	shader.setInt("texture2", 1); // 或者使用着色器类设置	
	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// 渲染代码
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 清空颜色缓冲

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f)); //背景
		
		// 清除深度缓冲和颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 绑定纹理

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture2));
		shader.use();
		// create transformations
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);

		projection = glm::perspective(glm::radians(camera.Zoom), screenWidth/screenHeight, 0.1f, 100.0f);
		// pass transformation matrices to the shader
		shader.setMat4("projection", projection);// 投影矩阵一般不变，无需每帧设置
		shader.setMat4("view", view);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// 使用线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		 // render boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



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
	GLCall(glViewport(0, 0, width, height));
}
void setpos_callback(GLFWwindow* window, double width, double height)
{
	GLCall(glViewport(0, 0, width, height));
}
void processInput(GLFWwindow *window)
{
	// esc关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// wasd移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw 鼠标回调
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: 鼠标滚轮回调
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void load_texturepic(const char* fpath,uint32 type) {
	// 加载并生成纹理
	int width1, height1, nrChannels1;
	unsigned char *data1 = stbi_load(fpath, &width1, &height1, &nrChannels1, 0);
	if (data1)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, type, width1, height1, 0, type, GL_UNSIGNED_BYTE, data1));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data1);

}