#include <string.h>
#include <iostream>
#include <class_leica/geocom_class_leica.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <class_leica/QtcpSocket/my_client/mytcpclient.h>

int main(int argc, char* argv[])
{
    GeoCom_Class_Leica leica;
    if(leica.initTotalStation()==-1) return 0;

    std::vector<std::string> coordinateMea;
    double x,y,z,time;

    while(true) //BUCLE INFINITO
    {
        coordinateMea = leica.TMC_GetCoordinate();
        x = stod(coordinateMea[1]);
        y = stod(coordinateMea[2]);
        z = stod(coordinateMea[3]);
        time = stod(coordinateMea[4]);
        std::cout << "X: " << x << ", Y: " << y << ", Z: " << z << ", Time: " << time << std::endl;
    }
    return 0;
}
