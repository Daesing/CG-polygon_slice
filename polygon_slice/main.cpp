#include "header.h"



GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ���� 
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid_shape{ 3,4 };
std::uniform_int_distribution<int> uid_dir{ 1,4 };
std::uniform_real_distribution<float> urd_color{ 0, 1 };


Shape shape[ARRAYSIZE];
Line line;




void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(60, TimerFunction, 1);
	glutMainLoop();
}




GLvoid drawScene()
{
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);

	glm::mat4 Tx0 = glm::mat4(1.0f);
	glm::mat4 Tx1 = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);

	Tx1 =
		glm::translate(Tx1, glm::vec3(shape[0].x_translation, 0.0, 0.0)) *
		glm::translate(Tx1, glm::vec3(0.0, shape[0].y_translation, 0.0));


	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); //--- ���� ���� ����: [-100.0, 100.0]
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- ���� ��ȯ �� ����
	//glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


	glBindVertexArray(vao);


	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Tx0));

	if (line.line_flag) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line.obj), &line.obj[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line.color), &line.color[0]);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//--- ���̴� ���α׷����� modelTransform ���� ��ġ ��������
	
	

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Tx1));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shape->obj), &shape[0].obj);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shape->color), &shape[0].color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, shape[0].shape_type);


	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// �������� ���� ��ġ�� ����
		line.obj[0][0] = convert_x(x);
		line.obj[0][1] = convert_y(y);
		line.obj[0][2] = 0;

		// ���� �ʱ�ȭ (�̵� �� �����)
		line.obj[1][0] = line.obj[0][0];
		line.obj[1][1] = line.obj[0][1];
		line.obj[1][2] = 0;

		line.line_flag = true; // �� �׸��⸦ Ȱ��ȭ
		glutPostRedisplay();

		//std::cout << "Fixed point set - x: " << line.obj[0][0] << ", y: " << line.obj[0][1] << '\n';
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		line.line_flag = false; // ���콺 ��ư���� ���� ���� �� �׸��� ��Ȱ��ȭ
		line_line(line.obj[0][0], line.obj[0][1], line.obj[1][0], line.obj[1][1], shape[0].obj_cord[0][0], shape[0].obj_cord[0][1], shape[0].obj_cord[1][0], shape[0].obj_cord[1][1]);
		glutPostRedisplay();
	}
}

void Motion(int x, int y) {
	if (line.line_flag) {
		// ���� ���콺 ��ġ�� �������� ������Ʈ
		line.obj[1][0] = convert_x(x);
		line.obj[1][1] = convert_y(y);

		//std::cout << "Moving - x: " << line.obj[1][0] << ", y: " << line.obj[1][1] << '\n';

		glutPostRedisplay(); // ȭ�� �ٽ� �׸��� ��û
	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 's':
		shape[0].reset_shape(uid_shape(dre),uid_dir(dre));
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

	
	
	glutPostRedisplay();
}

void TimerFunction(int value) {
	switch (value) {
	case 1:

		shape[0].update_pos();
		std::cout << "obj_cord_x: " << shape[0].obj_cord[0][0]<<'\n';

		shape[0].move();
		if(shape[0].x_translation > 2 || shape[0].x_translation < -2)
			shape[0].reset_shape(uid_shape(dre), uid_dir(dre));

		
		break;

	}

	glutPostRedisplay();
	glutTimerFunc(60, TimerFunction, 1);
}


//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);


}


void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);

}



void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}


void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}