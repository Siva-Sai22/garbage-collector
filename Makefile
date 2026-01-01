test_ref: reference_counted_gc/test.c
	cd reference_counted_gc && gcc -std=c11 test.c oxy_object.c ../munit/munit.c -o test_ref && ./test_ref

clean:
	rm reference_counted_gc/test_ref
