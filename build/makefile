CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw3 -lvulkan-1

VulkanTest:
	g++ $(CFLAGS) -I ../include -L ../lib -o VulkanTest ../src/main.cpp $(LDFLAGS)

test: VulkanTest
	./VulkanTest