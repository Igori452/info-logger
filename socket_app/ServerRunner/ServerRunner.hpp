#pragma once

#include "MessageStatistics/MessageStatistics.hpp"
#include "SocketReceiver/SocketReceiver.hpp"

namespace ServerRunner 
{

void printStatistic(const MessageStatistics& msgStat, std::ostream& os);

void run (int sock, size_t T, size_t N, std::ostream& os);

} /* namespace ServerRunner */