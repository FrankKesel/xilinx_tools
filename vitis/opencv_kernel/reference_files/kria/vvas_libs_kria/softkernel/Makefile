
INC += /usr/include/opencv4

CPPFLAGS  += $(addprefix -I, $(INC))

LDLIBS += /usr/lib/aarch64-linux-gnu/libopencv_imgproc.so.4.5.4d
LDLIBS += /usr/lib/aarch64-linux-gnu/libopencv_core.so.4.5.4d
LDLIBS += $(addprefix -l, $(LIBS))

NAME = softkernel

lib$(NAME).so: $(NAME).cpp.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.cpp.o: %.cpp $(DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm *.o
