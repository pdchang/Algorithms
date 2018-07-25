#Brandon Lo
#CS325
#!/bin/bash
rm -rf output*
rm -rf 2OPTTEST*
echo "Example Input Tests" >> output.txt
gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST tsp_example_1.txt
(python tsp-verifier.py tsp_example_1.txt tsp_example_1.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST tsp_example_2.txt
(python tsp-verifier.py tsp_example_2.txt tsp_example_2.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST tsp_example_3.txt
(python tsp-verifier.py tsp_example_3.txt tsp_example_3.txt.tour) >> output.txt

echo "Competition Input Tests" >> output.txt
gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-1.txt -c
(python tsp-verifier.py  test-input-1.txt  test-input-1.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-2.txt -c
(python tsp-verifier.py  test-input-2.txt  test-input-2.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-3.txt -c
(python tsp-verifier.py  test-input-3.txt  test-input-3.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-4.txt -c
(python tsp-verifier.py  test-input-4.txt  test-input-4.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-5.txt -c
(python tsp-verifier.py  test-input-5.txt  test-input-5.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-6.txt -c
(python tsp-verifier.py  test-input-6.txt  test-input-6.txt.tour) >> output.txt

gcc -O3 -lm -fopenmp -g 2OPT.c -o 2OPTTEST
./2OPTTEST test-input-7.txt -c
(python tsp-verifier.py  test-input-7.txt  test-input-7.txt.tour) >> output.txt

