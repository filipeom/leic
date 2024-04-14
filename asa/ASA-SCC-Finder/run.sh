tests=.
V=$(( 10 * 1024 ))
E=$(( 30 * 1024 ))
S=$(( 10 * 1024 ))
MULT=2
mkdir -p results;
for ((i = 1; i < 10; i++));
do
    echo Running test$i ...&
    ./$tests/gerador $V $E $S > $tests/test$i.in;
    echo "V=$V E=$E";
    echo "V=$V E=$E" > results/time$i.out;
    { time ./graph < $tests/test$i.in > $tests/test$i.out; } 2>> results/time$i.out;
    head $tests/test$i.out;
    echo "V=$V E=$E" > results/mem$i.out;
    ({ valgrind $1 ./graph < $tests/test$i.in > $tests/test$i.out; } 2>> results/mem$i.out; echo "FINISHING VALGRING #$i ITERATION...FINISHED")&
    V=$(( $V * $MULT ));
    E=$(( $E * $MULT ));
    S=$(( $S * $MULT ));
    rm $tests/test$i.*;
done
clear
