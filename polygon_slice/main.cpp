#include "header.h"



GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수 
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid_shape{ 3,4 };
std::uniform_int_distribution<int> uid_dir{ 1,4 };
std::uniform_real_distribution<float> urd_color{ 0, 1 };


Shape shape[ARRAYSIZE];
Line line;




void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
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


	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);

	glm::mat4 Tx0 = glm::mat4(1.0f);
	glm::mat4 Tx1 = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);

	Tx1 =
		glm::translate(Tx1, glm::vec3(shape[0].x_translation, 0.0, 0.0)) *
		glm::translate(Tx1, glm::vec3(0.0, shape[0].y_translation, 0.0));


	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); //--- 투영 공간 설정: [-100.0, 100.0]
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
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

	//--- 세이더 프로그램에서 modelTransform 변수 위치 가져오기
	
	

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
		// 고정점을 현재 위치로 설정
		line.obj[0][0] = convert_x(x);
		line.obj[0][1] = convert_y(y);
		line.obj[0][2] = 0;

		// 끝점 초기화 (이동 시 변경됨)
		line.obj[1][0] = line.obj[0][0];
		line.obj[1][1] = line.obj[0][1];
		line.obj[1][2] = 0;

		line.line_flag = true; // 선 그리기를 활성화
		glutPostRedisplay();

		//std::cout << "Fixed point set - x: " << line.obj[0][0] << ", y: " << line.obj[0][1] << '\n';
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		line.line_flag = false; // 마우스 버튼에서 손을 떼면 선 그리기 비활성화
		line_line(line.obj[0][0], line.obj[0][1], line.obj[1][0], line.obj[1][1], shape[0].obj_cord[0][0], shape[0].obj_cord[0][1], shape[0].obj_cord[1][0], shape[0].obj_cord[1][1]);
		glutPostRedisplay();
	}
}

void Motion(int x, int y) {
	if (line.line_flag) {
		// 현재 마우스 위치를 끝점으로 업데이트
		line.obj[1][0] = convert_x(x);
		line.obj[1][1] = convert_y(y);

		//std::cout << "Moving - x: " << line.obj[1][0] << ", y: " << line.obj[1][1] << '\n';

		glutPostRedisplay(); // 화면 다시 그리기 요청
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


//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	glBindVertexArray(vao); //--- VAO를 바인드하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);


}


void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);

}



void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}


void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
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