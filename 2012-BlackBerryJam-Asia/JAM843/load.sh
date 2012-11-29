# This script can be used to sideload a BAR file
# Dependencies: BlackBerry 10 WebWorks SDK
#   Change the tools variable to match the path of the WebWorks SDK installed on your machine:
#
tools=/Developer/SDKs/Research\ In\ Motion/BlackBerry\ 10\ WebWorks\ SDK\ 1.0.3.8/dependencies/tools/bin

echo $tools
device=$1
password=$2
if [[ $device == '' ]]; then
    echo Missing parameters.  Correct usage: load.sh device-ip device-pwd
    exit
fi
if [[ $password == '' ]]; then
    echo Missing parameters.  Correct usage: load.sh device-ip device-pwd
    exit
fi

blackberry-deploy -installApp -launchApp -package app/device/wic.bar -device $1  -password $2