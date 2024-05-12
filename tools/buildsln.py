import project_settings 
import os , subprocess , sys
import shutil

args = project_settings.ProcessArguments(sys.argv)
print(args)
CONFIG = project_settings.GetArgValue(args , "c" , "debug")

print("Building {} in [{}] Configuration".format(project_settings.EXE_NAME , CONFIG))
print("\n")

ret = 0

first_build = False
if not os.path.exists("bin/{}/{}".format(CONFIG , project_settings.EXE_NAME)):
    first_build = True

if project_settings.IsWindows():
    MSBUILD = os.environ["MSBUILD"][8:-1]
    MSBUILD = "C:\\\\" + MSBUILD
    ret = subprocess.call(
        [
            "cmd.exe" , "/c" , "{}".format(MSBUILD) , 
            "{}.sln".format(project_settings.PROJECT_NAME) , 
            "/property:Configuration={}".format(CONFIG)
        ]
    )
    if (ret != 0):
        sys.exit(ret)

if project_settings.IsLinux():
    ret = subprocess.call(["make" , "config={}".format(CONFIG)])

if project_settings.IsMac():
    ret = subprocess.call(["make" , "config={}".format(CONFIG)])

sys.exit(ret)

