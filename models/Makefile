scadsource = $(shell find . -name '*.scad' -a ! -name '*def*.scad' ! -name '*test*.scad')
models = $(scadsource:%.scad=%.off)

all: $(models)

$(models): %.off: %.scad
	openscad $< -o $@

clean:
	rm -f $(models)
