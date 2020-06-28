PROGRAM = proj1

INCL_DIRS = -I"${USERPROFILE}\source\repos\imgui\imgui"

proj1.exe: proj1.o
	g++ proj1.o -o proj1.exe

proj1.o: proj1.cpp proj1.h
	g++ -c proj1.cpp ${INCL_DIRS}

clean:
	rm *.o proj1.exe