g++ -c -Wall application.cpp -L/usr/local/lib -I/usr/local/include  -framework OpenGL -lglfw3 -lglew -lpng \
  && g++ -c -Wall main.cpp -L/usr/local/lib -I/usr/local/include -framework OpenGL -lglfw3 -lglew \
  && g++ application.o main.o -L/usr/local/lib -I/usr/local/include -framework OpenGL -lglfw3 -lglew -lpng -o juliaglsl \
  && ./juliaglsl 
