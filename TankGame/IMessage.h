#pragma once

#include <glm/vec4.hpp>
#include <string>
#include <vector>

struct sMessage
{
	std::string command;
	std::vector<std::string> sData;
	std::vector<glm::vec4> fData;
	std::vector<void*> pData;
};

class IMessage
{
public:
	virtual ~IMessage() = default;

	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool SetReciever(IMessage* pTheReciever) = 0;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) = 0;
};