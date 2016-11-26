MCR=/sys/class/tacho-motor/motor0
MCL=/sys/class/tacho-motor/motor2
GYRO_VAL=/sys/class/lego-sensor/sensor4/value0
SPEEDR=$1
SPEEDL=$1
FIX=$3
echo reset > $MCR/command
echo reset > $MCL/command

start_angle=$(cat $GYRO_VAL)

#echo $3 > $MCR/speed_pid/Kd
#echo $3 > $MCL/speed_pid/Kd

#echo $4 > $MCR/speed_pid/Ki
#echo $4 > $MCL/speed_pid/Ki

#echo $5 > $MCR/speed_pid/Kp
#echo $5 > $MCL/speed_pid/Kp

echo brake > $MCR/stop_action
echo brake > $MCL/stop_action

echo inversed > $MCR/polarity
echo inversed > $MCL/polarity

echo $SPEEDR > $MCR/speed_sp
echo $SPEEDR > $MCL/speed_sp

echo "run-forever" > $MCR/command
echo "run-forever" > $MCL/command


for T in $(seq 0 $2)
do
	angle=$(cat $GYRO_VAL)
	err=$((start_angle-angle))
	echo "Error is: $err"
	
	SPEEDR=$((SPEEDR-(err*FIX)))
	SPEEDL=$((SPEEDL+(err*FIX)))
	echo "Speed L: $SPEEDL Speed R: $SPEEDR"
	echo $SPEEDR > $MCR/speed_sp
	echo $SPEEDL > $MCL/speed_sp

	echo "run-forever" > $MCR/command
	echo "run-forever" > $MCL/command
	
	sleep 0.1
done



echo "stop" > $MCR/command
echo "stop" > $MCL/command
