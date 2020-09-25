#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>

#include <GLEW\glew.h>
#include <GLFW\glfw3.h>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo = 0;

const int rowCount = 15, colCount = 15;
float rowEntries[rowCount];
float colEntries[colCount];
float points[rowCount * colCount * 3];
float dir[rowCount * colCount * 3];

int lineCount = 0;
float lines[rowCount * rowCount * colCount * colCount * 6];


void GenPoints() {
	for (int i = 0; i < rowCount; i++) {
		rowEntries[i] = ((rand() % 200) - 100) / 100.00f;
	}
	for (int i = 0; i < colCount; i++) {
		colEntries[i] = ((rand() % 200) - 100) / 100.00f;
	}

	int i = 0;
	for (int r = 0; r < rowCount; r++) {
		for (int c = 0; c < colCount; c++) {
			points[i] = rowEntries[r];
			dir[i] = ((rand() % 200) - 100) / 50000.00f;
			i++;
			points[i] = colEntries[c];
			dir[i] = ((rand() % 200) - 100) / 50000.00f;
			i++;
			points[i] = -1.0f;
			dir[i] = ((rand() % 200) - 100) / 50000.00f;
			i++;
		}
	}
}

void UpdatePoints() {
	for(int i = 0; i < rowCount*colCount*3; i++){
		points[i] += dir[i];
		if (points[i] > 1 || points[i] < -1) {
			dir[i] *= -1.0;
		}
	}
}

float dist(float x1, float y1, float z1, float x2, float y2, float z2) {
	float result = 0;
	result += (x1 - x2) * (x1 - x2);
	result += (y1 - y2) * (y1 - y2);
	result += (z1 - z2) * (z1 - z2);
	return sqrtf(result);
}

void GenLines() {
	int count = 0;
	for (int i = 0; i < rowCount * colCount * 3; i+=3) {
		for (int j = i + 3; j < rowCount * colCount * 3; j += 3) {
			if (dist(points[i], points[i + 1], points[i + 2], points[j], points[j + 1], points[j + 2]) < 0.31f) {
				lines[count] = points[i];   count++;
				lines[count] = points[i+1]; count++;
				lines[count] = points[i+2]; count++;
				lines[count] = points[j];   count++;
				lines[count] = points[j+1]; count++;
				lines[count] = points[j+2]; count++;
			}
		}
	}

	lineCount = count / 6;
}

void PrintShaderLog(GLuint shader) {
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

bool CheckOpenGLErr() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}

	return glErr;
}

string ReadShaderSource(const char* filepath) {
	string content;
	ifstream fileStream(filepath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint CreateShaderProgram() {
	GLint vCompiled;
	GLint fCompiled;

	string vertShaderStr = ReadShaderSource("GLSL/vs.glsl");
	string fragShaderStr = ReadShaderSource("GLSL/fs.glsl");

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);

	CheckOpenGLErr();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vCompiled);
	if (vCompiled != 1) {
		cout << "vertex shader compilation failed" << endl;
	}
	else {
		cout << "vert success" << endl;
	}
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fCompiled);
	if (fCompiled != 1) {
		cout << "fragment shader compilation failed" << endl;
	}
	else {
		cout << "frag success" << endl;
	}

	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);

	return vfProgram;
}

void init(GLFWwindow* window) {
	renderingProgram = CreateShaderProgram();
	glEnable(GL_DEPTH_TEST);
	GenPoints();
	//GenLines();
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glBindVertexArray(vao[0]);
	glPointSize(5.0f);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_LINES, 0, 8);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, rowCount * colCount * 3 * sizeof(float), points, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, rowCount * colCount);
	glBufferData(GL_ARRAY_BUFFER, lineCount * 6 * sizeof(float), lines, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, lineCount*2);
	GenLines();
	UpdatePoints();
}


int main() {
	
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		cout << "glew err" << endl;
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {

		display(window, glfwGetTime());

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}