Import("env")
import time
import getpass
from datetime import datetime
import subprocess
import serial

print("Setting buildID")

# garbage code. eat my ass.
git_proc = subprocess.Popen(['git', 'rev-parse', "--short", "HEAD"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
commit_hash = str(git_proc.stdout.readline().rstrip()).partition('\'')[2].partition('\'')[0]
git_proc.kill()

buildID_file = open("src/buildID.h", "w")
buildID_file.write("const char* BUILD_TIME = \"" + datetime.now().strftime("%d_%m_%Y_%Hh%Mm%Ss\";") + "\n")
buildID_file.write("const char* BUILT_BY = \"" + getpass.getuser() + "\";\n")
buildID_file.write("const char* COMMIT_HASH = \"" + commit_hash + "\";\n")
buildID_file.close()
