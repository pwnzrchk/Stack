CC = g++
CFLAGS = -Og -ggdb -Wall -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations   \
		 -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy     \
		 -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2     \
		 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith       \
		 -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo           \
		 -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef   \
		 -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-range 			     \
		 -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast 			 \
		 -Wno-varargs -Wstack-protector -Wsuggest-override -Wbounds-attributes-redundant \
		 -Wlong-long -Wopenmp -fcheck-new -fsized-deallocation -fstack-protector 		 \
		 -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-protector  \
		 -fPIE -Werror=vla -fsanitize=address


ASM = Assembler/main.cpp Assembler/translator.cpp
ASM_OBJ = $(ASM:.cpp=.o)

$(ASM_OBJ): %.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@


COMM = Common/fileFunc.cpp Common/countSymb.cpp Common/swags.cpp
COMM_OBJ = $(COMM:.cpp=.o)

$(COMM_OBJ): %.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@


MAIN_SOURSE = Proc/main.cpp
MAIN_OBJ = $(MAIN_SOURSE:.cpp=.o)

$(MAIN_OBJ): %.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@


PROC = Proc/proccesor.cpp
PROC_OBJ = $(PROC:.cpp=.o)

$(PROC_OBJ): %.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@



all: $(COMM_OBJ) $(MAIN_OBJ) $(PROC_OBJ)
	@$(CC) $(CFLAGS) $^  -o $@ -lm

process: $(COMM_OBJ) $(MAIN_OBJ) $(PROC_OBJ)
	@echo "Processing.."
	@$(CC) $(CFLAGS) $^ -o $@ -lm

asm: $(ASM_OBJ) $(COMM_OBJ)
	@echo "Assembly..."
	@$(CC) $(CFLAGS) $(ASM_OBJ) $(COMM_OBJ) -o $@ -lm

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "cleaning..."
	@rm -f $(MAIN_OBJ) $(PROC_OBJ) $(ASM_OBJ) $(COMM_OBJ) all

.PHONY: clean
