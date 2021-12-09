GPP=g++
CPPFLAGS=
LDFLAGS=
LDLIBS=
OBJDIR=obj
CLANG_FORMATTER=clang-format

SRCS=$(shell find src -type f \( -name "*.cpp" ! -name "*main.cpp" \))
SRCDIRS=$(shell find src -type d)
OBJFILES=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
OBJDIRS=$(patsubst %, $(OBJDIR)/%, $(SRCDIRS))

TARGET=koda

$(TARGET): $(OBJDIRS)
$(TARGET): $(OBJFILES)
$(TARGET): src/main.cpp
$(TARGET):
	${GPP} -o ${TARGET} $(CPPFLAGS) $(OBJFILES) $(LDFLAGS) $(LDLIBS) src/main.cpp

$(OBJDIR)/%.o: %.cpp
	$(GPP) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c $< -o $@

$(OBJDIR)/%:
	mkdir -p $@

format:
	for SFILE in $$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) ); \
	do \
		${CLANG_FORMATTER} -style=file -i $$SFILE; \
	done
