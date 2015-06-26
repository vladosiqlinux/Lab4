make >> log.txt
sudo insmod ./Calculator.ko

echo "2 + 3" >> /proc/calc_write
cat /proc/calc_result

echo "-2 + 3" >> /proc/calc_write
cat /proc/calc_result

echo "2 + -3" >> /proc/calc_write
cat /proc/calc_result

echo "2 - 3" >> /proc/calc_write
cat /proc/calc_result

echo "-2 - 3" >> /proc/calc_write
cat /proc/calc_result

echo "2 * 3" >> /proc/calc_write
cat /proc/calc_result

echo "2 * -3" >> /proc/calc_write
cat /proc/calc_result

echo "2 / 3" >> /proc/calc_write
cat /proc/calc_result

echo "-6 / -3" >> /proc/calc_write
cat /proc/calc_result

sudo rmmod Calculator
make clean >> log.txt
