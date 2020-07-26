PROGRAM = proj1

INCL_DIRS = \
			-I"${USERPROFILE}\source\glfw-3.3.2.bin.WIN64\glfw-3.3.2.bin.WIN64\include\GLFW" \
			-I"${USERPROFILE}\source\glew-2.1.0-win32\glew-2.1.0\include\GL"
			
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

tetrs3.exe: proj1.o Application.o
	g++ proj1.o Application.o -o tetrs3.exe ${LIB_DIRS} ${COMMON_LIBS}

proj1.o: proj1.cpp proj1.h
	g++ -c proj1.cpp ${INCL_DIRS}
	
Application.o: Application.cpp Application.h
	g++ -c Application.cpp ${INCL_DIRS}

run:
	${USERPROFILE}\source\repos\tetrs3\tetrs3.exe

clean:
	rm *.o *.exe