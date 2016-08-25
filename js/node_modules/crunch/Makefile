test:
	@if [ "$(NODE_ENV)" = "" ]; then NODE_ENV=test ./node_modules/.bin/mocha --reporter list; else ./node_modules/.bin/mocha --reporter list; fi
install:
	npm install .
.PHONY: test