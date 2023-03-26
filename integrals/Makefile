LIBS = -lm
C_NAME = main
EXEC_NAME = exec
ASM_NAME = funcs

clean: all
	rm -rf *.o
all:
	gcc -m32 -c ${C_NAME}.c -o ${C_NAME}.o
	nasm -f elf -o ${ASM_NAME}.o ${ASM_NAME}.asm
	gcc -m32 ${C_NAME}.o ${ASM_NAME}.o ${LIBS} -o ${EXEC_NAME}

start: clean
	./${EXEC_NAME}

test_integral: clean
	./${EXEC_NAME} -i

test_root: clean
	./${EXEC_NAME} -r

test_root_manual: clean
	./${EXEC_NAME} --test-root-manual

test_integral_manual: clean
	./${EXEC_NAME} --test-integral-manual

