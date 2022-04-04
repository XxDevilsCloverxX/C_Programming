for file in *.c; do echo $file : && printf "\n" && gcc -o build.out $file && ./build.out && printf "\n\n"; done
