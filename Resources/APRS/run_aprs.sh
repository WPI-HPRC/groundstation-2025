## Thank god for ChatGPT

cat > /tmp/dw_config.conf
#echo "------ Using Dire Wolf Config ------"
#cat /tmp/dw_config.conf
#echo "------------------------------------"

rtl_fm -d 1 -f [FREQVHF] -s 22050 -g 20.7 | direwolf -c /tmp/dw_config.conf -r 22050 -D 1 -B 1200 -
rtl_fm -d 0 -f [FREQUHF] -s 22050 -g 20.7 | direwolf -c /tmp/dw_config.conf -r 22050 -D 0 -B 1200 -