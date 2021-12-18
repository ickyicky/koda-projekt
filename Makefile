GPP=g++
CPPFLAGS=
LDFLAGS=
LDLIBS=
OBJDIR=obj
CLANG_FORMATTER=clang-format

CODER_SRCS=$(shell find coder_lzss -type f \( -name "*.cpp" ! -name "*main.cpp" \))
CODER_SRCDIRS=$(shell find coder_lzss -type d)
CODER_OBJFILES=$(patsubst %.cpp, $(OBJDIR)/%.o, $(CODER_SRCS))
CODER_OBJDIRS=$(patsubst %, $(OBJDIR)/%, $(CODER_SRCDIRS))
CODER_TARGET=coder

all: coder decoder

$(CODER_TARGET): $(CODER_OBJDIRS)
$(CODER_TARGET): $(CODER_OBJFILES)
$(CODER_TARGET): coder_lzss/main.cpp
$(CODER_TARGET):
	${GPP} -o ${CODER_TARGET} $(CPPFLAGS) $(CODER_OBJFILES) $(LDFLAGS) $(LDLIBS) coder_lzss/main.cpp

DECODER_SRCS=$(shell find decoder_lzss -type f \( -name "*.cpp" ! -name "*main.cpp" \))
DECODER_SRCDIRS=$(shell find decoder_lzss -type d)
DECODER_OBJFILES=$(patsubst %.cpp, $(OBJDIR)/%.o, $(DECODER_SRCS))
DECODER_OBJDIRS=$(patsubst %, $(OBJDIR)/%, $(DECODER_SRCDIRS))
DECODER_TARGET=decoder

$(DECODER_TARGET): $(DECODER_OBJDIRS)
$(DECODER_TARGET): $(DECODER_OBJFILES)
$(DECODER_TARGET): decoder_lzss/main.cpp
$(DECODER_TARGET):
	${GPP} -o ${DECODER_TARGET} $(CPPFLAGS) $(DECODER_OBJFILES) $(LDFLAGS) $(LDLIBS) decoder_lzss/main.cpp

$(OBJDIR)/%.o: %.cpp
	$(GPP) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c $< -o $@

$(OBJDIR)/%:
	mkdir -p $@

format:
	for SFILE in $$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) ); \
	do \
		${CLANG_FORMATTER} -style=file -i $$SFILE; \
	done
