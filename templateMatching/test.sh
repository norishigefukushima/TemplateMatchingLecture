#!/bin/sh
#imagemagick���g�킸�ɉ摜�𒼐ڃe���v���[�g�}�b�`���O

for image in $1/*; do
    x=0	#�ϐ��ɂO�����D�ŏ���񂾂�c�ŃN���A���邽�߂̃t���O
    echo `basename ${image}`:#�������Ă���摜�̖��O��\��
    for template in $2/*; do
	echo `basename ${template}`#�������Ă���e���v���[�g��\��
	if [ $x = 0 ]
	then
#	    echo ./matching "${image}" "${template}" 0.5 cp
	    ./matching "${image}" "${template}" 0.5 cp
	    x=1	#x���O�ȊO�ɂ��āC�摜�Q�߂���͂��I�v�V�������Ă΂Ȃ��悤�ɂ���
	else
#	    echo ./matching "${image}" "${template}" 0.5 p
	    ./matching "${image}" "${template}" 0.5 p
	fi
    done
    echo ""#���s����ǉ�
done
