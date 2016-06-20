GCC=gcc
#CPPFLAGS = -g -fPIC -Wall -W -Winline -Werror  -Wno-unused-parameter -Wno-unused-function -std=c99
CPPFLAGS = -g -fPIC -std=c99

INCLUDE_PATH=-I. \

LIB_PATH=-L. -lpthread

TEST=test_resp
SO=libresp.so

all : $(TEST) $(SO) output;
$(TEST) : \
	test_resp.o \
	resp.o \

	$(GCC) -o $@ $^  $(INCLUDE_PATH) $(LIB_PATH) 
	
$(SO) : \
	resp.o \

	$(GCC) -shared -Wl,-soname,$@ -o $@ $^

%.o : %.c
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_PATH)

output :
	rm -rf ../output
	mkdir -p ../output 
	cp resp.h ../output
	cp $(SO) ../output
	cp $(TEST) ../output

.PHONY: clean 
clean:
	rm -f *.o $(TEST) $(SO);
	rm -fr ../output;
