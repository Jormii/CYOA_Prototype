import os

MAKEFILE = """
TARGET = {psp_target}
OBJS = {objs}

LIBS = {libs}

INCDIR = {include_dirs}
CFLAGS = {c_flags}
CXXFLAGS = $(CFLAGS) -fno-rtti -fexceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = {eboot_title}

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
"""


class Makefile:

    def __init__(self):
        self.psp_target = "PSP_TARGET"
        self.eboot_title = "EBOOT"
        self.c_flags = []

        self.objs = IWhereList()
        self.libs = IWhereList()
        self.include_dirs = IWhereList()

    def create(self, output_dir):
        path = os.path.join(output_dir, "Makefile")
        with open(path, "w") as fd:
            fd.write(MAKEFILE.format(
                psp_target=self.psp_target,
                eboot_title=self.eboot_title,
                c_flags=" ".join(self.c_flags),

                objs=self.objs.where(),
                libs=self.libs.where(),
                include_dirs=self.include_dirs.where()
            ))


class IWhere:

    def where(self):
        raise NotImplementedError()

    def __repr__(self):
        return self.where()


class Dir(IWhere):

    def __init__(self, path):
        self.path = path
        self.files = IWhereList()

    def where(self):
        if len(self.files.elements) == 0:
            return self.path

        where_stringify = []
        for f in self.files.elements:
            where_stringify.append(os.path.join(self.path, f.where()))

        return " ".join(where_stringify)


class File(IWhere):

    def __init__(self, path):
        self.path = path

    def where(self):
        return self.path


class IWhereList(IWhere):

    def __init__(self):
        self.elements = []

    def append(self, iwhere):
        self.elements.append(iwhere)

    def where(self):
        where_stringify = []
        for e in self.elements:
            where_stringify.append(e.where())

        return " ".join(where_stringify)


def add_all_object_files_in_dir(dir):
    for filename in os.listdir(dir.path):
        path = os.path.join(dir.path, filename)
        if not os.path.isfile(path):
            continue

        head, tail = os.path.splitext(filename)
        if tail != ".c":
            continue

        obj_filename = head + ".o"
        dir.files.append(File(obj_filename))


def main():
    makefile = Makefile()

    # Titles and flags
    makefile.eboot_title = "Prototype"
    makefile.c_flags = ["-G0", "-O2", "-Wall"]

    # Objs
    src = Dir("../src")
    main = File("../main.o")
    add_all_object_files_in_dir(src)
    for _src in [src, main]:
        makefile.objs.append(_src)

    # Libs
    pspp_lib = File("../compilation/pspp.a")
    for _lib in [pspp_lib]:
        makefile.libs.append(_lib)

    # Includes
    pspp_include = Dir("../include/pspp/")
    include = Dir("../include/")
    for _inc in [pspp_include, include]:
        makefile.include_dirs.append(_inc)

    makefile.create("./")


if __name__ == "__main__":
    main()
