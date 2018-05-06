#!/bin/sh
#imagemagickを使わずに画像を直接テンプレートマッチング

for image in $1/*; do
    x=0	#変数に０を代入．最初一回だけcでクリアするためのフラグ
    echo `basename ${image}`:#処理している画像の名前を表示
    for template in $2/*; do
	echo `basename ${template}`#処理しているテンプレートを表示
	if [ $x = 0 ]
	then
#	    echo ./matching "${image}" "${template}" 0.5 cp
	    ./matching "${image}" "${template}" 0.5 cp
	    x=1	#xを０以外にして，画像２つめからはｃオプションを呼ばないようにする
	else
#	    echo ./matching "${image}" "${template}" 0.5 p
	    ./matching "${image}" "${template}" 0.5 p
	fi
    done
    echo ""#改行を一つ追加
done
