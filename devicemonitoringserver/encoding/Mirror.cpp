#include "BaseEncoderExecutor.h"

class Mirror final : public BaseEncoderExecutor
{
public:
    Mirror() = default;


    std::string encode(std::string message) const final
    {
        std:: string result = "";
        for (int i = 0 ; i < message.length(); i++)
        {
            if (isalpha(message[i]) || std::isdigit(message[i]) && i < message.length()
            && isalpha(message[i + 1]) || std::isdigit(message[i + 1]))
            {
                result.push_back(message[i + 1]);
                result.push_back(message[i]);
            }
            else result += message[i];
        }
        return  result;
    }

    std::string decode(std::string message) const final
    {
        std:: string result = "";
        for (int i = 0 ; i < message.length(); i++)
        {
            if (isalpha(message[i]) || std::isdigit(message[i]) && i < message.length()
                                       && isalpha(message[i + 1]) || std::isdigit(message[i + 1]))
            {
                result.push_back(message[i + 1]);
                result.push_back(message[i]);
            }
            else result += message[i];
        }
        return  result;
    }

};