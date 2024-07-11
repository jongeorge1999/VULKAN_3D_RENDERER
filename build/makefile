CFLAGS = -std=c++17 -O3
LDFLAGSLINUX = -lglfw -lvulkan
LDFLAGSWINDOWS = -lgfw3 -lvulkan-1

buildAll:
	build compileShaders

buildlinux:
	g++ $(CFLAGS) -I ../include -L ../lib -o vulkan ../src/*.cpp $(LDFLAGSLINUX)

buildwindows:
	g++ $(CFLAGS) -I ../include -L ../lib -o vulkan ../src/*.cpp $(LDFLAGSWINDOWS)

compileShaders:
	compile.bat