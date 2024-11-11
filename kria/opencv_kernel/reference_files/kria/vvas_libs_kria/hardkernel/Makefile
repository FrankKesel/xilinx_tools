
CPPFLAGS += $(addprefix -I, $(INC))

LDLIBS += $(addprefix -l, $(LIBS))

NAME = hardkernel

lib$(NAME).so: $(NAME).cpp.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.cpp.o: %.cpp $(DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<
	
.PHONY: clean
clean:
	rm *.o
