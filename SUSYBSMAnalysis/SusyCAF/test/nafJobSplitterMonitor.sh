#!/bin/zsh
REPEAT=4
if [ $1 ]
then
REPEAT=$1
fi

echo $HOST
echo $$
for ((i = 0; i < $REPEAT; i++))
do
for dir in `ls | grep naf_ | grep _cfg`
do
for errFile in `nafJobSplitter.pl check $dir | grep cmsRun | awk '{print $6}'`
do
set -- "${errFile%.txt*}";
errFile="${1##*: }".txt
rm $errFile
echo $errFile
done
nafJobSplitter.pl check $dir
echo `ls $dir | grep .root | wc -l`
done
value=`expr $REPEAT - 1`;
if [[ $i -lt $value ]]
then
sleep 1200
fi
done
qsub -cwd -V -l h_rt=2:30:00 -l h_vmem=1700M ./nafJobSplitterMonitor.sh
