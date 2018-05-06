#!/bin/sh
#imagemagickで何か画像処理をして，/imgprocにかきこみ，テンプレートマッチング
for image in $1/*; do
    bname=`basename ${image}`
    name="imgproc/"$bname
    x=0    	#
    convert "${image}" $name #何もしない画像処理
#   convert -blur 2x6 "${image}" "imgproc/${image}"
#   convert -median 1 "${image}" "imgproc/${image}"
    echo $bname:
    for template in $2/*; do
	echo `basename ${template}`
	if [ $x = 0 ]
	then
	    ./matching $name "${template}" 0.5 cp
	    x=1
	else
	    ./matching $name "${template}" 0.5 p
	fi
    done
    echo ""
done
