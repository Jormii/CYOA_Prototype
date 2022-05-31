rm ./EBOOT.PBP
rm ./PARAM.SFO
rm ./PSP_TARGET.elf
rm ./PSP_TARGET.prx

python ./makefile_script.py
if [ $? -eq 0 ]; then
    make -B

    # Delete .o files
    DIRS=".. ../../PSPPrettyPrint ../../NarrativeLanguage"
    for DIR in $DIRS; do
        OBJ_FILES=$(find $DIR -name *.o)
        rm $OBJ_FILES
    done
fi
