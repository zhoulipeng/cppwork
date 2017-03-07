# for linux ####################################################################
cat /proc/sys/kernel/random/uuid
#or 
uuidgen
#or yum install uuid 
uuid
# shell scripts turn [a-z] to [A-Z]
GUID=""
declare -u GUID
GUID=$(uuid)
# fow windows ##################################################################
powershell -Command "[guid]::NewGuid().ToString()"
#or 
#Drop the following code into a new file name uuid.vbs

set obj = CreateObject("Scriptlet.TypeLib")
WScript.StdOut.WriteLine obj.GUID
#Then you can run it from the command line like so:
cscript //NoLogo uuid.vbs
