# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -I/opt/local/include -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -limageIO -lm
LFLAGS = -L$(LIBDIR) -L/opt/local/lib

# put all of the relevant include files here
_DEPS = ppmIO.h image.h fractals.h color.h graphics.h list.h polygon.h bezier.h lighting.h ray.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = xdisp_ray test_ray test_ray_forest test_ray_tex
# put a list of all the object files here for all executables (with .o endings)
_OBJ = imagetest.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i); done
	
test6a: $(ODIR)/test6a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
test6b: $(ODIR)/test6b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
test6cube: $(ODIR)/test6cube.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
xdisp: $(ODIR)/xdisp.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11

forest: $(ODIR)/forest_live.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11
	
test7b: $(ODIR)/test7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

spheregif: $(ODIR)/spheregif.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

teapot: $(ODIR)/teapot.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11

bezier_live: $(ODIR)/bezier_live.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11	
	
flag: $(ODIR)/flag.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11	

snowman: $(ODIR)/snowman.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11	
	
test9a: $(ODIR)/test9a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)	

test9d: $(ODIR)/test9d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)	

test_ray: $(ODIR)/test_ray.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)	

test_ray_forest: $(ODIR)/test_ray_forest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)	

xdisp_ray: $(ODIR)/xdisp_ray.o
	$(CC)  -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) -lX11	
	
test_ray_cel: $(ODIR)/test_ray_cel.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)		

test_ray_tex: $(ODIR)/test_ray_tex.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)		
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

