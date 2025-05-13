# Makefile portátil para programas OpenGL com GLUT em C e C++
# Uso: make nome-do-arquivo (sem extensão)

# Detecta o sistema operacional
UNAME_S := $(shell uname -s)

# Compiladores padrão
CC = gcc
CXX = g++
CFLAGS = -O2
CXXFLAGS = -O2

# Configurações específicas por sistema
ifeq ($(UNAME_S), Darwin)
    # macOS usa frameworks
    GL_LIBS = -framework GLUT -framework OpenGL
else
    # Linux usa bibliotecas tradicionais
    INCDIR = /usr/include
    LIBDIR = /usr/lib
    XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11
    GL_LIBS = -L$(LIBDIR) -lglut -lGLU -lGL -lm -lpng $(XLIBS)
endif

# Suportes de sufixo
.SUFFIXES: .c .cpp

# Regras de compilação
.c:
	$(CC) $(CFLAGS) $< -o $@ $(GL_LIBS)

.cpp:
	$(CXX) $(CXXFLAGS) $< -o $@ $(GL_LIBS)

# Limpeza
clean:
	rm -f *.o *~ a.out
