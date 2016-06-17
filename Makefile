
default: all

.DEFAULT:
	cd src && $(MAKE) $@

.PHONY: test testpy
test:
	mkdir -p test/data
	cd output && ./test_db
testpy:
	cd py-binding && python bt_wrapper.py && cd -
