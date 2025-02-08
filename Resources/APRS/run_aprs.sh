## Thank god for ChatGPT

cat > /tmp/dw_config.conf
echo "------ Using Dire Wolf Config ------"
cat /tmp/dw_config.conf
echo "------------------------------------"

rtl_fm -f 434.55M -s 22050 -g 20.7 | direwolf -c /tmp/dw_config.conf -r 22050 -D 1 -B 1200 -

