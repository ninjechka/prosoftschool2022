#ifndef DEVICEMONITORINGSERVER_MESSAGEENCODER_H
#define DEVICEMONITORINGSERVER_MESSAGEENCODER_H


#include <string>
#include <map>
#include "BaseEncoderExecutor.h"
#include <vector>
/*!
 * \brief класс для шифрования и дешифрования сериализованных сообщений
 * для последующей передачи через клиент-серверное соединение.
 */
class MessageEncoder final
        {
public:
    MessageEncoder();
    ~MessageEncoder();
    std::vector<std::string> getAllAlgorithms();
    /*!
    * \brief Зашифровать сообщение
    * * \param message - сообщение
    * \return зашифрованное сообщение
   */
    std::string encode(const std::string& message);
    /*!
   * \brief Дешифровать сообщение
    * * \param message - сообщение
    * \return дешифрованное сообщение
   */
    std::string decode(const std::string& message);
    /*!
   * \brief выбрать алгоритм шифрования
    * * \param algorithm - имя алгоритма
    * \return false - если в числе зарегистрированных алгоритмов такого нет
   */
    bool chooseAlgorithm(std::string algorithm);
    /*!
   * \brief добавить алгоритм шифрования
    * * \param algorithm - невладеющий указатель на алгоритм
    * \return false - если не удалось
   */
    bool addAlgorithm(BaseEncoderExecutor* algorithm);
private:
    BaseEncoderExecutor* currentAlgorithm;
    std::map<std::string, BaseEncoderExecutor*> allAlgorithms;
};


#endif //DEVICEMONITORINGSERVER_MESSAGEENCODER_H