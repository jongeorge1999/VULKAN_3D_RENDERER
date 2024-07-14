#!/bin/bash

/usr/bin/glslc ../shaders/simple_shader.vert -o ../shaders/compiled_shaders/simple_shader.vert.spv
/usr/bin/glslc ../shaders/simple_shader.frag -o ../shaders/compiled_shaders/simple_shader.frag.spv
/usr/bin/glslc ../shaders/point_light.vert -o ../shaders/compiled_shaders/point_light.vert.spv
/usr/bin/glslc ../shaders/point_light.frag -o ../shaders/compiled_shaders/point_light.frag.spv
make buildlinux
./vulkan