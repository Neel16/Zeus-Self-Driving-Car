"""
Project-specific subsidary SConscript
"""

import os

from fsops import scan_tree, filter_files, ch_filename_ext
from helper import is_windows, is_linux
from osops import prepend_env_path

__version__ = "1.0.0"


"""
CLI arguments
"""
AddOption("--no_default_toolchain",
          action="store_true",
          help="DIsable default ARM toolchain.")

no_default_toolchain = GetOption("no_default_toolchain")


"""
Define naming conventions
"""
TARGET_NAME = Dir(".").name


"""
Define file nodes and directory nodes
"""
ARM_TOOLCHAIN_DIR = Dir(os.path.join(Dir("#").abspath, "..", "toolchain", "bin"))
PROJECT_DIR = Dir(".")
VARIANT_DIR = Dir("build")
OBJ_DIR = VARIANT_DIR.Dir("obj")
MAP_FILE = VARIANT_DIR.File("{}.map".format(TARGET_NAME))

INCLUDE_DIRS = [
    Dir("."),
    Dir("newlib"),
    Dir("L0_LowLevel"),
    Dir("L1_FreeRTOS"),
    Dir("L1_FreeRTOS/include"),
    Dir("L1_FreeRTOS/portable"),
    Dir("L1_FreeRTOS/portable/no_mpu"),
    Dir("L2_Drivers"),
    Dir("L2_Drivers/base"),
    Dir("L3_Utils"),
    Dir("L3_Utils/tlm"),
    Dir("L4_IO"),
    Dir("L4_IO/fat"),
    Dir("L4_IO/wireless"),
    Dir("L5_Application"),
]

INCLUDE_DIRS_ROOT = [
]

SRC_DIRS = [
    Dir("newlib"),
    Dir("L0_LowLevel"),
    Dir("L1_FreeRTOS"),
    Dir("L2_Drivers"),
    Dir("L3_Utils"),
    Dir("L4_IO"),
    Dir("L5_Application"),
]

LINKER_FILES = [
    File("loader.ld")
]

EXCLUDED_SRC_FILES = [
    File("L1_FreeRTOS/portable/mpu/port.c")
]


"""
Define build environments
"""
Import("arm_env")

if is_windows() and not no_default_toolchain:
    prepend_env_path(arm_env, ARM_TOOLCHAIN_DIR)

arm_env.VariantDir(variant_dir=VARIANT_DIR, src_dir=Dir("."), duplicate=0)

arm_env["CPPDEFINES"].extend([
    "BUILD_CFG_MPU=0",
])

arm_env["LINKFLAGS"].extend([
    "-Wl,-Map,{}".format(MAP_FILE.abspath),
])


"""
Search and group build files
"""

""" Search and group source files and source directories """
target_src_filenodes = []
target_src_dirnodes = []
for dir in SRC_DIRS:
    src_filenodes, src_dirnodes, _ = scan_tree(dir)
    target_src_filenodes.extend(src_filenodes)
    target_src_dirnodes.extend(src_dirnodes)

""" Search and group linker scripts """
for linker_file in LINKER_FILES:
    arm_env["LINKFLAGS"].append("-T{}".format(File(linker_file).abspath))

""" Search and group include paths """
arm_env["CPPPATH"].extend(INCLUDE_DIRS)
for dir in INCLUDE_DIRS_ROOT:
    _, _, include_dirnodes = scan_tree(dir)
    arm_env["CPPPATH"].extend(include_dirnodes)

""" Filter build files """
target_src_filenodes = filter_files(target_src_filenodes, EXCLUDED_SRC_FILES)


"""
Perform builds
"""
obj_filenodes = []
for src_filenode in target_src_filenodes:
    new_filename = ch_filename_ext(src_filenode, "o")
    dest_filepath = OBJ_DIR.File(new_filename.name)
    new_obj_filenodes = arm_env.Object(target=dest_filepath, source=src_filenode)
    obj_filenodes.extend(new_obj_filenodes)

elf_filenodes = arm_env.Program(target=VARIANT_DIR.File("{}.elf".format(TARGET_NAME)), source=obj_filenodes)
bin_filenodes = arm_env.Objcopy(target=VARIANT_DIR.File("{}.bin".format(TARGET_NAME)), source=elf_filenodes)
hex_filenodes = arm_env.Objcopy(target=VARIANT_DIR.File("{}.hex".format(TARGET_NAME)), source=elf_filenodes)
lst_filenodes = arm_env.Objdump(target=VARIANT_DIR.File("{}.lst".format(TARGET_NAME)), source=elf_filenodes)
arm_env.Size(elf_filenodes)

Depends(elf_filenodes, LINKER_FILES)
