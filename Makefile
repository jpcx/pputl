all: docs

docs: gen/build.js gen/Doxyfile
	node $<

clean:
	find .               \
		! -path './.git*'  \
		! -path './gen*'   \
		! -name 'LICENSE'  \
		! -name 'Makefile' \
		! -path '*master*' \
		! -path '.'        \
		-delete
	rm gen*.html

.PHONY: all docs clean
