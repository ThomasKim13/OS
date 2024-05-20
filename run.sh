echo "Testing FCFS Scheduling"
./project2 7 0 A 0 0 0 &
./project2 5 1 B 0 0 0 &
./project2 3 2 C 0 0 0 &
wait

echo "Testing SRTF Scheduling"
./project2 7 0 A 2 0 0 &
./project2 5 1 B 2 0 0 &
./project2 3 2 C 2 0 0 &
wait

echo "Testing RR Scheduling"
./project2 7 0 A 1 0 2 &
./project2 5 1 B 1 0 2 &
./project2 3 2 C 1 0 2 &
wait

echo "Testing Priority Scheduling"
./project2 7 0 A 3 2 0 &
./project2 5 1 B 3 1 0 &
./project2 3 2 C 3 3 0 &
wait

