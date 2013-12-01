TARGET  := assign3
SRCS    := $(wildcard src/*.c)
OBJS    := $(addprefix bin/, $(notdir ${SRCS:.c=.o}))

CCFLAGS = -std=gnu99 -Wall -MMD
LDFLAGS = 
LIBS    = 

all:: ${TARGET} 

${TARGET}: ${OBJS} 
	${CC} ${LDFLAGS} -o bin/$@ $^ ${LIBS} 

${OBJS}: bin/%.o: src/%.c
	${CC} ${CCFLAGS} -o $@ -c $< 

clean:
	@-find ./bin -type f -not -name 'README' | xargs rm
	@-find ./bin/* -type d | xargs rm -r

