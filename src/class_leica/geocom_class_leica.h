#ifndef GEOCOM_CLASS_LEICA_H
#define GEOCOM_CLASS_LEICA_H

#include <string>
#include <iostream>
#include <sys/types.h>
#include <QtNetwork>
#include "QtcpSocket/my_client/mytcpclient.h"
#include <vector>

/**
 * @class GeoCom_Class_Leica
 * @brief Librería para la comunicación y manejo de la estación total MS50 de Leica
 *
 * \author $Autor: Javier Prada Delgado $
 *
 * \date $Fecha: 21/11/17 $
 */
class GeoCom_Class_Leica
{
public:
    GeoCom_Class_Leica();
    /** @fn split
    * @brief Separa la cadena de caracteres de la respuesta en los diferentes parametros
    * @param buf Cadena de caracteres con información de la petición
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> split(char *buf);
    /** @fn TMC_GetAngle
    * @brief Obtiene los ángulos que tenga el instrumento en ese momento
    * @param mode Modo de medición. Por defecto es 1
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> TMC_GetAngle(int mode);
    /** @fn TMC_GetSimpleMea
    * @brief Obtiene una medición de la posición del prisma
    * @param WaitTime Tiempo de espera en milisegundos para realizar la medición
    * @param mode Modo de medición. Por defecto es 1
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> TMC_GetSimpleMea(int WaitTime, int mode);
    /** @fn TMC_QuickDist
    * @brief Medida de la distancia al prisma
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> TMC_QuickDist();
    /** @fn TMC_GetCoordinate
    * @brief Obtiene una medición de la posición del prisma en formato X Y Z
    * @param WaitTime Tiempo de espera en milisegundos para realizar la medición
    * @param mode Modo de medición. Por defecto es 1
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> TMC_GetCoordinate(int WaitTime=100,int mode=1);
    /** @fn TMC_GetStation
    * @brief Obtiene la posición que tiene la estación total en el sistema de referencia
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> TMC_GetStation();
    /** @fn TMC_SetStation
    * @brief Fija la estación total en la nueva posición indicada por parámetros
    * @param eastCoord Coordenada Este de la estación total
    * @param northCoord Coordenada Norte de la estación total
    * @param heightCoord Coordenada Altura de la estación total
    * @param instrumentHeight Altura de la estación total respecto del suelo
    * @param offset Distancia constante que se le suma a la altura del instrumento. Por defecto dejar en -1.
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> TMC_SetStation(double eastCoord, double northCoord, double heightCoord, double instrumentHeight, double offset = -1);
    /** @fn connectSocket
    * @brief Conexión al socket que se encarga de comunicar los datos con la estación total.
    * @param _port Puerto del socket donde se realiza la conexión
    * @param _IP Dirección IP de la estación total
    * @return bool Devuelve true si la conexión se ha establecido correctamente.
    */

    int initTotalStation();
    /** @fn SetPrismType
    * @brief Establece el tipo de prisma o reflector a usar
    * @param prism Entero que indica el tipo de prisma a usar
    * @return void.
    */
    void SetPrismType(int prism);
    /** @fn TMC_SetOrientation
    * @brief Establece la orientación de la estación total en función a un ángulo dado. Si el ángulo es cero entonces el eje Y será allá donde esté apuntando la TS
    * @param orientation Ángulo entre la orientación actual y la deseada para el eje Y.
    * @return void.
    */
    void TMC_SetOrientation(int orientation);
    /** @fn TMC_SetEdmMode
    * @brief Establece el modo de medición que se va a usar para las medidas de distancia EDM.
    * @param mode Número que representa el modo de medición.
    * @return void.
    */
    void TMC_SetEdmMode(int mode);
    /** @fn TMC_DoMeasure
    * @brief Hace una primera medición de distancia para comprobar que todo es correcto. No devuelve ningun valor de distancia ni medida.
    * @param cmd Modo de medición TMC
    * @param mode Modo de inclinación del sensor
    * @return void.
    */
    void TMC_DoMeasure(int cmd,int mode);
    /** @fn AUT_Search
    * @brief Busca el reflector en la ventana establecida por los parámetros.
    * @param Hz_Area Ángulo en radianes que determina la zona horizontal de búsqueda del reflector.
    * @param V_Area Ángulo en radianes que determina la zona vertical de búsqueda del reflector.
    * @param bDummy Variable reservada para futuros usos. Establecer siempre como False.
    * @return void.
    */
    void AUT_Search(double Hz_Area,double V_Area,bool bDummy);
    /** @fn AUT_FineAdjust
    * @brief Ajusta de forma precisa la crucetra de la cámara telescópica sobre el reflector y mide las desviaciones horizontales y verticales.
    * @param dSrchHz Ángulo en radianes que determina la zona horizontal de búsqueda del reflector.
    * @param dSrchV Ángulo en radianes que determina la zona vertical de búsqueda del reflector.
    * @param bDummy Variable reservada para futuros usos. Establecer siempre como False.
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    void AUT_FineAdjust(double dSrchHz,double dSrchV,bool bDummy);
    /** @fn AUS_SetUserLockState
    * @brief Activa o desactiva la posibilidad de seguimiento contínuo del prisma. Esto no significa que si está activado el prisma esté fijado. @see AUT_LockIn
    * @param state Estado: 1 activado, 0 desactivado.
    * @return void.
    */
    void AUS_SetUserLockState(bool state);
    /** @fn AUT_LockIn
    * @brief Si está activado el modo de seguimiento (@see AUS_SetUserLockState) esta función fijará el prisma y lo seguirá de forma continua.
    * @return Variable de error para determianr si todo es correcto. Si devuelve 0 todo OK.
    */
    std::string AUT_LockIn();
    /** @fn writeAndRead
    * @brief Escribe una petición en el socket que manda a la TS y lee la respuesta del instrumento.
    * @param cmd Cadena de caracteres con la petición en formato ASCII.
    * @return std::vector<std::string> Un vector con los parámetros de respuesta.
    */
    std::vector<std::string> writeAndRead(std::string cmd);
    /** @fn createRequest
    * @brief Crea una cadena de caracteres en formato ASCII para posteriormente ser enviada a la TS.
    * @param cmd Número que identifica a la etición que se va a realizar.
    * @param args Argumentos adicionales que determinan la petición y todas sus características.
    * @return std::string Cadena de caracteres con la petición.
    */
    std::string createRequest(int cmd,std::string args);

private:
    bool connectSocket(int _port, char* _IP);
    /** @fn initTotalStation
    * @brief llama de forma automática una serie de funciones que sirven para inicializar la estación total la primera vez
    * @return void.
    */
    myTCPclient clientSocket; ///< Variable del socket Leica
    char serverReply[2000]; ///< buffer con la respuesta de la Total Station
};

#endif // GEOCOM_CLASS_LEICA_H
