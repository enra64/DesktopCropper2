#! /bin/bash

wantedName="$1"
scriptDir=$(realpath "$0")
scriptDir=$(dirname $scriptDir)
cd $scriptDir

#extract filenames for every contained split image
fileNames=($(find -iname "*_l.jpg" | cut -c3- | rev | cut -c6- | rev))
fileCount=${#fileNames[@]}

#get last used
if [ -a lastused.txt ];
then
	lastUsed=$(cat lastused.txt)
else
	lastUsed=0
fi

#determine next used image
useNext=$(((lastUsed + 1) % fileCount))

#save this as last used image
echo $useNext > lastused.txt

echo Now using image $useNext

fileName="${fileNames[useNext]}"

if [ ! -z "$1" ];
then
	fileName=$1 #$(echo $wantedName | rev | cut -c6- | rev)
	echo $fileName
fi

pathLeft="$scriptDir"/"$fileName"l.jpg
pathMiddle="$scriptDir"/"$fileName"m.jpg
pathRight="$scriptDir"/"$fileName"r.jpg

echo $fileName
echo $fileNames
echo $fileCount

#set image parts as background
xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorDVI-0/workspace0/last-image -s "$pathLeft"
xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorDVI-0/workspace0/image-style -s 3

xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorDisplayPort-0/workspace0/last-image -s "$pathMiddle"
xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorDisplayPort-0/workspace0/image-style -s 3

xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorHDMI-0/workspace0/last-image -s "$pathRight"
xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitorHDMI-0/workspace0/image-style -s 3
