python ./makefile_script.py
make -B

# Delete .o files
OBJ_FILES=$(find .. -name *.o)
rm $OBJ_FILES