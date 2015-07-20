CXX = arm-none-eabi-g++
CXXFLAGS = -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -fpic \
	-ffreestanding -fno-exceptions -std=c++14 -O3 -Wall -Wextra
SFLAGS = -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -fpic -ffreestanding

all:
	$(CXX) $(CXXFLAGS) -c src/kernel.cpp      -o obj/kernel.o
	$(CXX) $(CXXFLAGS) -c src/mmio.cpp        -o obj/mmio.o
	$(CXX) $(CXXFLAGS) -c src/mailbox.cpp     -o obj/mailbox.o
	$(CXX) $(CXXFLAGS) -c src/framebuffer.cpp -o obj/framebuffer.o
	$(CXX) $(CXXFLAGS) -c src/memory.cpp      -o obj/memory.o
	$(CXX) $(SFLAGS)   -c src/boot.S          -o obj/boot.o

	arm-none-eabi-g++ -T src/linker.ld -o bin/kernel7.elf -ffreestanding -O3 \
		-nostdlib obj/boot.o obj/kernel.o
	arm-none-eabi-objcopy bin/kernel7.elf -O binary kernel7.img
