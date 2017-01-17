sources = $(wildcard big/*.jpg)
targets := $(patsubst big/%, small/%, $(sources)) 

all: $(targets)

small/%.jpg : big/%.jpg
	convert -resize 20% $< $@

clean:
	rm -f small/*.jpg
