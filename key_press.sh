#!bin/bash

#modify key bindings in this script
# xdotool key / keydown / keyup
# wmctrl -R <window_name>

simulate=false 	#set true if you want the output to go to terminal (testing purposes)
target_window_name="warcraft"

if $simulate; then
	target_window_name="kristjan"	#going for terminal
fi
#wmctrl -R "$target_window_name" 	#don't target

function random_sleep {
	str1="0."
	str2=$((5 + RANDOM%5 ))
	str3="s"
	str=$str1$str2$str3
	sleep $str
}

case "$1" in
	"MOUSEMOVE")
		xdotool mousemove "$2" "$3"
		;;
	"LOOT")
		random_sleep
		echo "loot"
		xdotool keydown Shift
		xdotool click 3
		sleep 0.5s
		xdotool keyup Shift 
		;;
	"THROW")
		random_sleep
		echo "throwing"
		xdotool key 7 
		;;
	*)
		echo "error: Undefined command."
		;;
esac
