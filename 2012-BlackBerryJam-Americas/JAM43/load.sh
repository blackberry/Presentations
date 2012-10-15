# This script can be used to sideload a BAR file
# Dependencies: BlackBerry 10 WebWorks SDK v 1.0.2.9
tools=/Developer/SDKs/Research\ In\ Motion/BlackBerry\ 10\ WebWorks\ SDK\ 1.0.2.9/dependencies/tools/bin
echo $tools
package=$1
device=$2
password=$3
if [[ $package == '' ]]; then
    echo Missing parameters.  Correct usage: load.sh package-name device-ip device-pwd
    exit
fi
if [[ $device == '' ]]; then
    echo Missing parameters.  Correct usage: load.sh package-name device-ip device-pwd
    exit
fi
if [[ $password == '' ]]; then
    echo Missing parameters.  Correct usage: load.sh package-name device-ip device-pwd
    exit
fi

blackberry-deploy -installApp -launchApp -package $1 -device $2  -password $3
