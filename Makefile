SRC=sigattach.c
SO=sigattach.so

all: $(SO)

clean:
	rm -f $(SO)

$(SO): $(SRC)
	gcc -Wall -shared -fPIC -ggdb3 -O0 $^ -o $(SO) -ldl
