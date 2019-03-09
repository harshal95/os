537make: main.o proc_creation_prog_exe.o text_parsing.o build_spec_graph.o build_spec_repr.o  
	gcc -g -o0 -o 537make main.o proc_creation_prog_exe.o text_parsing.o build_spec_graph.o build_spec_repr.o 

main.o: main.c text_parsing.h build_spec_graph.h	
	gcc -c -Wall -Wextra main.c

build_spec_graph.o: build_spec_graph.c proc_creation_prog_exe.h build_spec_repr.h 
	gcc -c -Wall -Wextra build_spec_graph.c

build_spec_repr.o: build_spec_repr.c build_spec_repr.h
	gcc -c -Wall -Wextra build_spec_repr.c

text_parsing.o: text_parsing.c build_spec_repr.h
	gcc -c -Wall -Wextra text_parsing.c

proc_creation_prog_exe.o: proc_creation_prog_exe.c proc_creation_prog_exe.h
	gcc -c -Wall -Wextra proc_creation_prog_exe.c

clean:
	rm -rf scan-build-out

scan-build: clean
	scan-build -o scan-build-out make

scan-view: scan-build
	firefox -new-window scan-build-out/*/index.html
