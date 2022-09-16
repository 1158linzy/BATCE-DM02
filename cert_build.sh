#!/bin/bash



rm -rf ./bin

mkdir bin && cd ./bin

mkdir acts hilink  kitfwk  kitfwk_token 

cd ../

rm -rf ./ohos_config.json

hb set

#####
git checkout whole_house_sdk-hi3861-acts
hb clean
cd ./device/hisilicon/hi3861/sdk_liteos/
./build.sh clean
cd ../../../..
./test/xts/tools/lite/build.sh product=wifiiot xts=acts
mv ./out/hi3861/hi3861/suites/acts.zip ./bin/acts/acts.zip

#####
git checkout whole_house_sdk-hi3861-kitfwk
hb clean
cd ./device/hisilicon/hi3861/sdk_liteos/
./build.sh clean
cd ../../../..
hb build -f
mv ./out/hi3861/hi3861/Hi3861_wifiiot_app_allinone.bin ./bin/kitfwk/harmonyOS_cert-kitfwk.bin

#####
git checkout whole_house_sdk-hi3861-kitfwk_token
hb clean
cd ./device/hisilicon/hi3861/sdk_liteos/
./build.sh clean
cd ../../../..
hb build -f
mv ./out/hi3861/hi3861/Hi3861_wifiiot_app_allinone.bin ./bin/kitfwk_token/harmonyOS_cert-kitfwk_token.bin

#####
git checkout whole_house_sdk-hi3861
hb clean
cd ./device/hisilicon/hi3861/sdk_liteos/
./build.sh clean
cd ../../../..
hb build -f -b release
mv ./out/hi3861/hi3861/Hi3861_wifiiot_app_allinone.bin ./bin/hilink/harmonyOS_cert-hilink.bin
mv ./out/hi3861/hi3861/Hi3861_wifiiot_app_ota.bin ./bin/hilink/harmonyOS_cert-hilink_ota.bin

tree ./bin
