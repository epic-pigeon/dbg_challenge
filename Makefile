RAND = java -jar rand.jar
HEADERS = $(wildcard *.h)
C_SOURCES = $(filter-out $(wildcard *.rand.c), $(wildcard *.c))
OBJECTS = ${C_SOURCES:.c=.o}

SEED = 0 # specify with `make maze SEED=1234`

maze: ${OBJECTS}
	gcc -o $@ $^

run: maze
	./$<

%.o: %.rand.c ${HEADERS}
	gcc -Wno-nullability-completeness -Wno-undef-prefix -Wno-unused-command-line-argument \
		-fno-stack-protector -z execstack -no-pie -c $< -o $@

%.rand.c: %.c
	$(RAND) $< $@ $(SEED)

clean:
	rm -rf *.rand.c *.o maze

.SUFFIXES:
