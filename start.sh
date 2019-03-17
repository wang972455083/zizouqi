#!/bin/sh
cd /opt/CardServer/
ulimit -c unlimited

echo "start GameServer"
cd /root/wangzg/Server/GameServer
./XLGameServer




echo "start LogicManager"
cd /root/wangzg/Server/LogicManager
./XLLogicManager
sleep 1



echo "start LogicServer"
cd /root/wangzg/Server/LogicServer
./XLLogicServer
sleep 1


echo "start GateServer"
cd /root/wangzg/Server/GateServer
./XLGateServer
sleep 1


ps -ef|grep XL|grep -v grep

