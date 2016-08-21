# Patterns matching CSS files that should be minified. Files with a -min.css
# suffix will be ignored.

TARGET = NETLIGHT
#TARGET = RUDY

ifeq ($(TARGET),NETLIGHT)
	PNG_FILES = $(wildcard favicons/*.png)
else
	PNG_FILES = $(wildcard favicons-rudy/*.png)
endif

HTML_FILES = $(wildcard \
	*.html \
	**/*.html \
)


CSS_FILES = $(filter-out %-min.css,$(wildcard \
	*.css \
	**/*.css \
))

# Patterns matching JS files that should be minified. Files with a -min.js
# suffix will be ignored.
JS_FILES = $(filter-out %-min.js,$(wildcard \
	*.js \
	**/*.js \
))

# Command to run to execute the YUI Compressor.
YUI_COMPRESSOR = yuicompressor

# Flags to pass to the YUI Compressor for both CSS and JS.
YUI_COMPRESSOR_FLAGS = --charset utf-8 --verbose

# Command to run to execute the GZIP Compressor.
GZIP_COMPRESSOR = gzip

# Flags to pass to the GZIP Compressor for HTML, CSS and JS.
GZIP_COMPRESSOR_FLAGS = --verbose --keep --force

CSS_MINIFIED = $(CSS_FILES:.css=-min.css)
JS_MINIFIED = $(JS_FILES:.js=-min.js)

CSS_GZIPED = $(CSS_MINIFIED:-min.css=-min.css.gz)
JS_GZIPED = $(JS_MINIFIED:-min.js=-min.js.gz)
HTML_GZIPED = $(HTML_FILES:.html=.html.gz)
PNG_GZIPED = $(PNG_FILES:.png=.png.gz)

DATA_DIR = ../data/

data-dir: gzip
	rm -f $(DATA_DIR)*
	cp $(CSS_GZIPED) $(JS_GZIPED) $(HTML_GZIPED) $(PNG_GZIPED) $(DATA_DIR)

gzip: gzip-css gzip-js gzip-html gzip-png

gzip-css: $(CSS_MINIFIED) $(CSS_GZIPED)
gzip-js: $(JS_MINIFIED) $(JS_GZIPED)
gzip-html: $(HTML_FILES) $(HTML_GZIPED)
gzip-png: $(PNG_FILES) $(PNG_GZIPED)

# target: minify - Minifies CSS and JS.
minify: minify-css minify-js

# target: minify-css - Minifies CSS.
minify-css: $(CSS_FILES) $(CSS_MINIFIED)

# target: minify-js - Minifies JS.
minify-js: $(JS_FILES) $(JS_MINIFIED)

%-min.css: %.css
	@echo '==> Minifying $<'
	$(YUI_COMPRESSOR) $(YUI_COMPRESSOR_FLAGS) --type css $< >$@
	@echo

%-min.js: %.js
	@echo '==> Minifying $<'
	$(YUI_COMPRESSOR) $(YUI_COMPRESSOR_FLAGS) --type js $< >$@
	@echo

%.gz: %
	@echo '==> Compressing $<'
	$(GZIP_COMPRESSOR) $(GZIP_COMPRESSOR_FLAGS) "$<"
	@echo

# target: clean - Removes minified CSS and JS files.
clean:
	rm -f $(CSS_MINIFIED) $(JS_MINIFIED)

# target: help - Displays help.
help:
	@egrep "^# target:" Makefile