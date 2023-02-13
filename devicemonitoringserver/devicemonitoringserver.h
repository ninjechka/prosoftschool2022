#ifndef DEVICEMONITORINGSERVER_H
#define DEVICEMONITORINGSERVER_H

#include "common.h"
#include "CommandCenter.h"
#include "encoding/MessageEncoder.h"
#include "serialization/MessageSerializator.h"
#include <cstdint>
#include <string>
#include <map>

struct DeviceWorkSchedule;
class AbstractConnectionServer;
class AbstractConnection;

/*!
 * \brief Класс сервера для мониторинга состояния устройств.
 */
class DeviceMonitoringServer
{
    NON_COPYABLE(DeviceMonitoringServer)
public:
    /*!
     * \brief Конструктор.
     * \param connectionServer - владеющий указатель на сервер для приема подключений
     */
    DeviceMonitoringServer(AbstractConnectionServer* connectionServer);
    ~DeviceMonitoringServer();
    /*!
     * \brief Установить план работы устройств.
     */
    void setDeviceWorkSchedule(const DeviceWorkSchedule&);
    /*!
     * \brief Начать прием подключений по идентификатору \a serverId
     */
    bool listen(uint64_t serverId);
    /*!
    * \brief возвращает СКО ошибки выполения плана для текущей фазы
    */
    double getStandardDeviation(uint64_t deviceId);
    /*!
     * \brief Выбор алгоритма кодирования сообщений
     * \param name - название алгоритма
     * \return результат попытки назначить алгоритм кодировщику
     */
    bool chooseEncodingAlgorithm(const std::string& name);
    /*!
     * \brief Регистрация нового алгоритма шифрования
     * \param algorithm - объект класса, унаследованный от BaseEncoderExecutor
     * \return результат попытки добавления алгоритма
     */
    bool registerEncodingAlgorithm(BaseEncoderExecutor* algorithm);

private:
    /*!
     * \brief Отправить сообщение устройству.
     * \param deviceId - идентификатор устройства
     * \param message - сообщение
     */
    void sendMessage(uint64_t deviceId, const std::string& message);
    /*!
     * \brief Обработчик приема нового сообщения от устройства.
     * \param deviceId - идентификатор устройства
     * \param message - сообщение
     */
    void onMessageReceived(uint64_t deviceId, const std::string& message);
    /*!
     * \brief Обработчик поступления нового входящего подключения.
     * \param conn - невладеющий указатель на объект подключения
     */
    void onNewIncomingConnection(AbstractConnection* conn);
    /*!
     * \brief Обработчик разрыва соединения.
     * \param deviceId - идентификатор устройства
     */
    void onDisconnected(uint64_t clientId);

private:
    void addMessageHandler(AbstractConnection* conn);
    void addDisconnectedHandler(AbstractConnection* conn);

private:
    MessageSerializator m_serializator;
    MessageEncoder m_encoder;
    CommandCenter m_commandCenter;
    AbstractConnectionServer* m_connectionServer = nullptr;
};

#endif // DEVICEMONITORINGSERVER_H
