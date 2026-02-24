cd src
make clean && make
cd ..

date
echo "Starting.."
./TickMerger --data data --output output
echo "Ending.."

echo "Content of output file"
cat output/*
date
