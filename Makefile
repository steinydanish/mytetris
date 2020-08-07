PROGRAM = proj1

INCL_DIRS = \
			-I"${USERPROFILE}\source\glfw-3.3.2.bin.WIN64\glfw-3.3.2.bin.WIN64\include\GLFW" \
			-I"${USERPROFILE}\source\glew-2.1.0-win32\glew-2.1.0\include\GL" \
			-I"${USERPROFILE}\source\repos\std_image\stb"
			
LIB_DIRS = \
			-L"${USERPROFILE}\source\glew-2.1.0-win32\glew-2.1.0\lib\Release\x64" \
			-L"${USERPROFILE}\source\glfw-3.3.2.bin.WIN64\glfw-3.3.2.bin.WIN64\lib-mingw-w64"

COMMON_LIBS = -lglu32 \
			  -lopengl32 \
			  -lglfw3 \
			  -lglew32 \
			  -lgdi32 \
			  -mwindows

.PHONY: all clean run

all: tetrs3.exe

tetrs3.exe: FILE_OPERATOR.o Program.o ShaderBank.o Application.o proj1.o VertexArray.o BufferObject.o
	g++ proj1.o Application.o Program.o ShaderBank.o FILE_OPERATOR.o VertexArray.o BufferObject.o -o tetrs3.exe ${LIB_DIRS} ${COMMON_LIBS}

Program.o: Program.cpp Program.h
	g++ -c Program.cpp ${INCL_DIRS}

FILE_OPERATOR.o: FILE_OPERATOR.cpp FILE_OPERATOR.h
	g++ -c FILE_OPERATOR.cpp ${INCL_DIRS}

ShaderBank.o: ShaderBank.cpp ShaderBank.h
	g++ -c ShaderBank.cpp ${INCL_DIRS}

proj1.o: proj1.cpp proj1.h
	g++ -c proj1.cpp ${INCL_DIRS}
	
Application.o: Application.cpp Application.h
	g++ -c Application.cpp ${INCL_DIRS}

VertexArray.o: VertexArray.cpp VertexArray.h
	g++ -c VertexArray.cpp ${INCL_DIRS}

BufferObject.o: BufferObject.cpp BufferObject.h
	g++ -c BufferObject.cpp ${INCL_DIRS}


run:
	${USERPROFILE}\source\repos\tetrs3\tetrs3.exe

clean:
	rm *.o *.exe