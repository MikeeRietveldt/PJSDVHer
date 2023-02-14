src = $(wildcard */*.cpp) $(wildcard *.cpp)
flags = -Os

app: $(src)
	g++ $(src) $(flags) -o app
