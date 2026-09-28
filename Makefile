TolesA1:TolesA1.cpp
	g++ -DtestMode=0 TolesA1.cpp -o TolesA1
	./TolesA1
test: TolesA1.cpp
	g++ -DtestMode=1 TolesA1.cpp -o TolesA1
	./TolesA1
	diff -B output.txt expected_output.txt
clean:
	rm TolesA1
	rm output.txt