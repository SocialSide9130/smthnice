debug:
	g++ -g -std=c++17 -Wextra -o smth token.cc node.cc codegen.cc main.cc -D Smth_dbg -fsanitize=undefined

release:
	g++ -std=c++17 -O3 -o smth token.cc node.cc codegen.cc main.cc

clean:
	rm smth tmp*

.PHONY: clean