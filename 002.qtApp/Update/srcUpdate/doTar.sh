#/bin/sh

# Create by : QiuWeibo

rm ./Target.tar.gz -f
rm ./Target/updateTime -f

date >> ./Target/updateTime
tar czvf Target.tar.gz ./Target
cp Target.tar.gz /tmp/ -f
