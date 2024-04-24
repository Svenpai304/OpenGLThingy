#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void initWindow(GLFWwindow*& window);
void processInput(GLFWwindow* window);
void loadFile(const char* filename, char** output);

void createTriangle(GLuint& vao, int& size);
void createShaders();
void createProgram(GLuint& program, const char* vertex, const char* fragment);

GLuint simpleProgram;

int main() {
	std::cout << "we ballin" << std::endl;

	GLFWwindow* window;
	initWindow(window);

	GLuint triVAO;
	int triSize;
	createTriangle(triVAO, triSize);
	createShaders();

	// Create viewport
	glViewport(0, 0, 1920, 1080);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Rendering
		glClearColor(0.0f, 0.7f, 0.65f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(simpleProgram);

		glBindVertexArray(triVAO);
		glDrawArrays(GL_TRIANGLES, 0, triSize);

		// Swap & poll
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void initWindow(GLFWwindow*& window) {
	// Set GLFW version
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open window
	window = glfwCreateWindow(1920, 1080, "Graphics!!!", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void loadFile(const char* filename, char** output) {
	std::ifstream file(filename, std::ios::binary);

	if (file.is_open()) {
		file.seekg(0, file.end);
		int total = file.tellg();
		*output = new char[total + 1];

		file.seekg(0, file.beg);
		file.read(*output, total);

		(*output)[total] = '\0';
		file.close();
	}
}

void createTriangle(GLuint& vao, int& size) {
	
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	size = sizeof(vertices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void createShaders() {
	createProgram(simpleProgram, "shaders/shaderVertex.shader", "shaders/shaderFragment.shader");
}

void createProgram(GLuint& programID, const char* vertex, const char* fragment) {
	char* vertexSrc;
	char* fragmentSrc;
	loadFile(vertex, &vertexSrc);
	loadFile(fragment, &fragmentSrc);

	GLuint vertexShaderID, fragmentShaderID;

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexSrc, nullptr);
	glCompileShader(vertexShaderID);

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentShaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
		std::cout << "Error compiling vertex shader: " << infoLog << std::endl;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		std::cout << "Error linking program: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	delete vertexSrc;
	delete fragmentSrc;
}
