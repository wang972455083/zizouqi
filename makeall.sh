echo "start to build"

echo "building XLCenterServer"


cd ./GameServer
sh make.sh
make clean
make

echo "build end...."
cp XLGameServer /root/wangzg/Server/GameServer
echo "copy end...."

cd ..
echo "building XLLogicManager"
cd ./LogicManager
sh make.sh
make clean
make

echo "build end...."
cp XLLogicManager /root/wangzg/Server/LogicManager
echo "copy end...."

cd ..
echo "building XLLogicServer"
cd ./LogicServer
sh make.sh
make clean
make

echo "build end...."
cp XLLogicServer /root/wangzg/Server/LogicServer
echo "copy end...."

cd ..
echo "building XLGateServer"
cd ./GateServer
sh make.sh
make clean
make

echo "build end...."
cp XLGateServer /root/wangzg/Server/GateServer
echo "copy end...."


echo "building finished!!!"

cd ..

echo "please check creation date of all executable files"

ls -Rl|grep XL

