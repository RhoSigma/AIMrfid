Import("env")
import getpass
from datetime import datetime


# def prebuild_set_build_id(source, target, env):
print("Setting buildID")

buildID_file = open("src/buildID.h", "w")
buildID_file.write("const char* BUILD_TIME = \"" + datetime.now().strftime("%d_%m_%Y_%Hh%Mm%Ss\";") + "\n")
buildID_file.write("const char* BUILT_BY = \"" + getpass.getuser() + "\"\n;")
buildID_file.close()
    # getpass.getuser()
    # print(getpass.getuser())


# env.AddPreAction("$PROGPATH", prebuild_set_build_id)


