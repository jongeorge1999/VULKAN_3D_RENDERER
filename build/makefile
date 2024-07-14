CFLAGS = -std=c++17 -O3
LDFLAGSLINUX = -lglfw -lvulkan
LDFLAGSWINDOWS = -lglfw3 -lvulkan-1

buildlinux:
	g++ $(CFLAGS) -I ../include -L ../lib -o vulkan ../src/*.cpp ../src/systems/*.cpp $(LDFLAGSLINUX)

buildwindows:
	g++ $(CFLAGS) -I ../include -L ../lib -o vulkan ../src/*.cpp ../src/systems/*.cpp $(LDFLAGSWINDOWS)

clean:
	rm vulkan vulkan.exe