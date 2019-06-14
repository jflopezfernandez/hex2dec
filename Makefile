
vpath %.c src
vpath %.h include

CP                 = cp -f -u
RM                 = rm -f

OBJS               = main.o

COMPILER           = $(CC)
PREPROCESSOR_FLAGS = -I include -D_POSIX_C_SOURCE -D_GNU_SOURCE $(CPPFLAGS)
COMPILATION_FLAGS  = $(CFLAGS)

LINKER             = $(COMPILER)
LINKER_FLAGS       = $(LDFLAGS)
LIBRARIES          = -lm -lgmp $(LIBS)

COMPILE            = $(COMPILER) $(PREPROCESSOR_FLAGS) $(COMPILATION_FLAGS) -c
LINK               = $(LINKER) $(COMPILATION_FLAGS) $(LINKER_FLAGS) $(LIBRARIES)
OUTPUT             = -o $@
DEPENDENCIES       = $^

TARGET   = hex2dec

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) $(OUTPUT) $(DEPENDENCIES)

%.o: %.c
	$(COMPILE) $(OUTPUT) $(DEPENDENCIES)

.PHONY: clean
clean:
	$(RM) ./*.{o,asm,lst} $(TARGET)

.PHONY: install
install: $(TARGET)
	$(CP) ./$(TARGET) /usr/bin

.PHONY: uninstall
uninstall:
	$(RM) /usr/bin/$(TARGET)

