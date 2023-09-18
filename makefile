src=./src
files=$(src)/token.cc $(src)/node.cc $(src)/codegen.cc $(src)/main.cc

debug:
	g++ -g -std=c++17 -Wextra -o smth $(files) -fsanitize=undefined

debug_stdin:
	g++ -g -std=c++17 -Wextra -o smth $(files) -D Smth_dbg -fsanitize=undefined

release:
	g++ -std=c++17 -O3 -o smth $(files)

clean:
	rm smth tmp*

test:
	make debug
	./test.sh
	make clean

.PHONY: clean test