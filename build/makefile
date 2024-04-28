CFLAGS = -std=c++17 -O3
LDFLAGS = -lglfw3 -lvulkan-1

buildAll:
	build compileShaders

build:
	g++ $(CFLAGS) -I ../include -L ../lib -o vulkan ../src/*.cpp $(LDFLAGS)

compileShaders:
	compile.bat