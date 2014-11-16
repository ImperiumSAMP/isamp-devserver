#!/bin/bash

clear
echo "Reconfigurando el sistema, esta operación puede demorar algunos minutos..."
sudo dpkg --add-architecture i386 > marp-install.log
sudo apt-get update >> marp-install.log

echo "Instalando dependencias..."
sudo apt-get -y install libc6:i386 libncurses5:i386 libstdc++6:i386 libmysqlclient18:i386 >> marp-install.log

echo ""
echo "A continuacion se instalara la base de datos MySQL."
echo "El proceso de instalacion le pedira ingresar una clave para el usuario root. Por favor, tome nota de la misma, le sera solicitada nuevamente." 
echo "*** Presione enter para continuar ***"
read

sudo apt-get -y install mysql-server mysql-common mysql-client

echo ""
echo "A continuación se instalará phpMyAdmin para administrar la base de datos, cuando se le solicite un password ingrese el mismo utilizado en el paso anterior"
echo "*** Presione enter para continuar ***"
read

sudo apt-get -y install phpmyadmin

echo ""
echo "A continuación se instalará la base de datos del juego, por favor, cuando le sea solicitado ingrese la clave de la base de datos."
echo "*** Presione enter para continuar ***"
read
mysql -u root -p < scriptfiles/isamp-data/db.sql

echo "*** Instalación completa ***