prj_dir=$(pwd -P | sed 's/\/default//g')
prj_name=$(pwd -P | awk -F "/" '{print $(NF-1)}')
xcb_com=COM5
all_dir=(
	res	
	lib
	lib_a
	data
)


log() {
	echo 
	echo log [start]
	xcb logcat -c
	xcb logcat '*:e'
}

#main
if [ "$1" = "clean" ]; then
	xcb connect com:$xcb_com
	xcb installer uaip MAINAPP
	exit
fi

if [ "$1" = "sys" ]; then
	xcb connect com:$xcb_com
	xcb connect com:$xcb_com
	xcb telnetd
	exit
fi

if [ "$1" = "exec" ]; then
	export LANG=en_US
	ret_file=ret.tmp
	make 2> $ret_file
	if cat "$ret_file" | grep Error
	then 
		echo make [fail]
		rm $ret_file
		exit
	else
		echo make [ok]
	fi
	rm $ret_file
fi

if [ "$1" = "recover" ]; then
	xcb kill-server
	xcb start-server
	xcb connect com:$xcb_com
	xcb connect com:$xcb_com
	xcb telnetd
	exit
fi


if [ "$1" = "log" ]; then
	xcb connect com:$xcb_com
	xcb connect com:$xcb_com
	log
	exit
fi

aip=a.aip
rm -f $aip
if [ "$1" = "file" ]; then
	zip -rj $aip $prj_dir/appinfo $prj_dir/default/$prj_name $prj_dir/$2
else
	zip -rj $aip $prj_dir/appinfo $prj_dir/default/$prj_name
	if [ "$1" = "all" ]; then
		dir_save=$(pwd -P)
		cd $prj_dir
		zip	-r $dir_save/$aip ${all_dir[@]}
		cd $dir_save
	fi
fi
xcb connect com:$xcb_com
echo "start loading...."
xcb installer aip $aip
rm $aip

if [ "$2" = "log" ]; then
	log
fi

if [ "$3" = "log" ]; then
	log
fi
