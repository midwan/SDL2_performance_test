g++ -lSDL2 -lSDL2_image sdl2_renderer_test.cpp -o sdl2_renderer_test
g++ sdl2_pixelformat_test.cpp `pkg-config --cflags --libs sdl2` -o sdl2_pixelformat_test